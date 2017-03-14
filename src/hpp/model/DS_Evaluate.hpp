#pragma once

#include "h/model/DS_Evaluate.h"

#include "hpp/model/DS_Model.hpp"

using namespace std;
using namespace DSTypes;
using namespace DSLang;
using namespace DSUtil;
using namespace DSLib;

namespace DSFunc {
	template <typename TIdx, typename TId>
	Table<TIdx, TId> confusionFrac(Table<TIdx, TId> &conf, TIdx digits, Double mult, bool perClass) {
		try {
			Matrix<TIdx> in = conf.template mat<TIdx>(0);
			Matrix<Double> out;
			if (in.rows.count() != in.cols.count())
				throw Error(ecIncompatible, "confusionFrac", "Not a confusion matrix");
			convert(out, in);
			if (perClass) {
				for (TIdx c=0;c<out.cols.count();c++) {		
					(out[c] / sum(out[c])) * mult;
					auto out2 = out[c];
					DSFunc::round(out2, digits);
				}
			} else {
				out / sum(out) * mult;
				DSFunc::round(out, digits);
			}
			return (conf.rows.ids() | (conf.cols.ids() ^ (ctConfusion | out)));
		} catch (const exception e) {
			throw Error(ecGeneral, "confusionFrac", e.what());
		}
	}
}

namespace DSModel {

	/*********************/
	/** Class Confusion **/
	/*********************/
	template<typename TClassType, typename TIdx, typename TId>
	Confusion<TClassType, TIdx, TId>::Confusion(const Matrix<TClassType, TIdx> &classes) {
		Table<TIdx, TId> tab = ((Matrix<Literal, TIdx>() | "Val") |
			((Matrix<Literal, TIdx>() | "Classes") ^
				(ctParameter | (Matrix<Matrix<TClassType>>() | (Matrix<TClassType>() | classes)))
				)
			);
		this->registerParameters(tab);		
	};

	template<typename TClassType, typename TIdx, typename TId>
	void Confusion<TClassType, TIdx, TId>::updateParameters() {
		this->template parameterValueById<Matrix<TClassType>>("Classes"); //check if exists
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Confusion<TClassType, TIdx, TId>::registerInputs(const Table<TIdx, TId> &table) {
		this->registerInput(dataType<TClassType>(), (dtContentType|ctTarget|ctResult));	
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Confusion<TClassType, TIdx, TId>::registerOutputs(const Table<TIdx, TId> &input) {
		Matrix<UInt32, TIdx> outputmat;		
		Matrix<TClassType> classes = this->template parameterValueById<Matrix<TClassType>>("Classes");
		this->addStages(dtString | "Prepare" | "Calculate");

		outputmat.resize(classes.vec().count(), classes.vec().count());
		outputmat = 0U;
		Matrix<TId, TIdx> classNames;
		convert(classNames, classes);
		Table<TIdx, TId> output = classNames ^ (classNames | (ctConfusion | outputmat));
		this->sampleCount(~classNames);
		this->registerOutput(output);
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Confusion<TClassType, TIdx, TId>::train(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
		Matrix<TClassType, TIdx> eval = this->checkInput(input(ctTarget), "Target");
		Matrix<TClassType, TIdx> result = this->checkInput(input(ctResult), "Result");
		Matrix<UInt32, TIdx> out = output;
		Matrix<TClassType> classes = this->template parameterValueById<Matrix<TClassType>>("Classes");

		if (eval.cols.count() > 1)
			throw Error(ecIncompatible, "Confusion::train()", "Multiple target columns");
		if (result.cols.count() > 1)
			throw Error(ecIncompatible, "Confusion::train()", "Multiple result columns");
		this->setStage("Prepare");
		this->setMinProgress(0);
		this->setMaxProgress(1);
		eval |= eval;
		result |= result;
		(eval | result);
		this->setStage("Calculate");
		this->setMinProgress(0);
		this->setMaxProgress(~classes * ~classes);
		//Reference at the x-axis (fixed)
		//Classifier result ar the y-axis (variable)
		for (TIdx r=0;r<~classes;r++) {
			for (TIdx c = 0;c<~classes;c++) {
				auto mask = (Matrix<TClassType, TIdx>() | classes.vec(r) | classes.vec(c));
				out(c,1,r,1) = (dtUInt32 | ~(eval == mask));
				this->incProgress();
			}
		}
	}

	template<typename TClassType, typename TIdx, typename TId>
	void Confusion<TClassType, TIdx, TId>::apply(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
		train(table, input, output);
	}

	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_DESTRUCTOR_DEFAULT(Confusion, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_DEFAULT(Confusion, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_COPY(Confusion, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MOVE(Confusion, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_COPY(Confusion, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_MOVE(Confusion, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_FILE(Confusion, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_COPY(Confusion, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_MOVE(Confusion, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY(Confusion, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE(Confusion, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY_2(Confusion, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE_2(Confusion, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_COPY(Confusion, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CPP_MODEL_CLONE_MOVE(Confusion, TClassType, TIdx, TId)
	template<typename TClassType, typename TIdx, typename TId> M_CCP_MODEL_OPERATOR_SLICE(Confusion, TClassType, TIdx, TId)
}