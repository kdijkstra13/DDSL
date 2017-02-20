#pragma once
#include "h/model/DS_Transform.h"

#include "hpp/model/DS_Model.hpp"

#ifndef DDSL_NO_EXT_LIB_PNG
	#include "hpp/lib/DS_Image_PNG.hpp"
#endif

using namespace std;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSUtil;
using namespace DSLib;

namespace DSModel {
	/*****************/
	/** Class Paste **/
	/*****************/
	template<typename TIdx, typename TId>
	Paste<TIdx, TId>::Paste(const Table<TIdx, TId> &tab) {
		tab_ = const_cast<Table<TIdx, TId>&>(tab);
	};

	template<typename TIdx, typename TId>
	Paste<TIdx, TId>::Paste(const DSTypes::String &filename) {
		std::ifstream ifs(filename, ios::binary);
		if (!ifs)
			throw Error(ecNotFound, "Paste::Paste()", SS("File not found: " << filename));
		this->readParams(ifs);
		this->readModel(ifs, filename);
	}

	template<typename TIdx, typename TId>
	void Paste<TIdx, TId>::clone(const Paste<TIdx, TId> & other) {
		tab_ = other.tab_;
		Model<TIdx, TId>::clone(other);
	}

	template<typename TIdx, typename TId>
	void Paste<TIdx, TId>::clone(Paste<TIdx, TId> && other) {
		tab_ = std::move(other.tab_);
		Model<TIdx, TId>::clone(std::move(other));
	}

	template<typename TIdx, typename TId>
	void Paste<TIdx, TId>::registerInputs(const Table<TIdx, TId> &table) {
	}

	template<typename TIdx, typename TId>
	void Paste<TIdx, TId>::registerOutputs(const Table<TIdx, TId> &input) {
		this->registerOutput(tab_);
	}

	template<typename TIdx, typename TId>
	void Paste<TIdx, TId>::train(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
		if (output.isSameStructure(tab_)) {
			output = tab_; //DSA
		} else {
			throw Error(ecIncompatible, "Paste::train()", SS("Table size (" << endl << tab_.printLess() << ") differs from output size (" << endl << output.printLess() << ")"));
		}
	}

	template<typename TIdx, typename TId>
	void Paste<TIdx, TId>::apply(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
		train(table, input, output);
	}

	template<typename TIdx, typename TId>
	void Paste<TIdx, TId>::resetModel() {
		tab_.clear();
	}

	template<typename TIdx, typename TId>
	void Paste<TIdx, TId>::readModel(std::istream &input, const DSTypes::String &filename) {
		this->updateParameters();
		tab_.read(input);
	}

	template<typename TIdx, typename TId>
	void Paste<TIdx, TId>::writeModel(std::ostream &output, const DSTypes::String &filename) {
		tab_.write(output);
	}

	template<typename TIdx, typename TId> M_CPP_MODEL_DESTRUCTOR_DEFAULT(Paste, TIdx, TId)
	template<typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_DEFAULT(Paste, TIdx, TId)
	template<typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_COPY(Paste, TIdx, TId)
	template<typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MOVE(Paste, TIdx, TId)
	template<typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_COPY(Paste, TIdx, TId)
	template<typename TIdx, typename TId> M_CPP_MODEL_CONSTRUCTOR_MODEL_MOVE(Paste, TIdx, TId)
	template<typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_COPY(Paste, TIdx, TId)
	template<typename TIdx, typename TId> M_CPP_MODEL_ASSIGNMENT_MOVE(Paste, TIdx, TId)
	template<typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY(Paste, TIdx, TId)
	template<typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE(Paste, TIdx, TId)
	template<typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_COPY_2(Paste, TIdx, TId)
	template<typename TIdx, typename TId> M_CPP_MODEL_CLONE_MODEL_MOVE_2(Paste, TIdx, TId)
	template<typename TIdx, typename TId> M_CCP_MODEL_OPERATOR_SLICE(Paste, TIdx, TId)


	/*******************/
	/** Class ZScore **/
	/*******************/
	template<typename TIdx, typename TId>
	ZScore<TIdx, TId>::ZScore(const DSTypes::String &filename) {		
		std::ifstream ifs(filename, ios::binary);
		if (!ifs)
			throw Error(ecNotFound, "ZScore::ZScore()", SS("File not found: " << filename));
		this->readParams(ifs);
		this->readModel(ifs, filename);
	}

	template<typename TIdx, typename TId>
	ZScore<TIdx, TId>::ZScore(const ZScore<TIdx, TId> & other) {
		clone(other);
	}

	template<typename TIdx, typename TId>
	ZScore<TIdx, TId>::ZScore(ZScore<TIdx, TId> && other) {
		clone(std::move(other));		
	}

	template<typename TIdx, typename TId>
	ZScore<TIdx, TId>::ZScore(const Model<TIdx, TId>& other) {
		clone(dynamic_cast<const ZScore<TIdx, TId> &>(other));
	}

	template<typename TIdx, typename TId>
	ZScore<TIdx, TId>::ZScore(Model<TIdx, TId>&& other) {
		clone(std::move(dynamic_cast<ZScore<TIdx, TId> &>(other)));
	}

	template<typename TIdx, typename TId>
	ZScore<TIdx, TId> & ZScore<TIdx, TId>::operator=(const ZScore<TIdx, TId> & other) {
		clone(other);
		return *this;
	}

	template<typename TIdx, typename TId>
	ZScore<TIdx, TId> & ZScore<TIdx, TId>::operator=(ZScore<TIdx, TId> && other) {
		clone(std::move(other));
		return *this;
	}

	template<typename TIdx, typename TId>
	void ZScore<TIdx, TId>::clone(const ZScore<TIdx, TId> & other) {
		mean_ = other.mean_;
		sd_ = other.sd_;
		Model<TIdx, TId>::clone(other);
	}

	template<typename TIdx, typename TId>
	void ZScore<TIdx, TId>::clone(ZScore<TIdx, TId> && other) {
		mean_ = std::move(other.mean_);
		sd_ = std::move(other.sd_);
		Model<TIdx, TId>::clone(std::move(other));
	}

	template<typename TIdx, typename TId>
	ZScore<TIdx, TId> ZScore<TIdx, TId>::operator()(const TIdx count) {
		ZScore<TIdx, TId> m;
		this->copy(count, m);
		return std::move(m);
	}

	template<typename TIdx, typename TId>
	void ZScore<TIdx, TId>::registerInputs(const Table<TIdx, TId> &table) {
		this->registerInput(dtDouble, ctFeature);
		#ifndef DDSL_NO_EXT_LIB_PNG
			this->registerInput(dtImagePNGDouble, ctFeature);
			this->registerInput(dtImagePNGFloat, ctFeature);
		#endif
		this->registerInput(dtMatrixDouble, ctFeature);
		this->registerInput(dtMatrixFloat, ctFeature);
	}

	template<typename TIdx, typename TId>
	void ZScore<TIdx, TId>::registerOutputs(const Table<TIdx, TId> &input) {
		Table<TIdx, TId> output = input;
		this->addStages(dtString | "Mean" | "SD" | "Copy" | "Apply");
		output.resizeRows(0);
		Matrix<TId, TIdx>() ^ output;
		this->registerOutput(output);
	}
	
	template <typename TIdx, typename TId>
	template <typename T>
	void ZScore<TIdx, TId>::calcFeatures_(Matrix<T, TIdx> &&in, Matrix<T, TIdx> &&out, bool train) {		
		Matrix<T, TIdx> mean;
		Matrix<T, TIdx> sd;
		if (train) {
			this->setStage("Mean");
			DSFunc::colMean(mean, in);
			this->setStage("SD");		
			DSFunc::colSD(sd, in, mean);
			mean_ | (ctFeature | mean);
			sd_ | (ctFeature | sd);
		} else {
			mean = mean_(dataType<T>(), ctFeature);
			sd = sd_(dataType<T>(), ctFeature);
		}
		this->setStage("Copy");
		out = in; //Direct slice assignment copy
		this->setStage("Apply");
		(out - mean) / sd;
	}

	template <typename TIdx, typename TId>
	template <typename T, template <typename, typename> class TMat>
	void ZScore<TIdx, TId>::calcMatFeatures_(Matrix<TMat<T, TIdx>, TIdx> &&in, Matrix<TMat<T, TIdx>, TIdx> &&out, bool train) {
		Matrix<TMat<T, TIdx>, TIdx> mean;
		Matrix<TMat<T, TIdx>, TIdx> sd;
		if (train) {
			this->setStage("Mean");
			DSFunc::colMean(mean, in);
			this->setStage("SD");
			DSFunc::colSD(sd, in, mean);
			mean_ | (ctFeature | mean);
			sd_ | (ctFeature | sd);
		} else {
			mean = (Matrix<TMat<T, TIdx>>) mean_(dataType<TMat<T, TIdx>>(), ctFeature);
			sd = (Matrix<TMat<T, TIdx>>) sd_(dataType<TMat<T, TIdx>>(), ctFeature);
		}		
		this->setStage("Copy");
		out = in;
		this->setStage("Apply");
		(out - mean) / sd;
	}

	template<typename TIdx, typename TId>
	void ZScore<TIdx, TId>::train(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
		if (input.cols.count() == 0)
			throw Error(ecIncompatible, "train", "ZScore has no effect");
		mean_.clear();
		sd_.clear();
		if (~input(dtDouble, ctFeature) != 0)
			calcFeatures_<Double>(input(dtDouble, ctFeature), output(dtDouble, ctFeature), true);
		if (~input(dtMatrixDouble, ctFeature) != 0)
			calcMatFeatures_<Double, Matrix>(input(dtMatrixDouble, ctFeature), output(dtMatrixDouble, ctFeature), true);
		if (~input(dtFloat, ctFeature) != 0)
			calcFeatures_<Float>(input(dtFloat, ctFeature), output(dtFloat, ctFeature), true);
		if (~input(dtMatrixFloat, ctFeature) != 0)
			calcMatFeatures_<Float, Matrix>(input(dtMatrixFloat, ctFeature), output(dtMatrixFloat, ctFeature), true);
		#ifndef DDSL_NO_EXT_LIB_PNG
		if (~input(dtImagePNGDouble, ctFeature) != 0)
			calcMatFeatures_<Double, ImagePNG>(input(dtImagePNGDouble, ctFeature), output(dtImagePNGDouble, ctFeature), true);		
		if (~input(dtImagePNGFloat, ctFeature) != 0)
			calcMatFeatures_<Float, ImagePNG>(input(dtImagePNGFloat, ctFeature), output(dtImagePNGFloat, ctFeature), true);
		#endif

	}

	template<typename TIdx, typename TId>
	void ZScore<TIdx, TId>::apply(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
		if (input.cols.count() == 0)
			throw Error(ecIncompatible, "train", "ZScore has no effect");
		if (~input(dtDouble, ctFeature) != 0)
			calcFeatures_<Double>(input(dtDouble, ctFeature), output(dtDouble, ctFeature), false);
		if (~input(dtMatrixDouble, ctFeature) != 0)
			calcMatFeatures_<Double, Matrix>(input(dtMatrixDouble, ctFeature), output(dtMatrixDouble, ctFeature), false);
		if (~input(dtFloat, ctFeature) != 0)
			calcFeatures_<Float>(input(dtFloat, ctFeature), output(dtFloat, ctFeature), false);
		if (~input(dtMatrixFloat, ctFeature) != 0)
			calcMatFeatures_<Float, Matrix>(input(dtMatrixFloat, ctFeature), output(dtMatrixFloat, ctFeature), false);
		
		#ifndef DDSL_NO_EXT_LIB_PNG
		if (~input(dtImagePNGDouble, ctFeature) != 0)
			calcMatFeatures_<Double, ImagePNG>(input(dtImagePNGDouble, ctFeature), output(dtImagePNGDouble, ctFeature), false);
		if (~input(dtImagePNGFloat, ctFeature) != 0)
			calcMatFeatures_<Float, ImagePNG>(input(dtImagePNGFloat, ctFeature), output(dtImagePNGFloat, ctFeature), false);
		#endif

	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId>* ZScore<TIdx, TId>::clone() const {
		auto * m = new ZScore<TIdx, TId>();
		m->clone(*this);
		return m;
	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId>* ZScore<TIdx, TId>::move() {
		auto * m = new ZScore<TIdx, TId>();
		m->clone(std::move(*this));
		return m;
	}

	template<typename TIdx, typename TId>
	void ZScore<TIdx, TId>::resetModel() {
		sd_.clear();
		mean_.clear();
	}

	template<typename TIdx, typename TId>
	void ZScore<TIdx, TId>::readModel(std::istream &input, const DSTypes::String &filename) {
		this->updateParameters();
		mean_.read(input);
		sd_.read(input);
	}
	
	template<typename TIdx, typename TId>
	void ZScore<TIdx, TId>::writeModel(std::ostream &output, const DSTypes::String &filename) {
		mean_.write(output);
		sd_.write(output);
	}

	/********************/
	/** Class MMScale **/
	/********************/

	template<typename TIdx, typename TId>
	MMScale<TIdx, TId>::MMScale(const MMScale<TIdx, TId> & other) {
		clone(other);		
	}

	template<typename TIdx, typename TId>
	MMScale<TIdx, TId>::MMScale(MMScale<TIdx, TId> && other) {
		clone(std::move(other));		
	}

	template<typename TIdx, typename TId>
	MMScale<TIdx, TId>::MMScale(const Model<TIdx, TId> & other) {
		clone(dynamic_cast<const MMScale<TIdx, TId> &>(other));
	}

	template<typename TIdx, typename TId>
	MMScale<TIdx, TId>::MMScale(Model<TIdx, TId> && other) {
		clone(std::move(dynamic_cast<MMScale<TIdx, TId> &>(other)));
	}

	template<typename TIdx, typename TId>
	MMScale<TIdx, TId> & MMScale<TIdx, TId>::operator=(const MMScale<TIdx, TId> & other) {
		clone(other);
		return *this;
	}

	template<typename TIdx, typename TId>
	MMScale<TIdx, TId> & MMScale<TIdx, TId>::operator=(MMScale<TIdx, TId> && other) {
		clone(std::move(other));
		return *this;
	}

	template<typename TIdx, typename TId>
	void MMScale<TIdx, TId>::clone(const MMScale<TIdx, TId> & other) {
		min_ = other.min_;
		diff_ = other.diff_;
		Model<TIdx, TId>::clone(other);
	}

	template<typename TIdx, typename TId>
	void MMScale<TIdx, TId>::clone(MMScale<TIdx, TId> && other) {
		min_ = std::move(other.min_);
		diff_ = std::move(other.diff_);
		Model<TIdx, TId>::clone(std::move(other));		
	}

	template<typename TIdx, typename TId>
	MMScale<TIdx, TId> MMScale<TIdx, TId>::operator()(const TIdx count) {
		MMScale<TIdx, TId> m;
		this->copy(count, m);
		return std::move(m);
	}

	template<typename TIdx, typename TId>
	void MMScale<TIdx, TId>::registerInputs(const Table<TIdx, TId> &table) {
		this->registerInput(dtDouble, ctFeature);
		this->registerInput(dtMatrixDouble, ctFeature);
		this->registerInput(dtFloat, ctFeature);
		this->registerInput(dtMatrixFloat, ctFeature);

		#ifndef DDSL_NO_EXT_LIB_PNG
			this->registerInput(dtImagePNGDouble, ctFeature);
			this->registerInput(dtImagePNGFloat, ctFeature);
		#endif
	}

	template<typename TIdx, typename TId>
	void MMScale<TIdx, TId>::registerOutputs(const Table<TIdx, TId> &input) {
		Table<TIdx, TId> output = input;
		this->addStages(dtString | "Min" | "Max" | "Copy" | "Apply");
		output.resizeRows(0);
		Matrix<TId, TIdx>() ^ output;
		this->registerOutput(output);
	}

	template<typename TIdx, typename TId>
	void MMScale<TIdx, TId>::resetModel() {
		min_.clear();
		diff_.clear();
	}

	template <typename TIdx, typename TId>
	template <typename T>
	void MMScale<TIdx, TId>::calcFeatures_(Matrix<T, TIdx> &&in, Matrix<T, TIdx> &&out, bool train) {		
		Matrix<T, TIdx> min;
		Matrix<T, TIdx> diff;
		if (train) {
			this->setStage("Min");
			colMin(min, in);
			this->setStage("Max");		
			colMax(diff, in);
			diff - min;
			min_ | (ctFeature | min);
			diff_ | (ctFeature | diff);
		} else {
			T dummy;
			min = min_(dataType(dummy), ctFeature);
			diff = diff_(dataType(dummy), ctFeature);
		}
		this->setStage("Copy");
		out = in;
		this->setStage("Apply");
		(out - min) / diff;
	}

	template <typename TIdx, typename TId>
	template <typename T, template <typename, typename> class TMat>
	void MMScale<TIdx, TId>::calcMatFeatures_(Matrix<TMat<T, TIdx>, TIdx> &&in, Matrix<TMat<T, TIdx>, TIdx> &&out, bool train) {
		Matrix<TMat<T, TIdx>, TIdx> min;
		Matrix<TMat<T, TIdx>, TIdx> diff;
		TMat<T, TIdx> dummy;
		if (train) {
			this->setStage("Min");
			colMin(min, in);
			this->setStage("Max");
			colMax(diff, in);
			diff - min;
			min_ | (ctFeature | min);
			diff_ | (ctFeature | diff);
		} else {
			min = min_(dataType(dummy), ctFeature);
			diff = diff_(dataType(dummy), ctFeature);
		}
		this->setStage("Copy");
		out = in;
		this->setStage("Apply");
		(out - min) / diff;
	}

	template<typename TIdx, typename TId>
	void MMScale<TIdx, TId>::train(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
		min_.clear();
		diff_.clear();
		if (~input(dtDouble, ctFeature) != 0)
			calcFeatures_<Double>(input(dtDouble, ctFeature), output(dtDouble, ctFeature), true);
		if (~input(dtMatrixDouble, ctFeature) != 0)
			calcMatFeatures_<Double, Matrix>(input(dtMatrixDouble, ctFeature), output(dtMatrixDouble, ctFeature), true);

		if (~input(dtFloat, ctFeature) != 0)
			calcFeatures_<Float>(input(dtFloat, ctFeature), output(dtFloat, ctFeature), true);
		if (~input(dtMatrixDouble, ctFeature) != 0)
			calcMatFeatures_<Float, Matrix>(input(dtMatrixFloat, ctFeature), output(dtMatrixFloat, ctFeature), true);
		
		#ifndef DDSL_NO_EXT_LIB_PNG
		if (~input(dtImagePNGDouble, ctFeature) != 0)
			calcMatFeatures_<Double, ImagePNG>(input(dtImagePNGDouble, ctFeature), output(dtImagePNGDouble, ctFeature), true);
		if (~input(dtImagePNGFloat, ctFeature) != 0)
			calcMatFeatures_<Float, ImagePNG>(input(dtImagePNGFloat, ctFeature), output(dtImagePNGFloat, ctFeature), true);
		#endif
	}

	template<typename TIdx, typename TId>
	void MMScale<TIdx, TId>::apply(const Table<TIdx, TId> &table, Table<TIdx, TId> &input, Table<TIdx, TId> &output) {
		if (~input(dtDouble, ctFeature) != 0)
			calcFeatures_<Double>(input(dtDouble, ctFeature), output(dtDouble, ctFeature), false);
		if (~input(dtMatrixDouble, ctFeature) != 0)
			calcMatFeatures_<Double, Matrix>(input(dtMatrixDouble, ctFeature), output(dtMatrixDouble, ctFeature), false);

		if (~input(dtFloat, ctFeature) != 0)
			calcFeatures_<Float>(input(dtFloat, ctFeature), output(dtFloat, ctFeature), false);
		if (~input(dtMatrixDouble, ctFeature) != 0)
			calcMatFeatures_<Float, Matrix>(input(dtMatrixFloat, ctFeature), output(dtMatrixFloat, ctFeature), false);

		#ifndef DDSL_NO_EXT_LIB_PNG
		if (~input(dtImagePNGDouble, ctFeature) != 0)
			calcMatFeatures_<Double, ImagePNG>(input(dtImagePNGDouble, ctFeature), output(dtImagePNGDouble, ctFeature), false);
		if (~input(dtImagePNGFloat, ctFeature) != 0)
			calcMatFeatures_<Float, ImagePNG>(input(dtImagePNGFloat, ctFeature), output(dtImagePNGFloat, ctFeature), false);
		#endif
	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId>* MMScale<TIdx, TId>::clone() const {
		auto * m = new MMScale<TIdx, TId>();
		m->clone(*this);
		return m;
	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId>* MMScale<TIdx, TId>::move() {
		auto * m = new MMScale<TIdx, TId>();
		m->clone(std::move(*this));
		return m;
	}

	template<typename TIdx, typename TId>
	void MMScale<TIdx, TId>::readModel(std::istream &input, const DSTypes::String &filename) {
		this->updateParameters();
		min_.read(input);
		diff_.read(input);
	}
	
	template<typename TIdx, typename TId>
	void MMScale<TIdx, TId>::writeModel(std::ostream &output, const DSTypes::String &filename) {
		min_.write(output);
		diff_.write(output);
	}

};
