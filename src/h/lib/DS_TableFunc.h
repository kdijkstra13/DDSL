#pragma once
#include "h/DS_Types.h"
#include "h/lib/DS_Matrix.h"
#include "h/lib/DS_Table.h"

namespace DSFunc {

	template <typename TIdx, typename TId>
	bool sortById(DSLib::MatrixBase::IndexBase &index, const DSLib::Matrix<TId, TIdx> &srcIdxToId, std::map<TId, TIdx> &dstIdToIdx);
	
	template <typename TIdx, typename TId>
	void test(DSLib::Matrix<TIdx, TIdx> &dst, const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs, const std::function<void(DSLib::MatrixBase &dst, const DSLib::MatrixBase &lhs, const DSLib::MatrixBase &rhs)> &func);

	template <typename TIdx, typename TId>
	DSLib::Matrix<TId, TIdx> genIds(const TId &prefix, const TIdx from, const TIdx count);

	template<typename TIdx, typename TId>
	std::ostream & operator<<(std::ostream & str, const DSLib::Table<TIdx, TId>& tab);

	template<typename TIdx, typename TId>
	std::ostream && operator<<(std::ostream && str, const DSLib::Table<TIdx, TId>& tab);

	template<typename TIdx, typename TId>
	std::istream & operator>>(std::istream & str, DSLib::Table<TIdx, TId>& tab);

	template<typename TIdx, typename TId>
	std::istream && operator>>(std::istream && str, DSLib::Table<TIdx, TId>& tab);

}
