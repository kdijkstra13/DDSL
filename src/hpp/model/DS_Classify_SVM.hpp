#pragma once
#include "h/model/DS_Classify_SVM.h"

#include "hpp/model/DS_Model.hpp"
#include "svm.h"


using namespace std;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSUtil;
using namespace DSLib;

namespace DSModel {

	//*********//
	//** SVM **//
	//*********//

	template<typename TClassType, typename TIdx, typename TId>
	DSTypes::Double SVM<TClassType, TIdx, TId>::classify(TClassType & res, const DSLib::CellsContinuous<Double, TIdx> &sample) {
		CellsContinuous<Double, TIdx>& smp = const_cast<CellsContinuous<Double, TIdx>&>(sample);
		double cf = 0;
		double ans = 0;
		svm_node* nodes = (struct svm_node*)malloc(sizeof(struct svm_node)*(smp.count() + 1));
		TIdx n = 0;
		for (auto cell = smp.begin(); cell != smp.end(); cell++, n++) {
			nodes[n].index = n;
			nodes[n].value = *cell;
		}
		nodes[n].index = -1;
		if (svm_check_probability_model(svm_) == 0) {
			ans = svm_predict(svm_, nodes);
		}
		else {
			double * conf = (double*)malloc(sizeof(double) * classes_.vec().count());
			ans = svm_predict_probability(svm_, nodes, conf);
			cf = conf[(TIdx)ans];
			delete conf;
		}
		res = classes_.vec((TIdx)ans);
		delete nodes;
		return cf;
	}

	template<typename TClassType, typename TIdx, typename TId>
	void SVM<TClassType, TIdx, TId>::updateParameters() {
		classes_ = this->template parameterValueById<Matrix<TClassType>>("Classes");
		weights_ = this->template parameterValueById<Matrix<Double>>("Weights");
		kt_ = (KernelType) this->template parameterValueById<UInt32>("KernelType");
		svmt_ = (SVMType) this->template parameterValueById<UInt32>("SVMType");
		degree_ = this->template parameterValueById<UInt32>("Degree");
		gamma_ = this->template parameterValueById<Double>("Gamma");
		coef0_ = this->template parameterValueById<Double>("Coef0");
		cost_ = this->template parameterValueById<Double>("Cost");
		nu_ = this->template parameterValueById<Double>("Nu");
		epsilon_ = this->template parameterValueById<Double>("Epsilon");
		cachesize_ = this->template parameterValueById<Double>("CacheSize");
		eTolerance_ = this->template parameterValueById<Double>("ETolerance");
		shrinking_ = this->template parameterValueById<UInt32>("Shrinking");
		probabilityEstimates_ = this->template parameterValueById<UInt32>("ProbabilityEstimates");
		classesIdx_.clear();
		TIdx idx = 0;
		for (auto cell = classes_.vec().begin(); cell != classes_.vec().end(); cell++, idx++)
			classesIdx_.insert(pair<TClassType, TIdx>(*cell, idx));

		svmparms_.C = cost_;
		svmparms_.cache_size = cachesize_;
		svmparms_.coef0 = coef0_;
		svmparms_.degree = degree_;
		svmparms_.eps = eTolerance_;
		svmparms_.gamma = gamma_;

		switch (kt_) {
		case ktLinear: svmparms_.kernel_type = 0; break;
		case ktRadialBasis: svmparms_.kernel_type = 1; break;
		case ktSigmoid: svmparms_.kernel_type = 2; break;
			//case ktPreComputed: parms.kernel_type = 3;break;
		default: throw Error(ecInvalidEnum, "updateParameters", SS("Unsupported KernelType: " << kt_));
		}

		svmparms_.nu = nu_;
		svmparms_.p = epsilon_;
		svmparms_.probability = probabilityEstimates_;
		svmparms_.shrinking = shrinking_;
		svmparms_.nr_weight = weights_.vec().count();
		if (svmparms_.nr_weight > 0) {
			svmparms_.weight = (double*)realloc(svmparms_.weight, svmparms_.nr_weight*sizeof(double));
			svmparms_.weight_label = (int*)realloc(svmparms_.weight_label, svmparms_.nr_weight*sizeof(int));
			for (int i = 0; i<svmparms_.nr_weight; i++) {
				svmparms_.weight[i] = weights_.vec(i);
				svmparms_.weight_label[i] = i;
			}
		}

		switch (svmt_) {
		case svmtCSVC: svmparms_.svm_type = 0; break;
		case svmtNuSVC: svmparms_.svm_type = 1; break;
		case svmtOneClass: svmparms_.svm_type = 2; break;
			//case svmtEpsilonSVR : svmparms_.svm_type = 3;break;
			//case svmtNuSVR: svmparms_.svm_type = 4;break;
		default: throw Error(ecInvalidEnum, "updateParameters", SS("Unsupported SVM Type: " << svmt_));
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	void SVM<TClassType, TIdx, TId>::registerInputs(const Table<TIdx, TId> &table) {
		TClassType t;
		this->registerInput(dtDouble, ctFeature);
		this->registerInput(dataType(t), ctTarget);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void SVM<TClassType, TIdx, TId>::registerOutputs(const Table<TIdx, TId> &input) {
		Matrix<TClassType> classes = this->template parameterValueById<Matrix<TClassType>>("Classes");
		Table<TIdx, TId> output = (ctResult | (Matrix<TClassType>(oColMajor) | classes.val(0, 0))) | (ctConfidence | (Matrix<Double>(oColMajor) | 0.0));
		output.resizeRows(0);
		this->registerOutput(output);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void SVM<TClassType, TIdx, TId>::train(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
		TClassType t;
		Matrix<TClassType> out = output(dataType(t), ctResult);
		Matrix<Double> conf = output(dtDouble, ctConfidence);
		Matrix<Double> feat = input(dtDouble, ctFeature);
		Matrix<TClassType> target = input(dataType(t), ctTarget);
		if (target.cols.count() != 1)
			throw Error(ecIncompatible, "SVM.train", "Should have one ctTarget col");

		//Malloc data for SVM
		//Calculate gamma
		if (gamma_ == 0) {
			gamma_ = 1.0 / ~feat(0); //count cols
			this->template parameterValueById<Double>("Gamma") = gamma_;
		}
		//realloc data
		prob_.l = ~feat[0]; //count rows
		prob_.y = (double*)realloc(prob_.y, prob_.l*sizeof(double));
		prob_.x = (struct svm_node**)realloc(prob_.x, prob_.l*sizeof(struct svm_node*));
		nodecnt_ = ~input + prob_.l; //prob_.l for the terminators
		nodes_ = (struct svm_node*)realloc(nodes_, nodecnt_ * sizeof(svm_node));

		//Copy input to SVM problem
		TIdx n = 0;
		for (TIdx r = 0; r<feat.rows.count(); r++) {
			prob_.x[r] = &(nodes_[n]);
			auto classidx = classesIdx_.find(target.vec(r));
			if (classidx != classesIdx_.end())
				prob_.y[r] = classidx->second;
			else
				throw Error(ecIncompatible, "train", SS("Class not found in class list: " << target.vec(r)));
			for (TIdx c = 0; c<feat.cols.count(); c++) {
				nodes_[n].index = c;
				nodes_[n].value = feat.val(r, c);
				n++;
			}
			nodes_[n].index = -1;
			n++;
		}

		//Train SVM
		svm_ = svm_train(&prob_, &svmparms_);

		//Classify with SVM
		auto re = out.rows->begin();
		auto cf = conf.rows->begin();
		for (typename Matrix<Double>::Index::Iterator r = feat.rows.begin(); r != feat.rows.end(); r++) {
			*cf = classify(*re, *r);
			re++;
			cf++;
		}
		
	}

	template<typename TClassType, typename TIdx, typename TId>
	void SVM<TClassType, TIdx, TId>::apply(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
		TClassType t;
		Matrix<TClassType> out = output(dataType(t), ctResult);
		Matrix<Double> conf = output(dtDouble, ctConfidence);
		Matrix<Double> feat = input(dtDouble, ctFeature);
		auto re = out.rows->begin();
		auto cf = conf.rows->begin();
		for (typename Matrix<Double>::Index::Iterator r = feat.rows.begin(); r != feat.rows.end(); r++) {
			*cf = classify(*re, *r);
			re++;
			cf++;
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	void SVM<TClassType, TIdx, TId>::readModel(std::istream &input, const DSTypes::String &filename) {
		resetModel();
		svm_ = svm_load_model(String(filename + ".svm").c_str());
	}

	template<typename TClassType, typename TIdx, typename TId>
	void SVM<TClassType, TIdx, TId>::writeModel(std::ostream &output, const DSTypes::String &filename) {
		if (svm_ == nullptr)
			throw Error(ecNotFound, "writeModel", "No trained SVM model present");
		svm_save_model(String(filename + ".svm").c_str(), svm_);
	}

	template<typename TClassType, typename TIdx, typename TId>
	SVM<TClassType, TIdx, TId>::SVM(const Matrix<TClassType, TIdx>& classes, const Matrix<Double, TIdx> &weights, KernelType kt, SVMType svmt, DSTypes::UInt32 shrinking, DSTypes::UInt32 probabilityEstimates, DSTypes::Double cachesize, DSTypes::Double gamma, DSTypes::UInt32 degree, DSTypes::Double coef0, DSTypes::Double cost, DSTypes::Double nu, DSTypes::Double epsilon, DSTypes::Double eTolerance){
		Table<TIdx, TId> tab =
			(Matrix<Literal, TIdx>() | "Val") |
			((Matrix<Literal, TIdx>() | "Classes") ^ (ctParameter | (Matrix<Matrix<TClassType>>() | (Matrix<TClassType>() | classes))) |
			(Matrix<Literal, TIdx>() | "Weights") ^ (ctParameter | (Matrix<Matrix<Double>>() | (Matrix<Double>() | weights))) |
			(Matrix<Literal, TIdx>() | "KernelType") ^ (ctParameter | (Matrix<UInt32>() | (UInt32)kt)) |
			(Matrix<Literal, TIdx>() | "SVMType") ^ (ctParameter | (Matrix<UInt32>() | (UInt32)svmt)) |
			(Matrix<Literal, TIdx>() | "Degree") ^ (ctParameter | (Matrix<UInt32>() | degree)) |
			(Matrix<Literal, TIdx>() | "Shrinking") ^ (ctParameter | (Matrix<UInt32>() | shrinking)) |
			(Matrix<Literal, TIdx>() | "ProbabilityEstimates") ^ (ctParameter | (Matrix<UInt32>() | probabilityEstimates)) |
			(Matrix<Literal, TIdx>() | "Gamma") ^ (ctParameter | (Matrix<Double>() | gamma)) |
			(Matrix<Literal, TIdx>() | "Coef0") ^ (ctParameter | (Matrix<Double>() | coef0)) |
			(Matrix<Literal, TIdx>() | "Cost") ^ (ctParameter | (Matrix<Double>() | cost)) |
			(Matrix<Literal, TIdx>() | "Nu") ^ (ctParameter | (Matrix<Double>() | nu)) |
			(Matrix<Literal, TIdx>() | "Epsilon") ^ (ctParameter | (Matrix<Double>() | epsilon)) |
			(Matrix<Literal, TIdx>() | "CacheSize") ^ (ctParameter | (Matrix<Double>() | cachesize)) |
			(Matrix<Literal, TIdx>() | "ETolerance") ^ (ctParameter | (Matrix<Double>() | eTolerance)));
		svmparms_.weight = nullptr;
		svmparms_.weight_label = nullptr;
		nodes_ = nullptr;
		prob_.l = 0;
		prob_.x = nullptr;
		prob_.y = nullptr;
		svm_ = nullptr;
		this->registerParameters(tab);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void SVM<TClassType, TIdx, TId>::resetModel() {
		if (svmparms_.weight != nullptr) delete svmparms_.weight;
		if (svmparms_.weight_label != nullptr) delete svmparms_.weight_label;
		if (nodes_ != nullptr) delete nodes_;
		if (prob_.x != nullptr) delete prob_.x;
		if (prob_.y != nullptr) delete prob_.y;
		if (svm_ != nullptr) svm_free_and_destroy_model(&svm_);
		svmparms_.weight = nullptr;
		svmparms_.weight_label = nullptr;
		nodes_ = nullptr;
		prob_.x = nullptr;
		prob_.y = nullptr;
		svm_ = nullptr;
	}

	template<typename TClassType, typename TIdx, typename TId>
	SVM<TClassType, TIdx, TId>::~SVM() {
		resetModel();
	}

	template<typename TClassType, typename TIdx, typename TId>
	void SVM<TClassType, TIdx, TId>::clone(const SVM<TClassType, TIdx, TId> & other) {
		//Copy parameters
		classesIdx_ = other.classesIdx_;
		classes_ = other.classes_;
		weights_ = other.weights_;

		svmt_ = other.svmt_;
		kt_ = other.kt_;

		degree_ = other.degree_;
		gamma_ = other.gamma_;
		coef0_ = other.coef0_;
		cost_ = other.cost_;
		nu_ = other.nu_;
		epsilon_ = other.epsilon_;
		cachesize_ = other.cachesize_;
		eTolerance_ = other.eTolerance_;
		shrinking_ = other.shrinking_;
		probabilityEstimates_ = other.probabilityEstimates_;

		//Copy svm parameters
		svmparms_ = other.svmparms_;
		svmparms_.weight = (double*)malloc(svmparms_.nr_weight*sizeof(double));
		svmparms_.weight_label = (int*)malloc(svmparms_.nr_weight*sizeof(int));
		memcpy(svmparms_.weight, other.svmparms_.weight, svmparms_.nr_weight*sizeof(double));
		memcpy(svmparms_.weight_label, other.svmparms_.weight_label, svmparms_.nr_weight*sizeof(int));

		//Alloc prepared data if present
		prob_ = other.prob_;
		nodecnt_ = other.nodecnt_;
		prob_.y = (double*)malloc(prob_.l);
		prob_.x = (svm_node**)malloc(sizeof(struct svm_node*) * prob_.l);
		nodes_ = (struct svm_node*)malloc(sizeof(struct svm_node) * nodecnt_);

		//Copy prepared data
		memcpy(prob_.y, other.prob_.y, prob_.l);
		memcpy(nodes_, other.nodes_, nodecnt_);
		TIdx n = 0;
		for (int r = 0; r<prob_.l; r++) {
			prob_.x[r] = &(nodes_[n]);
			for (int c = 0; c<(nodecnt_ - r) / r; c++) {
				nodes_[n].index = other.nodes_[n].index;
				nodes_[n].value = other.nodes_[n].value;
				n++;
			}
			if (nodes_[n].index != -1)
				throw Error(ecInternal, "clone", "Data not sane.");
			n++;
		}
		Model<TIdx, TId>::clone(other);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void SVM<TClassType, TIdx, TId>::clone(SVM<TClassType, TIdx, TId> && other) {
		//Move parameters
		classesIdx_ = other.classesIdx_;
		classes_ = other.classes_;
		weights_ = other.weights_;

		svmt_ = other.svmt_;
		kt_ = other.kt_;

		degree_ = other.degree_;
		gamma_ = other.gamma_;
		coef0_ = other.coef0_;
		cost_ = other.cost_;
		nu_ = other.nu_;
		epsilon_ = other.epsilon_;
		cachesize_ = other.cachesize_;
		eTolerance_ = other.eTolerance_;
		shrinking_ = other.shrinking_;
		probabilityEstimates_ = other.probabilityEstimates_;

		//Move svm parameters. Move semantics at work for c code :-)
		svmparms_ = other.svmparms_;
		nodecnt_ = other.nodecnt_;
		nodes_ = other.nodes_;
		prob_ = other.prob_;

		other.svmparms_.nr_weight = 0;
		other.svmparms_.weight = nullptr;
		other.svmparms_.weight_label = nullptr;
		other.prob_.l = 0;
		other.prob_.x = nullptr;
		other.prob_.y = nullptr;
		other.nodes_ = nullptr;
		Model<TIdx, TId>::clone(std::move(other));
	}

	//template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_DESTRUCTOR_DEFAULT(SVM, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_DEFAULT(SVM, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_COPY(SVM, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MOVE(SVM, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_COPY(SVM, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_MOVE(SVM, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_FILE(SVM, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_COPY(SVM, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_MOVE(SVM, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY(SVM, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE(SVM, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY_2(SVM, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE_2(SVM, TClassType, TIdx, TId)
	//template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_COPY(SVM, TClassType, TIdx, TId)
	//template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MOVE(SVM, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CCP_MODEL_OPERATOR_SLICE(SVM, TClassType, TIdx, TId)
}