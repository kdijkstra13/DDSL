#pragma once

#include "h/model/DS_Model.h"

using namespace std;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSUtil;
using namespace DSLib;

namespace DSModel {
	#define ASYNCPOLICY std::launch::async
	//#define ASYNCPOLICY std::launch::deferred

	template <typename TIdx, typename TId> 
	Table<TIdx, TId> & Model<TIdx, TId>::checkInput(Table<TIdx, TId> &tab) {
		if (~tab == 0)
			throw Error(ecIncompatible, "checkInput", "A model input is not present. Is the correct passThroughType specified?");
		return tab;
	}

	template <typename TIdx, typename TId> 
	Table<TIdx, TId> && Model<TIdx, TId>::checkInput(Table<TIdx, TId> &&tab) {
		return std::move(checkInput(tab));
	}

	template <typename TIdx, typename TId> 
	Table<TIdx, TId> & Model<TIdx, TId>::checkOutput(Table<TIdx, TId> &tab) {
		if (~tab == 0)
			throw Error(ecIncompatible, "checkOutput", "A model output is not present. Is the correct passThroughType specified?");
		return tab;
	}

	template <typename TIdx, typename TId> 
	Table<TIdx, TId> && Model<TIdx, TId>::checkOutput(Table<TIdx, TId> &&tab) {
		return std::move(checkInput(tab));
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::print(std::ostream & output) const {
		output << "/*Model*/" << endl;
		const_cast<Model<TIdx, TId>*>(this)->first().print_(output, *this);
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::printLess(std::ostream & output) const {
		const_cast<Model<TIdx, TId>*>(this)->first().printLess_(output);
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::printLess_(std::ostream & output) const {
		if (prev_ != nullptr) {
			switch (execType_) {
			case etSynchronous: output << " | ";break;
			case etAsynchronous: output << " ^ ";break;
			}
		}
		switch (ptt_) {
		case pttNone: output << "-";break;
		case pttAll: output << "+";break;
		case pttRelevant: break;
		}
		TIdx tidx;
		TId tid;
		output << modelName()  << "<" << typeName(tidx) << "," << typeName(tid) << ">()";

		if (next_ != nullptr)
			next_->printLess_(output);
		else
			output << endl;
	}


	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::printProgress(std::ostream & output) {
		first().printProgress_(output);
	}
	
	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::print_(std::ostream & output, const Model<TIdx, TId> &printer) const {		
		if (prev_ != nullptr) {
			switch (execType_) {
			case etSynchronous: output << " |" << endl;break;
			case etAsynchronous: output << " ^" << endl;break;
			}
		}
		if (this == &printer)
			output << "//*Printer*//" << endl;
		switch (ptt_) {
		case pttNone: output << "-";break;
		case pttAll: output << "+";break;
		case pttRelevant: break;
		}
		stringstream ss;
		parms_.print(ss);
		string s = ss.str();
		TIdx tidx;
		TId tid;
		output << modelName()  << "<" << typeName(tidx) << "," << typeName(tid) << ">(";
		if (parms_.isInitialized()) {			
			output << endl;
			parms_.print(output);
		}
		output << ")";

		if (next_ != nullptr)
			next_->print_(output, printer);
		else
			output << endl;
	}

	template<typename TIdx, typename TId>
	TIdx Model<TIdx, TId>::count_(TIdx &count) const {
		if (next_ == nullptr)
			return count;
		return next_->count_(++count);
	}

	template<typename TIdx, typename TId>
	TIdx Model<TIdx, TId>::modelIndex_(TIdx & idx) const {
		if (prev_ != nullptr) {
			return prev_->modelIndex_(++idx);
		} else
			return idx;
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::registerParameters(const Table<TIdx, DSTypes::Literal> &parms) {
		parms_.clear();
		parms_ = parms;
		updateParameters();
	}

	template<typename TIdx, typename TId>
	DSTypes::String Model<TIdx, TId>::print() const {
		stringstream ss;
		print(ss);
		return ss.str();
	}

	template<typename TIdx, typename TId>
	DSTypes::String Model<TIdx, TId>::modelName() const {
		string name = typeid(*this).name();
		name = name.substr(name.find_first_of(':') + 2, name.find_first_of('<') - name.find_first_of(':') - 2);
		return name;
	}

	template<typename TIdx, typename TId>
	TIdx Model<TIdx, TId>::modelIndex() const {
		TIdx idx=0;
		return modelIndex_(idx);
	}
		
	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::registerOutput(Table<TIdx, TId> &proto) {
		this->cols.add(proto);
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::registerInput(const DataType dt, const ContentType ct) { //Only strict dt/ct pair
		inputDataTypes_ | dt;
		inputContentTypes_ | ct;
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::registerInput(const DataType dt, const DSLib::Matrix<ContentType> cts) { //Strict dt, but any of the cts
		inputDataTypes_ = (dtDataType | dt);
		inputContentTypes_ = cts;
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::registerInput(const DSLib::Matrix<DataType> dts, const ContentType ct) { //strict ct, but any of the dts
		inputContentTypes_ = (dtContentType | ct);
		inputDataTypes_ = dts;
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::registerInput(const DSLib::Matrix<DSTypes::ContentType> cts) { //any of the cts
		inputDataTypes_.clear();
		inputContentTypes_ = cts;
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::registerInput(const DSLib::Matrix<DSTypes::DataType> dts) { //any of the dts
		inputContentTypes_.clear();
		inputDataTypes_ = dts;
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::registerInput(const DSTypes::ContentType ct) {
		inputDataTypes_.clear();
		inputContentTypes_ = (dtContentType | ct);
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::registerInput(const DSTypes::DataType dt) {
		inputContentTypes_.clear();
		inputDataTypes_ = (dtDataType | dt);		
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::registerInputAll() {
		inputAll_ = true;
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::printProgress_(std::ostream & output) {		
		progress_.print(output);
		if (next_ != nullptr)
			next_->printProgress_(output);
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::addStages(const DSLib::Matrix<String>& stages) {		
		auto stgs = stages;!stgs;
		for (auto it=stgs.vec().begin(); it!= stgs.vec().end(); it++)
			*it = modelName() + "." + *it;
		progress_.addStages(stgs);
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::setMinProgress(Double min) {
		progress_.min(min);
		progress_.progress(min);
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::setMaxProgress(Double max) {
		progress_.max(max);
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::setStage(const DSTypes::String & stage) {
		progress_.setStage(modelName() + "." + stage);
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::setStage(const DSTypes::String &stage, DSTypes::Double min, DSTypes::Double max) {
		setStage(stage);
		setMinProgress(min);
		setMaxProgress(max);
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::setStage(const DSTypes::String &stage, DSTypes::Double min, DSTypes::Double max, DSTypes::Double progress) {
		setStage(stage);
		setMinProgress(min);
		setMaxProgress(max);
		setProgress(progress);
	}


	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::nextStage() {
		progress_.nextStage();
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::setStageDone() {
		progress_.stageDone();
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::incProgress(DSTypes::Double amount) {
		progress_.incProgress(amount);
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::setProgress(DSTypes::Double progress) {
		progress_.progress(progress);
	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId>::Model() {		
		ptt_ = pttRelevant;
		execType_ = etSynchronous;
		next_ = nullptr;
		prev_ = nullptr;
	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId>::Model(const Table<TIdx, DSTypes::Literal>& parms) {
		registerParameters(parms);
	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId>::Model(const Model<TIdx, TId> &other) {
		if (this != &other)
			clone(other);
	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId>::Model(Model<TIdx, TId> && other) {
		if (this != &other)
			clone(other);
	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId> & Model<TIdx, TId>::operator=(const Model<TIdx, TId> & other) {
		if (this == &other)
			return *this;
		clone(other);
		return *this;
	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId> & Model<TIdx, TId>::operator=(Model<TIdx, TId> && other){
		if (this == &other)
			return *this;
		clone(std::move(other));
		return *this;
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::clone(const Model<TIdx, TId> & other) {		
		if (this != &other) {
			Table<TIdx, TId>::clone(other);

			progress_ = other.progress_;

			inputContentTypes_ = other.inputContentTypes_;
			inputDataTypes_ = other.inputDataTypes_;

			inputIndices_ = other.inputIndices_;
			outputIndices_ = other.outputIndices_;

			inputSlice_ = other.inputSlice_;
			outputSlice_ = other.outputSlice_;
			
			parms_ = other.parms_;

			ptt_ = other.ptt_;
			execType_ = other.execType_;

			//remove connections when copying Models.
			prev_ = nullptr;
			next_ = nullptr;

			srcTable_ = other.srcTable_;

			timeTrain_ = other.timeTrain_;
			timeApply_ = other.timeApply_;
		}
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::clone(Model<TIdx, TId> && other) {
		if (this != &other) {
			Table<TIdx, TId>::clone(std::move(other)); //TODO need testing, added std::move
			
			progress_ = std::move(other.progress_);

			inputContentTypes_ = std::move(other.inputContentTypes_);
			inputDataTypes_ = std::move(other.inputDataTypes_);

			inputIndices_ = std::move(other.inputIndices_);
			outputIndices_ = std::move(other.outputIndices_);

			inputSlice_ = std::move(other.inputSlice_);
			outputSlice_ = std::move(other.outputSlice_);
			
			parms_ = std::move(other.parms_);

			ptt_ = other.ptt_;
			execType_ = other.execType_;			

			//Take over connections when moving Models (move semantics)			
			if (other.prev_ != nullptr)
				other.prev_->next_ = this;
			prev_ = other.prev_;
			other.prev_ = nullptr;

			if (other.next_ != nullptr)
				other.next_->prev_ = this;
			next_ = other.next_;
			other.next_ = nullptr;

			srcTable_ = std::move(other.srcTable_);

			timeTrain_ = other.timeTrain_;
			timeApply_ = other.timeApply_;
		}
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::read(const DSTypes::String &filename) {
		ifstream ifs(filename, ios::binary);
		if (!ifs)
			throw Error(ecNotFound ,"Model::read()", SS("File not found " << filename));
		readParams(ifs);
		readModel(ifs, filename);
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::write(const DSTypes::String &filename) {
		ofstream ofs(filename, ios::binary);
		if (!ofs)
			throw Error(ecNotFound ,"Model::write()", SS("File not found " << filename));
		writeParams(ofs);
		writeModel(ofs, filename);		
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::writeParams(std::ostream &output) {
		DSTypes::String m = "Model";
		DSTypes::String mn = modelName();
		DSUtil::write(output, m);
		DSUtil::write(output, mn);
		DSUtil::write(output, ptt_);
		DSUtil::write(output, execType_);
		parms_.write(output);
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::readParams(std::istream &input) {
		DSTypes::String m;
		DSUtil::read(input, m);
		if (m != "Model")
			throw Error(ecIncompatible, "Model::read_()", SS("Invalid file type: " << m));
		DSTypes::String mn;
		DSUtil::read(input, mn);
		if (mn != modelName())
			throw Error(ecIncompatible, "Model::read_()", SS("Invalid model type: " << mn << ", should be " << modelName()));
		DSUtil::read(input, ptt_);
		DSUtil::read(input, execType_);
		parms_.read(input);
		updateParameters();
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::readParams(const DSTypes::String &filename) {
		ifstream ifs(filename, ios::binary);
		if (!ifs)
			throw Error(ecNotFound, "Model::readParams", SS("File not found: " << filename));
		readParams(ifs);
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::writeParams(const DSTypes::String &filename) {
		ofstream ofs(filename, ios::binary);
		if (!ofs)
			throw Error(ecNotFound, "Model::writeParams", SS("File not found: " << filename));
		writeParams(ofs);
	}


	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::next(const Model<TIdx, TId>& next) {
		if (next_ != nullptr && next_->prev_ != this)
			throw Error(ecInternal, "Model::next()", "next_->prev_ and this disagree");
		if (next_ != nullptr)
			delete next_;
		next_ = next.clone();
		next_->prev_ = this;
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::next(Model<TIdx, TId>&& next) {
		if (next_ != nullptr && next_->prev_ != this)
			throw Error(ecInternal, "Model::next()", "next_->prev_ and this disagree");
		if (next_ != nullptr)
			delete next_;
		next_ = next.move();
		next_->prev_ = this;
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::prev(const Model<TIdx, TId>& prev) {
		if (prev_ != nullptr && prev_->next_ != this)
			throw Error(ecInternal, "Model::prev()", "prev_->next_ and this disagree");
		if (prev_ != nullptr)
			delete prev_;
		prev_ = prev.clone();
		prev_->next_ = this;
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::prev(Model<TIdx, TId>&& prev) {
		if (prev_ != nullptr && prev_->next_ != this)
			throw Error(ecInternal, "Model::prev()", "prev_->next_ and this disagree");
		if (prev_ != nullptr)
			delete prev_;
		prev_ = prev.move();
		prev_->next_ = this;
	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId>& Model<TIdx, TId>::last() {
		if (next_ != nullptr) 
			return next_->last(); 
		else 
			return *this; 	
	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId>& Model<TIdx, TId>::first() {		
		if (prev_ != nullptr) 
			return prev_->first(); 
		else 
			return *this;
	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId>& Model<TIdx, TId>::next(TIdx index) {
		if (index == 0)
			return *this;
		if (next_ == nullptr)
			throw Error(ecRangeError, "Model::next", "Out of range at " + modelName());
		return next_->next(--index);
	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId> & Model<TIdx, TId>::copy_(TIdx count, Model<TIdx, TId>& dst) {
		if (count == 0)
			return dst;	
		dst.last().next(*this);
		if (next_ == nullptr && count > 1)
			throw Error(ecRangeError, "Model::copy", "Out of range at " + modelName());
		return next_->copy_(--count, dst);
	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId> & Model<TIdx, TId>::copy(TIdx count, Model<TIdx, TId>& dst) {
		if (count == 0)
			count = this->count();
		dst = *this;
		if (next_ == nullptr)
			return dst;
		else
			return next_->copy_(--count, dst);
	}

	template<typename TIdx, typename TId>
	Double Model<TIdx, TId>::timeAllTrain() {				
		Model<TIdx, TId> * m = &(this->first());
		Double t= m->timeTrain_;
		while (m->next_ != nullptr) {			
			m = m->next_;
			t += m->timeTrain_;
		}
		return t;
	}

	template<typename TIdx, typename TId>
	Double Model<TIdx, TId>::timeAllApply() {
		Model<TIdx, TId> * m = &(this->first());
		Double t = m->timeApply_;
		while (m->next_ != nullptr) {			
			m = m->next_;
			t += m->timeApply_;
		}
		return t;
	}

	template<typename TIdx, typename TId>
	TIdx Model<TIdx, TId>::count() const {
		TIdx c=1;
		return count_(c);
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::wait() {
		if (future_.valid()) {
			future_.get();
			progress_.done();
		}
		if (prev_ != nullptr)
			prev_->wait();
	}

	template<typename TIdx, typename TId>
	bool Model<TIdx, TId>::wait(UInt32 ms) {
		auto start = std::chrono::system_clock::now();

		bool done=false;
		if (future_.valid()) {
			if (future_.wait_for(std::chrono::milliseconds(ms)) == std::future_status::ready) {
				future_.get();
				progress_.done();
				done = true;
			}
		} else 
			done = true;
		if (prev_ != nullptr) {
			auto end = std::chrono::system_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
			if (elapsed.count() >= ms)
				return false;
			return done && prev_->wait(ms - (TIdx)elapsed.count());
		} else
			return done;
	}

	template <typename TIdx, typename TId>
	void Model<TIdx, TId>::progressLoop(std::ostream &output, DSTypes::UInt32 updateTimeMs) {
		while (!this->last().wait(updateTimeMs)) {
			this->printProgress(output);
		}
		this->printProgress(output);
	}
	
	template <typename TIdx, typename TId>
	void Model<TIdx, TId>::reset() {
		progress_.setStage(0);
		resetModel();
	}

	template <typename TIdx, typename TId>
	void Model<TIdx, TId>::resetAll_() {
		reset();
		if (next_ != nullptr) {
			next_->resetAll_();
		}
	}

	template <typename TIdx, typename TId>
	void Model<TIdx, TId>::resetAll() {
		if (prev_ != nullptr) {
			first().resetAll();
		} else {
			resetAll_();
		}
	}


	template<typename TIdx, typename TId>
	Model<TIdx, TId>::~Model() {
#ifdef _DEBUG
		if (prev_ != nullptr && prev_->next_ != this)
			cout << "~Model => prev_->next_ and this disagree";
		if (next_ != nullptr && next_->prev_ != this)
			cout << "~Model => next_->prev_ and this disagree";
#endif
		clear();
		if (next_ != nullptr)
			delete next_;
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::prepareOutput(Table<TIdx, TId> &srcTable) {
		const string f = "Model::prepareOutput()";
		if (!srcTable.isInitialized() && ptt_ != pttNone)
			throw Error(ecNotImplemented, f, "When passing an empty table the PassThroughType should be pttNone"); //This is allowed when ptt_ == pttNone

		//Generate colIds if neccesary
		if (ptt_ != pttNone && srcTable.cols.count() > 0 && !srcTable.cols.hasIds() && this->cols.hasIds())
			Matrix<TId, TIdx>() ^ *this;

		if ((srcTable.cols.count() == 0 || srcTable.cols.hasIds()) && !this->cols.hasIds()) {
			String mi;
			convertValue(modelIndex(), mi);
			genIds(modelName() + "." + mi + ".", TIdx(0), this->cols.count()) ^ *this;
		}
		
		//Make dstTable_		
		this->resizeRows(sampleCount_);
		outputSlice_ = this->tab();
		
		//Add from srcTable_ to dstTable_
		if (ptt_ == pttAll) {
			outputIndices_ = ((DSLib::Matrix<TIdx, TIdx>() | (TIdx)0) || srcTable_.cols.count()-1); //Added -1 (shouldtest)
			if (!this->rows.hasIds() && srcTable.rows.hasIds())
				this->rows.addIds(srcTable.rows.idxToId(), 0);
			this->cols.take(srcTable);
		} else if (ptt_ == pttRelevant) {
			if (srcTable.cols.count() > 0) {
				DSLib::Matrix<TIdx> allIndices = ((DSLib::Matrix<TIdx, TIdx>() | (TIdx)0) || srcTable.cols.count()-1);
				Table<TIdx, TId> notTable;
				setDifference(outputIndices_, allIndices, inputIndices_);
				if (!outputIndices_.isInitialized())
					return;
				notTable = srcTable[outputIndices_];				
				if (!this->rows.hasIds() && notTable.rows.hasIds())
					this->rows.addIds(notTable.rows.idxToId(), 0);
				this->cols.take(notTable);
			}
		}
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::prepareInput(Table<TIdx, TId> &srcTable) {		
		if (inputDataTypes_.isInitialized() || inputContentTypes_.isInitialized())
			inputIndices_ = srcTable.slice(inputSlice_, inputDataTypes_, inputContentTypes_);
		else if (inputAll_)
			inputIndices_ = srcTable.slice(inputSlice_);
		else
			inputIndices_.clear();
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::clear() {
		inputSlice_.clear();
		srcTable_.clear();
		outputSlice_.clear();
		Table<TIdx, TId>::clear(); //After clearing all slices to prevent copying.
		inputContentTypes_.clear();
		inputDataTypes_.clear();
		inputIndices_.clear();
		outputIndices_.clear();
	}


	template <typename TIdx, typename TId>
	void Model<TIdx, TId>::clearAll_() {
		Table<TIdx, TId>::clearAll();
		inputContentTypes_.clearAll();
		inputDataTypes_.clearAll();
		inputIndices_.clearAll();
		outputIndices_.clearAll();
		if (next_ != nullptr) {
			next_->clearAll_();
		}
	}

	template <typename TIdx, typename TId>
	void Model<TIdx, TId>::clearAll() {
		if (prev_ != nullptr) {
			first().clearAll();
		} else {
			clearAll_();
		}
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::prepare(const Table<TIdx, TId> &table) {		
		try {			
			prepare_ = false;
			inputAll_ = false;
			progress_.clear();
			progress_.addStages(dtString | (modelName() + ".Train"));
			progress_.addStages(dtString | (modelName() + ".Apply"));
			clear();
			srcTable_ = const_cast<Table<TIdx, TId>&>(table);
			sampleCount(srcTable_.rows.count());			
			registerInputs(srcTable_);
			prepareInput(srcTable_);
			registerOutputs(inputSlice_);
			prepareOutput(srcTable_);
			progress_.addStages(dtString | (modelName() + ".Done"));
		} catch (Error e) {
			throw Error(ecGeneral, modelName() + "::prepare()", e.what());
		}
		if (next_ != nullptr)
			next_->prepare(*this);
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::train_(const Table<TIdx, TId> &table) {
		try {
			progress_.setStage(modelName() + ".Train");
			if (next_ != nullptr) {
				auto f = [this](){train_(this->srcTable_, this->inputSlice_, this->outputSlice_); };
				future_ = std::async(ASYNCPOLICY, f);
				if (execType_ == etSynchronous)
					wait();
			} else {
				auto start = std::chrono::high_resolution_clock::now();
				train(this->srcTable_, this->inputSlice_, this->outputSlice_);
				auto end = std::chrono::high_resolution_clock::now();
				timeTrain_ = std::chrono::duration<double, std::milli>(end-start).count();
				//If this is the last model, sync on the future is handled by train()
			}
		}
		catch (Error e) {
			throw Error(ecGeneral, modelName() + "::train()", e.what());
		}
		catch (exception e) {
			throw Error(ecGeneral, modelName() + "::train()", e.what());
		}
		catch (...) {
			throw Error(ecGeneral, modelName() + "::train()", "Unknown error");
		}
		if (next_ != nullptr)
			next_->train_(*this);
	}

	template<typename TIdx, typename TId>
	Table<TIdx, TId> & Model<TIdx, TId>::train(const Table<TIdx, TId> &table) {
		if (!table.isSameData(srcTable_) || prepare_)
			prepare(table);

		auto f = [this](){train_(this->srcTable_); };
		last().future_ = std::async(ASYNCPOLICY, f);

		if (last().execType() == etSynchronous)
			last().wait();

		return last();
	}


	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::train_(const Table<TIdx, TId> &table, const Model<TIdx, TId> &till) {
		try {
			progress_.setStage(modelName() + ".Apply");
			if (next_ != nullptr) {
				auto f = [this](){train_(this->srcTable_, this->inputSlice_, this->outputSlice_); };
				future_ = std::async(ASYNCPOLICY, f);
				if (execType_ == etSynchronous)
					wait();
			} else {
				auto start = std::chrono::high_resolution_clock::now();
				train(this->srcTable_, this->inputSlice_, this->outputSlice_);
				auto end = std::chrono::high_resolution_clock::now();
				timeTrain_ = std::chrono::duration<double, std::milli>(end-start).count();				
				//If this is the last model, sync on the future is handled by train()
			}
		}
		catch (Error e) {
			throw Error(ecGeneral, modelName() + "::train()", e.what());
		}
		catch (exception e) {
			throw Error(ecGeneral, modelName() + "::train()", e.what());
		}
		catch (...) {
			throw Error(ecGeneral, modelName() + "::train()", "Unknown error");
		}
		if (next_ != nullptr && next_ != till)
			next_->train_(*this, till);
	}

	template<typename TIdx, typename TId>
	Table<TIdx, TId> & Model<TIdx, TId>::train(const Table<TIdx, TId> &table, const Model<TIdx, TId>& till) {
		if (!table.isSameData(srcTable_) || prepare_)
			prepare(table);

		auto f = [this, &till](){train_(this->srcTable_, &till);};
		last().future_ = std::async(ASYNCPOLICY, f);

		if (last().execType() == etSynchronous)
			last().wait();

		//static_cast because till is also in the linked list of this (non-const).
		return const_cast<Model<TIdx, TId>&>(till).prev();
	}
	
	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::apply_(const Table<TIdx, TId> &table) {
		try {
			progress_.firstStage();			
			if (next_ != nullptr) {
				auto f = [this](){apply_(this->srcTable_, this->inputSlice_, this->outputSlice_); };
				future_ = std::async(ASYNCPOLICY, f);
				if (execType_ == etSynchronous)
					wait();
			} else {
				auto start = std::chrono::high_resolution_clock::now();
				apply(this->srcTable_, this->inputSlice_, this->outputSlice_);
				auto end = std::chrono::high_resolution_clock::now();
				timeApply_ = std::chrono::duration<double, std::milli>(end-start).count();
				//If this is the last model, sync on the future is handled by apply()
			}
		}
		catch (Error e) {
			throw Error(ecGeneral, modelName() + "::apply()", e.what());
		}
		catch (exception e) {
			throw Error(ecGeneral, modelName() + "::apply()", e.what());
		}
		catch (...) {
			throw Error(ecGeneral, modelName() + "::apply()", "Unknown error");
		}
		if (next_ != nullptr)
			next_->apply_(*this);
	}

	template<typename TIdx, typename TId>
	Table<TIdx, TId> & Model<TIdx, TId>::apply(const Table<TIdx, TId> &table) {
		if (!table.isSameData(srcTable_) || prepare_)
			prepare(table);

		auto f = [this](){apply_(this->srcTable_); };
		last().future_ = std::async(ASYNCPOLICY, f);

		if (last().execType() == etSynchronous)
			last().wait();

		return last();
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::apply_(const Table<TIdx, TId> &table, const Model<TIdx, TId>& till) {
		try {
			progress_.firstStage();			
			if (next_ != nullptr) {
				auto f = [this](){apply_(this->srcTable_, this->inputSlice_, this->outputSlice_); };
				future_ = std::async(ASYNCPOLICY, f);
				if (execType_ == etSynchronous)
					wait();
			} else {
				auto start = std::chrono::high_resolution_clock::now();
				apply(this->srcTable_, this->inputSlice_, this->outputSlice_);
				auto end = std::chrono::high_resolution_clock::now();
				timeApply_ = std::chrono::duration<double, std::milli>(end-start).count();
				//If this is the last model, sync on the future is handled by apply()
			}
		}
		catch (Error e) {
			throw Error(ecGeneral, modelName() + "::apply()", e.what());
		}
		catch (exception e) {
			throw Error(ecGeneral, modelName() + "::apply()", e.what());
		}
		catch (...) {
			throw Error(ecGeneral, modelName() + "::apply()", "Unknown error");
		}
		if (next_ != nullptr && next_ != &till)
			next_->apply_(*this, till);
	}

	template<typename TIdx, typename TId>
	Table<TIdx, TId> & Model<TIdx, TId>::apply(const Table<TIdx, TId> &table, const Model<TIdx, TId>& till) {
		if (!table.isSameData(srcTable_) || prepare_)
			prepare(table);

		auto f = [this, &till](){apply_(this->srcTable_, till); };
		last().future_ = std::async(ASYNCPOLICY, f);

		if (last().execType() == etSynchronous)
			last().wait();

		//const_cast because till is also in the linked list of this (non-const).
		return const_cast<Model<TIdx, TId>&>(till).prev();
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::train_(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) {
		auto start = std::chrono::high_resolution_clock::now();
		train(table, input, output);
		auto end = std::chrono::high_resolution_clock::now();
		timeTrain_ = std::chrono::duration<double, std::milli>(end-start).count();
	}

	template<typename TIdx, typename TId>
	void Model<TIdx, TId>::apply_(const DSLib::Table<TIdx, TId> &table, DSLib::Table<TIdx, TId> &input, DSLib::Table<TIdx, TId> &output) {
		auto start = std::chrono::high_resolution_clock::now();
		apply(table, input, output);
		auto end = std::chrono::high_resolution_clock::now();
		timeApply_ = std::chrono::duration<double, std::milli>(end-start).count();
	}

	//DSLang
	template<typename TIdx, typename TId>
	Model<TIdx, TId>& Model<TIdx, TId>::operator[](const TIdx index) {
		return next(index);
	}

	//*****************//
	//** ProgressBar **//
	//*****************//
	namespace {
		void ProgressBar::init_(DSTypes::Double min, DSTypes::Double max, const DSLib::Matrix<DSTypes::String>& stages) {
			min_ = min;
			max_ = max;
			currentProgress_ = min_;
			currentStage_ = 0;
			stages_ = stages;
			head_ = "[";
			tail_ = "]";
			full_ = "#";
			empty_ = ".";
			len_ = 20;
			old_ = "";
		}

		void ProgressBar::clone(const ProgressBar &other) {
			min_ = other.min_;
			max_ = other.max_;
			currentProgress_ = other.currentProgress_;
			stages_ = other.stages_;
			currentStage_ = other.currentStage_;
			head_ = other.head_;
			tail_ = other.tail_;
			full_ = other.full_;
			empty_ = other.empty_;
			len_ = other.len_;
			old_ = other.old_;
		}

		void ProgressBar::clone(ProgressBar &&other) {
			min_ = std::move(other.min_);
			max_ = std::move(other.max_);
			currentProgress_ =std::move( other.currentProgress_);
			stages_ = std::move(other.stages_);
			currentStage_ = std::move(other.currentStage_);
			head_ = std::move(other.head_);
			tail_ = std::move(other.tail_);
			full_ = std::move(other.full_);
			empty_ = std::move(other.empty_);
			len_ = std::move(other.len_);
			old_ = std::move(other.old_);
		}

		ProgressBar::ProgressBar() {
			init_(0, 1, genIds((String)"Stage ", 0U, 1U));
		}

		ProgressBar::ProgressBar(DSTypes::Double min, DSTypes::Double max, DSTypes::UInt32 stages) {			
			if (stages > 0)
				init_(0, 1, genIds((String)"Stage ", 0U, stages));
			else
				init_(0, 1, genIds((String)"Stage ", 0U, 1U));
			min_ = min;
			max_ = max;
		}

		ProgressBar::ProgressBar(DSTypes::Double min, DSTypes::Double max, const Matrix<String> &stages) {
			init_(min, max, stages);
		}

		void ProgressBar::addStages(const DSLib::Matrix<DSTypes::String>& stages) {
			std::lock_guard<std::mutex> lock(lock_);
			stages_ | stages;
		}

		void ProgressBar::min(DSTypes::Double & min) {
			std::lock_guard<std::mutex> lock(lock_);
			min_= min;
		}

		void ProgressBar::max(DSTypes::Double & max) {
			std::lock_guard<std::mutex> lock(lock_);
			max_ = max;
		}

		DSTypes::Double ProgressBar::min() {
			std::lock_guard<std::mutex> lock(lock_);
			return min_;
		}

		DSTypes::Double ProgressBar::max() {
			std::lock_guard<std::mutex> lock(lock_);
			return max_;
		}

		void ProgressBar::setStage(DSTypes::UInt32 stage) {
			std::lock_guard<std::mutex> lock(lock_);
			if (stage < ~stages_) {
				currentProgress_ = min_;
				currentStage_ = stage;
			} else 
				throw Error(ecNotFound, "ProgressBar::SetStage", "Stage exceeds stage number of stages");
		}

		void ProgressBar::setStage(const String &stage) {		
			for (auto i = 0U;i != ~stages_;i++) {
				if (stages_.vec(i) == stage) {
					std::lock_guard<std::mutex> lock(lock_);
					currentProgress_ = min_;
					currentStage_ = i;
					return;
				}
			}
			throw Error(ecNotFound, "ProgressBar::SetStage", "Stage not found");
		}

		String ProgressBar::getStage() {
			std::lock_guard<std::mutex> lock(lock_);
			return stages_.vec(currentStage_);
		}

		UInt32 ProgressBar::getCurrentStage() {
			std::lock_guard<std::mutex> lock(lock_);
			return currentStage_;
		}

		DSTypes::UInt32 ProgressBar::getStageCount() {
			std::lock_guard<std::mutex> lock(lock_);
			return ~stages_;
		}

		void ProgressBar::clear() {
			stages_.clear();
			firstStage();
			progress(min_);
		}

		void ProgressBar::firstStage() {
			std::lock_guard<std::mutex> lock(lock_);
			currentStage_ = 0;
		}

		void ProgressBar::lastStage() {
			std::lock_guard<std::mutex> lock(lock_);
			currentStage_ = ~stages_-1;		
		}

		void ProgressBar::done() {
			lastStage();
			currentProgress_ = max_;
		}

		void ProgressBar::stageDone() {
			progress(max_);
		}

		bool ProgressBar::isDone() {
			return (currentStage_ == ~stages_-1) && (currentProgress_ == max_);
		}

		void ProgressBar::nextStage() {
			setStage(currentStage_+1);
		}

		void ProgressBar::start() {
			firstStage();
			progress(min_);
		}

		void ProgressBar::progress(DSTypes::Double progress) {
			std::lock_guard<std::mutex> lock(lock_);
			if (progress >= min_ && progress <= max_)
				currentProgress_ = progress;
			else
				throw Error(ecRangeError, "ProgressBar::progress", SS("Out of range " << progress << " should be >= " << min_ << " and <= " << max_));
		}

		void ProgressBar::progress(DSTypes::Double min, DSTypes::Double max, DSTypes::Double progress) {
			this->min(min);
			this->max(max);
			this->progress(progress);
		}

		void ProgressBar::incProgress(DSTypes::Double amount) {
			progress(currentProgress_+amount);
		}

		DSTypes::Double ProgressBar::progress() {
			std::lock_guard<std::mutex> lock(lock_);
			return (currentProgress_ - min_) / (max_- min_);
		}

		void ProgressBar::print(DSTypes::Double min, DSTypes::Double max, DSTypes::Double progress, std::ostream &os) {
			this->min(min);
			this->max(max);
			this->progress(progress);
			print(os);
		}

		void ProgressBar::print(std::ostream & os) {
			stringstream s;

			UInt32 p = (UInt32)((Double)progress() * (Double)len_);
			
			if (~stages_ > 1) {
				s << head_;
				for (UInt32 i=0;i<getStageCount();i++) {
					if (i<=getCurrentStage()) {
						s << i+1;
					} else {
						s << empty_;
					}
					if (i+1!=getStageCount())
						s << empty_;
				}
				s << tail_;
			}

			s << head_;
			for (UInt32 i=0;i<p;i++)
				s << full_;
			for (UInt32 i = 0;i<len_-p;i++)
				s << empty_;
			s << tail_;

			s << " " << round(progress() * 100.0) << "%";
			if (~stages_ > 1)
				s << " " << getStage();
			s << endl;
			if (s.str() != old_) {
				old_ = s.str();
				os << s.str();
			}
		}
	
		DSTypes::String ProgressBar::print() {
			stringstream ss;
			print(ss);
			return ss.str();
		}

	}
};
