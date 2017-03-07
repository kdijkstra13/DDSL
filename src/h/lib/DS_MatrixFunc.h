#pragma once
#include "h/DS_Types.h"
#include "h/lib/DS_Matrix.h"

namespace DSFunc {
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> repeat(const DSLib::Matrix<T, TInt> &mat, const TInt n, const bool repeatAsRows=false);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & scale(DSLib::Matrix<T, TInt> &mat, const T minSrc, const T maxSrc, const T minDst, const T maxDst);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> uniqueValues(const DSLib::Matrix<T, TInt> &mat, const T ignoreValue);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> uniqueValues(const DSLib::Matrix<T, TInt> &mat);

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> indexLUT(const DSLib::Matrix<T, TInt> &mat, const DSLib::Matrix<T, TInt> &lut);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> valueLUT(const DSLib::Matrix<TInt, TInt> &mat, const DSLib::Matrix<T, TInt> &lut);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> randomInt(TInt rows, TInt cols, T min, T max);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> randomReal(TInt rows, TInt cols, T min, T max);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> random(TInt rows, TInt cols, const DSLib::Matrix<T> &vec);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & shuffle(DSLib::Matrix<T, TInt> &mat, DSTypes::Double amount = 0.5);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> shuffle(DSLib::Matrix<T, TInt> &&mat, DSTypes::Double amount = 0.5) { return std::move(shuffle(mat, amount)); }

	template <typename ToT, typename FromT>
	ToT & convertValue(const FromT &from, ToT &to);

	template <typename ToT, typename FromT, typename TInt>
	DSLib::Matrix<ToT, TInt> & convert(DSLib::Matrix<ToT, TInt> &dst, const DSLib::Matrix<FromT, TInt> &src);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & convert(DSLib::Matrix<T, TInt> &dst, const DSLib::Matrix<T, TInt> &src);

	template <typename TInt>
	DSLib::MatrixBase * createFromDataType(DSTypes::DataType dt, TInt tint=0);

	template <typename T, typename TInt>
	DSLib::MatrixBase * convertFromDataType(DSTypes::DataType dstType, DSLib::Matrix<T, TInt> &src);

	template <typename T, typename TInt>
	void transpose(DSLib::Matrix<T, TInt> &dst, DSLib::Matrix<T, TInt> &src);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> t(const DSLib::Matrix<T, TInt> &src); //inline transpose

	template <typename T, typename TInt>
	void multiply(DSLib::Matrix<T, TInt> &dst, DSLib::Matrix<T, TInt> &src1, DSLib::Matrix<T, TInt> &src2);	

	template <typename T, typename TInt>
	void inverse(DSLib::Matrix<T, TInt> &dst, DSLib::Matrix<T, TInt> &src);

	template <typename T, typename TInt>
	void determinant(T &dst, DSLib::Matrix<T, TInt> &src);

	template <typename T, typename TInt>
	void test(DSLib::Matrix<TInt, TInt> &dst, DSLib::Matrix<T, TInt> &lhs, DSLib::Matrix<T, TInt> &rhs, const std::function<bool(const T&, const T&)> &func);
	
	template <typename T, typename TInt>
	void test(DSLib::Matrix<TInt, TInt> &dst, DSLib::Matrix<T, TInt> &lhs, const T &rhs, const std::function<bool(const T&, const T&)> &func);

	template <typename T, typename TInt>
	void apply(DSLib::Matrix<T, TInt> &dst, DSLib::Matrix<T, TInt> &lhs, DSLib::Matrix<T, TInt> &rhs, const std::function<T(T&, const T&)> &func);

	template <typename T, typename TInt>
	void apply(DSLib::Matrix<T, TInt> &dst, DSLib::Matrix<T, TInt> &lhs, const T &rhs, const std::function<T(T&, const T&)> &func);

	template <typename T, typename TInt> 
	void setIntersection(DSLib::Matrix<T, TInt> &dst, DSLib::Matrix<T, TInt> &lhs, DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt> 
	void setUnion(DSLib::Matrix<T, TInt> &dst, DSLib::Matrix<T, TInt> &lhs, DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt>
	void setDifference(DSLib::Matrix<T, TInt> &dst, DSLib::Matrix<T, TInt> &lhs, DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt> 
	void range(DSLib::Matrix<T, TInt> &dst, DSLib::Matrix<T, TInt> &lhs, const T &rhs);

	template <typename T, typename TInt>
	void colMean(DSLib::Matrix<T, TInt> &dst, const DSLib::Matrix<T, TInt> &src);

	template <typename T, typename TInt, template <typename, typename> class TMat>
	void colMean(DSLib::Matrix<TMat<T, TInt>, TInt> &dst, const DSLib::Matrix<TMat<T, TInt>, TInt> &src);

	template <typename T, typename TInt>
	void colSD(DSLib::Matrix<T, TInt> &dst, const DSLib::Matrix<T, TInt> &src, const DSLib::Matrix<T, TInt> &mean = DSLib::Matrix<T, TInt>());

	template <typename T, typename TInt, template <typename, typename> class TMat>
	void colSD(DSLib::Matrix<TMat<T, TInt>, TInt> &dst, const DSLib::Matrix<TMat<T, TInt>, TInt> &src, const DSLib::Matrix<TMat<T, TInt>, TInt> &mean = DSLib::Matrix<TMat<T, TInt>, TInt>());

	template <typename T, typename TInt>
	void colVar(DSLib::Matrix<T, TInt> &dst, const DSLib::Matrix<T, TInt> &src, const DSLib::Matrix<T, TInt> &mean = DSLib::Matrix<T, TInt>());

	template <typename T, typename TInt, template <typename, typename> class TMat>
	void colVar(DSLib::Matrix<TMat<T, TInt>, TInt> &dst, const DSLib::Matrix<TMat<T, TInt>, TInt> &src, const DSLib::Matrix<TMat<T, TInt>, TInt> &mean = DSLib::Matrix<TMat<T, TInt>, TInt>());

	template <typename T, typename TInt>
	void colMin(DSLib::Matrix<T, TInt> &dst, const DSLib::Matrix<T, TInt> &src);

	template <typename T, typename TInt, template <typename, typename> class TMat>
	void colMin(DSLib::Matrix<TMat<T, TInt>, TInt> &dst, const DSLib::Matrix<TMat<T, TInt>, TInt> &src);

	template <typename T, typename TInt>
	void colMax(DSLib::Matrix<T, TInt> &dst, const DSLib::Matrix<T, TInt> &src);

	template <typename T, typename TInt, template <typename, typename> class TMat>
	void colMax(DSLib::Matrix<TMat<T, TInt>, TInt> &dst, const DSLib::Matrix<TMat<T, TInt>, TInt> &src);

	template <typename T, typename TInt>
	T sum(const DSLib::Matrix<T, TInt> &src);
	
	template <typename T, typename TInt>
	void round(DSLib::Matrix<T, TInt> &dst, TInt digits=0);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & eye(DSLib::Matrix<T, TInt> &dst, const TInt n);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & diag(DSLib::Matrix<T, TInt> &dst, const DSLib::Matrix<T, TInt> &v);

	template <typename T, typename TInt>
	std::ostream & operator<< (std::ostream &str, const DSLib::Matrix<T, TInt> &mat);

	template <typename T, typename TInt>
	std::ostream && operator<< (std::ostream &&str, const DSLib::Matrix<T, TInt> &mat);

	template <typename T, typename TInt>
	std::istream & operator>> (std::istream &str, DSLib::Matrix<T, TInt> &mat);

	template <typename T, typename TInt>
	std::istream && operator>> (std::istream &&str, DSLib::Matrix<T, TInt> &mat);

}
