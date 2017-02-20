#pragma once
#include "h/model/DS_Classify_AF.h"

#include "hpp/model/DS_Model.hpp"
#include "hpp/lib/DS_MatrixFunc_AF.hpp"
#include "ArrayFireDDSL.h"

using namespace std;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSUtil;
using namespace DSLib;

namespace DSModel {
	namespace ArrayFire {
		//*****************//
		//** Correlation **//
		//*****************//

		template<typename TClassType, typename TIdx, typename TId>
		Correlation<TClassType, TIdx, TId>::Correlation(const DSTypes::String &filename) {		
			std::ifstream ifs(filename, ios::binary);
			if (!ifs)
				throw Error(ecNotFound, "ArrayFire::Correlation()", SS("File not found: " << filename));
			this->readParams(ifs);
			this->readModel(ifs, filename);
		}

		template<typename TClassType, typename TIdx, typename TId>
		Correlation<TClassType, TIdx, TId>::Correlation(const Matrix<TClassType, TIdx> &classes) {
			Table<TIdx, TId> tab = ((Matrix<Literal, TIdx>() | "Val") |
				((Matrix<Literal, TIdx>() | "Classes") ^
				(ctParameter | (Matrix<Matrix<TClassType>>() | (Matrix<TClassType>() | classes)))
					)
				);
			this->registerParameters(tab);
		};

		template<typename TClassType, typename TIdx, typename TId>
		void Correlation<TClassType, TIdx, TId>::resetModel() {
			patterns_.clear();
		}

		template<typename TClassType, typename TIdx, typename TId>
		void Correlation<TClassType, TIdx, TId>::readModel(std::istream &input, const DSTypes::String &filename) {
			updateParameters();
			DSUtil::read(input, patterns_);
		}
	
		template<typename TClassType, typename TIdx, typename TId>
		void Correlation<TClassType, TIdx, TId>::writeModel(std::ostream &output, const DSTypes::String &filename) {
			DSUtil::write(output, patterns_);
		}

		template<typename TClassType, typename TIdx, typename TId>
		void Correlation<TClassType, TIdx, TId>::updateParameters() {
		}

		template<typename TClassType, typename TIdx, typename TId>
		void Correlation<TClassType, TIdx, TId>::registerInputs(const Table<TIdx, TId> &table) {
			TClassType t;
			this->registerInput(dtFloat, ctFeature);
			this->registerInput(dataType(t), ctTarget);
		}

		template<typename TClassType, typename TIdx, typename TId>
		void Correlation<TClassType, TIdx, TId>::registerOutputs(const Table<TIdx, TId> &input) {
			Matrix<TClassType> classes = this->template parameterValueById<Matrix<TClassType>>("Classes");			
			Table<TIdx, TId> output =	(ctResult | (Matrix<TClassType>(oColMajor) | classes.val(0, 0))) | 
										(ctConfidence | (Matrix<Float>(oColMajor) | 0.0f)) | 
										(ctCorrelation | Matrix<Float>(1, classes.vec().count(), oColMajor));
			output.resizeRows(0);
			this->registerOutput(output);
			this->addStages(dtString | "Train" | "Classify" | "Result");
		}		

		template<typename TClassType, typename TIdx, typename TId>
		void Correlation<TClassType, TIdx, TId>::train(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
			try {
				//Train
				patterns_.clear();
				Matrix<TClassType, TIdx> classes = this->template parameterValueById<Matrix<TClassType, TIdx>>("Classes");
				Matrix<Float, TIdx> in = this->checkInput(input(dtFloat, ctFeature));
				Matrix<TClassType, TIdx> tar = this->checkInput(input(dataType<TClassType>(), ctTarget));
				Matrix<TClassType, TIdx> ret = this->checkOutput(output(dataType<TClassType>(), ctResult));
				Matrix<Float, TIdx> conf = this->checkOutput(output(dtFloat, ctConfidence));
				Matrix<Float, TIdx> cors = this->checkOutput(output(dtFloat, ctCorrelation));

				this->setStage("Train");
				this->setMinProgress(0);
				this->setMaxProgress(classes.vec().count());
				this->setProgress(0);			
				for (auto c = classes.vec().begin(); c != classes.vec().end(); c++) {
					Matrix<Float, TIdx> in_part = in(tar == *c);
					Matrix<Float, TIdx> pat;
					DSFunc::colMean(pat, in_part);
					patterns_ ^ pat;
					this->incProgress();
				}

				this->setStage("Classify");
				this->setMinProgress(1);
				this->setMaxProgress(9);
				this->setProgress(1);
				//Classify
				Array<Float, TIdx> afin(in, oColMajor);			
				Array<Float, TIdx> afpat(patterns_, oColMajor);
				Array<Float, TIdx> afcov(oColMajor);
				Array<Float, TIdx> afcor(oColMajor);
				Array<Float, TIdx> afinsd(oColMajor);
				Array<Float, TIdx> afpatsd(oColMajor);
				Array<Float, TIdx> afsd(oColMajor);

				//Define some dimensions
				unsigned int nsamples = (unsigned int)afin.arr().dims(0);
				unsigned int nclasses = ~classes;
				unsigned int ndim = in.cols.count();

				//Calculate covariance on centered data				
				afin = (afin.arr() - tile(af::mean(afin, 1), 1, ndim));
				this->incProgress();
				afpat = (afpat.arr() - tile(af::mean(afpat, 1), 1, ndim));
				this->incProgress();
				afcov = af::matmul(afin, afpat, AF_MAT_NONE, AF_MAT_TRANS);
				this->incProgress();
				afcov = afcov.arr() / (Float)ndim;
				this->incProgress();

				//Normalise (Pearson)
				afinsd = af::stdev(afin, 1);
				this->incProgress();
				afpatsd = af::stdev(afpat, 1);
				this->incProgress();
				afsd = af::matmul(afinsd, afpatsd, AF_MAT_NONE, AF_MAT_TRANS);
				this->incProgress();
				afcor = afcov.arr() / afsd.arr();
				this->incProgress();

				//Calculate result in DDSL
				if (afcor.arr().dims(0) != cors.rows.count() || afcor.arr().dims(1) != cors.cols.count())
					throw Error(ecInternal, "train", SS("Dimensions returned from ArrayFire do not agree with output slice: " << afcor.arr().dims(0) << "x" << afcor.arr().dims(1) << " != " << cors.rows.count() << "x" << cors.cols.count()));
				cors = afcor.mat(); //DSA
				auto retIt = ret.rows->begin();
				auto confIt = conf.rows->begin();

				this->setStage("Result");
				this->setMinProgress(0);
				this->setMaxProgress(cors.rows.count());
				this->setProgress(0);
				for (auto r = cors.rows.begin(); r!= cors.rows.end(); r++) {
					Float max = -1;
					TIdx pat = 0;
					TIdx idx = 0;
					Float runnerUp=-1;
					Float runner=0;
					for (auto c = r->begin(); c!=r->end(); c++, idx++) {						
						if (*c > max) {
							runner = max;
							max = *c;
							pat = idx;
						} else {
							runner = *c;
						}
						if (runner > runnerUp) {
							runnerUp = runner;
						}
					}					
					*retIt = classes.vec(pat);
					*confIt = abs(max - runnerUp) / 2; //max correlation span is 2
					retIt++;
					confIt++;
					this->incProgress();
				}
			} catch (af::exception &e) {
				throw Error(ecExternalLibrary, "Correlation::train", e.what());
			}
		}

		template<typename TClassType, typename TIdx, typename TId>
		void Correlation<TClassType, TIdx, TId>::apply(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
			try {
				Matrix<TClassType, TIdx> classes = this->template parameterValueById<Matrix<TClassType, TIdx>>("Classes");
				Matrix<Float, TIdx> in = input(dtFloat, ctFeature);			
				Matrix<TClassType, TIdx> ret = output(dataType<TClassType>(), ctResult);
				Matrix<Float, TIdx> conf = output(dtFloat, ctConfidence);
				Matrix<Float, TIdx> cors = output(dtFloat, ctCorrelation);
				this->setStage("Classify");
				this->setMinProgress(1);
				this->setMaxProgress(9);
				this->setProgress(1);
				//Classify
				Array<Float, TIdx> afin(in, oColMajor);			
				Array<Float, TIdx> afpat(patterns_, oColMajor);
				Array<Float, TIdx> afcov(oColMajor);
				Array<Float, TIdx> afcor(oColMajor);
				Array<Float, TIdx> afinsd(oColMajor);
				Array<Float, TIdx> afpatsd(oColMajor);
				Array<Float, TIdx> afsd(oColMajor);

				//Define some dimensions
				unsigned int nsamples = (unsigned int)afin.arr().dims(0);
				unsigned int nclasses = ~classes;
				unsigned int ndim = in.cols.count();

				//Calculate covariance on centered data				
				afin = (afin.arr() - tile(af::mean(afin, 1), 1, ndim));
				this->incProgress();
				afpat = (afpat.arr() - tile(af::mean(afpat, 1), 1, ndim));
				this->incProgress();
				afcov = af::matmul(afin, afpat, AF_MAT_NONE, AF_MAT_TRANS);
				this->incProgress();
				afcov = afcov.arr() / (Float)ndim;
				this->incProgress();

				//Normalise (Pearson)
				afinsd = af::stdev(afin, 1);
				this->incProgress();
				afpatsd = af::stdev(afpat, 1);
				this->incProgress();
				afsd = af::matmul(afinsd, afpatsd, AF_MAT_NONE, AF_MAT_TRANS);
				this->incProgress();
				afcor = afcov.arr() / afsd.arr();
				this->incProgress();

				//Calculate result in DDSL
				if (afcor.arr().dims(0) != cors.rows.count() || afcor.arr().dims(1) != cors.cols.count())
					throw Error(ecInternal, "train", SS("Dimensions returned from ArrayFire do not agree with output slice: " << afcor.arr().dims(0) << "x" << afcor.arr().dims(1) << " != " << cors.rows.count() << "x" << cors.cols.count()));
				cors = afcor.mat(); //DSA
				auto retIt = ret.rows->begin();
				auto confIt = conf.rows->begin();

				this->setStage("Result");
				this->setMinProgress(0);
				this->setMaxProgress(cors.rows.count());
				this->setProgress(0);
				for (auto r = cors.rows.begin(); r!= cors.rows.end(); r++) {
					Float max = -1;
					TIdx pat = 0;
					TIdx idx = 0;
					Float runnerUp=-1;
					Float runner=0;
					for (auto c = r->begin(); c!=r->end(); c++, idx++) {						
						if (*c > max) {
							runner = max;
							max = *c;
							pat = idx;
						} else {
							runner = *c;
						}
						if (runner > runnerUp) {
							runnerUp = runner;
						}
					}					
					*retIt = classes.vec(pat);
					*confIt = abs(max - runnerUp) / 2; //max correlation span is 2
					retIt++;
					confIt++;
					this->incProgress();
				}
			} catch (af::exception &e) {
				throw Error(ecExternalLibrary, "Correlation::apply", e.what());
			}
		}
	
		template<typename TClassType, typename TIdx, typename TId>
		void Correlation<TClassType, TIdx, TId>::clone(const Correlation<TClassType, TIdx, TId> & other) {
			patterns_.clear();
			patterns_ = other.patterns_;
			Model<TIdx, TId>::clone(other);
		}

		template<typename TClassType, typename TIdx, typename TId>
		void Correlation<TClassType, TIdx, TId>::clone(Correlation<TClassType, TIdx, TId> && other) {
			patterns_.clear();
			patterns_ = std::move(other.patterns_);
			Model<TIdx, TId>::clone(std::move(other));
		}

		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_DESTRUCTOR_DEFAULT(Correlation, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_DEFAULT(Correlation, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_COPY(Correlation, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MOVE(Correlation, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_COPY(Correlation, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_MOVE(Correlation, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_COPY(Correlation, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_MOVE(Correlation, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY(Correlation, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE(Correlation, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY_2(Correlation, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE_2(Correlation, TClassType, TIdx, TId)
		template<typename TClassType, typename TIdx, typename TId> M_CCP_MODEL_OPERATOR_SLICE(Correlation, TClassType, TIdx, TId)
	}
}