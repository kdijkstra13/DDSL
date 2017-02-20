#pragma once

#include "h/model/DS_Generate.h"

#include "hpp/model/DS_Model.hpp"

using namespace std;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSUtil;
using namespace DSLib;

namespace DSModel {
	template<typename T, typename TIdx, typename TId>
	void GenGauss<T, TIdx, TId>::makeDistrib_(const Matrix<T, TIdx>& eigenVectors, const Matrix<T, TIdx>& eigenValues, const Matrix<T, TIdx>& mean){
		if (!eigenVectors.isSquare())
			throw Error(ecSizeMismatch, "GenGauss", "eigenVectors should be a square matrix");
		if (!eigenVectors.isSameSize(eigenValues))
			throw Error(ecSizeMismatch, "GenGauss", "eigenVectors and eigenValues should have the same size");
		if (!mean.isVector())
			throw Error(ecIncompatible, "GenGauss", "mean should be a vector");
		if (eigenVectors.cols.count() != mean.vec().count())
			throw Error(ecSizeMismatch, "GenGauss", "mean has the wrong amount of dimensions");
		Matrix<T, TIdx> evec;
		Matrix<T, TIdx> eval;
		//force to oColMajor for faster matrix multiplication
		evec |= eigenVectors; 
		eval |= eigenValues;
		Table<TIdx, TId> parms =	(Matrix<Literal, TIdx>() | "Value") | (
										((Matrix<Literal, TIdx>() | "eigenVectors")	^ (ctParameter | (Matrix<Matrix<T, TIdx>, TIdx>() | evec))) | 
										((Matrix<Literal, TIdx>() | "eigenValues")	^ (ctParameter | (Matrix<Matrix<T, TIdx>, TIdx>() | eval))) |
										((Matrix<Literal, TIdx>() | "mean")			^ (ctParameter | (Matrix<Matrix<T, TIdx>, TIdx>() | mean)))										
									);
		this->registerParameters(parms);
	}

	template<typename T, typename TIdx, typename TId>
	void GenGauss<T, TIdx, TId>::updateParameters() {}

	template<typename T, typename TIdx, typename TId>
	void GenGauss<T, TIdx, TId>::registerInputs(const Table<TIdx, TId>& table) {}

	
	template<typename T, typename TIdx, typename TId>
	void GenGauss<T, TIdx, TId>::registerOutputs(const Table<TIdx, TId>& input) {
		const TIdx d = this->template parameterValueById<Matrix<T>>("eigenValues").cols.count();
		Matrix<T> op(oColMajor);
		op.resize(TIdx(0), d);
		Table<TIdx, TId> output = (ctFeature | op);
		this->registerOutput(output);
		this->addStages(dtString | "Generate" | "Transform" | "Copy" | "Mean");
	}
	
	template<typename T, typename TIdx, typename TId>
	void GenGauss<T, TIdx, TId>::train(const Table<TIdx, TId>& table, Table<TIdx, TId>& input, Table<TIdx, TId>& output) {
		Matrix<T, TIdx> & eigenVectors = this->template parameterValueById<Matrix<T, TIdx>>("eigenVectors");
		Matrix<T, TIdx> & eigenValues = this->template parameterValueById<Matrix<T, TIdx>>("eigenValues");
		Matrix<T, TIdx> & mean = this->template parameterValueById<Matrix<T, TIdx>>("mean");			
		random_device rd;
		mt19937 gen(rd());
		Matrix<T, TIdx> & m = output.template mat<T>(0);
		this->setStage("Generate");
		this->setMinProgress(0);
		this->setMaxProgress(m.cols.count());

		for (TIdx c = 0;c<m.cols.count();c++) {
			for (TIdx r=0;r<m.rows.count();r++) {			
				m.val(r, c) = nd_(gen);
			}
			this->incProgress();			
		}
		this->nextStage();
		Matrix<T, TIdx> m2 = m;
		Matrix<T, TIdx> ev = eigenValues; !ev;
		pow(ev, T(0.5));
		m2 *= ev; //Stretch points with eigen values
		m2 *= eigenVectors; //Rotate points with eigen vectors
		if (!m2.isSameSize(m))
			throw Error(ecInternal, "train", "Not the same amount of dimensions after applying eigenValues and eigenVectors (Are all eigenVectors and eigenValues present?)");
		this->nextStage();
		m = m2; //Direct slice assignment if the above check holds.
		this->nextStage();
		m + mean; //Add means		
	}

	template<typename T, typename TIdx, typename TId>
	void GenGauss<T, TIdx, TId>::apply(const Table<TIdx, TId>& table, Table<TIdx, TId>& input, Table<TIdx, TId>& output) {
		train(table, input, output);
	}

	template<typename T, typename TIdx, typename TId>
	Model<TIdx, TId>* GenGauss<T, TIdx, TId>::clone() const {
		auto * m = new GenGauss<T, TIdx, TId>();
		m->clone(*this);
		return m;
	}

	template<typename T, typename TIdx, typename TId>
	Model<TIdx, TId>* GenGauss<T, TIdx, TId>::move() {
		auto * m = new GenGauss<T, TIdx, TId>();
		m->clone(std::move(*this));
		return m;
	}

	template<typename T, typename TIdx, typename TId>
	GenGauss<T, TIdx, TId>::GenGauss(const Matrix<T, TIdx>& eigenVectors, const Matrix<T, TIdx>& eigenValues, const Matrix<T, TIdx>& mean) {
		Matrix<T, TIdx> ev;
		makeDistrib_(eigenVectors, diag(ev, eigenValues), mean);
	}

	template<typename T, typename TIdx, typename TId>
	GenGauss<T, TIdx, TId>::GenGauss(const Matrix<T, TIdx>& deviation, const Matrix<T, TIdx>& mean) {
		if (!mean.isVector())
			throw Error(ecIncompatible, "GenGauss", "mean should be a vector");
		if (!deviation.isSameSize(mean))
			throw Error(ecIncompatible, "GenGauss", "mean and deviation should have the same size");
		Matrix<T, TIdx> dev = deviation;!dev;pow(dev, T(2));
		Matrix<T, TIdx> eigenVectors;
		Matrix<T, TIdx> eigenValues;
		diag(eigenValues, dev);
		eye(eigenVectors, mean.vec().count());
		makeDistrib_(eigenVectors, eigenValues, mean);
	}

	template<typename T, typename TIdx, typename TId>
	GenGauss<T, TIdx, TId>::GenGauss(const GenGauss & other) {
		clone(other);
	}

	template<typename T, typename TIdx, typename TId>
	GenGauss<T, TIdx, TId>::GenGauss(GenGauss && other) {
		clone(std::move(other));
	}

	template<typename T, typename TIdx, typename TId>
	GenGauss<T, TIdx, TId>::GenGauss(const Model<TIdx, TId>& other) {
		clone(dynamic_cast<const GenGauss<TIdx, TId> &>(other));
	}

	template<typename T, typename TIdx, typename TId>
	GenGauss<T, TIdx, TId>::GenGauss(Model<TIdx, TId>&& other) {
		clone(std::move(dynamic_cast<GenGauss<TIdx, TId> &>(other)));
	}

	template<typename T, typename TIdx, typename TId>
	GenGauss<T, TIdx, TId> & GenGauss<T, TIdx, TId>::operator=(const GenGauss<T, TIdx, TId> & other) {
		clone(other);
		return *this;		
	}

	template<typename T, typename TIdx, typename TId>
	GenGauss<T, TIdx, TId> & GenGauss<T, TIdx, TId>::operator=(GenGauss<T, TIdx, TId> && other) {
		clone(std::move(other));
		return *this;
	}

	template<typename T, typename TIdx, typename TId>
	void GenGauss<T, TIdx, TId>::clone(const GenGauss & other) {
		Model<TIdx, TId>::clone(other);
	}

	template<typename T, typename TIdx, typename TId>
	void GenGauss<T, TIdx, TId>::clone(GenGauss && other) {
		Model<TIdx, TId>::clone(std::move(other));
	}

	template<typename T, typename TIdx, typename TId>
	GenGauss<T, TIdx, TId> GenGauss<T, TIdx, TId>::operator()(const TIdx count) {
		GenGauss<TIdx, TId> m;
		this->copy(count, m);
		return std::move(m);
	}
	
};
