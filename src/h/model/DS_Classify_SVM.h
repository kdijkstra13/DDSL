#pragma once
#include "DS_Model.h"
#include "svm.h"

namespace DSModel {

	/*	0 -- linear: u'*v\n"
	1 -- polynomial: (gamma*u'*v + coef0)^degree
	2 -- radial basis function: exp(-gamma*|u-v|^2)
	3 -- sigmoid: tanh(gamma*u'*v + coef0)
	4 -- precomputed kernel (kernel values in training_set_file) */
	enum KernelType { ktLinear, klPolynomial, ktRadialBasis, ktSigmoid, ktPreComputed };

	/*	0 -- C-SVC		(multi-class classification)
	1 -- nu-SVC		(multi-class classification)
	2 -- one-class SVM
	3 -- epsilon-SVR	(regression)
	4 -- nu-SVR		(regression) */
	enum SVMType { svmtCSVC, svmtNuSVC, svmtOneClass, svmtEpsilonSVR, svmtNuSVR };

	template<typename TClassType = DSTypes::UInt32, typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
	class SVM : public Model<TIdx, TId> {
	private:
		svm_model * svm_;
		svm_problem prob_;
		svm_parameter svmparms_;
		int nodecnt_;
		svm_node *nodes_;

		SVMType svmt_;
		KernelType kt_;

		DSTypes::UInt32 degree_; //degree in kernel function (default 3)
		DSTypes::Double gamma_; //gamma in kernel function (default 1/num_features)
		DSTypes::Double coef0_; //coef0 in kernel function (default 0)
		DSTypes::Double cost_; //the parameter C of C-SVC, epsilon-SVR, and nu-SVR (default 1)
		DSTypes::Double nu_; //the parameter nu of nu-SVC, one-class SVM, and nu-SVR (default 0.5)
		DSTypes::Double epsilon_; //the epsilon in loss function of epsilon-SVR (default 0.1)
		DSTypes::Double cachesize_; //cache memory size in MB (default 100)
		DSTypes::Double eTolerance_; //tolerance of termination criterion (default 0.001)
		DSTypes::UInt32 shrinking_; //whether to use the shrinking heuristics, 0 or 1 (default 1)
		DSTypes::UInt32 probabilityEstimates_; //whether to train a SVC or SVR model for probability estimates, 0 or 1 (default 0)

		DSLib::Matrix<DSTypes::Double, TIdx>  weights_; //the parameter C of class i to weight*C, for C-SVC (default 1)	
		DSLib::Matrix<TClassType, TIdx> classes_;
		std::map<TClassType, TIdx> classesIdx_;

		DSTypes::Double classify(TClassType &res, const DSLib::CellsContinuous<DSTypes::Double, TIdx> &sample);
	protected:
		void updateParameters() override;
		void registerInputs(const DSLib::Table<TIdx, TId> &table) override;
		void registerOutputs(const DSLib::Table<TIdx, TId> &input) override;
		void train(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) override;
		void apply(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) override;
		void readModel(std::istream &input, const DSTypes::String &filename) override;
		void writeModel(std::ostream &output, const DSTypes::String &filename) override;
	public:
		void resetModel() override;

		using Model<TIdx, TId>::train;
		using Model<TIdx, TId>::apply;

		SVM(const DSLib::Matrix<TClassType, TIdx> &classes, const DSLib::Matrix<DSTypes::Double, TIdx> &weights = DSLib::Matrix<DSTypes::Double, TIdx>(),
			KernelType kt = ktLinear,
			SVMType svmt = svmtCSVC,
			DSTypes::UInt32 shrinking = 0,
			DSTypes::UInt32 probabilityEstimates = 1,
			DSTypes::Double cachesize = 100,
			DSTypes::Double gamma = 0,
			DSTypes::UInt32 degree = 3,
			DSTypes::Double coef0 = 0,
			DSTypes::Double cost = 1,
			DSTypes::Double nu = 0.5,
			DSTypes::Double epsilon = 0.1,
			DSTypes::Double eTolerance = 0.001
			);

		M_HDR_MODEL_DESTRUCTOR(SVM)
		M_HDR_MODEL_CONSTRUCTOR(SVM, TClassType, TIdx, TId)
		M_HDR_MODEL_CLONE(SVM, TClassType, TIdx, TId)
		M_HDR_MODEL_ASSIGNMENT(SVM, TClassType, TIdx, TId)
		M_HDR_MODEL_OPERATOR(SVM, TClassType, TIdx, TId)
	};
}