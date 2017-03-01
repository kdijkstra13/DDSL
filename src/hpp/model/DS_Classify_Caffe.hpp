#pragma once
#include "h/model/DS_Classify_Caffe.h"

#include "hpp/model/DS_Model.hpp"
#include "hpp/lib/DS_Image_PNG.hpp"

#include "caffe/layers/memory_data_layer.hpp"
#include "caffe/layers/inner_product_layer.hpp"
#include "caffe/layers/conv_layer.hpp"

#include "caffe/sgd_solvers.hpp"
#include "caffe/solver_factory.hpp"
#include "caffe/util/db.hpp"

//#include "caffe/util/io.hpp"
#include "google/protobuf/text_format.h"
#ifndef CPU_ONLY
  #include "cuda.h"
  #include "cuda_runtime.h"
#endif

//#include "direct.h"

using namespace std;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSUtil;
using namespace DSLib;
using namespace caffe;
using namespace db;

namespace DSTypes {
	static const char* UnitTypeStr[] = {"TanH", "Sigmoid", "ReLU"};
	static const char* SolverTypeStr[] = {"SGD", "AdaDelta", "AdaGrad", "Adam", "Nesterov", "RMSProp"};
	static const char* LearnRatePolicyStr[] = {"fixed", "step", "exp", "inv", "multistep", "poly", "sigmoid"};
	static const char* BackendStr[] = {"LMDB", "LevelDB"};
	static const char* DBModeStr[] = {"Append", "New"};
}

namespace DSFunc {
	inline DB * openDB(const String filename, const DBMode mode, const Backend be) {
		const String f = "openDB";
		DB * dat;
		switch (be) {
		case beLMDB: dat = GetDB(DataParameter_DB_LMDB); break;
		case beLevelDB: dat = GetDB(DataParameter_DB_LEVELDB); break;
		default: throw Error(ecIncompatible, f, "Unknown database backend");
		}
		if (MKDIR(filename.c_str()) == 0) {
			RMDIR(filename.c_str());
			if (mode == dbmNew || mode == dbmAppendOrNew)
				dat->Open(filename, NEW);					
			else
				throw Error(ecUnexpected, "writeImageDB", SS("Database " << filename << " does not exist"));				
		} else {
			if (mode == dbmAppend || mode == dbmAppendOrNew)
				dat->Open(filename, WRITE);
			else
				throw Error(ecUnexpected, "writeImageDB", SS("Database " << filename << " already exists"));
		}
		return dat;
	}

	template <typename T, typename TInt, template <typename, typename> class TImg>
	inline void writeImageDB(DSLib::Matrix<TImg<T, TInt>, TInt> &images, DSLib::Matrix<T, TInt> &labels, const DSTypes::String &filename, const ProgressCallback &progressCallback, const DBMode mode, const Backend be) {
		const String f = "writeImageDB";
		if (images.rows.count() == 0)
			return;
		bool hasLabels = (labels.vec().count() != 0);
		DB * dat = nullptr;
		Transaction * trans = nullptr;
		dat = openDB(filename, mode, be);
		trans = dat->NewTransaction();

		try {			
			//Investigate dataset
			TInt channels = 0;
			TInt width = 0;
			TInt height = 0;
			for (auto img_it = images.rows[0]->begin(); img_it != images.rows[0]->end(); img_it++) {
				channels += img_it->getChannelCount();
				bool hadImage = img_it->hasImage();
				if (!hadImage) img_it->loadImage();
				if (width == 0) {
					width = img_it->getImageWidth();
					height = img_it->getImageHeight();
				}
				if (width != img_it->getImageWidth() || height != img_it->getImageHeight())
					throw Error(ecIncompatible, f, SS("Cannot use images with varying sizes: " << height << "x" << width << " != " << img_it->getImageHeight() << "x" << img_it->getImageWidth()));
				if (!hadImage) img_it->freeImage();
			}
			//Initialize datum
			Datum sample;
			sample.set_channels(channels);
			sample.set_width(width);
			sample.set_height(height);

			TInt rowIdx = 0;
			TInt bytes = 0;
			TInt min = 0;
			TInt max = images.rows.count();
			TInt p = 0;
			for (auto r = images.rows.begin(); r != images.rows.end(); r++, rowIdx++, p++) {
				Matrix<T, TInt> mcimg;
				//Concat all images in a row
				for (auto cell = r->begin(); cell != r->end(); cell++) {
					bool hadImage = cell->hasImage();
					if (!hadImage) cell->loadImage();
					if (cell->getChannelCount() > 1)
						cell->convertRGBToPlanarV();
					mcimg ^ *cell;
					if (!hadImage) cell->freeImage();
				}
				//Assign Matrix to Datum
				sample.clear_data();
				sample.clear_float_data();
				for (auto it = mcimg.rows->begin(); it != mcimg.rows->end(); it++) {
					sample.add_float_data((Float)*it);
				}
				if (hasLabels)
					sample.set_label((google::protobuf::int32)labels.vec(rowIdx));
				//Write to Transaction
				string out;
				sample.SerializeToString(&out);
				trans->Put(SS(rowIdx), out);
				bytes += height * width * channels * sizeof(Float);
				if (bytes > CHUNK_BYTES) {
					trans->Commit();
					delete trans;
					trans = dat->NewTransaction();
					bytes = 0;
				}
				progressCallback(min, max, p);
			}
			if (bytes > 0)
				trans->Commit();
			freeAndNull(&dat);
			freeAndNull(&trans);
			progressCallback(min, max, max);
		} catch (const exception &e) {
			freeAndNull(&dat);
			freeAndNull(&trans);
			throw Error(ecGeneral, f, e.what());
		}
	}

	template <typename TInt>
	void dbStats(const String &filename, TInt &size, TInt &width, TInt &height, TInt &channels, bool &hasLabels, const DSTypes::Backend be) {
		const String f = "dbStats";
		DB * dat = nullptr;
		Cursor * cur = nullptr;
		size = 0;
		channels = 0;
		width = 0;
		height = 0;
		hasLabels = false;
		try {
			switch (be) {
				case beLMDB: dat = GetDB(DataParameter_DB_LMDB); break;
				case beLevelDB: dat = GetDB(DataParameter_DB_LEVELDB); break;
				default: throw Error(ecIncompatible, f, "Unknown database backend");
			}
			dat->Open(filename, READ);
			Cursor * cur = dat->NewCursor();
			Datum sample;
			cur->SeekToFirst();
			if (cur->valid()) {
				sample.ParseFromString(cur->value());
				hasLabels = sample.has_label();
				channels = sample.channels();
				width = sample.width();
				height = sample.height();
			}
			while (cur->valid()) {
				cur->Next();
				size++;
			}
			freeAndNull(&cur);
			freeAndNull(&dat);
		} catch (const exception &e) {
			freeAndNull(&cur);
			freeAndNull(&dat);			
			throw Error(ecGeneral, "WriteDB", e.what());
		}
	}

	template <typename T, typename TInt, template <typename, typename> class TImg>
	inline void readImageDB(DSLib::Matrix<TImg<T, TInt>, TInt> &images, DSLib::Matrix<T, TInt> &labels, const DSTypes::String &filename, const DSTypes::String &cacheFolder, const ProgressCallback &progressCallback, const DSTypes::Backend be) {
		const String f = "readImageDB";
		DB * dat = nullptr;
		Cursor * cur = nullptr;
		if (cacheFolder != "")
			CHDIR(cacheFolder.c_str());
		try {
			//Get database content
			Datum sample;
			TInt rowCount = 0;
			TInt colCount = 0;
			TInt w = 0;
			TInt h = 0;
			bool hasLabels;
			dbStats(filename, rowCount, w, h, colCount, hasLabels, be);
			if (rowCount == 0 || colCount == 0 || w == 0 || h == 0)
				throw Error(ecNotFound, f, SS("Database contains no data"));

			//Open database
			switch (be) {
				case beLMDB: dat = GetDB(DataParameter_DB_LMDB); break;
				case beLevelDB: dat = GetDB(DataParameter_DB_LEVELDB); break;
				default: throw Error(ecIncompatible, f, "Unknown database backend");
			}
			dat->Open(filename, READ);
			cur = dat->NewCursor();	
			cur->SeekToFirst();

			//Resize Matrices
			if (images.rows.count() == 0) {
				images.resize(rowCount, colCount);
				if (hasLabels) labels.resize(rowCount, 1);
			} else if (images.rows.count() != rowCount || images.cols.count() != colCount)
				throw Error(ecIncompatible, f, SS("Invalid size for Images Matrix: " << images.rows.count() << "x" << images.cols.count() << " != " << rowCount << "x" << colCount));
			if (!hasLabels && labels.vec().count() > 0)
				throw Error(ecIncompatible, f, "LMDB contains no labels, but label Matrix is initialized");
			
			auto row_it = images.rows.begin();
			auto lab_it = labels.vec().begin();
			TInt idx = 0;
			while (cur->valid()) {

				//Convert from LMDB to Datum and then to TImg
				sample.ParseFromString(cur->value());
				const google::protobuf::RepeatedField<Float> & fdata = sample.float_data();
				if (sample.encoded())
					throw Error(ecIncompatible, "ReadImageDB", "Cannot read encoded data.");
				auto p = fdata.begin();
				TInt chan = 0;
				for (auto cell = row_it->begin(); cell != row_it->end(); cell++, chan++) {

					//Alloc image
					cell->mat().setOrder(oRowMajor);
					cell->mat().resize(h, w);

					//Copy data from Datum
					for (auto it = cell->mat().rows->begin(); it != cell->mat().rows->end(); it++, p++)
						*it = *p;

					//Set image type
					cell->setImageType(itM8);

					//Save image if needed
					if (cacheFolder != "") {
						String filename = SS("Key_" << cur->key() << "_Channel_" << chan << "_Label_" << (hasLabels ? *lab_it : T(0)) << cell->getExt());
						cell->setFilename(filename);
						cell->saveImage();
						cell->freeImage();
					}
				}
				if (hasLabels) {
					*lab_it = (google::protobuf::int32) sample.label();
					lab_it++;
				}
				row_it++;
				cur->Next();
				idx++;
				progressCallback(0, rowCount, idx);
			}
			freeAndNull(&cur);
			freeAndNull(&dat);
			progressCallback(0, rowCount, rowCount);
		} catch (const exception &e) {
			freeAndNull(&cur);
			freeAndNull(&dat);			
			throw Error(ecGeneral, f, e.what());
		}
	}

	template <typename T, typename TInt, template <typename, typename> class TMat>
	inline void writeDB(DSLib::Matrix<TMat<T, TInt>, TInt> &matrices, DSLib::Matrix<T, TInt> &labels, const DSTypes::String &filename, const ProgressCallback &progressCallback, const DBMode mode, const DSTypes::Backend be) {
		const String f = "writeDB";
		if (matrices.rows.count() == 0)
			return;
		bool hasLabels = (labels.vec().count() != 0);
		DB * dat = nullptr;
		Transaction * trans = nullptr;
		dat = openDB(filename, mode, be);
		trans = dat->NewTransaction();
		try {			
			//Investigate dataset
			TInt channels = 0;
			TInt width = 0;
			TInt height = 0;
			for (auto mat_it = matrices.rows[0]->begin(); mat_it != matrices.rows[0]->end(); mat_it++) {
				channels++;
				width = mat_it->cols.count();
				height = mat_it->rows.count();		
				if (width != mat_it->cols.count() || height != mat_it->rows.count())
					throw Error(ecIncompatible, f, SS("Cannot use matrices with varying sizes: " << height << "x" << width << " != " << mat_it->rows.count() << "x" << mat_it->cols.count()));
			}
			//Initialize datum
			Datum sample;
			sample.set_channels(channels);
			sample.set_width(width);
			sample.set_height(height);

			TInt rowIdx = 0;
			TInt bytes = 0;
			TInt min = 0;
			TInt max = matrices.rows.count();
			TInt p = 0;
			for (auto r = matrices.rows.begin(); r != matrices.rows.end(); r++, rowIdx++, p++) {
				Matrix<T, TInt> mat;
				//Concat all images in a row
				for (auto cell = r->begin(); cell != r->end(); cell++) {
					mat ^ *cell;
				}
				//Assign Matrix to Datum
				sample.clear_data();
				sample.clear_float_data();
				for (auto it = mat.rows->begin(); it != mat.rows->end(); it++) {
					sample.add_float_data((Float)*it);
				}
				if (hasLabels)
					sample.set_label((google::protobuf::int32)labels.vec(rowIdx));
				//Write to Transaction
				string out;
				sample.SerializeToString(&out);
				trans->Put(SS(rowIdx), out);
				bytes += height * width * channels * sizeof(Float);
				if (bytes > CHUNK_BYTES) {
					trans->Commit();
					delete trans;
					trans = dat->NewTransaction();
					bytes = 0;
				}
				progressCallback(min, max, p);
			}
			if (bytes > 0)
				trans->Commit();
			freeAndNull(&dat);
			freeAndNull(&trans);
			progressCallback(min, max, max);
		}
		catch (const exception &e) {
			freeAndNull(&dat);
			freeAndNull(&trans);
			throw Error(ecGeneral, f, e.what());
		}
	}

	template <typename T, typename TInt, template <typename, typename> class TMat>
	inline void readDB(DSLib::Matrix<TMat<T, TInt>, TInt> &matrices, DSLib::Matrix<T, TInt> &labels, const DSTypes::String &filename, const ProgressCallback &progressCallback, const DSTypes::Backend be) {
		const String f = "readDB";
		DB * dat = nullptr;
		Cursor * cur = nullptr;
		try {
			//Get database content
			Datum sample;
			TInt rowCount = 0;
			TInt colCount = 0;
			TInt w = 0;
			TInt h = 0;
			bool hasLabels;
			dbStats(filename, rowCount, w, h, colCount, hasLabels, be);
			if (rowCount == 0 || colCount == 0 || w == 0 || h == 0)
				throw Error(ecNotFound, f, SS("Database contains no data"));

			//Open database
			switch (be) {
			case beLMDB: dat = GetDB(DataParameter_DB_LMDB); break;
			case beLevelDB: dat = GetDB(DataParameter_DB_LEVELDB); break;
			default: throw Error(ecIncompatible, f, "Unknown database backend");
			}
			dat->Open(filename, READ);
			cur = dat->NewCursor();
			cur->SeekToFirst();

			//Resize Matrices
			if (matrices.rows.count() == 0) {
				matrices.resize(rowCount, colCount);
				if (hasLabels) labels.resize(rowCount, 1);
			}
			else if (matrices.rows.count() != rowCount || matrices.cols.count() != colCount)
				throw Error(ecIncompatible, f, SS("Invalid size for Matrix: " << matrices.rows.count() << "x" << matrices.cols.count() << " != " << rowCount << "x" << colCount));
			if (!hasLabels && labels.vec().count() > 0)
				throw Error(ecIncompatible, f, "LMDB contains no labels, but label Matrix is initialized");

			auto row_it = matrices.rows.begin();
			auto lab_it = labels.vec().begin();
			TInt idx = 0;
			while (cur->valid()) {

				//Convert from LMDB to Datum and then to TImg
				sample.ParseFromString(cur->value());
				const google::protobuf::RepeatedField<Float> & fdata = sample.float_data();
				auto p = fdata.begin();
				TInt chan = 0;
				for (auto cell = row_it->begin(); cell != row_it->end(); cell++, chan++) {

					//Alloc image
					cell->setOrder(oRowMajor);
					cell->resize(h, w);

					//Copy data from Datum
					for (auto it = cell->rows->begin(); it != cell->rows->end(); it++, p++)
						*it = *p;
				}
				if (hasLabels) {
					*lab_it = sample.label();
					lab_it++;
				}
				row_it++;
				cur->Next();
				idx++;
				progressCallback(0, rowCount, idx);
			}
			freeAndNull(&cur);
			freeAndNull(&dat);
			progressCallback(0, rowCount, rowCount);
		}
		catch (const exception &e) {
			freeAndNull(&cur);
			freeAndNull(&dat);
			throw Error(ecGeneral, f, e.what());
		}
	}

	template <typename T, typename TInt>
	inline void writeDB(DSLib::Matrix<T, TInt> &mat, DSLib::Matrix<T, TInt> &labels, const DSTypes::String &filename, const ProgressCallback &progressCallback, const DBMode mode, const DSTypes::Backend be) {
		const String f = "writeDB";
		if (mat.rows.count() == 0)
			return;
		bool hasLabels = (labels.vec().count() != 0);
		DB * dat = nullptr;
		Transaction * trans = nullptr;
		dat = openDB(filename, mode, be);
		trans = dat->NewTransaction();
		try {			
			//Initialize datum
			Datum sample;
			sample.set_channels(mat.cols.count());
			sample.set_width(1);
			sample.set_height(1);

			TInt rowIdx = 0;
			TInt bytes = 0;
			TInt min = 0;
			TInt max = mat.rows.count();
			TInt p = 0;
			for (auto r = mat.rows.begin(); r != mat.rows.end(); r++, rowIdx++, p++) {
				//Assign Row to Datum
				sample.clear_data();
				sample.clear_float_data();
				for (auto it = r->begin(); it != r->end(); it++) {
					sample.add_float_data((Float)*it);
				}
				if (hasLabels) {
					sample.set_label((google::protobuf::int32)labels.vec(rowIdx));
				}				
				//Write to Transaction
				string out;
				sample.SerializeToString(&out);
				trans->Put(SS(rowIdx), out);
				bytes += mat.cols.count() * sizeof(Float);
				if (bytes > CHUNK_BYTES) {
					trans->Commit();
					delete trans;
					trans = dat->NewTransaction();
					bytes = 0;
				}
				progressCallback(min, max, p);
			}
			if (bytes > 0)
				trans->Commit();
			freeAndNull(&dat);
			freeAndNull(&trans);
			progressCallback(min, max, max);
		}
		catch (const exception &e) {
			freeAndNull(&dat);
			freeAndNull(&trans);
			throw Error(ecGeneral, f, e.what());
		}
	}

	template <typename T, typename TInt>
	inline void readDB(DSLib::Matrix<T, TInt> &mat, DSLib::Matrix<T, TInt> &labels, const DSTypes::String &filename, const ProgressCallback &progressCallback, const DSTypes::Backend be) {
		const String f = "readDB";
		DB * dat = nullptr;
		Cursor * cur = nullptr;
		try {
			//Get database content
			Datum sample;
			TInt rowCount = 0;
			TInt colCount = 0;
			TInt w = 0;
			TInt h = 0;
			TInt c = 0;
			bool hasLabels;
			dbStats(filename, rowCount, w, h, c, hasLabels, be);
			if (rowCount == 0 || c == 0 || w == 0 || h == 0)
				throw Error(ecNotFound, f, SS("Database contains no data"));
			colCount = w * h * c;
			
			//Open database
			switch (be) {
			case beLMDB: dat = GetDB(DataParameter_DB_LMDB); break;
			case beLevelDB: dat = GetDB(DataParameter_DB_LEVELDB); break;
			default: throw Error(ecIncompatible, f, "Unknown database backend");
			}
			dat->Open(filename, READ);
			cur = dat->NewCursor();
			cur->SeekToFirst();

			//Resize Matrices
			if (mat.rows.count() == 0) {
				mat.resize(rowCount, colCount);
				if (hasLabels) labels.resize(rowCount, 1);
			} else if (mat.rows.count() != rowCount || mat.cols.count() != colCount)
				throw Error(ecIncompatible, f, SS("Invalid size for Matrix: " << mat.rows.count() << "x" << mat.cols.count() << " != " << rowCount << "x" << colCount));
			if (!hasLabels && labels.vec().count() > 0)
				throw Error(ecIncompatible, f, "LMDB contains no labels, but label Matrix is initialized");

			auto row_it = mat.rows.begin();
			auto lab_it = labels.vec().begin();
			TInt idx = 0;
			while (cur->valid()) {

				//Convert from LMDB to Datum and then to TImg
				sample.ParseFromString(cur->value());
				const google::protobuf::RepeatedField<Float> & fdata = sample.float_data();
				auto p = fdata.begin();
				for (auto cell = row_it->begin(); cell != row_it->end(); cell++, p++) {
					*cell = *p;
				}
				if (hasLabels) {
					*lab_it = sample.label();
					lab_it++;					
				}
				row_it++;
				cur->Next();
				idx++;
				progressCallback(0, rowCount, idx);
			}
			freeAndNull(&cur);
			freeAndNull(&dat);
			progressCallback(0, rowCount, rowCount);
		} catch (const exception &e) {
			freeAndNull(&cur);
			freeAndNull(&dat);
			throw Error(ecGeneral, f, e.what());
		}
	}
}

namespace DSModel {
		/********************/
		/** Class ExportDB **/
		/********************/
		template<typename TIdx, typename TId>
		ExportDB<TIdx, TId>::ExportDB(const String &filename, const Backend be) {			
			Table<TIdx, TId> tab =	(
										(Matrix<Literal, TIdx>() | "Val") | (										
											((Matrix<Literal, TIdx>() | "Filename") ^ (ctParameter | (dtString | filename))) |
											((Matrix<Literal, TIdx>() | "Backend") ^ (ctParameter | (dtString | (String)BackendStr[be])))
										)
									);
			this->registerParameters(tab);
		};

		template<typename TIdx, typename TId>
		ExportDB<TIdx, TId>::ExportDB(const DSTypes::String &filename) {
			std::ifstream ifs(filename, ios::binary);
			if (!ifs)
				throw Error(ecNotFound, "ExportDB::ExportDB()", SS("File not found: " << filename));
			this->readParams(ifs);
			this->readModel(ifs, filename);
		}

		template<typename TIdx, typename TId>
		void ExportDB<TIdx, TId>::clone(const ExportDB<TIdx, TId> & other) {
			Model<TIdx, TId>::clone(other);
		}

		template<typename TIdx, typename TId>
		void ExportDB<TIdx, TId>::clone(ExportDB<TIdx, TId> && other) {
			Model<TIdx, TId>::clone(std::move(other));
		}

		template<typename TIdx, typename TId>
		void ExportDB<TIdx, TId>::registerInputs(const Table<TIdx, TId> &table) {
			this->registerInput(dtFloat, ctFeature);
			this->registerInput(dtImagePNGFloat, ctFeature);
			this->registerInput(dtMatrixFloat, ctFeature);
			this->registerInput(dtDouble, ctFeature);
			this->registerInput(dtImagePNGDouble, ctFeature);
			this->registerInput(dtMatrixDouble, ctFeature);
			
			this->registerInput(dtFloat, ctTarget);
			this->registerInput(dtDouble, ctTarget);
			this->registerInput(dtString, ctTarget);
			this->registerInput(dtUInt32, ctTarget);
		}

		template<typename TIdx, typename TId>
		void ExportDB<TIdx, TId>::registerOutputs(const Table<TIdx, TId> &input) {
			this->addStages(dtString | "Export" | "Append");
		}

		template<typename TIdx, typename TId>
		void ExportDB<TIdx, TId>::getData_(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Matrix<Float, TIdx> &outputFloat, Matrix<Matrix<Float, TIdx>, TIdx> &outputMatrixFloat, Matrix<ImagePNG<Float, TIdx>, TIdx> &outputImagePNGFloat, Matrix<Float, TIdx> &outputTargetFloat) {
			//Get and convert input types
			if (~input(dtFloat, ctFeature) != 0) {
				outputFloat = input(dtFloat, ctFeature);
			} else if (~input(dtDouble, ctFeature) != 0) {
				Matrix<Double, TIdx> m = input(dtDouble, ctFeature);
				convert(outputFloat, m);
			} else if (~input(dtImagePNGFloat, ctFeature) != 0) {
				outputImagePNGFloat = input(dtImagePNGFloat, ctFeature);
			} else if (~input(dtImagePNGDouble, ctFeature) != 0) {
				Matrix<ImagePNG<Double, TIdx>, TIdx> m = input(dtImagePNGDouble, ctFeature);
				convert(outputImagePNGFloat, m);
			} else if (~input(dtMatrixFloat, ctFeature) != 0) {
				Matrix<Matrix<Float, TIdx>, TIdx> m = input(dtMatrixFloat, ctFeature);
				outputMatrixFloat = m;
			} else if (~input(dtMatrixDouble, ctFeature) != 0) {
				Matrix<Matrix<Double, TIdx>, TIdx> m = input(dtMatrixDouble, ctFeature);
				convert(outputMatrixFloat, m);
			} else {
				throw Error(ecNotFound, "ExportDB::getData_", "No compatible input features found");
			}

			//Get and convert targets
			if (~input(dtFloat, ctTarget) != 0) {
				outputTargetFloat = input(dtFloat, ctTarget);
			} else if (~input(dtDouble, ctTarget) != 0) {
				Matrix<Double, TIdx> m = input(dtDouble, ctTarget);
				convert(outputTargetFloat, m);
			} else if (~input(dtUInt32, ctTarget) != 0) {
				Matrix<UInt32, TIdx> m = input(dtUInt32, ctTarget);
				convert(outputTargetFloat, m);
			} else if (~input(dtString, ctTarget) != 0) {
				//cout << "Hint: Possible problem with ordering if not all possible strings are present! Use with caution" << endl;
				Matrix<String, TIdx> classes = uniqueValues<String, TIdx>(input(dtString, ctTarget));
				Matrix<TIdx, TIdx> m = indexLUT<String, TIdx>(input(dtString, ctTarget), classes);
				convert(outputTargetFloat, m);
			} else 
				throw Error(ecNotFound, "ExportDB::getData_", "No compatible targets found");
		}

		template<typename TIdx, typename TId>
		void ExportDB<TIdx, TId>::train(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
			this->setStage("Export");

			Backend be = (this->template parameterValueById<String>("Backend") == "LMDB")?beLMDB:beLevelDB;
			String fn = this->template parameterValueById<String>("Filename");

			//Get and convert inputs
			Matrix<Float, TIdx> outputFloat;
			Matrix<Matrix<Float, TIdx>, TIdx> outputMatrixFloat;
			Matrix<ImagePNG<Float, TIdx>, TIdx> outputImagePNGFloat;
			Matrix<Float, TIdx> outputTargetFloat;
			getData_(table, input, outputFloat, outputMatrixFloat, outputImagePNGFloat, outputTargetFloat);
			
			ProgressBar &p = this->progress();
			ProgressCallback pcb = [&p](Double mn, Double mx, Double pr){p.progress(mn, mx, pr);};

			//Write DB
			if (~outputFloat != 0) {
				writeDB(outputFloat, outputTargetFloat, fn, pcb, dbmNew, be);
			} else if (~outputMatrixFloat != 0) {
				writeDB(outputMatrixFloat, outputTargetFloat, fn, pcb, dbmNew, be);
			} else if (~outputImagePNGFloat != 0) {
				writeImageDB(outputImagePNGFloat, outputTargetFloat, fn, pcb, dbmNew, be);
			}
		}

		template<typename TIdx, typename TId>
		void ExportDB<TIdx, TId>::apply(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
			this->setStage("Append");

			Backend be = (this->template parameterValueById<String>("Backend") == "LMDB")?beLMDB:beLevelDB;
			String fn = this->template parameterValueById<String>("Filename");

			//Get and convert inputs
			Matrix<Float, TIdx> outputFloat;
			Matrix<Matrix<Float, TIdx>, TIdx> outputMatrixFloat;
			Matrix<ImagePNG<Float, TIdx>, TIdx> outputImagePNGFloat;
			Matrix<Float, TIdx> outputTargetFloat;
			getData_(table, input, outputFloat, outputMatrixFloat, outputImagePNGFloat, outputTargetFloat);
			
			ProgressBar &p = this->progress();
			ProgressCallback pcb = [&p](Double mn, Double mx, Double pr){p.progress(mn, mx, pr);};

			//Write DB
			if (~outputFloat != 0) {
				writeDB(outputFloat, outputTargetFloat, fn, pcb, dbmAppendOrNew, be);
			} else if (~outputMatrixFloat != 0) {
				writeDB(outputMatrixFloat, outputTargetFloat, fn, pcb, dbmAppendOrNew, be);
			} else if (~outputImagePNGFloat != 0) {
				writeImageDB(outputImagePNGFloat, outputTargetFloat, fn, pcb, dbmAppendOrNew, be);
			}
		}

		template<typename TIdx, typename TId>
		void ExportDB<TIdx, TId>::resetModel() {
		}

		template<typename TIdx, typename TId>
		void ExportDB<TIdx, TId>::readModel(std::istream &input, const DSTypes::String &filename) {
			this->updateParameters();
		}

		template<typename TIdx, typename TId>
		void ExportDB<TIdx, TId>::writeModel(std::ostream &output, const DSTypes::String &filename) {
		}

		template<typename TIdx, typename TId> M_CPP_MODEL_DESTRUCTOR_DEFAULT(ExportDB, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_DEFAULT(ExportDB, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_COPY(ExportDB, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MOVE(ExportDB, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_COPY(ExportDB, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_MOVE(ExportDB, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_COPY(ExportDB, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_MOVE(ExportDB, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY(ExportDB, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE(ExportDB, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY_2(ExportDB, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE_2(ExportDB, TIdx, TId)

		template<typename TIdx, typename TId> M_CCP_MODEL_OPERATOR_SLICE(ExportDB, TIdx, TId)


	//*****************//
	//** Class Caffe **//
	//*****************//

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::updateParameters() {
		bool reload = false;

		//check changes
		if (this->template parameterValueById<Matrix<TClassType>>("Classes").vec().count() != classes_.vec().count() ||
			this->template parameterValueById<String>("NetProtoFile") != netProtoFile_ ||
			this->template parameterValueById<String>("SolverProtoFile") != solverProtoFile_) {
			reload = true;
		}

		classes_ = this->template parameterValueById<Matrix<TClassType>>("Classes");
		netProtoFile_ = this->template parameterValueById<String>("NetProtoFile");
		solverProtoFile_ = this->template parameterValueById<String>("SolverProtoFile");

		classToNum_.clear();
		Float i = 0;
		for (auto c = classes_.vec().begin(); c != classes_.vec().end(); c++, i++)
			classToNum_.insert(pair<TClassType, Float>(*c, i));
		if (reload)
			updateCaffeModel(false);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::freeNet_(Net<Float>** net) {
		if (*net != nullptr)
			delete *net;
		*net = nullptr;
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::freeSolver_(Solver<Float>** solver) {
		if (*solver != nullptr)
			delete *solver;
		*solver = nullptr;
	}

	template<typename TClassType, typename TIdx, typename TId>
	Solver<Float> * Caffe<TClassType, TIdx, TId>::readSolver_(const String &filename, const String snapshot) {
		SolverParameter sp;
		String proto = DSUtil::readToString(filename);
		if (!google::protobuf::TextFormat::ParseFromString(proto, &sp))
			throw Error(ecGeneral, "Caffe::readSolver_()", "Invalid description for solver prototxt");		
		Solver<Float> * s = caffe::SolverRegistry<Float>::CreateSolver(sp);
		if (snapshot != "") s->Restore(snapshot.c_str());
		return (s);
	}

	template<typename TClassType, typename TIdx, typename TId>
	Net<Float> * Caffe<TClassType, TIdx, TId>::readNet_(const String &filename, const String snapshot) {
		NetParameter np;
		String proto = DSUtil::readToString(filename);
		if (!google::protobuf::TextFormat::ParseFromString(proto, &np))
			throw Error(ecGeneral, "Caffe::readNet_()", "Invalid description for net prototxt");
		np.mutable_state()->set_phase(TEST);
		Net<Float> * n = new Net<Float>(np);
		return (n);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::shareWeights_(Net<Float> &netFrom, Net<Float> &netTo) {
		netTo.ShareTrainedLayersWith(&netFrom);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::copyWeights_(const Net<Float> &netFrom, Net<Float> &netTo) {
		NetParameter np;
		netFrom.ToProto(&np);
		netTo.CopyTrainedLayersFrom(np);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::loadCaffeModel_(Net<Float> **net, Solver<Float> **solver) {
		freeNet_(net);
		freeSolver_(solver);	
		if (solverProtoFile_ != "") {
			*solver = readSolver_(solverProtoFile_, snapshotModelFile_);
			*net = readNet_(netProtoFile_);
			shareWeights_(*((*solver)->net().get()), **net);
		} else if (netProtoFile_ != "") {
			*net = readNet_(netProtoFile_, snapshotModelFile_);
		} else 
			throw Error(ecIncompatible, "loadCaffeModel", "Not solver and net filenames known");
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::readModel(std::istream &input, const DSTypes::String &filename) {
		updateParameters();
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::writeModel(std::ostream &input, const DSTypes::String &filename) {		
		if (solver_ == nullptr) 
			throw Error(ecNotFound, "Caffe::writeModel()", "No solver present");

/*		string solvertxt = filename + ".solver.prototxt";

		//Set current path
		using namespace boost::filesystem;			
		path newPath(filename);
		newPath.remove_filename();
		path oldPath = boost::filesystem::current_path();
		boost::system::error_code ec;
		if (newPath != "") current_path(newPath, ec);		
		if (ec) throw Error(ecGeneral, "CaffeMLP::writeSolverToFile_()", ec.message());

		//Save solver to text file for deployment
		ofstream ofs(solvertxt);
		if (!ofs) throw Error(ecNotFound, "CaffeMLP::writeSolverToFile_()", SS("Cannot open solver file: " << solvertxt));
		ofs << solverProto_;

		//Save solver state
		solver_->Snapshot();
		
		//Restore path
		current_path(oldPath, ec);
		if (ec) throw Error(ecGeneral, "CaffeMLP::writeSolverToFile_()", ec.message());
		*/
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::registerInputs(const Table<TIdx, TId> &table) {		
		this->registerInput(dtContentType | ctFeature | ctTarget);
		this->addStages(dtString | "Copy" | "Train" | "Apply");
		if (net_ == nullptr)
			throw Error(ecIncompatible, "Caffe::registerInputs()", "Not solver or net loaded");
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::registerOutputBlob_(Table<TIdx, TId> &output, const ContentType ct, DataType dt, TIdx blobWidth, TIdx blobHeight, TIdx blobChannels) {
		if (ct == ctTarget && dt == dataType<TClassType>()) {
			output | (ct | Matrix<TClassType, TIdx>(1, 1));
		} else {
			switch (dt) {
				case dtFloat: output |			(ct | Matrix<Float, TIdx>(1, blobWidth * blobHeight * blobChannels));break;
				case dtDouble: output |			(ct | Matrix<Double, TIdx>(1, blobWidth * blobHeight * blobChannels));break;
   				case dtUInt32: output |			(ct | Matrix<UInt32, TIdx>(1, blobWidth * blobHeight * blobChannels));break;
   				case dtImagePNGFloat: output |	(ct | Matrix<ImagePNG<Float, TIdx>, TIdx>(1, blobChannels));break;
   				case dtImagePNGDouble: output | (ct | Matrix<ImagePNG<Double, TIdx>, TIdx>(1, blobChannels));break;
   				case dtMatrixFloat: output |	(ct | Matrix<Matrix<Float, TIdx>, TIdx>(1, blobChannels));break;
				case dtMatrixDouble: output |	(ct | Matrix<Matrix<Double, TIdx>, TIdx>(1, blobChannels));break;
				default: throw Error(ecIncompatible, "registerOutputBlob", SS("Unsupported DataType: " << etos(dt)));
			}
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::registerOutputs(const Table<TIdx, TId> &input) {
		try {
			Table<> output;
			const String outputName = "ddslOutput";
			vector<String> names = net_->blob_names();
			for (auto blobName=names.begin();blobName!=names.end();blobName++) {
				if (blobName->substr(0, outputName.size()) == outputName) {
					boost::shared_ptr<Blob<Float>> outputBlob = net_->blob_by_name(*blobName);
					TIdx bw = outputBlob->width();
					TIdx bh = outputBlob->height();
					TIdx bc = outputBlob->channels();
					TIdx bw2, bh2, bc2;
					ContentType ct, ct2;
					DataType dt, dt2;
					parseName_(*blobName, ct, dt);
					if (ct == ctResult && dt == dataType<TClassType>()) {
						registerOutputBlob_(output, ct, dt, bw, bh, 1);
						parseSecondaryName_(*blobName, ct2, dt2, bw2, bh2, bc2);
						if (ct2 != ctUnknown) registerOutputBlob_(output, ct2, dt2, bw2, bh2, 1);
					} else {
						registerOutputBlob_(output, ct, dt, bw, bh, bc);
						parseSecondaryName_(*blobName, ct2, dt2, bw2, bh2, bc2);
						if (ct2 != ctUnknown) registerOutputBlob_(output, ct2, dt2, bw2, bh2, bc2);
					}
				}
			}
			output.resizeRows(0);
			this->registerOutput(output);
		} catch (Error &e) {
			throw Error(ecExternalLibrary, "registerOutputs",  e.what());
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::loadCaffeModel() {
		loadCaffeModel_(&net_, &solver_);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::clearCaffeModel() {
		freeNet_(&net_);
		freeSolver_(&solver_);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::updateCaffeModel(bool copyWeights) {		
		caffe::Solver<Float> * tempSolver = nullptr;
		caffe::Net<Float> * tempNet = nullptr;
		try {
			loadCaffeModel_(&tempNet, &tempSolver);
			if (copyWeights) {
				if (tempSolver != nullptr)
					copyWeights_(*(solver_->net().get()), *(tempSolver->net().get()));
				else
					copyWeights_(*net_, *tempNet);
			}
			clearCaffeModel();
			net_ = tempNet;
			solver_ = tempSolver;
			tempNet = nullptr;
			tempSolver = nullptr;
			return;
		} catch (...) {			
			freeNet_(&tempNet);
			freeSolver_(&tempSolver);
			throw;
		}
	}
	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::init_() {
		//Should only be called from the constructor, otherwise call clearCaffeModel_() first
		solver_ = nullptr;
		net_ = nullptr;
		gpuDevices_ = 0;
		solverProtoFile_ = "";
		netProtoFile_ = "";
		snapshotModelFile_ = "";
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::resetModel() {
		clearCaffeModel();
		loadCaffeModel();
	}

	template<typename TClassType, typename TIdx, typename TId>
	Caffe<TClassType, TIdx, TId>::Caffe(const DSTypes::String &filename, TIdx gpuDevices, const DSTypes::String snapshotModelFile) {
		init_();
		gpuDevices_ = gpuDevices;
		//this->readModel(filename, loadSolver, loadNet);
	}

	/*template<typename TClassType, typename TIdx, typename TId>
	Caffe<TClassType, TIdx, TId>::Caffe(const DSTypes::String &filename, TIdx gpuDevices, const DSTypes::String snapshotModelFile) {
	init_();
	gpuDevices_ = gpuDevices;
	ifstream ifs(filename, ios::binary);
	if (!ifs)
	throw Error(ecNotFound ,"Caffe::Caffe()", SS("File not found " << filename));
	readParams(ifs);
	loadCaffeModel();
	}*/

	template<typename TClassType, typename TIdx, typename TId>
	Caffe<TClassType, TIdx, TId>::Caffe() {
		init_();
	}	

	template<typename TClassType, typename TIdx, typename TId>
	Caffe<TClassType, TIdx, TId>::Caffe(const Caffe<TClassType, TIdx, TId> & other) {
		init_();
		clone(other);
	}

	template<typename TClassType, typename TIdx, typename TId>	
	Caffe<TClassType, TIdx, TId>::Caffe(Caffe<TClassType, TIdx, TId> && other) {
		init_();
		clone(std::move(other));
	}

	template<typename TClassType, typename TIdx, typename TId>
	Caffe<TClassType, TIdx, TId>::Caffe(const Model<TIdx, TId>& other) {
		init_();
		try {
			clone(dynamic_cast<const Caffe<TClassType, TIdx, TId> &>(other));
		} catch (exception &e) {
			throw Error(ecGeneral, "In copy constructor with Model<>", e.what());
		}
	}
	
	template<typename TClassType, typename TIdx, typename TId>
	Caffe<TClassType, TIdx, TId>::Caffe(Model<TIdx, TId>&& other) {
		init_();
		try {
			clone(std::move(dynamic_cast<Caffe<TClassType, TIdx, TId> &>(other)));
		} catch (exception &e) {
			throw Error(ecGeneral, "In move constructor with Model<>", e.what());
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	Caffe<TClassType, TIdx, TId>::Caffe(const DSLib::Matrix<TClassType, TIdx> &classes, const DSTypes::String netProtoFile, const DSTypes::String solverProtoFile, TIdx gpuDevices) {
		init_();		
		gpuDevices_ = gpuDevices;
		Table<TIdx, TId> tab =
			(Matrix<Literal, TIdx>() | "Val") |
			(
			(((Matrix<Literal, TIdx>() | "Classes")			^ (ctParameter | (Matrix<Matrix<TClassType>>() | (Matrix<TClassType>() | classes))))) |
			(((Matrix<Literal, TIdx>() | "NetProtoFile")		^ (ctParameter | (Matrix<String>() | netProtoFile)))) |
			(((Matrix<Literal, TIdx>() | "SolverProtoFile")	^ (ctParameter | (Matrix<String>() | solverProtoFile))))
			);
		this->registerParameters(tab);
	}
	
	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::getBlobSize_(const DSTypes::String &name, TIdx &bw, TIdx &bh, TIdx &bc) {
		bw = (getActiveNet_()->blob_by_name(name)->width());
		bh = (getActiveNet_()->blob_by_name(name)->height());
		bc = (getActiveNet_()->blob_by_name(name)->channels());
	}

	template<typename TClassType, typename TIdx, typename TId>
	template<typename T> 
	void Caffe<TClassType, TIdx, TId>::fitToBlob_(const DSTypes::String &blobName, Matrix<T, TIdx> &mat) {
		TIdx bw, bh, bc;
		getBlobSize_(blobName, bw, bh, bc);
		mat.resize(batchSize_, bw * bh * bc);
	}

	template<typename TClassType, typename TIdx, typename TId>
	template<typename T> 
	void Caffe<TClassType, TIdx, TId>::fitToBlob_(const DSTypes::String &blobName, Matrix<Matrix<T>, TIdx> &mat) {
		TIdx bw, bh, bc;
		getBlobSize_(blobName, bw, bh, bc);
		mat.resize(batchSize_, bc);
	}

	template<typename TClassType, typename TIdx, typename TId>
	template<typename T> 
	void Caffe<TClassType, TIdx, TId>::fitToBlob_(const DSTypes::String &blobName, Matrix<ImagePNG<T>, TIdx> &mat) {
		TIdx bw, bh, bc;
		getBlobSize_(blobName, bw, bh, bc);
		mat.resize(batchSize_, bc);
	}


	template<typename TClassType, typename TIdx, typename TId>
	template<typename T> 
	void Caffe<TClassType, TIdx, TId>::getBlobData(const String &blobName, Matrix<T, TIdx> &out) {
		if (getActiveNet_()->blob_by_name(blobName)->count() == 1) {
			out.resize(1,1);
			out.val(0,0) = *(getActiveNet_()->blob_by_name(blobName)->cpu_data());
			return;
		}
		fitToBlob_(blobName, out);
		getBlobData_(blobName, out);
	}

	template<typename TClassType, typename TIdx, typename TId>
	template<typename T> 
	void Caffe<TClassType, TIdx, TId>::getBlobData(const String &blobName, Matrix<ImagePNG<T, TIdx>, TIdx> &out) {
		fitToBlob_(blobName, out);
		getBlobData_(blobName, out);
	}

	template<typename TClassType, typename TIdx, typename TId>
	template<typename T> 
	void Caffe<TClassType, TIdx, TId>::getBlobData(const String &blobName, Matrix<Matrix<T, TIdx>, TIdx> &out) {
		fitToBlob_(blobName, out);
		getBlobData_(blobName, out);
	}

	template<typename TClassType, typename TIdx, typename TId>
	template<typename T> 
	void Caffe<TClassType, TIdx, TId>::getLayerBlob(const String &layerName, const TIdx blobIdx, Matrix<T, TIdx> &mat) {
		if (blobIdx >= getLayerBlobCount(layerName))
			throw Error(ecRangeError, "getLayerBlob", SS("Blob index " << blobIdx << " out of range."));
		boost::shared_ptr<Blob<Float>> blob = getActiveNet_()->layer_by_name(layerName)->blobs()[blobIdx];
		TIdx rowcnt=1;
		if (blob->shape().size() == 0) {
			mat.resize(0,0);
			return;
		} else if (blob->shape().size() > 1){
			for (auto i = blob->shape().begin()+1; i!=blob->shape().end();i++)
				rowcnt *= *i;
		} 
		Matrix<Float, TIdx> matFloat(rowcnt, blob->shape()[0]);
		auto elm = matFloat.rows->begin();
		Float * f = blob->mutable_cpu_data();
		for (auto elm=matFloat.rows->begin(); elm!=matFloat.rows->end();elm++,f++)
			*elm = *f;
		mat.resize(matFloat.rows.count(), matFloat.cols.count());
		convert(mat, matFloat);
	}

	template<typename TClassType, typename TIdx, typename TId>
	TIdx Caffe<TClassType, TIdx, TId>::getLayerBlobCount(const String &layerName) {
		return getActiveNet_()->layer_by_name(layerName)->blobs().size();
	}


	template<typename TClassType, typename TIdx, typename TId>
	template<typename T> 
	void Caffe<TClassType, TIdx, TId>::getInnerProductData(const String &layerName, Matrix<T, TIdx> &weights, Matrix<T, TIdx> &bias) {
		getLayerBlob(layerName, 0, weights);
		getLayerBlob(layerName, 1, bias);
	}

	template<typename TClassType, typename TIdx, typename TId>
	template<typename T> void Caffe<TClassType, TIdx, TId>::getConvolutionData(const String &layerName, Matrix<T, TIdx> &out) {
		//TODO
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::parseName_(const std::string &name, ContentType &ct, DataType &dt) {
		Matrix<String> s = split(name, '.', ' ');
		if (~s < 3)
			throw Error(ecIncompatible, "parseName", "Cannot parse name: " + name);
		ct = stringToContentType(s.vec(1));
		dt = stringToDataType(s.vec(2));
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::parseSecondaryName_(const std::string &name, ContentType &ct, DataType &dt, TIdx &width, TIdx &height, TIdx &channels) {
		Matrix<String> s = split(name, '.', ' ');
		if (~s < 8) {
			ct = ctUnknown;
			dt = dtUnknown;
			return;
		}			
		ct = stringToContentType(s.vec(3));
		dt = stringToDataType(s.vec(4));
		width = atoi(s.vec(5).c_str());
		height = atoi(s.vec(6).c_str());
		channels = atoi(s.vec(7).c_str());
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::parseSecondaryName_(const std::string &name, ContentType &ct, DataType &dt) {
		Matrix<String> s = split(name, '.', ' ');
		if (~s < 5) {
			ct = ctUnknown;
			dt = dtUnknown;
			return;
		}			
		ct = stringToContentType(s.vec(3));
		dt = stringToDataType(s.vec(4));
	}

	template<typename TClassType, typename TIdx, typename TId>
	template<typename T>
	void Caffe<TClassType, TIdx, TId>::addBlobData_(const String &blobName, Matrix<T, TIdx> &in) {
		TIdx bw = (getActiveNet_()->blob_by_name(blobName)->width());
		TIdx bh = (getActiveNet_()->blob_by_name(blobName)->height());
		TIdx bc = (getActiveNet_()->blob_by_name(blobName)->channels());
		blobWidth_ | bw;
		blobHeight_ | bh;
		blobChannels_ | bc;
		blobName_ | blobName;
		Matrix<Float> rowData(oRowMajor);
		convert(rowData, in);
		if (rowData.cols.count() != bw * bh) //Allow 1d to 2d coercing
			throw Error(ecIncompatible, "addBlobData_", SS("Blob size mismatch between Caffe and DDSL: " << bw << "x" << bh << " != " << rowData.cols.count()));
		blobData_ | rowData;
	}

	template<typename TClassType, typename TIdx, typename TId>
	template<typename T>
	void Caffe<TClassType, TIdx, TId>::addBlobData_(const String &blobName, Matrix<ImagePNG<T>, TIdx> &in) {
		//Check and init blob width, height and channels
		TIdx bc = 0;
		TIdx bw = 0;
		TIdx bh = 0;

		for (auto it=in.rows[0]->begin(); it!=in.rows[0]->end(); it++) {
			bc += it->getChannelCount();
			Matrix<T, TIdx> img = it->getImage();
			TIdx imageWidth = img.cols.count() / it->getChannelCount();
			if (bh == 0) bh = img.rows.count();
			if (bw == 0) bw = imageWidth;
			if (imageWidth != bw || img.rows.count() != bh)
				throw Error(ecIncompatible, "addBlobData_", SS("Hyperspectral input images should all have the same size: Current is " << bh << " x " << bw << ". Requested is " << img.rows.count() << " x " << img.rows.count())); 
		}
		
		//Check with Caffe blob
		if (bw != (TIdx)net_->blob_by_name(blobName)->width() || bh != (TIdx)net_->blob_by_name(blobName)->height() || bc != (TIdx)net_->blob_by_name(blobName)->channels())
			throw Error(ecIncompatible, "addBlobData_", SS("Blob size mismatch between Caffe and DDSL:  " << net_->blob_by_name(blobName)->width() << "x" << net_->blob_by_name(blobName)->height() << "x" << net_->blob_by_name(blobName)->channels() << " != "  << bw << "x" << bh << "x" << bc));

		blobWidth_ | bw;
		blobHeight_ | bh;
		blobChannels_ | bc;
		blobName_ | blobName;

		//Copy image data to blob
		TIdx y = 0;
		this->setMinProgress(0);
		this->setMaxProgress(in.rows.count());
		Matrix<Float, TIdx> rowData(bh * bc * in.rows.count(), bw, oRowMajor);
		for (auto r = in.rows.begin(); r!= in.rows.end(); r++) {
			for (auto c = r->begin(); c != r->end(); c++) {
				TIdx cc = c->getChannelCount();
				if (cc == 3)
					c->convertRGBToPlanarV();
				Matrix<Float, TIdx> slice;
				if (dataType<T>() == dataType<Float>())
					slice = rowData(y, c->getImage().rows.count(), 0, c->getImage().cols.count());
				else
					convert(slice, rowData(y, c->getImage().rows.count(), 0, c->getImage().cols.count()));
				if (slice.isSameSize(c->getImage()))
					slice = c->getImage();
				else
					throw Error(ecInternal, "addBlobData<ImagePNG<>>", "Cannot assign data. Direct slice assignment size mismatch");
				y += c->getImage().rows.count();
			}
			this->incProgress();
		}
		//ImagePNG<Float, TIdx> png(rowData);
		//png.saveImage("c:/temp/blob.png", itM8);

		blobData_ | rowData;
	}

	template<typename TClassType, typename TIdx, typename TId>
	template<typename T>
	void Caffe<TClassType, TIdx, TId>::addBlobData_(const String &blobName, Matrix<Matrix<T>, TIdx> &in) {
		//Check and init blob width, height and channels
		TIdx bc = 0;
		TIdx bw = 0;
		TIdx bh = 0;
		for (auto it=in.rows[0]->begin(); it!=in.rows[0]->end(); it++) {
			bc++;
			if (bh == 0) bh = it->rows.count();
			if (bw == 0) bw = it->cols.count();
			if (it->cols.count() != bw || it->rows.count() != bh)
				throw Error(ecIncompatible, "addblobData_", SS("Hyperspectral input matrices should all have the same size: Current is " << bh << " x " << bw << ". Requested is " << it->rows.count() << " x " << it->rows.count())); 
		}
		
		//Check with Caffe blob
		if (bw != net_->blob_by_name(blobName)->width() || bh != net_->blob_by_name(blobName)->height() || bc != net_->blob_by_name(blobName)->channels())
			throw Error(ecIncompatible, "addBlobData_", SS("Blob size mismatch between Caffe and DDSL:  " << net_->blob_by_name(blobName)->width() << "x" << net_->blob_by_name(blobName)->height() << "x" << net_->blob_by_name(blobName)->channels() << " != "  << bw << "x" << bh << "x" << bc));

		blobWidth_ | bw;
		blobHeight_ | bh;
		blobChannels_ | bc;
		blobName_ | blobName;

		//Copy image data to blob
		TIdx y = 0;
		this->setMinProgress(0);
		this->setMaxProgress(in.rows.count());
		Matrix<Float, TIdx> rowData(bh * bc * in.rows.count(), bw, oRowMajor);
		for (auto r = in.rows.begin(); r!= in.rows.end(); r++) {
			for (auto c = r->begin(); c != r->end(); c++) {
				Matrix<Float, TIdx> slice;
				if (dataType<T>() == dataType<Float>())
					slice = rowData(y, c->rows.count(), 0, c->cols.count());
				else
					convert(slice, rowData(y, c->rows.count(), 0, c->cols.count()));
				if (slice.isSameSize(*c))
					slice = *c;
				else
					throw Error(ecInternal, "addBlobData<Matrix<>>", "Cannot assign data. Direct slice assignment size mismatch");
				y += c->rows.count();
			}
			this->incProgress();
		}
		blobData_ | rowData;
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::addLabelBlobData_(const String &blobName, Matrix<TClassType, TIdx> &in) {
		TIdx bw = (getActiveNet_()->blob_by_name(blobName)->width());
		TIdx bh = (getActiveNet_()->blob_by_name(blobName)->height());
		TIdx bc = (getActiveNet_()->blob_by_name(blobName)->channels());
		blobWidth_ | bw;
		blobHeight_ | bh;
		blobChannels_ | bc;
		blobName_ | blobName;
		if (bw != 1 || bh != 1 || bc != 1)
			throw Error(ecIncompatible, "addBlobData_", SS("Blob size mismatch between Caffe and DDSL: " << bw << "x" << bh << "x" << bc << " != " << "1x1x1"));
		Matrix<Float> rowData(in.rows.count(), in.cols.count(), oRowMajor);
		auto dst_t = rowData.vec().begin();
		for (auto t = in.vec().begin(); t != in.vec().end(); t++, dst_t++)
			*dst_t = classToNum_.find(*t)->second;
		blobData_ | rowData;
	}

	template<typename TClassType, typename TIdx, typename TId>
	template<typename T>
	void Caffe<TClassType, TIdx, TId>::getBlobData_(const String &blobName, Matrix<T, TIdx> &out) {
		boost::shared_ptr<Blob<Float>> outputBlob = getActiveNet_()->blob_by_name(blobName);
		if (outputBlob->width() * outputBlob->height() * outputBlob->channels() != out.cols.count())
			throw Error(ecIncompatible, "getBlobData", SS("Blob data does not fit the output of: " << blobName << " (" << outputBlob->width() << "x" << outputBlob->height() << "x" << outputBlob->channels() << " != " << out.cols.count() << ")"));
		if (batchSize_ != out.rows.count())
			throw Error(ecIncompatible, "getBlobData", SS("Blob data batch size does not fit the output of: " << blobName << " (" << batchSize_ << " != " << out.rows.count() << ")"));
		const Float * begin = outputBlob->cpu_data();
		const Float * end = begin + (batchSize_ * outputBlob->width() * outputBlob->height() * outputBlob->channels());
		std::copy(begin, end, out.rows->begin());
	}

	template<typename TClassType, typename TIdx, typename TId>
	template<typename T>
	void Caffe<TClassType, TIdx, TId>::getBlobData_(const String &blobName, Matrix<ImagePNG<T, TIdx>, TIdx> &out) {
		boost::shared_ptr<Blob<Float>> outputBlob = getActiveNet_()->blob_by_name(blobName);
		if (outputBlob->channels() != out.cols.count())
			throw Error(ecIncompatible, "getBlobData<ImagePNG>", SS("Blob data does not fit the output of: " << blobName << "(" << outputBlob->channels() << " != " << out.cols.count() << ")"));
		if (batchSize_  != out.rows.count())
			throw Error(ecIncompatible, "getBlobData", SS("Blob data batch size does not fit the output of: " << blobName << " (" << batchSize_ << " != " << out.rows.count() << ")"));
		const Float * begin = outputBlob->cpu_data();
		const Float * end = begin + (batchSize_ * outputBlob->width() * outputBlob->height() * outputBlob->channels());
		vector<Float> blob(begin, end);
		auto p = blob.begin();
		for (TIdx b=0;b<batchSize_;b++) {
			for (TIdx c=0;c<out.cols.count();c++) {
				ImagePNG<T, TIdx> png(outputBlob->height(), outputBlob->width());
				std::copy(p, p + outputBlob->width() * outputBlob->height(), png.mat().rows->begin());
				out.val(b, c);
				p += outputBlob->width() * outputBlob->height();
			}
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	template<typename T>
	void Caffe<TClassType, TIdx, TId>::getBlobData_(const String &blobName, Matrix<Matrix<T, TIdx>, TIdx> &out) {
		boost::shared_ptr<Blob<Float>> outputBlob = getActiveNet_()->blob_by_name(blobName);
		if (outputBlob->channels() != out.cols.count())
			throw Error(ecIncompatible, "getBlobData<ImagePNG>", SS("Blob data does not fit the output of: " << blobName << "(" << outputBlob->channels() << " != " << out.cols.count() << ")"));
		if (batchSize_  != out.rows.count())
			throw Error(ecIncompatible, "getBlobData", SS("Blob data batch size does not fit the output of: " << blobName << " (" << batchSize_ << " != " << out.rows.count() << ")"));
		const Float * begin = outputBlob->cpu_data();
		const Float * end = begin + (batchSize_ * outputBlob->width() * outputBlob->height() * outputBlob->channels());
		vector<Float> blob(begin, end);
		auto p = blob.begin();
		for (TIdx b=0;b<batchSize_;b++) {
			for (TIdx c=0;c<out.cols.count();c++) {
				Matrix<T, TIdx> mat(outputBlob->height(), outputBlob->width());
				std::copy(p, p + outputBlob->width() * outputBlob->height(), mat.rows->begin());
				out.val(b, c);
				p += outputBlob->width() * outputBlob->height();
			}
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::getResultBlobData_(const String &blobName, Matrix<TClassType, TIdx> &result) {
		TIdx classCount = ~classes_;
		if (result.cols.count() != 1)
			throw Error(ecIncompatible, "getResultBlobData", SS("Wrong size for result Matrix: " << blobName << " (" << result.printSize() << ")"));
		
		boost::shared_ptr<Blob<Float>> outputBlob = getActiveNet_()->blob_by_name(blobName);
		if ((TIdx)outputBlob->width() * (TIdx)outputBlob->height() * (TIdx)outputBlob->channels() != classCount)
			throw Error(ecIncompatible, "getResultBlobData", SS("Caffe and DDSL class count mismatch for: " << blobName << " (" << outputBlob->width() * outputBlob->height() * outputBlob->channels() << " != " << classCount << ")"));

		const Float * begin = outputBlob->cpu_data();
		const Float * end = begin + (batchSize_ * classCount);
		//Iterate over all outputs in the batch
		const vector<Float> outputData(begin, end);
		auto out_it = result.vec().begin();
		for (TIdx s = 0; s < batchSize_; s++, out_it++) {
			//Get softmax output and calculate probability
			const vector<Float> sampleOutput(outputData.begin() + s * classCount, outputData.begin() + ((s + 1) * classCount));
			Float sum = 0;
			Float max = 0;
			TIdx maxIdx = 0;
			TIdx idx = 0;
			for (auto o = sampleOutput.begin(); o != sampleOutput.end(); o++, idx++) {
				if (*o >= max) {
					maxIdx = idx;
					max = *o;
				}
				sum += *o;
			}
			*out_it = classes_.vec(maxIdx);
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	template<typename T>
	void Caffe<TClassType, TIdx, TId>::getResultBlobData_(const String &blobName, Matrix<TClassType, TIdx> &result, Matrix<T, TIdx> &conf) {
		if (result.cols.count() != 1)
			throw Error(ecIncompatible, "getResultBlobData", SS("Wrong size for result Matrix: " << blobName << " (" << result.printSize() << ")"));
		if (conf.cols.count() != 1)
			throw Error(ecIncompatible, "getResultBlobData", SS("Wrong size for confidence Matrix: " << blobName << " (" << conf.printSize() << ")"));
		TIdx classCount = ~classes_;
		boost::shared_ptr<Blob<Float>> outputBlob = getActiveNet_()->blob_by_name(blobName);
		if (outputBlob->width() * outputBlob->height() * outputBlob->channels() != classCount)
			throw Error(ecIncompatible, "getResultBlobData", SS("Caffe and DDSL class count mismatch for: " << blobName << " (" << outputBlob->width() * outputBlob->height() << " != " << classCount << ")"));

		const Float * begin = outputBlob->cpu_data();
		const Float * end = begin + (batchSize_ * classCount);
		//Iterate over all outputs in the batch
		const vector<Float> outputData(begin, end);
		auto out_it = result.vec().begin();
		auto conf_it = conf.vec().begin();
		for (TIdx s = 0; s < batchSize_; s++, out_it++, conf_it++) {
			//Get softmax output and calculate probability
			const vector<Float> sampleOutput(outputData.begin() + s * classCount, outputData.begin() + ((s + 1) * classCount));
			Float sum = 0;
			Float max = 0;
			TIdx maxIdx = 0;
			TIdx idx = 0;
			for (auto o = sampleOutput.begin(); o != sampleOutput.end(); o++, idx++) {
				if (*o >= max) {
					maxIdx = idx;
					max = *o;
				}
				sum += *o;
			}
			*out_it = classes_.vec(maxIdx);
			*conf_it = (max / sum); //Probability
		}
	}

	//Dispatch for output data
	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::getLayerOutput_(Table<TIdx, TId> &output, const String &blobName, const ContentType ct, const DataType dt) {
		if (output.rows.count() != batchSize_)
			throw Error(ecInternal, "getLayerOutput_", SS("Batch size does not match the number of rows: " << batchSize_ << " != " << output.rows.count()));

		if (ct == ctResult && dt == dataType<TClassType>()) {
			Matrix<TClassType, TIdx> result = (Matrix<TClassType, TIdx>) this->checkOutput(output(ct, dt));
			ContentType confCT;
			DataType confDT;
			parseSecondaryName_(blobName, confCT, confDT);
			if (confCT != ctUnknown && confDT != dtUnknown) {
				if (confCT != ctConfidence)
					throw Error(ecIncompatible, "getLayerOutput", SS("Currently only secondary ContentType 'Confidence' supported in blob: " << blobName));
				switch (confDT) {
					case dtFloat: {
						Matrix<Float, TIdx> conf = (Matrix<Float, TIdx>) this->checkOutput(output(confCT, confDT));
						getResultBlobData_(blobName, result, conf);
					break;}
  					case dtDouble: {
						Matrix<Double, TIdx> conf = (Matrix<Double, TIdx>) this->checkOutput(output(confCT, confDT));
						getResultBlobData_(blobName, result, conf);
					break;}
				}
			}
			getResultBlobData_(blobName, result);
		} else {
			switch (dt) {
				case dtFloat: {
					Matrix<Float, TIdx> out = (Matrix<Float, TIdx>) this->checkOutput(output(ct, dt));
					getBlobData_(blobName, out);
				break;}
				case dtDouble: {
					Matrix<Double, TIdx> out = (Matrix<Double, TIdx>) this->checkOutput(output(ct, dt));
					getBlobData_(blobName, out);
				break;}
   				case dtUInt32: {
					Matrix<UInt32, TIdx> out = (Matrix<UInt32, TIdx>) this->checkOutput(output(ct, dt));
					getBlobData_(blobName, out);
				break;}
   				case dtImagePNGFloat: {
					Matrix<ImagePNG<Float, TIdx>, TIdx> out = (Matrix<ImagePNG<Float>, TIdx>) this->checkOutput(output(ct, dt));
					getBlobData_(blobName, out);
				break;}
   				case dtImagePNGDouble: {
					Matrix<ImagePNG<Double, TIdx>, TIdx> out = (Matrix<ImagePNG<Double>, TIdx>) this->checkOutput(output(ct, dt));
					getBlobData_(blobName, out);
				break;}
   				case dtMatrixFloat: {
					Matrix<Matrix<Float, TIdx>, TIdx> out = (Matrix<Matrix<Float>, TIdx>) this->checkOutput(output(ct, dt));
					getBlobData_(blobName, out);
				break;}
   				case dtMatrixDouble: {
					Matrix<Matrix<Double, TIdx>, TIdx> out = (Matrix<Matrix<Double>, TIdx>) this->checkOutput(output(ct, dt));
					getBlobData_(blobName, out);
				break;}
			}
		}
	}

	//Dispatch for memory data
	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::setMemoryDataInput_(Table<TIdx, TId> &input, const String &layerName, const String &dataName, const String &labelName, const ContentType dataCT, const DataType dataDT, const ContentType labelCT, const DataType labelDT) {
		switch (dataDT) {
			case dtFloat: {
				Matrix<Float, TIdx> in = (Matrix<Float, TIdx>) this->checkInput(input(dataCT, dataDT));
				addBlobData_(dataName, in);
				break;}
			case dtDouble: {
				Matrix<Double, TIdx> in = (Matrix<Double, TIdx>) this->checkInput(input(dataCT, dataDT));
				addBlobData_(dataName, in);
				break;}
			case dtImagePNGFloat: {
				Matrix<ImagePNG<Float>> in = (Matrix<ImagePNG<Float>>) this->checkInput(input(dataCT, dataDT));
				addBlobData_(dataName, in);
				break;}
			case dtImagePNGDouble: {
				Matrix<ImagePNG<Double>> in = (Matrix<ImagePNG<Double>>) this->checkInput(input(dataCT, dataDT));
				addBlobData_(dataName, in);
				break;}
			case dtMatrixFloat: {
				Matrix<Matrix<Float>> in = (Matrix<Matrix<Float>>) this->checkInput(input(dataCT, dataDT));
				addBlobData_(dataName, in);
				break;}
			case dtMatrixDouble: {
				Matrix<Matrix<Double>> in = (Matrix<Matrix<Double>>) this->checkInput(input(dataCT, dataDT));
				addBlobData_(dataName, in);
				break;}
			default: throw Error(ecIncompatible, "setMemoryDataInput_", SS(layerName << "." << dataName << " has an unsupported DataType: " <<  etos(dataCT)));
		}

		if (labelDT == dataType<TClassType>()) {
			Matrix<TClassType, TIdx> in;
			Table<TIdx, TId> ipt = input(labelCT, labelDT);
			if (~ipt == 0) 
				in.resize(input.rows.count(), 1); //dummy target if there is no target (production)
			else
				in = (Matrix<TClassType, TIdx>) ipt;
			addLabelBlobData_(labelName, in);
		} else {
			switch (labelDT) {
				case dtFloat: {
					Matrix<Float, TIdx> in = (Matrix<Float, TIdx>) this->checkInput(input(labelCT, labelDT));
					addBlobData_(labelName, in);
					break;}
				case dtUnknown: {
					Matrix<Float, TIdx> in(input.rows.count(), 1); //dummy
					addBlobData_(labelName, in);				
					break;}
				default: throw Error(ecIncompatible, "setMemoryDataInput_", SS(layerName << "." << dataName << " has an unsupported DataType: " << etos(labelDT)));
			}
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	Float * Caffe<TClassType, TIdx, TId>::getBlobDataByName_(const String &name) {
		TIdx idx=0;
		for (auto n=blobName_.vec().begin(); n != blobName_.vec().end(); n++, idx++)
			if (*n == name)
				return blobData_.vec(idx).getData();
		throw Error(ecNotFound, "getBlobDataByName_", SS("Blob with name: " << name << " not found "));
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::fillMemoryDataLayer_(const TIdx n) {
		batchSize_ = 0;
		TIdx idx=0;
		for (auto layerName = memoryDataLayerName_.vec().begin(); layerName != memoryDataLayerName_.vec().end(); layerName++, idx++) {
			boost::shared_ptr<MemoryDataLayer<Float>> dataLayer = dynamic_pointer_cast<MemoryDataLayer<Float>>(getActiveNet_()->layer_by_name(*layerName));
			if (batchSize_ == 0)
				batchSize_ = dataLayer->batch_size();
			else if (batchSize_ != dataLayer->batch_size())
				throw Error(ecIncompatible, "setMemoryDataInput_", "Batch sizes for MemoryData layers are different");
			dataLayer->Reset(getBlobDataByName_(dataBlobName_.vec(idx)), getBlobDataByName_(labelBlobName_.vec(idx)), n);
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::clearInputData_() {
		blobWidth_.clear();
		blobHeight_.clear();
		blobChannels_.clear();
		blobName_.clear();
		blobData_.clear();
		memoryDataLayerName_.clear();
		dataBlobName_.clear();
		labelBlobName_.clear();
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::setActiveNet_(boost::shared_ptr<caffe::Net<Float>> n) {
		activeNet_ = n;
	}

	template<typename TClassType, typename TIdx, typename TId>
	boost::shared_ptr<caffe::Net<Float>> Caffe<TClassType, TIdx, TId>::getActiveNet_() {
		return activeNet_;
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::setInputData_(Table<TIdx, TId> &input) {
		try {
			clearInputData_();
			const String inputName = "ddslMemoryDataInput";
			vector<String> names = getActiveNet_()->layer_names();
			for (auto layerName=names.begin();layerName!=names.end();layerName++) {
				if (layerName->substr(0, inputName.size()) == inputName) {				 
					boost::shared_ptr<MemoryDataLayer<Float>> dataLayer = dynamic_pointer_cast<MemoryDataLayer<Float>>(getActiveNet_()->layer_by_name(*layerName));
					caffe::LayerParameter lp;
					dataLayer->ToProto(&lp);
					DataType dataDT, labelDT;
					ContentType dataCT, labelCT;
					#ifdef _MSC_VER
					String dataName = lp.top()[0];
					String labelName = lp.top()[1];
					#else
					String dataName = *(lp.top().data()[0]);
					String labelName = *(lp.top().data()[1]);
					#endif
					parseName_(dataName, dataCT, dataDT);
					parseName_(labelName, labelCT, labelDT);
					if (labelCT != ctUnknown && labelDT != dataType<TClassType>())
						throw Error(ecIncompatible, "setInputData", SS("Incompatible label input types: " << dataTypeToString(labelDT) << " != " << dataTypeToString(dataType<TClassType>())));
					setMemoryDataInput_(input, *layerName, dataName, labelName, dataCT, dataDT, labelCT, labelDT);
					memoryDataLayerName_ | *layerName;
					dataBlobName_ | dataName;
					labelBlobName_ | labelName;
				}
			}
		} catch (Error &e) {
			throw Error(ecExternalLibrary, "setInputData_",  SS(e.what() << ". Are all the ddslinput layers of the correct type?"));
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::getOutputData_(Table<TIdx, TId> &output) {
		//Get result of a single batch and copy to output
		try {
			getActiveNet_()->Forward();
			const String outputName = "ddslOutput";
			vector<String> names = getActiveNet_()->blob_names();
			for (auto blobName=names.begin();blobName!=names.end();blobName++) {
				if (blobName->substr(0, outputName.size()) == outputName) {
					boost::shared_ptr<Blob<Float>> outputBlob = getActiveNet_()->blob_by_name(*blobName);
					ContentType ct;
					DataType dt;
					parseName_(*blobName, ct, dt);					
					getLayerOutput_(output, *blobName, ct, dt);
				}
			}
		} catch (Error &e) {
			throw Error(ecExternalLibrary, "setOutputData_",  e.what());
		}	
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::train(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
		if (solver_ == nullptr)
			throw Error(ecIncompatible, "CaffeMLP::train()", "Cannot train without a solver");
		
		//Keep batchsize for checking
		batchSize_ = 0;

		//Initialize all blobs
		this->setStage("Copy");

		//Set inputs of solver network
		setActiveNet_(solver_->net());
		setInputData_(input);
		fillMemoryDataLayer_(input.rows.count());

		//Train network
		this->setStage("Train");
		UInt32 iters = solver_->param().max_iter();
		this->setMinProgress(1);
		this->setMaxProgress(iters);

		//Train the classifier
		if (gpuDevices_ == 1) {
			while (solver_->iter() < (int)iters) {
				solver_->Step(1);
				this->setProgress(solver_->iter());
			}
		} else
			solver_->Solve();

		//Initialize		
		this->setStage("Apply");
		this->setMinProgress(0);
		this->setMaxProgress(output.rows.count());

		//Set inputs and weights of main network (for apply only)
		shareWeights_(*(solver_->net()), *net_);
		//boost::shared_ptr<caffe::Net<Float>> n(net_, [](caffe::Net<Float> *n){});
		//setActiveNet_(n);
		fillMemoryDataLayer_(input.rows.count());

		//Apply
		for (TIdx i=0;i<output.rows.count();i+=batchSize_) {
			Table<TIdx, TId> slice = output(i, batchSize_);
			getOutputData_(slice);
			this->incProgress(batchSize_);
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::apply(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
		if (net_ == nullptr)
			throw Error(ecIncompatible, "apply", "Cannot apply without a network");

		boost::shared_ptr<caffe::Net<Float>> n(net_, [](caffe::Net<Float> *n){});
		setActiveNet_(n);
		setInputData_(input);
		fillMemoryDataLayer_(input.rows.count());

		this->setMinProgress(0);
		this->setMaxProgress(output.rows.count());

		//Apply
		for (TIdx i=0;i<output.rows.count();i+=batchSize_) {
			Table<TIdx, TId> slice = output(i, batchSize_);
			getOutputData_(slice);
			this->incProgress(batchSize_);
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	Caffe<TClassType, TIdx, TId>::~Caffe() {
		clearCaffeModel();
		clearInputData_();
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::clone(const Caffe<TClassType, TIdx, TId> & other) {
		clearCaffeModel();
		init_();
		gpuDevices_ = other.gpuDevices_;

		solverProtoFile_ = other.solverProtoFile_;
		netProtoFile_ = other.netProtoFile_;
		snapshotModelFile_ = other.snapshotModelFile_;
		classToNum_ = other.classToNum_;
		classes_ = other.classes_;

		blobWidth_ = other.blobWidth_;
		blobHeight_ = other.blobHeight_;
		blobChannels_ = other.blobChannels_;
		blobName_ = other.blobName_;
		blobData_ = other.blobData_;

		memoryDataLayerName_ = other.memoryDataLayerName_;
		dataBlobName_ = other.dataBlobName_;
		labelBlobName_ = other.labelBlobName_;

		batchSize_ = other.batchSize_;

		loadCaffeModel();
		if (other.solver_ != nullptr) {
			copyWeights_(*(other.solver_->net().get()), *(solver_->net().get()));
		} else if (other.net_ != nullptr) {
			copyWeights_(*other.net_, *net_);
		}
		Model<TIdx, TId>::clone(other);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Caffe<TClassType, TIdx, TId>::clone(Caffe<TClassType, TIdx, TId> && other) {		
		clearCaffeModel();
		init_();
		gpuDevices_ = other.gpuDevices_;

		solverProtoFile_ = std::move(other.solverProtoFile_);
		netProtoFile_ = std::move( other.netProtoFile_);
		snapshotModelFile_ = std::move(other.snapshotModelFile_);
		classToNum_ = std::move(other.classToNum_);
		classes_ = std::move(other.classes_);

		blobWidth_ = std::move(other.blobWidth_);
		blobHeight_ = std::move(other.blobHeight_);
		blobChannels_  = std::move(other.blobChannels_);
		blobName_ = std::move(other.blobName_);
		blobData_ = std::move(other.blobData_);

		memoryDataLayerName_ = std::move( other.memoryDataLayerName_);
		dataBlobName_ = std::move( other.dataBlobName_);
		labelBlobName_ = std::move(other.labelBlobName_);

		batchSize_ = std::move(other.batchSize_);

		net_ = other.net_;
		solver_ = other.solver_;
	
		other.net_ = nullptr; //move semantics
		other.solver_ = nullptr; //move semantics
		other.clear();

		Model<TIdx, TId>::clone(std::move(other));
	}

	//template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_DESTRUCTOR_DEFAULT(Caffe, TClassType, TIdx, TId)
	//template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_DEFAULT(Caffe, TClassType, TIdx, TId)
	//template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_COPY(Caffe, TClassType, TIdx, TId)
	//template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MOVE(Caffe, TClassType, TIdx, TId)
	//template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_COPY(Caffe, TClassType, TIdx, TId)
	//template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_MOVE(Caffe, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_COPY(Caffe, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_MOVE(Caffe, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY(Caffe, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE(Caffe, TClassType, TIdx, TId)
	//template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_COPY_2(Caffe, TClassType, TIdx, TId)
	//template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MOVE_2(Caffe, TClassType, TIdx, TId)

	template<typename TClassType, typename TIdx, typename TId> 
	void Caffe<TClassType, TIdx, TId>::clone(const Model<TIdx, TId> & other) {
		try {
			clone(dynamic_cast<const Caffe<TClassType, TIdx, TId> &>(other));
		} catch (exception &e) {
			throw Error(ecGeneral, "In clone with Model<>", e.what());
		}
	}
	
	template<typename TClassType, typename TIdx, typename TId> 
	void Caffe<TClassType, TIdx, TId>::clone(Model<TIdx, TId> && other) {
		try {
			clone(std::move(dynamic_cast<Caffe<TClassType, TIdx, TId> &>(other)));
		} catch (exception &e) {
			throw Error(ecGeneral, "In clone with Model<>", e.what());
		}
	}

	template<typename TClassType, typename TIdx, typename TId> M_CCP_MODEL_OPERATOR_SLICE(Caffe, TClassType, TIdx, TId)


	//********************//
	//** Class CaffeMLP **//
	//********************//

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::updateParameters() {
		bool copyWeights = true;
		bool updateModel = false;

		//check changes
		if (this->template parameterValueById<Matrix<TClassType>>("Classes").vec().count() != classes_.vec().count() ||
			(~(this->template parameterValueById<Matrix<UInt32>>("Units") == units_) == 0) ||
			(UnitType)this->template parameterValueById<UInt32>("UnitType") != unitType_) {
			copyWeights = false;
			updateModel = true;
		}

		if (this->template parameterValueById<Double>("LearnRate") != learnRate_ ||
			this->template parameterValueById<Double>("Momentum") != momentum_ ||
			this->template parameterValueById<UInt32>("BatchSize") != batchSize_ ||
			this->template parameterValueById<UInt32>("StepSize") != stepSize_ ||
			this->template parameterValueById<UInt32>("Iterations") != iter_ ||
			this->template parameterValueById<Double>("Gamma") != gamma_ ||
			this->template parameterValueById<Double>("WeightDecay") != weightDecay_ ||
			this->template parameterValueById<Double>("Power") != power_ ||
			(LearnRatePolicy)this->template parameterValueById<UInt32>("LearnRatePolicy") != lrp_ ||			
			(SolverType)this->template parameterValueById<UInt32>("LearnRatePolicy") != solverType_) {
			updateModel = true;
		}

		classes_ = this->template parameterValueById<Matrix<TClassType>>("Classes");
		units_ = this->template parameterValueById<Matrix<UInt32>>("Units");
		stepValue_ = this->template parameterValueById<Matrix<UInt32>>("StepValue");
		learnRate_ = this->template parameterValueById<Double>("LearnRate");
		momentum_ = this->template parameterValueById<Double>("Momentum");
		batchSize_ = this->template parameterValueById<UInt32>("BatchSize");
		stepSize_ = this->template parameterValueById<UInt32>("StepSize");
		iter_ = this->template parameterValueById<UInt32>("Iterations");	
		iterDone_ = this->template parameterValueById<UInt32>("IterationsDone");
		lrp_ = (LearnRatePolicy)this->template parameterValueById<UInt32>("LearnRatePolicy");
		gamma_ = this->template parameterValueById<Double>("Gamma");
		power_ = this->template parameterValueById<Double>("Power");
		weightDecay_ = this->template parameterValueById<Double>("WeightDecay");
		unitType_ = (UnitType)this->template parameterValueById<UInt32>("UnitType");		
		solverType_ = (SolverType)this->template parameterValueById<UInt32>("SolverType");

		classToNum_.clear();
		Double i = 0;
		for (auto c = classes_.vec().begin(); c != classes_.vec().end(); c++, i++)
			classToNum_.insert(pair<TClassType, Double>(*c, i));

		if (updateModel)
			updateCaffeModel(copyWeights);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::updateCaffeModel(bool copyWeights) {
		caffe::Solver<Double> * tempSolver = solver_;
		caffe::Net<Double> * tempNet = net_;
		try {
			if (solver_ != nullptr) {
				solver_ = nullptr;
				net_ = nullptr;			
				genNetProto_();
				genSolverProto_();
				createSolver_();
				createNetFromSolver_();
			} else if (net_ != nullptr) {
				net_ = nullptr;
				genNetProto_();
				createNet_();
			} else {
				return;
			}
		} catch (...) {
			if (tempNet != nullptr) delete tempNet;
			if (tempSolver != nullptr) delete tempSolver;
			throw;
		}
		if (copyWeights) {
			NetParameter np;
			tempNet->ToProto(&np);
			net_->CopyTrainedLayersFrom(np);
		}
		if (tempNet != nullptr) delete tempNet;
		if (tempSolver != nullptr) delete tempSolver;
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::genSolverProto_() {
		if (netProto_ == "")
			throw Error(ecInternal, "CaffeMLP::getSolverProto_()", "netProto_ is empty");
		string brewMode = (gpuDevices_ > 0) ? "CPU" : "GPU";
		

		stringstream proto_s;
		proto_s <<	"type: \"" << SolverTypeStr[solverType_] << "\"" << endl << 
					"base_lr: " << learnRate_ << endl << 
					"momentum: " << momentum_ << endl << 
					"max_iter: " << iter_ << endl << 
					"solver_mode: " << brewMode << endl << 
					"snapshot_prefix: \"ddsl\"" << endl << 
					"lr_policy: \"" << LearnRatePolicyStr[lrp_] << "\"" << endl <<
					"gamma: " << gamma_ << endl <<
					"stepsize: " << stepSize_ << endl <<
					"weight_decay: " << weightDecay_ << endl <<
					"power: " << power_ << endl <<
					"display: " << 100 << endl;

		for (UInt32 i=0; i<stepValue_.vec().count();i++)
			proto_s << "stepvalue: " << stepValue_.vec(i) << endl;

		proto_s << "net_param: {" << endl << netProto_ << endl << "}";
				
		solverProto_ = proto_s.str();
		//cout << endl << solverProto_ << endl;
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::genNetProto_() {
		stringstream filler;
		filler << " weight_filler {type: \"xavier\"} bias_filler {type: \"xavier\"}";

		stringstream proto_n;
		proto_n <<	"layer {name: \"data_layer\" type: \"MemoryData\" top: \"data_layer\" top: \"target_layer\"" << endl <<
					"  memory_data_param {batch_size: " << batchSize_ << " channels: 1 height: 1 width: " << featureCount_ << "}}" << endl;
		proto_n <<	"layer {name: \"input_layer\" type: \"InnerProduct\" top: \"input_layer\" bottom: \"data_layer\"" << endl <<
					"  inner_product_param {num_output: " << featureCount_ << filler.str() << "}" << endl <<
					"  param {lr_mult: 1} param {lr_mult: 2}}" << endl;		
		proto_n <<	"layer {name: \"input_layer_transfer\" type: \"" << UnitTypeStr[unitType_] << "\" top: \"input_layer\" bottom: \"input_layer\"}" << endl;

		String prev = "\"input_layer\"";
		UInt32 c = 0;
		for (auto it = units_.vec().begin(); it != units_.vec().end(); it++,c++) {
			String name = SS("\"hidden_layer" << c << "\"");
			String name_transfer = SS("\"hidden_layer_transfer" << c << "\"");
			proto_n <<	"layer {name: " << name << " type: \"InnerProduct\" top: " << name << " bottom: " << prev << endl <<
						"  inner_product_param {num_output: " << *it << filler.str() << "}" << endl <<
						"  param {lr_mult: 1} param {lr_mult: 2}}" << endl;
			proto_n <<	"layer {name: " << name_transfer << " type: \"" << UnitTypeStr[unitType_] << "\" top: " << name << " bottom: " << name << "}" << endl;
			prev = name;
		}

		proto_n <<	"layer {name: \"output_layer\" type: \"InnerProduct\" top: \"output_layer\" bottom: " << prev << endl <<
					"  inner_product_param {num_output: " << classes_.vec().count() << filler.str() << "}" << endl <<
					"  param {lr_mult: 1} param {lr_mult: 2}}" << endl;
		proto_n <<	"layer {name: \"output_layer_transfer\" type: \"SoftmaxWithLoss\" top: \"output_layer_transfer\" bottom: \"output_layer\" bottom: \"target_layer\"" << endl <<
					"  include {phase: TRAIN}}" << endl;
		proto_n <<	"layer {name: \"output_layer_transfer\" type: \"Softmax\" top: \"output_layer_transfer\" bottom: \"output_layer\"" << endl <<
					"  include {phase: TEST}}";
		netProto_ = proto_n.str();
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::createSolver_() {
		SolverParameter sp;
		if (!google::protobuf::TextFormat::ParseFromString(solverProto_, &sp))
			throw Error(ecGeneral, "CaffeMLP::createsolver_()", "Invalid description for solver prototxt");
		if (solver_ != nullptr) delete solver_;
		if (net_ != nullptr) delete net_;
		solver_ = caffe::SolverRegistry<Double>::CreateSolver(sp);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::createSolverFromFile_(const DSTypes::String &filename) {
		if (solver_ != nullptr) delete solver_;
		if (net_ != nullptr) delete net_;		
		solver_ = nullptr;
		net_ = nullptr;
		ifstream ifs;

		//Fill solver string
		string solvertxtfile = filename + ".solver.prototxt";		
		solverProto_ = readToString(solvertxtfile);

		//Fill net string		
		string nettxtfile = filename + ".net.prototxt";
		netProto_ = readToString(nettxtfile);

		using namespace boost::filesystem;			
		path newPath(filename);
		newPath.remove_filename();
		path oldPath = boost::filesystem::current_path();
		boost::system::error_code ec;
		
		if (newPath != "") current_path(newPath, ec);
		if (ec) throw Error(ecGeneral, "CaffeMLP::createSolverFromFile_()", ec.message());

		string solverstatefile = SS("ddsl_iter_" << iterDone_ << ".solverstate");
		SolverParameter sp;
		ReadSolverParamsFromTextFileOrDie(solvertxtfile, &sp);
		solver_ = caffe::SolverRegistry<Double>::CreateSolver(sp);
		solver_->Restore(solverstatefile.c_str());

		current_path(oldPath, ec);
		if (ec) throw Error(ecGeneral, "CaffeMLP::createSolverFromFile_()", ec.message());
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::createNet_() {
		NetParameter np;
		if (!google::protobuf::TextFormat::ParseFromString(netProto_, &np))
			throw Error(ecGeneral, "train", "Invalid description for network prototxt");
		np.mutable_state()->set_phase(TEST);
		if (net_ != nullptr) delete net_;
		net_ = new Net<Double>(np);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::createNetFromSolver_() {
		createNet_();
		net_->ShareTrainedLayersWith(solver_->net().get());		
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::createNetFromFile_(const DSTypes::String &filename) {
		if (solver_ != nullptr) delete solver_;
		if (net_ != nullptr) delete net_;
		solver_ = nullptr;
		net_ = nullptr;
		
		//Fill net string
		string nettxtfile = filename + ".net.prototxt";
		ifstream ifs(nettxtfile);		
		if (!ifs) throw Error(ecNotFound, "CaffeMLP::createSolverFromFile_()", SS("Network file not found " << nettxtfile));
		ifs >> netProto_;
		ifs.close();

		string netstatefile = filename + ".net.caffemodel";
		NetParameter np;
		ReadNetParamsFromBinaryFileOrDie(netstatefile, &np);
		np.mutable_state()->set_phase(TEST);
		net_ = new Net<Double>(np);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::registerInputs(const Table<TIdx, TId> &table) {		
		TClassType t;
		this->registerInput(dtDouble, ctFeature);
		this->registerInput(dataType(t), ctTarget);
		this->addStages(dtString | "Train" | "Apply");
		featureCount_ = const_cast<Table<TIdx, TId> &>(table)(dtDouble, ctFeature).cols.count();

		if (solver_ == nullptr && net_ == nullptr) {
			genNetProto_();
			genSolverProto_();
			createSolver_();
			createNetFromSolver_();
		} else {
			if (net_ != nullptr) {
				TIdx inputcount = net_->blob_by_name("data_layer")->width();
				if (inputcount != featureCount_)
					throw Error(ecSizeMismatch, "CaffeMLP::registerInputs()", SS("The number of network inputs (" << inputcount << ") does not match ctFeature count (" << featureCount_ << "). Use clearCaffeModel() to start force re-init."));
			} else
				throw Error(ecInternal, "CaffeMLP::registerInputs()", "Model has an initialized solver_ without a net_");
		}
		
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::registerOutputs(const Table<TIdx, TId> &input) {
		Matrix<TClassType> classes = this->template parameterValueById<Matrix<TClassType>>("Classes");
		Table<TIdx, TId> output = (ctResult | (Matrix<TClassType>(oColMajor) | classes.val(0, 0))) | (ctConfidence | (Matrix<Double>(oColMajor) | 0.0));
		output.resizeRows(0);
		this->registerOutput(output);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::readModel(std::istream &input, const DSTypes::String &filename) {
		updateParameters();
		clearCaffeModel();
		createSolverFromFile_(filename);
		createNetFromSolver_();
	}
	
	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::readModel(const DSTypes::String &filename, bool loadSolver, bool loadNet) {
		ifstream ifs(filename);
		if (!ifs)
			throw Error(ecNotFound ,"CaffeMLP::readModel()", SS("File not found " << filename));
		this->readParams(ifs);
		if (loadSolver) {
			createSolverFromFile_(filename);
			createNetFromSolver_();
		} else if (loadNet) {
			createNetFromFile_(filename);
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::writeSolverToFile_(const DSTypes::String &filename) {
		if (solver_ == nullptr) 
			throw Error(ecNotFound, "CaffeMLP::writeSolverToFile_()", "No solver present");

		string solvertxt = filename + ".solver.prototxt";

		//Set current path
		using namespace boost::filesystem;			
		path newPath(filename);
		newPath.remove_filename();
		path oldPath = boost::filesystem::current_path();
		boost::system::error_code ec;
		if (newPath != "") current_path(newPath, ec);		
		if (ec) throw Error(ecGeneral, "CaffeMLP::writeSolverToFile_()", ec.message());

		//Save solver to text file for deployment
		ofstream ofs(solvertxt);
		if (!ofs) throw Error(ecNotFound, "CaffeMLP::writeSolverToFile_()", SS("Cannot open solver file: " << solvertxt));
		ofs << solverProto_;

		//Save solver state
		solver_->Snapshot();
		
		//Restore path
		current_path(oldPath, ec);
		if (ec) throw Error(ecGeneral, "CaffeMLP::writeSolverToFile_()", ec.message());
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::writeNetToFile_(const DSTypes::String &filename) {
		if (net_ == nullptr) 
			throw Error(ecNotFound, "CaffeMLP::writeSolverToFile_()", "No network present");
		string nettxt = filename + ".net.prototxt";
		string nettxt2 = filename + ".net.prototxt.caffemodel";
		string netbin = filename + ".net.caffemodel";

		ofstream ofs(nettxt);
		if (!ofs) throw Error(ecNotFound, "CaffeMLP::write()", SS("Cannot open net file: " << nettxt));
		ofs << netProto_;
			
		NetParameter np;
		net_->ToProto(&np);

		WriteProtoToBinaryFile(np, netbin);
		WriteProtoToTextFile(np, nettxt2);
	}


	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::writeModel(std::ostream &input, const DSTypes::String &filename) {		
		if (this->solver_ != nullptr)
			writeSolverToFile_(filename);		
		if (this->net_ != nullptr)
			writeNetToFile_(filename);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::init_() {
		//Should only be called from the constructor, otherwise call clearCaffeModel_() first
		solver_ = nullptr;
		net_ = nullptr;
		gpuDevices_ = 0;
		learnRate_ = 0;
		momentum_ = 0;
		units_.clear();
		iter_ = 0;
		batchSize_ = 0;
		featureCount_ = 0;
		stepSize_ = 0;
		stepValue_.clear();
		lrp_ = lrpFixed;
		gamma_ = 0;
		power_ = 0;
		weightDecay_ = 0;
		unitType_ = utTanH;
		solverType_ = stSGD;
		classes_.clear();
		classToNum_.clear();
		netProto_ = "";
		solverProto_ = "";
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::clearCaffeModel() {
		if (net_ != nullptr) delete net_;
		if (solver_ != nullptr) delete solver_;
		solver_ = nullptr;
		net_ = nullptr;
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::resetModel() {
		bool s = solver_ != nullptr;
		bool n = net_ != nullptr;
		clearCaffeModel();
		if (s) {
			genNetProto_();
			genSolverProto_();
			createSolver_();
			createNetFromSolver_();			
		} else if (n) {
			genSolverProto_();
			createNet_();
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	CaffeMLP<TClassType, TIdx, TId>::CaffeMLP(const DSLib::Matrix<TClassType, TIdx> &classes, TIdx gpuDevices, DSTypes::Double learnRate, DSTypes::Double momentum, Matrix<UInt32> units, TIdx iter, TIdx batchSize, DSTypes::LearnRatePolicy lrp, TIdx stepSize, Double gamma, Double power, Double weightDecay, DSTypes::UnitType unitType, DSTypes::SolverType solverType, Matrix<UInt32> stepValue) {
		init_();		
		gpuDevices_ = gpuDevices;
		Table<TIdx, TId> tab =
			(Matrix<Literal, TIdx>() | "Val") |
			(
			((Matrix<Literal, TIdx>() | "Classes") ^		(ctParameter | (Matrix<Matrix<TClassType>>() | (Matrix<TClassType>() | classes)))) |
			((Matrix<Literal, TIdx>() | "StepValue") ^	(ctParameter | (Matrix<Matrix<UInt32>>() | (Matrix<UInt32>() | stepValue)))) |
			((Matrix<Literal, TIdx>() | "Units") ^		(ctParameter | (Matrix<Matrix<UInt32>>() | (Matrix<UInt32>() | units)))) |
			((Matrix<Literal, TIdx>() | "LearnRate") ^	(ctParameter | (Matrix<Double>() | learnRate))) |
			((Matrix<Literal, TIdx>() | "Momentum") ^	(ctParameter | (Matrix<Double>() | momentum))) |
			((Matrix<Literal, TIdx>() | "Gamma") ^		(ctParameter | (Matrix<Double>() | gamma))) |
			((Matrix<Literal, TIdx>() | "WeightDecay") ^ (ctParameter | (Matrix<Double>() | weightDecay))) |
			((Matrix<Literal, TIdx>() | "Power") ^		(ctParameter | (Matrix<Double>() | power))) |
			((Matrix<Literal, TIdx>() | "BatchSize") ^		(ctParameter | (Matrix<UInt32>() | (UInt32)batchSize))) |
			((Matrix<Literal, TIdx>() | "Iterations") ^		(ctParameter | (Matrix<UInt32>() | (UInt32)iter))) |
			((Matrix<Literal, TIdx>() | "IterationsDone") ^	(ctParameter | (Matrix<UInt32>() | (UInt32)0))) |
			((Matrix<Literal, TIdx>() | "LearnRatePolicy") ^ (ctParameter | (Matrix<UInt32>() | (UInt32)lrp))) |
			((Matrix<Literal, TIdx>() | "UnitType") ^		(ctParameter | (Matrix<UInt32>() | (UInt32)unitType))) |
			((Matrix<Literal, TIdx>() | "SolverType") ^		(ctParameter | (Matrix<UInt32>() | (UInt32)solverType))) |
			((Matrix<Literal, TIdx>() | "StepSize") ^		(ctParameter | (Matrix<UInt32>() | (UInt32)stepSize)))
			);
		this->registerParameters(tab);
	}

	template<typename TClassType, typename TIdx, typename TId>
	CaffeMLP<TClassType, TIdx, TId>::CaffeMLP() {
		init_();
	}	

	template<typename TClassType, typename TIdx, typename TId>
	CaffeMLP<TClassType, TIdx, TId>::CaffeMLP(const CaffeMLP<TClassType, TIdx, TId> & other) {
		init_();
		clone(other);
	}

	template<typename TClassType, typename TIdx, typename TId>	
	CaffeMLP<TClassType, TIdx, TId>::CaffeMLP(CaffeMLP<TClassType, TIdx, TId> && other) {
		init_();
		clone(std::move(other));
	}

	template<typename TClassType, typename TIdx, typename TId>
	CaffeMLP<TClassType, TIdx, TId>::CaffeMLP(const Model<TIdx, TId>& other) {
		init_();
		try {
			clone(dynamic_cast<const CaffeMLP<TClassType, TIdx, TId> &>(other));
		} catch (exception &e) {
			throw Error(ecGeneral, "In copy constructor with Model<>", e.what());
		}
	}
	
	template<typename TClassType, typename TIdx, typename TId>
	CaffeMLP<TClassType, TIdx, TId>::CaffeMLP(Model<TIdx, TId>&& other) {
		init_();
		try {
			clone(std::move(dynamic_cast<CaffeMLP<TClassType, TIdx, TId> &>(other)));
		} catch (exception &e) {
			throw Error(ecGeneral, "In move constructor with Model<>", e.what());
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	CaffeMLP<TClassType, TIdx, TId>::CaffeMLP(const DSTypes::String &filename, TIdx gpuDevices, bool loadSolver, bool loadNet) {
		init_();
		gpuDevices_ = gpuDevices;
		this->readModel(filename, loadSolver, loadNet);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::train(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
		if (solver_ == nullptr)
			throw Error(ecIncompatible, "CaffeMLP::train()", "Cannot train without a solver");
		this->setStage("Train");
		this->setMinProgress(1);
		this->setMaxProgress(iter_);

		if (gpuDevices_ > 0) {
			caffe::Caffe::set_mode(caffe::Caffe::GPU);
			caffe::Caffe::set_solver_count(1);
		} else {
			caffe::Caffe::set_mode(caffe::Caffe::CPU);
			caffe::Caffe::set_solver_count(1);
		}		

		TClassType t;
		Matrix<TClassType> out = output(dataType(t), ctResult);
		Matrix<Double> conf = output(dtDouble, ctConfidence);
		Matrix<Double> feat = input(dtDouble, ctFeature);
		Matrix<TClassType> target = input(dataType(t), ctTarget);
		if (target.cols.count() != 1) throw Error(ecIncompatible, "train", "Should have one ctTarget col");
		
		Matrix<Double> rowMajorFeat;
		rowMajorFeat ^= (feat.isFullSlice())?feat:*feat;//*feat will copy the feat to get correct memory alignment for Caffe

		Matrix<Double> targetDouble(target.rows.count(), target.cols.count(), target.order());
		auto dst_t = targetDouble.vec().begin();
		for (auto t = target.vec().begin(); t != target.vec().end(); t++, dst_t++)
			*dst_t = classToNum_.find(*t)->second;

		dynamic_cast<MemoryDataLayer<Double>&>(*(solver_->net()->layer_by_name("data_layer"))).Reset(rowMajorFeat.getData(), targetDouble.getData(), input.rows.count());
		const int stepSize = 10;
		TIdx iterDonePrev = iterDone_;
		while (iterDone_ < (int)iter_) {			
			solver_->Step(stepSize);
			iterDone_ = iterDonePrev + solver_->iter();
			this->template parameterValueById<UInt32>("IterationsDone") = iterDone_;
			this->setProgress(iterDone_);
		}

		this->nextStage();
		this->setMinProgress(0);
		this->setMaxProgress(out.rows.count());

		dynamic_cast<MemoryDataLayer<Double>&>(*(net_->layer_by_name("data_layer"))).Reset(rowMajorFeat.getData(), targetDouble.getData(), input.rows.count());

		const UInt32 classCount = classes_.vec().count();		
		auto conf_it = conf.vec().begin();
		for (auto out_it = out.vec().begin(); out_it != out.vec().end();) {
			Blob<Double> * outputBlob = net_->Forward()[0];
			const Double * begin = outputBlob->cpu_data();
			const Double * end = begin + (batchSize_ * classCount);
			const vector<Double> outputData(begin, end);
			for (UInt32 s = 0; s < batchSize_; s++) {
				const vector<Double> sampleOutput(outputData.begin() + s * classCount, outputData.begin() + ((s + 1) * classCount));
				Double max = 0;
				TIdx maxIdx = 0;
				TIdx idx = 0;
				for (auto o = sampleOutput.begin(); o != sampleOutput.end(); o++, idx++) {
					if (*o >= max) {
						maxIdx = idx;
						max = *o;
					}
				}
				*out_it = classes_.vec(maxIdx);
				*conf_it = max;
				out_it++;
				conf_it++;
			}
			this->incProgress(batchSize_);
		}		
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::apply(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
		if (net_ == nullptr)
			throw Error(ecIncompatible, "CaffeMLP::apply()", "Cannot apply without a network");

		this->setStage("Apply");
		this->setMinProgress(0);
		this->setMaxProgress(output.rows.count());

		if (gpuDevices_ > 0) {
			caffe::Caffe::set_mode(caffe::Caffe::GPU);
			caffe::Caffe::set_solver_count(1);
		} else {
			caffe::Caffe::set_mode(caffe::Caffe::CPU);
			caffe::Caffe::set_solver_count(1);
		}

		TClassType t;
		Matrix<TClassType> out = output(dataType(t), ctResult);
		Matrix<Double> conf = output(dtDouble, ctConfidence);
		Matrix<Double> feat = input(dtDouble, ctFeature);

		Matrix<Double> rowMajorFeat;
		rowMajorFeat ^= (feat.isFullSlice()) ? feat : *feat;//*feat will copy the feat to get correct memory alignment for Caffe
		Matrix<Double> targetDouble(feat.rows.count(), 1); //dummy targets so the MemoryDataLayer can still be used

		dynamic_cast<MemoryDataLayer<Double>&>(*(net_->layer_by_name("data_layer"))).Reset(rowMajorFeat.getData(), targetDouble.getData(), input.rows.count());

		const UInt32 classCount = classes_.vec().count();
		auto conf_it = conf.vec().begin();
		for (auto out_it = out.vec().begin(); out_it != out.vec().end();) {
			Blob<Double> * outputBlob = net_->Forward()[0];
			const Double * begin = outputBlob->cpu_data();
			const Double * end = begin + (batchSize_ * classCount);
			const vector<Double> outputData(begin, end);
			for (UInt32 s = 0; s < batchSize_; s++) {
				const vector<Double> sampleOutput(outputData.begin() + s * classCount, outputData.begin() + ((s + 1) * classCount));
				Double max = 0;
				TIdx maxIdx = 0;
				TIdx idx = 0;
				for (auto o = sampleOutput.begin(); o != sampleOutput.end(); o++, idx++) {
					if (*o >= max) {
						maxIdx = idx;
						max = *o;
					}
				}
				*out_it = classes_.vec(maxIdx);
				*conf_it = max;
				out_it++;
				conf_it++;
			}
			this->incProgress(batchSize_);
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	CaffeMLP<TClassType, TIdx, TId>::~CaffeMLP() {
		clearCaffeModel();
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::clone(const CaffeMLP<TClassType, TIdx, TId> & other) {
		clearCaffeModel();
		init_();
		gpuDevices_ = other.gpuDevices_;
		learnRate_ = other.learnRate_;
		momentum_ = other.learnRate_;
		units_ = other.units_;
		iter_ = other.iter_;
		batchSize_ = other.batchSize_;
		featureCount_ = other.featureCount_;
		classes_ = other.classes_;
		classToNum_ = other.classToNum_;
		netProto_ = other.netProto_;
		solverProto_ = other.solverProto_;	

		stepSize_ = other.stepSize_;
		stepValue_ = other.stepValue_;
		lrp_ = other.lrp_;
		gamma_ = other.gamma_;
		power_ = other.power_;
		weightDecay_ = other.weightDecay_;
		unitType_ = other.unitType_;
		solverType_ = other.solverType_;

		if (other.solver_ != nullptr) {
			genNetProto_();
			genSolverProto_();
			createSolver_();
			createNetFromSolver_();
		} else {
			if (other.net_ != nullptr) {
				genNetProto_();
				createNet_();
				NetParameter np;
				other.net_->ToProto(&np);
				net_->CopyTrainedLayersFrom(np);
			}
		}
		Model<TIdx, TId>::clone(other);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void CaffeMLP<TClassType, TIdx, TId>::clone(CaffeMLP<TClassType, TIdx, TId> && other) {		
		clearCaffeModel();
		init_();
		solver_ = other.solver_;
		net_ = other.net_;
		gpuDevices_ = other.gpuDevices_;
		learnRate_ = other.learnRate_;
		momentum_ = other.learnRate_;
		units_ = std::move(other.units_);
		iter_ = other.iter_;
		batchSize_ = other.batchSize_;
		featureCount_ = other.featureCount_;
		classes_ = other.classes_;
		classToNum_ = other.classToNum_;
		netProto_ = other.netProto_;
		solverProto_ = other.netProto_;

		stepSize_ = other.stepSize_;
		stepValue_ = std::move(other.stepValue_);
		lrp_ = other.lrp_;
		gamma_ = other.gamma_;
		power_ = other.power_;
		weightDecay_ = other.weightDecay_;
		unitType_ = other.unitType_;
		solverType_ = other.solverType_;

		other.net_ = nullptr; //move semantics
		other.solver_ = nullptr; //move semantics
		other.clear();

		Model<TIdx, TId>::clone(std::move(other));
	}

	//template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_DESTRUCTOR_DEFAULT(CaffeMLP, TClassType, TIdx, TId)
	//template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_DEFAULT(CaffeMLP, TClassType, TIdx, TId)
	//template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_COPY(CaffeMLP, TClassType, TIdx, TId)
	//template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MOVE(CaffeMLP, TClassType, TIdx, TId)
	//template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_COPY(CaffeMLP, TClassType, TIdx, TId)
	//template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_MOVE(CaffeMLP, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_COPY(CaffeMLP, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_MOVE(CaffeMLP, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY(CaffeMLP, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE(CaffeMLP, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY_2(CaffeMLP, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE_2(CaffeMLP, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CCP_MODEL_OPERATOR_SLICE(CaffeMLP, TClassType, TIdx, TId)



}
