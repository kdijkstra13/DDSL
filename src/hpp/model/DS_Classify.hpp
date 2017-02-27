#pragma once
#include "h/model/DS_Classify.h"

#include "hpp/model/DS_Model.hpp"

using namespace std;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSUtil;
using namespace DSLib;

namespace DSModel {
	/********************/
	/** Class Random **/
	/********************/

	template<typename TClassType, typename TIdx, typename TId>
	Random<TClassType, TIdx, TId>::Random(const Matrix<TClassType, TIdx> &classes) {
		Table<TIdx, TId> tab =	((Matrix<Literal, TIdx>() | "Val") | 
									((Matrix<Literal, TIdx>() | "Classes") ^
										(ctParameter | (Matrix<Matrix<TClassType>>() | (Matrix<TClassType>() | classes)))
									)
								);
		this->registerParameters(tab);		
	};

	template<typename TClassType, typename TIdx, typename TId>
	void Random<TClassType, TIdx, TId>::updateParameters() {
		this->template parameterValueById<Matrix<TClassType>>("Classes");
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Random<TClassType, TIdx, TId>::registerInputs(const Table<TIdx, TId> &table) {
		this->registerInput(dtDouble, ctFeature);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Random<TClassType, TIdx, TId>::registerOutputs(const Table<TIdx, TId> &input) {
		Matrix<TClassType> classes = this->template parameterValueById<Matrix<TClassType>>("Classes");		
		Table<TIdx, TId> output = (ctResult | (Matrix<TClassType>(oColMajor) | classes.val(0,0)));
		output.resizeRows(0);
		this->registerOutput(output);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Random<TClassType, TIdx, TId>::train(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
		Matrix<TClassType> classes = this->template parameterValueById<Matrix<TClassType>>("Classes");
		Matrix<TClassType> out = output(DSTypes::dataType(classes.val(0, 0)), ctResult);
		for (auto cellit = out.cols[0]->begin();cellit != out.cols[0]->end();cellit++)
			*cellit = classes.vec()((rand() % classes.vec().count()));
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Random<TClassType, TIdx, TId>::apply(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
		Matrix<TClassType> classes = this->template parameterValueById<Matrix<TClassType>>("Classes");
		Matrix<TClassType> out = output(DSTypes::dataType(classes.val(0, 0)), ctResult);
		for (auto cellit = out.cols[0]->begin();cellit != out.cols[0]->end();cellit++)
			*cellit = classes.vec()((rand() % classes.vec().count()));
	}

	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_DESTRUCTOR_DEFAULT(Random, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_DEFAULT(Random, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_COPY(Random, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MOVE(Random, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_COPY(Random, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_MOVE(Random, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_FILE(Random, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_COPY(Random, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_MOVE(Random, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY(Random, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE(Random, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_COPY(Random, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MOVE(Random, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CCP_MODEL_OPERATOR_SLICE(Random, TClassType, TIdx, TId)


	//****************//
	//** NaiveBayes **//
	//****************//

	template<typename TClassType, typename TIdx, typename TId>
	NaiveBayes<TClassType, TIdx, TId>::NaiveBayes(const DSTypes::String &filename) {		
		std::ifstream ifs(filename, ios::binary);
		if (!ifs)
			throw Error(ecNotFound, "NaiveBayes::NaiveBayes()", SS("File not found: " << filename));
		this->readParams(ifs);
		this->readModel(ifs, filename);
	}

	template<typename TClassType, typename TIdx, typename TId>
	NaiveBayes<TClassType, TIdx, TId>::NaiveBayes(const Matrix<TClassType, TIdx> &classes) {
		Table<TIdx, TId> tab = ((Matrix<Literal, TIdx>() | "Val") |
			((Matrix<Literal, TIdx>() | "Classes") ^
			(ctParameter | (Matrix<Matrix<TClassType>>() | (Matrix<TClassType>() | classes)))
				)
			);
		this->registerParameters(tab);
	};

	template<typename TClassType, typename TIdx, typename TId>
	void NaiveBayes<TClassType, TIdx, TId>::resetModel() {
		mean_.clear();
		var_.clear();
		prior_.clear();
	}

	template<typename TClassType, typename TIdx, typename TId>
	void NaiveBayes<TClassType, TIdx, TId>::readModel(std::istream &input, const DSTypes::String &filename) {
		updateParameters();
		TIdx cnt = this->template parameterValueById<Matrix<TClassType>>("Classes").vec().count();
		TClassType cls;
		DSLib::Matrix<DSTypes::Double, TIdx> mat;
		Double dbl;
		//Read mean_
		mean_.clear();
		for (TIdx i=0;i<cnt;i++) {
			DSUtil::read(input, cls);
			mat.read(input);
			mean_.insert(pair<TClassType, Matrix<Double, TIdx>>(cls, mat));
		}	
		//Read var_
		var_.clear();
		for (TIdx i=0;i<cnt;i++) {
			DSUtil::read(input, cls);
			mat.read(input);
			var_.insert(pair<TClassType, Matrix<Double, TIdx>>(cls, mat));
		}
		//Read prior_
		prior_.clear();
		for (TIdx i=0;i<cnt;i++) {
			DSUtil::read(input, cls);
			DSUtil::read(input, dbl);
			prior_.insert(pair<TClassType, Double>(cls, dbl));
		}
	}
	
	template<typename TClassType, typename TIdx, typename TId>
	void NaiveBayes<TClassType, TIdx, TId>::writeModel(std::ostream &output, const DSTypes::String &filename) {
		for (auto it=mean_.begin();it!=mean_.end();it++) {
			DSUtil::write(output, it->first);
			it->second.write(output);
		}
		for (auto it=var_.begin();it!=var_.end();it++) {
			DSUtil::write(output, it->first);
			it->second.write(output);
		}
		for (auto it=prior_.begin();it!=prior_.end();it++) {
			DSUtil::write(output, it->first);
			DSUtil::write(output, it->second);
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	DSTypes::Double NaiveBayes<TClassType, TIdx, TId>::classify(TClassType & res, const DSLib::CellsContinuous<Double, TIdx> &sample) {
		//normal = (1 / sqrt(2 * pi*(10 ^ 2)))*e^-(((x - 30) ^ 2) / (2 * 10))
		CellsContinuous<Double, TIdx>& smp = const_cast<CellsContinuous<Double, TIdx>&>(sample);
		map<TClassType, Double> likelihood;
		auto m=mean_.begin();
		auto v=var_.begin();
		//Double ccnt = (Double)prior_.size();
		for (auto p = prior_.begin();p!=prior_.end();p++,m++,v++) {			
			TClassType cls = p->first;
			auto pp = p->second;
			auto mm = m->second.vec().begin();
			auto vv = v->second.vec().begin();
			Double likely = 0;
			for (auto cell = smp.begin(); cell!=smp.end(); cell++, mm++, vv++) {
				Double scale = log(1.0 / sqrt(2.0 * C_PI * *vv));
				Double dist = -(pow(*cell - *mm, 2.0) / (2.0 * *vv));
				likely = likely + scale + dist;
			}			
			likelihood.insert(pair<TClassType, Double>(cls, pp * exp(likely)));
		}
		Double maxl=0;
		Double sum=0;
		for (auto l=likelihood.begin(); l!=likelihood.end();l++) {
			if (l->second >= maxl) {
				maxl = l->second;
				res = l->first;
			}
			sum += l->second;
		}
		return (maxl/sum); //normalise likelihood
	}

	template<typename TClassType, typename TIdx, typename TId>
	void NaiveBayes<TClassType, TIdx, TId>::updateParameters() {
		this->template parameterValueById<Matrix<TClassType>>("Classes");
	}

	template<typename TClassType, typename TIdx, typename TId>
	void NaiveBayes<TClassType, TIdx, TId>::registerInputs(const Table<TIdx, TId> &table) {
		TClassType t;
		this->registerInput(dtDouble, ctFeature);
		this->registerInput(dataType(t), ctTarget);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void NaiveBayes<TClassType, TIdx, TId>::registerOutputs(const Table<TIdx, TId> &input) {
		Matrix<TClassType> classes = this->template parameterValueById<Matrix<TClassType>>("Classes");
		Table<TIdx, TId> output = (ctResult | (Matrix<TClassType>(oColMajor) | classes.val(0, 0))) | (ctConfidence | (Matrix<Double>(oColMajor) | 0.0));
		output.resizeRows(0);
		this->registerOutput(output);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void NaiveBayes<TClassType, TIdx, TId>::train(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
		//Get data
		TClassType t;
		Matrix<TClassType> classes = this->template parameterValueById<Matrix<TClassType>>("Classes");
		Matrix<TClassType> out = output(dataType(t), ctResult);
		Matrix<Double> conf = output(dtDouble, ctConfidence);
		Matrix<Double> feat = input(dtDouble, ctFeature);
		Matrix<TClassType> target = input(dataType(t), ctTarget);
		if (target.cols.count() != 1)
			throw Error(ecIncompatible, "NaiveBayes.train", "Should have one ctTarget col");
		//Train
		mean_.clear();
		var_.clear();
		prior_.clear();
		for (auto c=classes.vec().begin(); c != classes.vec().end(); c++) {
			Matrix<Double> feat_class = feat(target == *c);
			if (~feat_class == 0)
				throw Error(ecIncompatible, "NaiveBayes::train", SS("Class " << *c << " contains no samples"));
			Matrix<Double> mean;
			Matrix<Double> var;
			DSFunc::colMean(mean, feat_class);
			DSFunc::colVar(var, feat_class, mean);
			mean_.insert(pair<TClassType, Matrix<Double>>(*c, mean));
			var_.insert(pair<TClassType, Matrix<Double>>(*c, var));
			prior_.insert(pair<TClassType, Double>(*c, (Double)feat_class.rows.count() / (Double)input.rows.count()));
		}
		//Classify
		auto re = out.rows->begin();
		auto cf = conf.rows->begin();
		for (typename Matrix<Double>::Index::Iterator r = feat.rows.begin();r != feat.rows.end();r++) {
			*cf = classify(*re, *r);
			re++;
			cf++;
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	void NaiveBayes<TClassType, TIdx, TId>::apply(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
		TClassType t;
		Matrix<TClassType> out = output(dataType(t), ctResult);
		Matrix<Double> conf = output(dtDouble, ctConfidence);
		Matrix<Double> feat = input(dtDouble, ctFeature);
		auto re = out.rows->begin();
		auto cf = conf.rows->begin();
		for (typename Matrix<Double>::Index::Iterator r = feat.rows.begin();r != feat.rows.end();r++) {
			*cf = classify(*re, *r);
			re++;
			cf++;
		}
	}
	

	template<typename TClassType, typename TIdx, typename TId>
	void NaiveBayes<TClassType, TIdx, TId>::clone(const NaiveBayes<TClassType, TIdx, TId> & other) {
		mean_.clear();
		var_.clear();
		prior_.clear();
		mean_.insert(other.mean_.begin(), other.mean_.end());
		var_.insert(other.var_.begin(), other.var_.end());
		prior_.insert(other.prior_.begin(), other.prior_.end());
		Model<TIdx, TId>::clone(other);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void NaiveBayes<TClassType, TIdx, TId>::clone(NaiveBayes<TClassType, TIdx, TId> && other) {
		mean_.clear();
		var_.clear();
		prior_.clear();
		mean_.insert(other.mean_.begin(), other.mean_.end());
		var_.insert(other.var_.begin(), other.var_.end());
		prior_.insert(other.prior_.begin(), other.prior_.end());
		Model<TIdx, TId>::clone(std::move(other));
	}

	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_DESTRUCTOR_DEFAULT(NaiveBayes, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_DEFAULT(NaiveBayes, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_COPY(NaiveBayes, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MOVE(NaiveBayes, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_COPY(NaiveBayes, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_MOVE(NaiveBayes, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_COPY(NaiveBayes, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_MOVE(NaiveBayes, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY(NaiveBayes, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE(NaiveBayes, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY_2(NaiveBayes, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE_2(NaiveBayes, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CCP_MODEL_OPERATOR_SLICE(NaiveBayes, TClassType, TIdx, TId)
}
