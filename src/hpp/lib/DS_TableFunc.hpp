#pragma once

#include "h/lib/DS_TableFunc.h"

#include "hpp/lang/DS_TableLang.hpp"

using namespace std;
using namespace DSLib;
using namespace DSTypes;
using namespace DSLang;

namespace DSFunc {
	template <typename TIdx, typename TId>
	bool sortById(DSLib::MatrixBase::IndexBase &index, const DSLib::Matrix<TId, TIdx> &srcIdxToId, map<TId, TIdx> &dstIdToIdx) {
		if (srcIdxToId.vec().count() < 2)
			return false;
		if (srcIdxToId.vec().count() != dstIdToIdx.size())
			throw Error(ecUnexpected, "sortById()", "ids.vec().count() does not make sense in relation to Matrix rows/cols.");
		DSLib::Matrix<TIdx, TIdx> indices; //to.rows[i] = from.rows[indices[i]]
		indices.resize(srcIdxToId.vec().count(), 1);
		for (TIdx srcIdx=0;srcIdx<srcIdxToId.vec().count();srcIdx++) {
			TId srcId = *(srcIdxToId.vec()[srcIdx]);
			TIdx dstIdx = dstIdToIdx.find(srcId)->second;
			*(indices.vec()[dstIdx]) = srcIdx;
		}
		return index.sort(indices);
	}
	
	template <typename TIdx, typename TId>
	void test(DSLib::Matrix<TIdx, TIdx> &dst, const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs, const std::function<void(MatrixBase &dst, const MatrixBase &lhs, const MatrixBase &rhs)> &func) {
		const std::string f = "test()";
		if (rhs.rows.count() != 1 && lhs.rows.count() != rhs.rows.count())
			throw DSTypes::Error(DSTypes::ecSizeMismatch, f, SS("LHS row count does not match RHS row count (" << lhs.rows.count() << " is unequal to " << rhs.rows.count() << ") or RHS row count is unequal to 1"));

		typename vector<typename Table<TIdx, TId>::TableMatrix *>::const_iterator beginMatWrap, endMatWrap;
		DSLib::Table<TIdx, TId> llhs;
		if (lhs.cols.hasIds() && rhs.cols.hasIds()) {
			DSLib::Matrix<TId, TIdx> idsMat;
			idsMat.cols.add(rhs.cols.idxToId());
			llhs = const_cast<Table<TIdx, TId>&>(lhs)[idsMat]; //slice
			beginMatWrap = llhs.matrixByIdx().begin();
			endMatWrap = llhs.matrixByIdx().end();
			if (llhs.cols.count() != rhs.cols.count())
				throw DSTypes::Error(DSTypes::ecSizeMismatch, f, SS("After selecting cols by Id from LHS, the amount of LHS cols are uequal to the RHS cols (" << lhs.cols.count() << " is unequal to " << rhs.cols.count() << ")"));
		}
		else {
			beginMatWrap = lhs.matrixByIdx().begin();
			endMatWrap = lhs.matrixByIdx().end();
			if (lhs.cols.count() != rhs.cols.count())
				throw DSTypes::Error(DSTypes::ecSizeMismatch, f, SS("The amount of LHS cols are uequal to the RHS cols (" << lhs.cols.count() << " is unequal to " << rhs.cols.count() << ")"));
		}
		//if (lhs.matrixByIdx().size() != rhs.matrixByIdx().size())
			//throw DSTypes::Error(DSTypes::ecInternal, f, SS("Internal error. LHS does not contain the same amount of matrices as RHS"));

		auto rhsMatWrap = rhs.matrixByIdx().begin();
		Matrix<TIdx, TIdx> tempIndices;
		dst.clear();
		for (auto matWrap = beginMatWrap; matWrap != endMatWrap; matWrap++, rhsMatWrap++) {
			if ((*matWrap)->contentType() != (*rhsMatWrap)->contentType())
				throw DSTypes::Error(DSTypes::ecIncompatible, f, SS("ContentType for LHS differs from ContentType of RHS: " << etos((*matWrap)->contentType()) << " is unequal  to " << etos((*rhsMatWrap)->contentType()) << ")"), false);
			if ((*matWrap)->dataType() != (*rhsMatWrap)->dataType())
				throw DSTypes::Error(DSTypes::ecIncompatible, f, SS("DataType for LHS differs from DataType of RHS: " << etos((*matWrap)->dataType()) << " is unequal  to " << etos((*rhsMatWrap)->dataType()) << ")"));
			func(tempIndices, (*matWrap)->data(), (*rhsMatWrap)->data());
			if (matWrap == beginMatWrap)
				dst = tempIndices;
			else
				dst = dst && tempIndices; //DDSL intersection
		}
	}

	template<typename TIdx, typename TId>
	DSLib::Matrix<TId, TIdx> genIds(const TId & prefix, const TIdx from, const TIdx count) {
		DSLib::Matrix<TId, TIdx> cns;		
		for (TIdx cidx = from;cidx<from + count;cidx++) {
			stringstream ss;
			ss << prefix << cidx;
			cns | ss.str();
		}
		return cns;
	}
	
	template<typename TIdx, typename TId>
	std::ostream & operator<<(std::ostream & str, const DSLib::Table<TIdx, TId>& tab) {
		tab.write(str);
		return(str);
	}

	template<typename TIdx, typename TId>
	std::ostream && operator<<(std::ostream && str, const DSLib::Table<TIdx, TId>& tab) {
		tab.write(str);
		return std::move(str);
	}

	template<typename TIdx, typename TId>
	std::istream & operator>>(std::istream & str, DSLib::Table<TIdx, TId>& tab) {
		tab.read(str);
		return str;
	}

	template<typename TIdx, typename TId>
	std::istream && operator>>(std::istream && str, DSLib::Table<TIdx, TId>& tab) {
		tab.read(str);
		return std::move(str);
	}

}
