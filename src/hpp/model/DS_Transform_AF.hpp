#pragma once
#include "h/model/DS_Transform_AF.h"

#include "hpp/model/DS_Model.hpp"
#include "hpp/lib/DS_Image_PNG.hpp"

using namespace std;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSUtil;
using namespace DSLib;

namespace DSModel {
	namespace ArrayFire {
		/***************/
		/** Class PCA **/
		/***************/
		template<typename TIdx, typename TId>
		PCA<TIdx, TId>::PCA(const bool center, const bool scale, const UInt32 keepDims) {
			Table<TIdx, TId> tab =	(
										(Matrix<Literal, TIdx>() | "Val") | (										
											((Matrix<Literal, TIdx>() | "Center") ^ (ctParameter | (dtUInt32 | (center?1U:0U)))) |
											((Matrix<Literal, TIdx>() | "Scale") ^ (ctParameter | (dtUInt32 | (scale?1U:0U)))) |
											((Matrix<Literal, TIdx>() | "KeepDims") ^ (ctParameter | (dtUInt32 | keepDims)))
										)
									);
			this->registerParameters(tab);
		};

		template<typename TIdx, typename TId>
		PCA<TIdx, TId>::PCA(const DSTypes::String &filename) {
			std::ifstream ifs(filename, ios::binary);
			if (!ifs)
				throw Error(ecNotFound, "PCA::PCA()", SS("File not found: " << filename));
			this->readParams(ifs);
			this->readModel(ifs, filename);
		}

		template<typename TIdx, typename TId>
		void PCA<TIdx, TId>::clone(const PCA<TIdx, TId> & other) {
			vectors_ = other.vectors_;
			mean_ = other.mean_;
			sd_ = other.sd_;
			Model<TIdx, TId>::clone(other);
		}

		template<typename TIdx, typename TId>
		void PCA<TIdx, TId>::clone(PCA<TIdx, TId> && other) {
			vectors_ = std::move(other.vectors_);
			mean_ = std::move(other.mean_);
			sd_ = std::move(other.sd_);
			Model<TIdx, TId>::clone(std::move(other));
		}

		template<typename TIdx, typename TId>
		void PCA<TIdx, TId>::registerInputs(const Table<TIdx, TId> &table) {
			this->registerInput(dtFloat, ctFeature);
			//this->registerInput(dtImagePNGFloat, ctFeature);
			//this->registerInput(dtMatrixFloat, ctFeature);
			this->registerInput(dtDouble, ctFeature);
			//this->registerInput(dtImagePNGDouble, ctFeature);
			//this->registerInput(dtMatrixDouble, ctFeature);
		}

		template<typename TIdx, typename TId>
		void PCA<TIdx, TId>::registerOutputs(const Table<TIdx, TId> &input) {
			UInt32 kd = this->template parameterValueById<UInt32>("KeepDims");
			Table<> output;
			if (kd == 0)
				output = input;
			else {
				Table<TIdx, TId> & input2 = const_cast<Table<TIdx, TId> &>(input);
				output = input2(0, 1, 0, kd);
			}
			this->addStages((dtString | "PCA + Project" | "Project"));
			output.resizeRows(0);
			Matrix<TId, TIdx>() ^ output;
			this->registerOutput(output);
		}

		template <typename TIdx, typename TId>
		template <typename T>
		void PCA<TIdx, TId>::calcFeatures_(Matrix<T, TIdx> &&in, Matrix<T, TIdx> &&out, bool train) {
			bool ctr = this->template parameterValueById<UInt32>("Center") != 0;
			bool scl = this->template parameterValueById<UInt32>("Scale") != 0;
			UInt32 kd = this->template parameterValueById<UInt32>("KeepDims");
			T t;
			Matrix<T, TIdx> vec, proj, mn, sd;
			if (train) {
				this->setMinProgress(0);
				this->setMaxProgress(5);
				this->setProgress(0);
				this->setStage("PCA + Project");
				Matrix<T, TIdx> val;
				DSFunc::ArrayFire::PCA(in, proj, vec, val, mn, sd, ctr, scl, kd);
				this->incProgress();
				vectors_ = ctParameter | vec;
				this->incProgress();
				if (ctr) mean_ = ctParameter | mn;
				this->incProgress();
				if (scl) sd_ = ctParameter | sd;
				this->incProgress();
			} else {
				this->setMinProgress(0);
				this->setMaxProgress(4);
				this->setProgress(0);
				this->setStage("Project");
				vec = vectors_[dataType(t)];
				this->incProgress();
				if (ctr) mn = mean_[dataType(t)];
				this->incProgress();
				if (scl) sd = sd_[dataType(t)];
				this->incProgress();
				DSFunc::ArrayFire::project(in, proj, vec, mn, sd, kd);
				this->incProgress();
			}
			if (out.isSameSize(proj))
				out = proj; //DSA
			else
				throw Error(ecIncompatible, "PCA::CalcFeatures", SS("Projection matrix does not produce " << out.rows.count() << "x" << out.cols.count() << " Matrices but " << proj.rows.count() << "x" << proj.cols.count() << " Matrices"));
		}

		template <typename TIdx, typename TId>
		template <typename T, template <typename, typename> class TMat>
		void PCA<TIdx, TId>::calcMatFeatures_(Matrix<TMat<T, TIdx>, TIdx> &&in, Matrix<TMat<T, TIdx>, TIdx> &&out, bool train) {
		}

		template<typename TIdx, typename TId>
		void PCA<TIdx, TId>::train(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
			vectors_.clear();
			sd_.clear();
			mean_.clear();
			if (~input(dtDouble, ctFeature) != 0)
				calcFeatures_<Double>(input(dtDouble, ctFeature), output(dtDouble, ctFeature), true);
			else if (~input(dtImagePNGDouble, ctFeature) != 0)
				calcMatFeatures_<Double, ImagePNG>(input(dtImagePNGDouble, ctFeature), output(dtImagePNGDouble, ctFeature), true);
			else if (~input(dtMatrixDouble, ctFeature) != 0)
				calcMatFeatures_<Double, Matrix>(input(dtMatrixDouble, ctFeature), output(dtMatrixDouble, ctFeature), true);
			else if (~input(dtFloat, ctFeature) != 0)
				calcFeatures_<Float>(input(dtFloat, ctFeature), output(dtFloat, ctFeature), true);
			else if (~input(dtImagePNGFloat, ctFeature) != 0)
				calcMatFeatures_<Float, ImagePNG>(input(dtImagePNGFloat, ctFeature), output(dtImagePNGFloat, ctFeature), true);
			else if (~input(dtMatrixFloat, ctFeature) != 0)
				calcMatFeatures_<Float, Matrix>(input(dtMatrixFloat, ctFeature), output(dtMatrixFloat, ctFeature), true);
		}

		template<typename TIdx, typename TId>
		void PCA<TIdx, TId>::apply(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
			if (~input(dtDouble, ctFeature) != 0)
				calcFeatures_<Double>(input(dtDouble, ctFeature), output(dtDouble, ctFeature), false);
			else if (~input(dtImagePNGDouble, ctFeature) != 0)
				calcMatFeatures_<Double, ImagePNG>(input(dtImagePNGDouble, ctFeature), output(dtImagePNGDouble, ctFeature), false);
			else if (~input(dtMatrixDouble, ctFeature) != 0)
				calcMatFeatures_<Double, Matrix>(input(dtMatrixDouble, ctFeature), output(dtMatrixDouble, ctFeature), false);
			else if (~input(dtFloat, ctFeature) != 0)
				calcFeatures_<Float>(input(dtFloat, ctFeature), output(dtFloat, ctFeature), false);
			else if (~input(dtImagePNGFloat, ctFeature) != 0)
				calcMatFeatures_<Float, ImagePNG>(input(dtImagePNGFloat, ctFeature), output(dtImagePNGFloat, ctFeature), false);
			else if (~input(dtMatrixFloat, ctFeature) != 0)
				calcMatFeatures_<Float, Matrix>(input(dtMatrixFloat, ctFeature), output(dtMatrixFloat, ctFeature), false);
		}

		template<typename TIdx, typename TId>
		void PCA<TIdx, TId>::resetModel() {
			vectors_.clear();
			mean_.clear();
			sd_.clear();
		}

		template<typename TIdx, typename TId>
		void PCA<TIdx, TId>::readModel(std::istream &input, const DSTypes::String &filename) {
			this->updateParameters();
			vectors_.read(input);
			mean_.read(input);
			sd_.read(input);
		}

		template<typename TIdx, typename TId>
		void PCA<TIdx, TId>::writeModel(std::ostream &output, const DSTypes::String &filename) {
			vectors_.write(output);
			mean_.write(output);
			sd_.write(output);
		}

		template<typename TIdx, typename TId> M_CPP_MODEL_DESTRUCTOR_DEFAULT(PCA, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_COPY(PCA, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MOVE(PCA, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_COPY(PCA, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_MOVE(PCA, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_COPY(PCA, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_MOVE(PCA, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY(PCA, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE(PCA, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY_2(PCA, TIdx, TId)
		template<typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE_2(PCA, TIdx, TId)
		template<typename TIdx, typename TId> M_CCP_MODEL_OPERATOR_SLICE(PCA, TIdx, TId)


		/***************/
		/** Class LDA **/
		/***************/
		template<typename TClassType, typename TIdx, typename TId>
		LDA<TClassType, TIdx, TId>::LDA(const DSLib::Matrix<TClassType, TIdx> &classes, const bool center, const bool scale, const UInt32 keepDims) {
			Table<TIdx, TId> tab =	(
										(Matrix<Literal, TIdx>() | "Val") | (										
											((Matrix<Literal, TIdx>() | "Center") ^ (ctParameter | (dtUInt32 | (center?1U:0U)))) |
											((Matrix<Literal, TIdx>() | "Scale") ^ (ctParameter | (dtUInt32 | (scale?1U:0U)))) |
											((Matrix<Literal, TIdx>() | "KeepDims") ^ (ctParameter | (dtUInt32 | keepDims))) |
											((Matrix<Literal, TIdx>() | "Classes") ^ (ctParameter | (Matrix<Matrix<TClassType>>() | (Matrix<TClassType>() | classes))))
										)
									);
			this->registerParameters(tab);			
		}

		template<typename TClassType, typename TIdx, typename TId>
		void LDA<TClassType, TIdx, TId>::updateParameters() {
			classToNum_.clear();
			Float i = 0;
			Matrix<TClassType> classes = this->template parameterValueById<Matrix<TClassType>>("Classes");
			for (auto c = classes.vec().begin(); c != classes.vec().end(); c++, i++)
				classToNum_.insert(pair<TClassType, Float>(*c, i));
		}

		template<typename TClassType, typename TIdx, typename TId>
		LDA<TClassType, TIdx, TId>::LDA(const DSTypes::String &filename) {
			std::ifstream ifs(filename, ios::binary);
			if (!ifs)
				throw Error(ecNotFound, "LDA::LDA()", SS("File not found: " << filename));
			this->readParams(ifs);
			this->readModel(ifs, filename);
		}

		template<typename TClassType, typename TIdx, typename TId>
		void LDA<TClassType, TIdx, TId>::clone(const LDA<TClassType, TIdx, TId> & other) {
			vectors_ = other.vectors_;
			mean_ = other.mean_;
			sd_ = other.sd_;
			Model<TIdx, TId>::clone(other);
		}

		template<typename TClassType, typename TIdx, typename TId>
		void LDA<TClassType, TIdx, TId>::clone(LDA<TClassType, TIdx, TId> && other) {
			vectors_ = std::move(other.vectors_);
			mean_ = std::move(other.mean_);
			sd_ = std::move(other.sd_);
			Model<TIdx, TId>::clone(std::move(other));
		}

		template<typename TClassType, typename TIdx, typename TId>
		void LDA<TClassType, TIdx, TId>::registerInputs(const Table<TIdx, TId> &table) {
			this->registerInput(dataType<TClassType>(), ctTarget);
			this->registerInput(dtFloat, ctFeature);
			//this->registerInput(dtImagePNGFloat, ctFeature);
			//this->registerInput(dtMatrixFloat, ctFeature);
			this->registerInput(dtDouble, ctFeature);
			//this->registerInput(dtImagePNGDouble, ctFeature);
			//this->registerInput(dtMatrixDouble, ctFeature);
		}

		template<typename TClassType, typename TIdx, typename TId>
		void LDA<TClassType, TIdx, TId>::registerOutputs(const Table<TIdx, TId> &input) {
			UInt32 kd = this->template parameterValueById<UInt32>("KeepDims");
			Matrix<TClassType> classes = this->template parameterValueById<Matrix<TClassType>>("Classes");
			Table<TIdx, TId> & input2 = const_cast<Table<TIdx, TId> &>(input);
			Table<> output;
			if (kd == 0)
				output = input2(0, 1, 0, classes.vec().count() - 1);
			else {				
				output = input2(0, 1, 0, kd);
			}
			this->addStages((dtString | "LDA + Project" | "Project"));
			output.resizeRows(0);
			Matrix<TId, TIdx>() ^ (output  | (ctTarget | Matrix<TClassType, TIdx>(0, 1)));
			this->registerOutput(output);
		}

		template<typename TClassType, typename TIdx, typename TId>
		template <typename T>
		void LDA<TClassType, TIdx, TId>::calcFeatures_(Matrix<T, TIdx> &&in, Matrix<TClassType, TIdx> &&target, Matrix<T, TIdx> &&out, bool train) {
			bool ctr = this->template parameterValueById<UInt32>("Center") != 0;
			bool scl = this->template parameterValueById<UInt32>("Scale") != 0;
			Matrix<TClassType, TIdx> classes = this->template parameterValueById<Matrix<TClassType, TIdx>>("Classes");
			UInt32 kd = this->template parameterValueById<UInt32>("KeepDims");
			T t;
			Matrix<T, TIdx> vec, proj, mn, sd, tar, cls;
			tar.resize(target.vec().count(), 1);
			auto tin = target.vec().begin();
			for (auto t = tar.vec().begin(); t != tar.vec().end(); t++, tin++)
				*t = classToNum_.find(*tin)->second;
			
			cls.resize(classes.vec().count(), 1);
			auto clsin = classes.vec().begin();
			for (auto c = cls.vec().begin(); c != cls.vec().end(); c++, clsin++)
				*c = classToNum_.find(*clsin)->second;
			if (train) {
				this->setStage("LDA + Project");
				Matrix<T, TIdx> val;
				DSFunc::ArrayFire::LDA(in, tar, cls, proj, vec, val, mn, sd, ctr, scl, kd);
				//DSFunc::ArrayFire::PCA(in, proj, vec, val, mn, sd, ctr, scl, kd);
				vectors_ = ctParameter | vec;
				if (ctr) mean_ = ctParameter | mn;
				if (scl) sd_ = ctParameter | sd;
			} else {
				this->setStage("Project");
				vec = vectors_[dataType(t)];
				if (ctr) mn = mean_[dataType(t)];
				if (scl) sd = sd_[dataType(t)];
				DSFunc::ArrayFire::project(in, proj, vec, mn, sd, kd);
			}
			if (out.isSameSize(proj))
				out = proj; //DSA
			else
				throw Error(ecIncompatible, "LDA::CalcFeatures", SS("Projection matrix does not produce " << out.rows.count() << "x" << out.cols.count() << " Matrices but " << proj.rows.count() << "x" << proj.cols.count() << " Matrices"));
		}

		template<typename TClassType, typename TIdx, typename TId>
		template <typename T, template <typename, typename> class TMat>
		void LDA<TClassType, TIdx, TId>::calcMatFeatures_(Matrix<TMat<T, TIdx>, TIdx> &&in, Matrix<TClassType, TIdx> &&target, Matrix<TMat<T, TIdx>, TIdx> &&out, bool train) {
		}

		template<typename TClassType, typename TIdx, typename TId>
		void LDA<TClassType, TIdx, TId>::train(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
			vectors_.clear();
			sd_.clear();
			mean_.clear();
			if (~input(dataType<TClassType>(), ctTarget) == 0)
				throw Error(ecIncompatible, "LDA::train", SS("LDA needs a ctTarget of type " << etos(dataType<TClassType>()))); 
			//PassThrough ctTarget
			output(dataType<TClassType>(), ctTarget) = input(dataType<TClassType>(), ctTarget);
			if (~input(dtDouble, ctFeature) != 0)
				calcFeatures_<Double>(input(dtDouble, ctFeature), input(dataType<TClassType>(), ctTarget), output(dtDouble, ctFeature), true);
			else if (~input(dtImagePNGDouble, ctFeature) != 0)
				calcMatFeatures_<Double, ImagePNG>(input(dtImagePNGDouble, ctFeature), input(dataType<TClassType>(), ctTarget), output(dtImagePNGDouble, ctFeature), true);
			else if (~input(dtMatrixDouble, ctFeature) != 0)
				calcMatFeatures_<Double, Matrix>(input(dtMatrixDouble, ctFeature), input(dataType<TClassType>(), ctTarget), output(dtMatrixDouble, ctFeature), true);
			else if (~input(dtFloat, ctFeature) != 0)
				calcFeatures_<Float>(input(dtFloat, ctFeature), input(dataType<TClassType>(), ctTarget), output(dtFloat, ctFeature), true);
			else if (~input(dtImagePNGFloat, ctFeature) != 0)
				calcMatFeatures_<Float, ImagePNG>(input(dtImagePNGFloat, ctFeature), input(dataType<TClassType>(), ctTarget), output(dtImagePNGFloat, ctFeature), true);
			else if (~input(dtMatrixFloat, ctFeature) != 0)
				calcMatFeatures_<Float, Matrix>(input(dtMatrixFloat, ctFeature), input(dataType<TClassType>(), ctTarget), output(dtMatrixFloat, ctFeature), true);
		}

		template<typename TClassType, typename TIdx, typename TId>
		void LDA<TClassType, TIdx, TId>::apply(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
			//PassThrough ctTarget
			output(dataType<TClassType>(), ctTarget) = input(dataType<TClassType>(), ctTarget);
			if (~input(dtDouble, ctFeature) != 0)
				calcFeatures_<Double>(input(dtDouble, ctFeature), input(dataType<TClassType>(), ctTarget), output(dtDouble, ctFeature), false);
			else if (~input(dtImagePNGDouble, ctFeature) != 0)
				calcMatFeatures_<Double, ImagePNG>(input(dtImagePNGDouble, ctFeature), input(dataType<TClassType>(), ctTarget), output(dtImagePNGDouble, ctFeature), false);
			else if (~input(dtMatrixDouble, ctFeature) != 0)
				calcMatFeatures_<Double, Matrix>(input(dtMatrixDouble, ctFeature), input(dataType<TClassType>(), ctTarget), output(dtMatrixDouble, ctFeature), false);
			else if (~input(dtFloat, ctFeature) != 0)
				calcFeatures_<Float>(input(dtFloat, ctFeature), input(dataType<TClassType>(), ctTarget), output(dtFloat, ctFeature), false);
			else if (~input(dtImagePNGFloat, ctFeature) != 0)
				calcMatFeatures_<Float, ImagePNG>(input(dtImagePNGFloat, ctFeature), input(dataType<TClassType>(), ctTarget), output(dtImagePNGFloat, ctFeature), false);
			else if (~input(dtMatrixFloat, ctFeature) != 0)
				calcMatFeatures_<Float, Matrix>(input(dtMatrixFloat, ctFeature), input(dataType<TClassType>(), ctTarget), output(dtMatrixFloat, ctFeature), false);
		}

		template<typename TClassType, typename TIdx, typename TId>
		void LDA<TClassType, TIdx, TId>::resetModel() {
			vectors_.clear();
			mean_.clear();
			sd_.clear();
		}

		template<typename TClassType, typename TIdx, typename TId>
		void LDA<TClassType, TIdx, TId>::readModel(std::istream &input, const DSTypes::String &filename) {
			this->updateParameters();
			vectors_.read(input);
			mean_.read(input);
			sd_.read(input);
			DSUtil::read(input, classToNum_);
		}

		template<typename TClassType, typename TIdx, typename TId>
		void LDA<TClassType, TIdx, TId>::writeModel(std::ostream &output, const DSTypes::String &filename) {
			vectors_.write(output);
			mean_.write(output);
			sd_.write(output);
			DSUtil::write(output, classToNum_);
		}

		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_DESTRUCTOR_DEFAULT(LDA, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_DEFAULT(LDA, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_COPY(LDA, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MOVE(LDA, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_COPY(LDA, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_MOVE(LDA, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_COPY(LDA, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_MOVE(LDA, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY(LDA, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE(LDA, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY_2(LDA, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE_2(LDA, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CCP_MODEL_OPERATOR_SLICE(LDA, TClassType, TIdx, TId)
	}
};
