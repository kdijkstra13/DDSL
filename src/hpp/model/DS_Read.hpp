#pragma once
#include "h/model/DS_Read.h"

#include "hpp/model/DS_Model.hpp"

using namespace std;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSUtil;
using namespace DSLib;

namespace DSModel {
	//************//
	//** Import **//
	//************//
	inline istream & readLine(istream &is, String &s) {
		getline(is, s);
		if ((s.size() > 0) && s[s.size()-1] == '\r')	
			s = s.substr(0, s.size()-1);
		return is;
	}

	template<typename TIdx, typename TId>
	void Import<TIdx, TId>::prepare_(std::istream & f, DSLib::Table<TIdx, TId>& proto, TIdx &samples) {
		char d = this->template parameterValueById<String>("Delim")[0];
		String nl = this->template parameterValueById<String>("NewLine");
		char q = this->template parameterValueById<String>("Quote")[0];

		//Read head and count number of samples
		String line;
		readLine(f, line);
		head_ = split(line, d, q);
		samples = 0;
		while (getline(f, line))
			samples++;

		//Create a map of the sub matrices, colIdx and colId
		colIdx_.clear();
		colId_.clear();
		TIdx idx = 0;
		for (auto h = head_.vec().begin();h != head_.vec().end();h++, idx++) {
			Matrix<String> types = split(*h, '.', 0);
			if (~types != 3)
				throw Error(ecIncompatible, "prepare_", "Cannot reconstruct Table from file. Use \"Name.ContentType.DataType\" in your header to achieve this import");
			
			//Read types from header
			*h = types.vec(0);
			ContentType ct = stringToContentType(types.vec(1));
			DataType dt = stringToDataType(types.vec(2));

			if (ct == ctUnknown || dt == dtUnknown)
				throw Error(ecIncompatible, "prepare_", SS("Unknown DataType of ContentType (" << etos(ct) << ") or DataType (" << etos(dt) << ") for column " << types.vec(0) << "." << types.vec(1) << "." << types.vec(2)));

			//Find types in colIdx_ and colId_
			auto it = colIdx_.find(pair<ContentType, DataType>(ct, dt));
			if (it != colIdx_.end()) {
				it->second | idx;
				colId_.find(pair<ContentType, DataType>(ct, dt))->second | *h;
			} else {
				Matrix<TIdx> mIdx;
				mIdx | idx;
				colIdx_.insert(pair<pair<ContentType, DataType>, Matrix<TIdx>>(pair<ContentType, DataType>(ct, dt), mIdx));
				Matrix<TId> mId;
				mId | *h;
				colId_.insert(pair<pair<ContentType, DataType>, Matrix<TId>>(pair<ContentType, DataType>(ct, dt), mId));
			}
		}

		//Reconstruct the table from the sub matrix map		
		proto.clear();
		TIdx offset = 0;
		for (auto m = colIdx_.begin();m != colIdx_.end();m++) {			
			ContentType ct = m->first.first;
			DataType dt = m->first.second;
			MatrixBase * mb = createFromDataType<TIdx>(dt);
			TIdx colcount = m->second.vec().count();
			mb->resize2(0, colcount);
			proto.addMatrix(std::move(*mb), ct, dt, 0, colcount, offset);
			proto.cols.addIds(colId_.find(pair<ContentType, DataType>(ct, dt))->second, offset);			
			offset += colcount;
			proto.cols.count(offset);
		}
	}

	template<typename TIdx, typename TId>
	void Import<TIdx, TId>::read_(std::istream & f, DSLib::Table<TIdx, TId>& output) {
		char d = this->template parameterValueById<String>("Delim")[0];
		String nl = this->template parameterValueById<String>("NewLine");
		char q = this->template parameterValueById<String>("Quote")[0];
		//changed 16-02-2017
		this->setStage("Read");
		auto begin = f.tellg();
		f.seekg(0, f.end);
		auto end = f.tellg();		
		f.seekg(begin);
		this->setMinProgress((Double)begin);
		this->setMaxProgress((Double)end);

		String line;
		readLine(f, line);
		Matrix<String> body(0, ~head_);
		while (readLine(f, line)) {
			Matrix<String> ln = split(line, d, q);
			body ^ ln;
			if (!f.eof())
				this->setProgress((Double)f.tellg());
		}		
		this->nextStage();
		if (output.rows.count() != body.rows.count())
			throw Error(ecSizeMismatch, "read", "The number of output rows and the number of lines in the file are different");

		//Slice and convert from Matrix<String>
		//TIdx offset = 0;
		this->setMinProgress(0);
		this->setMaxProgress((Double)colIdx_.size());
		for (auto m = colIdx_.begin();m != colIdx_.end();m++) {
			Matrix<String> src = body[m->second]; //slice cols
			ContentType ct = m->first.first;
			DataType dt = m->first.second;
			MatrixBase * src2 = convertFromDataType(dt, src);
			MatrixBase & dst = output.findMatrix(ct, dt)->data();
			if (!dst.isSameSize(*src2))
				throw Error(ecInternal, "read", "Sub-matrix of output is of the wrong size");
			dst = *src2; //direct slice assignment
			this->incProgress();
		}
		this->setStageDone();
	}

	template<typename TIdx, typename TId>
	void Import<TIdx, TId>::updateParameters() {}

	template<typename TIdx, typename TId>
	void Import<TIdx, TId>::registerInputs(const Table<TIdx, TId>& table) {}

	template<typename TIdx, typename TId>
	void Import<TIdx, TId>::registerOutputs(const Table<TIdx, TId>& input) {
		ifstream f(this->template parameterValueById<String>("Filename"), ios::binary);
		if (!f) throw Error(ecGeneral, "Model::registerOutputs", SS("Cannot open input file: " << this->template parameterValueById<String>("Filename")));
		Table<TIdx, TId> output;
		prepare_(f, output, this->sampleCount());
		this->registerOutput(output);
		this->addStages((dtString | "Read" | "Parse"));
	}

	template<typename TIdx, typename TId>
	void Import<TIdx, TId>::train(const Table<TIdx, TId>& table, Table<TIdx, TId>& input, Table<TIdx, TId>& output) {
		ifstream f(this->template parameterValueById<String>("Filename"), ios::binary);
		if (!f) throw Error(ecGeneral, "Model::train", SS("Cannot open input file: " << this->template parameterValueById<String>("Filename")));		
		read_(f, output);
	}

	template<typename TIdx, typename TId>
	void Import<TIdx, TId>::apply(const Table<TIdx, TId>& table, Table<TIdx, TId>& input, Table<TIdx, TId>& output) {
		train(table, input, output);
	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId>* Import<TIdx, TId>::clone() const {
		auto * m = new Import<TIdx, TId>();
		m->clone(*this);
		return m;
	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId>* Import<TIdx, TId>::move() {
		auto * m = new Import<TIdx, TId>();
		m->clone(std::move(*this));
		return m;
	}

	template<typename TIdx, typename TId>
	Import<TIdx, TId>::Import(const String filename, const String delim, const String newline, const String quote) {
		if (quote.size() != 1 || delim.size() != 1)
			throw Error(ecIncompatible, "Import()", "Multi-character delimeters not allowed");
		//TODO: idToIdx_ is out of sync if IncludeTypes is one line down (Out of order static init of Table<>)
		Table<TIdx, TId> parms = (Matrix<Literal, TIdx>() | "Value") | (			
			((Matrix<Literal, TIdx>() | "Filename") ^ (ctParameter | (Matrix<String, TIdx>() | filename))) |
			((Matrix<Literal, TIdx>() | "Delim") ^ (ctParameter | (Matrix<String, TIdx>() | delim))) |
			((Matrix<Literal, TIdx>() | "NewLine") ^ (ctParameter | (Matrix<String, TIdx>() | newline))) |
			((Matrix<Literal, TIdx>() | "Quote") ^ (ctParameter | (Matrix<String, TIdx>() | quote)))

			);
		this->registerParameters(parms);
	}

	template<typename TIdx, typename TId>
	Import<TIdx, TId>::Import(const Import & other) {
		clone(other);
	}

	template<typename TIdx, typename TId>
	Import<TIdx, TId>::Import(Import && other) {
		clone(std::move(other));
	}

	template<typename TIdx, typename TId>
	Import<TIdx, TId>::Import(const Model<TIdx, TId>& other) {
		clone(dynamic_cast<const Import<TIdx, TId> &>(other));
	}

	template<typename TIdx, typename TId>
	Import<TIdx, TId>::Import(Model<TIdx, TId>&& other) {
		clone(std::move(dynamic_cast<Import<TIdx, TId> &>(other)));
	}

	template<typename TIdx, typename TId>
	Import<TIdx, TId> & Import<TIdx, TId>::operator=(const Import<TIdx, TId> & other) {
		clone(other);
		return *this;
	}

	template<typename TIdx, typename TId>
	Import<TIdx, TId> & Import<TIdx, TId>::operator=(Import<TIdx, TId> && other) {
		clone(std::move(other));
		return *this;
	}

	template<typename TIdx, typename TId>
	void Import<TIdx, TId>::clone(const Import & other) {
		Model<TIdx, TId>::clone(other);
	}

	template<typename TIdx, typename TId>
	void Import<TIdx, TId>::clone(Import && other) {
		Model<TIdx, TId>::clone(std::move(other));
	}

	template<typename TIdx, typename TId>
	Import<TIdx, TId> Import<TIdx, TId>::operator()(const TIdx count) {
		Import<TIdx, TId> m;
		this->copy(count, m);
		return std::move(m);
	}

	//**********//
	//** Read **//
	//**********//
	template<typename TIdx, typename TId>
	void Read<TIdx, TId>::prepare_(std::istream & f, DSLib::Table<TIdx, TId>& proto, TIdx &samples) {
		const String ff = "Table::read";

		String type;
		DSUtil::read(f, type);
		if (type != "Table")
			throw Error(ecIncompatible, ff, SS("File does not contain a Table but a: " << type));

		DSUtil::read(f, type); //Throw away human readable line

		DataType id;
		DataType idx;
		TId idtest;
		TIdx idxtest;
		DSUtil::read(f, id);
		DSUtil::read(f, idx);
		if (!dataTypeEqual(id, idtest))
			throw Error(ecIncompatible, ff, SS("Wrong Id DataType in file. Table type is " << typeName(idtest) << " while file contains " << etos(id)));
		if (!dataTypeEqual(idx, idxtest))
			throw Error(ecIncompatible, ff, SS("Wrong Idx DataType in file. Table type is " << typeName(idxtest) << " while file contains " << etos(idx)));

		DSUtil::read(f, samples);
		proto.read_(f); //only read structure
		proto.resizeRows(0);
	}

	template<typename TIdx, typename TId>
	void Read<TIdx, TId>::read_(std::istream & f, DSLib::Table<TIdx, TId>& output) {
		Table<TIdx, TId> tab;
		tab.read(f);
		if (output.isSameStructure(tab))
			output = tab; //Direct slice assignment
		else
			throw Error(ecIncompatible, "Read::read_", "Structures of output table and file input table are not the same");
	}

	template<typename TIdx, typename TId>
	void Read<TIdx, TId>::updateParameters() {}

	template<typename TIdx, typename TId>
	void Read<TIdx, TId>::registerInputs(const Table<TIdx, TId>& table) {}

	template<typename TIdx, typename TId>
	void Read<TIdx, TId>::registerOutputs(const Table<TIdx, TId>& input) {
		ifstream f(this->template parameterValueById<String>("Filename"), ios::binary);
		if (!f) throw Error(ecGeneral, "Read::registerOutputs", SS("Cannot open input file: " << this->template parameterValueById<String>("Filename")));
		Table<TIdx, TId> output;
		prepare_(f, output, this->sampleCount());
		this->registerOutput(output);
	}

	template<typename TIdx, typename TId>
	void Read<TIdx, TId>::train(const Table<TIdx, TId>& table, Table<TIdx, TId>& input, Table<TIdx, TId>& output) {
		ifstream f;
		f.open(this->template parameterValueById<String>("Filename"), fstream::binary);
		if (!f) throw Error(ecGeneral, "Read::train", SS("Cannot open input file: " << this->template parameterValueById<String>("Filename")));
		read_(f, output);
	}

	template<typename TIdx, typename TId>
	void Read<TIdx, TId>::apply(const Table<TIdx, TId>& table, Table<TIdx, TId>& input, Table<TIdx, TId>& output) {}

	template<typename TIdx, typename TId>
	Model<TIdx, TId>* Read<TIdx, TId>::clone() const {
		auto * m = new Read<TIdx, TId>();
		m->clone(*this);
		return m;
	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId>* Read<TIdx, TId>::move() {
		auto * m = new Read<TIdx, TId>();
		m->clone(std::move(*this));
		return m;
	}

	template<typename TIdx, typename TId>
	Read<TIdx, TId>::Read(const String filename) {
		Table<TIdx, TId> parms = (Matrix<Literal, TIdx>() | "Value") | (
				((Matrix<Literal, TIdx>() | "Filename") ^ (ctParameter | (Matrix<String, TIdx>() | filename)))
			);
		this->registerParameters(parms);
	}

	template<typename TIdx, typename TId>
	Read<TIdx, TId>::Read(const Read & other) {
		clone(other);
	}

	template<typename TIdx, typename TId>
	Read<TIdx, TId>::Read(Read && other) {
		clone(std::move(other));
	}

	template<typename TIdx, typename TId>
	Read<TIdx, TId>::Read(const Model<TIdx, TId>& other) {
		clone(dynamic_cast<const Read<TIdx, TId> &>(other));
	}

	template<typename TIdx, typename TId>
	Read<TIdx, TId>::Read(Model<TIdx, TId>&& other) {
		clone(std::move(dynamic_cast<Read<TIdx, TId> &>(other)));
	}

	template<typename TIdx, typename TId>
	Read<TIdx, TId> & Read<TIdx, TId>::operator=(const Read<TIdx, TId> & other) {
		clone(other);
		return *this;
	}

	template<typename TIdx, typename TId>
	Read<TIdx, TId> & Read<TIdx, TId>::operator=(Read<TIdx, TId> && other) {
		clone(std::move(other));
		return *this;
	}

	template<typename TIdx, typename TId>
	void Read<TIdx, TId>::clone(const Read & other) {
		Model<TIdx, TId>::clone(other);
	}

	template<typename TIdx, typename TId>
	void Read<TIdx, TId>::clone(Read && other) {
		Model<TIdx, TId>::clone(std::move(other));
	}

	template<typename TIdx, typename TId>
	Read<TIdx, TId> Read<TIdx, TId>::operator()(const TIdx count) {
		Import<TIdx, TId> m;
		this->copy(count, m);
		return std::move(m);
	}
};
