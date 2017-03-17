#pragma once

#ifdef MSVC_VER
  #pragma warning(push)
  #pragma warning(disable: 4244)
#endif

#include "h/DS_Types.h"
#include "h/lib/DS_Matrix.h"
#include "h/lib/DS_MatrixFunc.h"

namespace DSLang {
	//***********************//
	//** Matrix Assignment **//
	//***********************//

	//Transpose Matrix
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator+=(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	//Matrix Multiplication
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator*=(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	//Force Matrix Order to oColMajor
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator|=(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	//Force Matrix Order to oRowMajor
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator^=(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	//Inline BreakUp Matrix
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator!(DSLib::Matrix<T, TInt> &rhs);

	//Inline Duplicate Matrix
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> operator*(const DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator|=(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator^=(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator+=(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator*=(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator!(DSLib::Matrix<T, TInt> &&rhs);

	//***************************//
	//** Matrix Initialization **//
	//***************************//

	//Concat two Matrices
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator|(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator^(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	//Concat a value to a matrix (works only for vector matrices)
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator|(DSLib::Matrix<T, TInt> &lhs, const T &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator^(DSLib::Matrix<T, TInt> &lhs, const T &rhs);

	//Create new matrix
	template <typename T>
	DSLib::Matrix<T, DSTypes::UInt32> operator|(DSTypes::DataType lhs, const T &rhs);

	template <typename T>
	DSLib::Matrix<T, DSTypes::UInt32> operator^(DSTypes::DataType lhs, const T &rhs);

	//Generate a range (works only for ordinal types)
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator||(DSLib::Matrix<T, TInt> &lhs, const T &rhs);

	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator|(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator^(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator|(DSLib::Matrix<T, TInt> &&lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator^(DSLib::Matrix<T, TInt> &&lhs, const T &rhs);

	//Special templates to handle "literal" (Header file code)
	template <int T, typename TInt> DSLib::Matrix<DSTypes::Literal, TInt> & operator|(DSLib::Matrix<DSTypes::Literal, TInt> &lhs, const char(&rhs)[T]) { lhs.cols.add((const char *)rhs);return lhs; }
	template <int T, typename TInt> DSLib::Matrix<DSTypes::Literal, TInt> operator|(DSLib::Matrix<DSTypes::Literal, TInt> &&lhs, const char(&rhs)[T]) { return std::move(lhs | rhs); };
	template <int T, typename TInt> DSLib::Matrix<DSTypes::Literal, TInt> & operator^(DSLib::Matrix<DSTypes::Literal, TInt> &lhs, const char(&rhs)[T]) { lhs.rows.add((const char *)rhs);return lhs; }
	template <int T, typename TInt> DSLib::Matrix<DSTypes::Literal, TInt> operator^(DSLib::Matrix<DSTypes::Literal, TInt> &&lhs, const char(&rhs)[T]) { return std::move(lhs ^ rhs); };
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator||(DSLib::Matrix<T, TInt> &&lhs, const T &rhs) { return std::move(lhs || rhs); };
	template <int T> DSLib::Matrix<DSTypes::Literal, DSTypes::UInt32> operator|(DSTypes::DataType lhs, const char(&rhs)[T]) { DSLib::Matrix<DSTypes::Literal, DSTypes::UInt32> m(DSTypes::oRowMajor);m.rows.add((DSTypes::Literal)rhs);return m; }
	template <int T> DSLib::Matrix<DSTypes::Literal, DSTypes::UInt32> operator^(DSTypes::DataType lhs, const char(&rhs)[T]) { DSLib::Matrix<DSTypes::Literal, DSTypes::UInt32> m(DSTypes::oColMajor);m.rows.add((DSTypes::Literal)rhs);return m; }

	//*******************//
	//** Row Selection **//
	//*******************//
	//+ = Matrix<T>
	//- = Matrix<T>.isRowVector()
	//| = Matrix<T>.isColVector()
	//. = T
	//Return indices (+ @ +) matrix index, (+ @ -) row indices of rowVector of matrix, (+ @ |) col indices of colVector of matrix, (+ == .) row|col indices of matrix, (| == .) row indices, (- == .) col indices

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator==(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator==(const DSLib::Matrix<T, TInt> &lhs, const T &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator!=(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator!=(const DSLib::Matrix<T, TInt> &lhs, const T &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator<=(const DSLib::Matrix<T, TInt> &lhs, const T &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator>=(const DSLib::Matrix<T, TInt> &lhs, const T &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator<=(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator>=(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator<(const DSLib::Matrix<T, TInt> &lhs, const T &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator>(const DSLib::Matrix<T, TInt> &lhs, const T &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator<(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator>(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	//***************//
	//** Set logic **//
	//***************//

	//Set intersection between indices
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator&&(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	//Set union between indices
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator||(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator&&(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator||(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs);

	//****************//
	//** Arithmetic **//
	//****************//
	//+ = Matrix<T>
	//- = Matrix<T>.isRowVector()
	//| = Matrix<T>.isColVector()
	//. = T
	//Return indices (+ @ +) per cell of both matrices, (+ @ -) per rowVector of matrix, (+ @ |) per colVector of matrix, (+ == .) per cel of matrix, (| @ .) @ per cel of colVector, (- @ .) @ per cel of rowVector

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator+(DSLib::Matrix<T, TInt> &lhs, const T &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator+(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator-(DSLib::Matrix<T, TInt> &lhs, const T &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator-(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator-(DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator/(DSLib::Matrix<T, TInt> &lhs, const T &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator/(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator*(DSLib::Matrix<T, TInt> &lhs, const T &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator*(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & pow(DSLib::Matrix<T, TInt> &lhs, const T &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & pow(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & min(DSLib::Matrix<T, TInt> &lhs, const T &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & min(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & max(DSLib::Matrix<T, TInt> &lhs, const T &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & max(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator%(DSLib::Matrix<T, TInt> &lhs, const T &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator%(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator+(DSLib::Matrix<T, TInt> &&lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator+(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator-(DSLib::Matrix<T, TInt> &&rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator/(DSLib::Matrix<T, TInt> &&lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator/(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator*(DSLib::Matrix<T, TInt> &&lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator*(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> pow(DSLib::Matrix<T, TInt> &&lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> pow(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator%(DSLib::Matrix<T, TInt> &&lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator%(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs);

	//**************//
	//** Counting **//
	//**************//
	//Get the number of cells (slices give you the number of rows/cols

	//Return number of cells
	template <typename T, typename TInt>
	TInt operator~(const DSLib::Matrix<T, TInt> &rhs);

	//***********//
	//** Print **//
	//***********//

	//Print full Matrix
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator++(DSLib::Matrix<T, TInt> &lhs, int);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> operator++(DSLib::Matrix<T, TInt> &&lhs, int);

	template <typename T, typename TInt>
	const DSLib::Matrix<T, TInt> & operator++(const DSLib::Matrix<T, TInt> &lhs, int);

	//Print Matrix meta information
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> operator--(DSLib::Matrix<T, TInt> &&lhs, int);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator--(DSLib::Matrix<T, TInt> &lhs, int);

	template <typename T, typename TInt>
	const DSLib::Matrix<T, TInt> & operator--(const DSLib::Matrix<T, TInt> &lhs, int);

	//Print full matrix
    template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator++(DSLib::Matrix<T, TInt> &lhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> operator++(DSLib::Matrix<T, TInt> &&lhs);

	template <typename T, typename TInt>
	const DSLib::Matrix<T, TInt> & operator++(const DSLib::Matrix<T, TInt> &lhs);

	//Print Matrix meta information
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> operator--(DSLib::Matrix<T, TInt> &&lhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator--(DSLib::Matrix<T, TInt> &lhs);

	template <typename T, typename TInt>
	const DSLib::Matrix<T, TInt> & operator--(const DSLib::Matrix<T, TInt> &lhs);


	//**********************//
	//** Matrix streaming **//
	//**********************//
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator>>(DSLib::Matrix<T, TInt> &lhs, std::ostream &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> operator>>(DSLib::Matrix<T, TInt> &&lhs, std::ostream &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator<<(DSLib::Matrix<T, TInt> &lhs, std::istream &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> operator<<(DSLib::Matrix<T, TInt> &&lhs, std::istream &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator >> (DSLib::Matrix<T, TInt> &lhs, const DSTypes::String &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> operator >> (DSLib::Matrix<T, TInt> &&lhs, const DSTypes::String &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator<<(DSLib::Matrix<T, TInt> &lhs, const DSTypes::String &rhs);

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> operator<<(DSLib::Matrix<T, TInt> &&lhs, const DSTypes::String &rhs);

	//********************//
	//** Matrix Slicing **//
	//********************//
	//Defined as member functions
};


namespace DSScript {
	//***********************//
	//** Matrix Assignment **//
	//***********************//
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator+=(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator*=(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator|=(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator^=(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator!(DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator*(const DSLib::Matrix<T, TInt> &rhs);

	//***************************//
	//** Matrix Initialization **//
	//***************************//
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator|(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator^(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator|(DSLib::Matrix<T, TInt> &lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator^(DSLib::Matrix<T, TInt> &lhs, const T &rhs);
	template <typename T> DSLib::Matrix<T, DSTypes::UInt32> operator|(DSTypes::DataType lhs, const T &rhs);
	template <typename T> DSLib::Matrix<T, DSTypes::UInt32> operator^(DSTypes::DataType lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator||(DSLib::Matrix<T, TInt> &lhs, const T &rhs);

	//*******************//
	//** Row Selection **//
	//*******************//
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator==(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator==(const DSLib::Matrix<T, TInt> &lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator!=(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator!=(const DSLib::Matrix<T, TInt> &lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator<=(const DSLib::Matrix<T, TInt> &lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator>=(const DSLib::Matrix<T, TInt> &lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator<=(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator>=(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator<(const DSLib::Matrix<T, TInt> &lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator>(const DSLib::Matrix<T, TInt> &lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator<(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator>(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	//***************//
	//** Set logic **//
	//***************//
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator&&(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator||(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	//****************//
	//** Arithmetic **//
	//****************//
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator+(DSLib::Matrix<T, TInt> &lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator+(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator-(DSLib::Matrix<T, TInt> &lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator-(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator-(DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator/(DSLib::Matrix<T, TInt> &lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator/(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator*(DSLib::Matrix<T, TInt> &lhs, const T &rhs);
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator*(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> pow(DSLib::Matrix<T, TInt> &lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> pow(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> min(DSLib::Matrix<T, TInt> &lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> min(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> max(DSLib::Matrix<T, TInt> &lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> max(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator%(DSLib::Matrix<T, TInt> &lhs, const T &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator%(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs);

	//**************//
	//** Counting **//
	//**************//
	template <typename T, typename TInt> TInt operator~(const DSLib::Matrix<T, TInt> &rhs);

	//***********//
	//** Print **//
	//***********//
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator++(const DSLib::Matrix<T, TInt> &lhs, int);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator++(const DSLib::Matrix<T, TInt> &lhs);
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator--(const DSLib::Matrix<T, TInt> &lhs, int);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator--(const DSLib::Matrix<T, TInt> &lhs);

	//**********************//
	//** Matrix streaming **//
	//**********************//
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator>>(DSLib::Matrix<T, TInt> &lhs, std::ostream &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator<<(DSLib::Matrix<T, TInt> &lhs, std::istream &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator>>(DSLib::Matrix<T, TInt> &lhs, const DSTypes::String &rhs);
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator<<(DSLib::Matrix<T, TInt> &lhs, const DSTypes::String &rhs);

	//********************//
	//** Matrix Slicing **//
	//********************//
	/*template <typename T, typename TInt> DSLib::Matrix<T, TInt> &operator=(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &other);
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> &operator=(DSLib::Matrix<T, TInt> &lhs, const T &val);
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()();
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TInt> &lhs, const TInt row);
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<TInt, TInt> &rowIndices);
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TInt> &lhs, const TInt rowBegin, const TInt rowLength);
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TInt> &lhs, const TInt rowBegin, const TInt rowLength, const TInt colBegin, const TInt colLength);
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<TInt, TInt> &rowIndices, const DSLib::Matrix<TInt, TInt> &colIndices);
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator[](const DSLib::Matrix<T, TInt> &lhs, const TInt col);
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator[](const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<TInt, TInt> &colIndices);
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<TInt, TInt> &rowIndices, const TInt col);
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<TInt, TInt> &rowIndices, const TInt colBegin, const TInt colLength);
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TInt> &lhs, const TInt row, const Matrix<TInt, TInt> &colIndices);
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TInt> &lhs, const TInt rowBegin, const TInt rowLength, const DSLib::Matrix<TInt, TInt> &colIndices);
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TInt> &lhs, const TInt rowBegin, const TInt rowLength, const TInt col);*/
};

#ifdef MSVC_VER
	#pragma warning(pop)
#endif
