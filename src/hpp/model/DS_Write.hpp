#pragma once
#include "h/model/DS_Write.h"

#include "hpp/model/DS_Model.hpp"

using namespace std;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSUtil;
using namespace DSLib;

namespace DSModel {
	template<typename TIdx, typename TId>
	void Export<TIdx, TId>::updateParameters() {}

	template<typename TIdx, typename TId>
	void Export<TIdx, TId>::registerInputs(const Table<TIdx, TId>& table) {
		this->registerInputAll();
	}

	template<typename TIdx, typename TId>
	void Export<TIdx, TId>::registerOutputs(const Table<TIdx, TId>& input) {}

	template<typename TIdx, typename TId>
	void Export<TIdx, TId>::write_(ostream &f, Table<TIdx, TId>& input) {
		String d = this->template parameterValueById<String>("Delim");
		String nl = this->template parameterValueById<String>("NewLine");
		String q = this->template parameterValueById<String>("Quote");
		bool ity = this->template parameterValueById<UInt8>("IncludeTypes") == 1;
		vector<Matrix<String, TIdx>> mats;
		vector<typename Matrix<String, TIdx>::Index::Iterator> rows;
		vector<String> qu;
		rows.resize(input.matrixByIdx().size());
		mats.resize(input.matrixByIdx().size());
		qu.resize(input.matrixByIdx().size());

		//Convert all matrices to string and get string iterators
		TIdx mc = TIdx(0);
		for (auto matwrap = input.matrixByIdx().begin(); matwrap != input.matrixByIdx().end(); matwrap++, mc++) {
			qu[mc] = this->template parameterValueById<String>("Quote");
			if ((*matwrap)->dataType() != dtString) {
				(*matwrap)->data().toStringMatrix(mats[mc]);
				qu[mc] = "";
			}
			else {
				mats[mc] = (*matwrap)->data();
			}
			rows[mc] = mats[mc].rows.begin();
		}

		//Write colIds and prepare rowIds		
		Matrix<String, TIdx> rowIds;
		typename CellsContinuous<String, TIdx>::Iterator rId;
		if (input.rows.hasIds()) {
			convert(rowIds, input.rows.idxToId());
			rId = rowIds.vec().begin();
		}
		//Write colsIds or include types (ity) or none
		Matrix<String, TIdx> colIds;
		typename CellsContinuous<TId, TIdx>::Iterator v;
		if (input.cols.hasIds()) {
			convert(colIds, input.cols.idxToId());
			v = colIds.vec().begin();
		}
		if (input.cols.hasIds() || ity) {
			String de = "";
			for (TIdx vc = 0;vc != input.cols.count();vc++) {
				f << de << q;
				if (input.cols.hasIds()) {
					f << *v;
					v++;
				}
				if (ity) {
					f << "." << etos(input.getContentType(vc));
					f << "." << etos(input.getDataType(vc));
				}
				f << q;
				de = d;
			}
			f << nl;
		}

		for (TIdx r = 0;r<input.rows.count();r++) {
			if (input.rows.hasIds()) {
				f << q << *rId << q << d;
				rId++;
			}
			String de = "";
			for (TIdx m = 0;m<mc;m++) {
				for (auto c = rows[m]->begin();c != rows[m]->end();c++) {
					f << de << qu[m] << *c << qu[m];
					de = d;
				}
				rows[m]++;
			}
			f << nl;
		}
	}

	template<typename TIdx, typename TId>
	void Export<TIdx, TId>::train(const Table<TIdx, TId>& table, Table<TIdx, TId>& input, Table<TIdx, TId>& output) {		
		//This Model can be very memory consuming
		ofstream f(this->template parameterValueById<String>("Filename"), ios::binary);
		if (!f)
			throw Error(ecGeneral, "Model::train", "Cannot open output file");
		write_(f, input);
	}

	template<typename TIdx, typename TId>
	void Export<TIdx, TId>::apply(const Table<TIdx, TId>& table, Table<TIdx, TId>& input, Table<TIdx, TId>& output) {
		//ofstream f(this->template parameterValueById<String>("Filename"), fstream::app | ios::binary);
		ofstream f(this->template parameterValueById<String>("Filename"), ios::binary);
		if (!f)
			throw Error(ecGeneral, "Model::train", "Cannot open output file");
		write_(f, input);
	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId>* Export<TIdx, TId>::clone() const {
		auto * m = new Export<TIdx, TId>();
		m->clone(*this);
		return m;
	}

	template<typename TIdx, typename TId>
	Model<TIdx, TId>* Export<TIdx, TId>::move() {
		auto * m = new Export<TIdx, TId>();
		m->clone(std::move(*this));
		return m;
	}

	template<typename TIdx, typename TId>
	Export<TIdx, TId>::Export(const String filename, const bool includeTypes, const String delim, const String newline, const String quote) {
		//TODO: idToIdx_ is out of sync if IncludeTypes is one line down (Out of order static init of Table<>)
		Table<TIdx, TId> parms = (Matrix<Literal, TIdx>() | "Value") | (
			((Matrix<Literal, TIdx>() | "IncludeTypes") ^ (ctParameter | (Matrix<UInt8, TIdx>() | (includeTypes ? UInt8(1) : UInt8(0))))) |
			((Matrix<Literal, TIdx>() | "Filename") ^ (ctParameter | (Matrix<String, TIdx>() | filename))) |
			((Matrix<Literal, TIdx>() | "Delim") ^ (ctParameter | (Matrix<String, TIdx>() | delim))) |
			((Matrix<Literal, TIdx>() | "NewLine") ^ (ctParameter | (Matrix<String, TIdx>() | newline))) |
			((Matrix<Literal, TIdx>() | "Quote") ^ (ctParameter | (Matrix<String, TIdx>() | quote)))
			
			);
		this->registerParameters(parms);
	}

	template<typename TIdx, typename TId>
	Export<TIdx, TId>::Export(const Export & other) {
		clone(other);
	}

	template<typename TIdx, typename TId>
	Export<TIdx, TId>::Export(Export && other) {
		clone(std::move(other));
	}

	template<typename TIdx, typename TId>
	Export<TIdx, TId>::Export(const Model<TIdx, TId>& other) {
		clone(dynamic_cast<const Export<TIdx, TId> &>(other));
	}

	template<typename TIdx, typename TId>
	Export<TIdx, TId>::Export(Model<TIdx, TId>&& other) {
		clone(std::move(dynamic_cast<Export<TIdx, TId> &>(other)));
	}

	template<typename TIdx, typename TId>
	Export<TIdx, TId> & Export<TIdx, TId>::operator=(const Export<TIdx, TId> & other) {
		clone(other);
		return *this;
	}

	template<typename TIdx, typename TId>
	Export<TIdx, TId> & Export<TIdx, TId>::operator=(Export<TIdx, TId> && other) {
		clone(std::move(other));
		return *this;
	}

	template<typename TIdx, typename TId>
	void Export<TIdx, TId>::clone(const Export & other) {
		Model<TIdx, TId>::clone(other);
	}

	template<typename TIdx, typename TId>
	void Export<TIdx, TId>::clone(Export && other) {
		Model<TIdx, TId>::clone(std::move(other));
	}

	template<typename TIdx, typename TId>
	Export<TIdx, TId> Export<TIdx, TId>::operator()(const TIdx count) {
		Export<TIdx, TId> m;
		this->copy(count, m);
		return std::move(m);
	}
};
