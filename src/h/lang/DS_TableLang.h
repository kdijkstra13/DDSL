#pragma once

#ifdef MSVC_VER
	#pragma warning(push)
	//#pragma warning(disable: 4244)
#endif

#include "h/DS_Types.h"
#include "h/lib/DS_Table.h"

namespace DSLang {

	//****************//
	//** Assignment **//
	//****************//
	//BreakUp matrix
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator!(DSLib::Table<TIdx, TId> &rhs);

	template <typename TIdx, typename TId> 
	DSLib::Table<TIdx, TIdx> && operator!(DSLib::Table<TIdx, TId> &&rhs);

	//Inline Duplicate Table
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> operator*(const DSLib::Table<TIdx, TId> &rhs);

	//********************//
	//** Initialization **//
	//********************//

	//Initialize with a matrix
	template <typename T, typename TIdx, typename TId=DSTypes::TableId>
	DSLib::Table<TIdx> operator|(DSTypes::ContentType lhs, const DSLib::Matrix<T, TIdx> &rhs);

	template <typename T, typename TIdx>
	DSLib::Table<TIdx> operator^(DSTypes::ContentType lhs, const DSLib::Matrix<T, TIdx> &rhs);

	//*******************//
	//** Concatenation **//
	//*******************//
	//Concat two tables
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator|(DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs);

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator^(DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs);

	//Concat a table and a matrix
	template <typename TIdx, typename TId, typename T>
	DSLib::Table<TIdx, TId> & operator|(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs);

	template <typename TIdx, typename TId, typename T>
	DSLib::Table<TIdx, TId> & operator^(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs);

	template <typename TIdx, typename TId> DSLib::Table<TIdx, TId> operator|(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Table<TIdx, TId> &rhs);
	template <typename TIdx, typename TId> DSLib::Table<TIdx, TId> operator^(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Table<TIdx, TId> &rhs);
	template <typename TIdx, typename TId, typename T> DSLib::Table<TIdx, TId> operator|(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Matrix<T, TIdx> &rhs);
	template <typename TIdx, typename TId, typename T> DSLib::Table<TIdx, TId> operator^(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Matrix<T, TIdx> &rhs);

	//*************************//
	//** Specify Col/Row Ids **//
	//*************************//
	//Set RowIds
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator|(const DSLib::Matrix<TId, TIdx> &lhs, DSLib::Table<TIdx, TId> &rhs);

	//Set ColIds
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator^(const DSLib::Matrix<TId, TIdx> &lhs, DSLib::Table<TIdx, TId> &rhs);

	//Set ContentTypes
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator^(const DSLib::Matrix<DSTypes::ContentType, TIdx> &lhs, DSLib::Table<TIdx, TId> &rhs);

	template <typename TIdx, typename TId> DSLib::Table<TIdx, TId> operator|(const DSLib::Matrix<TId, TIdx> &lhs, DSLib::Table<TIdx, TId> &&rhs);
	template <typename TIdx, typename TId> DSLib::Table<TIdx, TId> operator^(const DSLib::Matrix<TId, TIdx> &lhs, DSLib::Table<TIdx, TId> &&rhs);
	template <typename TIdx, typename TId> DSLib::Table<TIdx, TId> operator^(const DSLib::Matrix<DSTypes::ContentType, TIdx> &lhs, DSLib::Table<TIdx, TId> &&rhs);

	//*******************//
	//** Selection **//
	//*******************//
	//+ = Table<>
	//- = Table<>.isRowTuple()
	//| = Table<>.isColTuple()
	//. = T
	//Return indices (+ @ +) Table index, (+ @ -) row indices of rowTuple of Table, (+ @ .) row|col indices of Table, (| @ .) row indices, (- @ .) col indices
	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator==(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs);

	template <typename TIdx, typename TId, typename T>
	DSLib::Matrix<TIdx, TIdx> operator==(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs);

	template <typename T, typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator==(const DSLib::Table<TIdx, TId> &lhs, const T &rhs);

	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator!=(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs);

	template <typename TIdx, typename TId, typename T>
	DSLib::Matrix<TIdx, TIdx> operator!=(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs);

	template <typename T, typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator!=(const DSLib::Table<TIdx, TId> &lhs, const T &rhs);

	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator<(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs);

	template <typename TIdx, typename TId, typename T>
	DSLib::Matrix<TIdx, TIdx> operator<(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs);

	template <typename T, typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator<(const DSLib::Table<TIdx, TId> &lhs, const T &rhs);

	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator>(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs);

	template <typename TIdx, typename TId, typename T>
	DSLib::Matrix<TIdx, TIdx> operator>(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs);
	
	template <typename TIdx, typename TId, int Len>
	DSLib::Matrix<TIdx, TIdx> operator==(const DSLib::Table<TIdx, TId> &lhs, const char(&rhs)[Len]){return lhs == String(rhs);}

	//For string literal
	template <typename TIdx, typename TId, int Len> DSLib::Matrix<TIdx, TIdx> operator!=(const DSLib::Table<TIdx, TId> &lhs, const char(&rhs)[Len]){return lhs != String(rhs);}
	template <typename TIdx, typename TId, int Len> DSLib::Matrix<TIdx, TIdx> operator>(const DSLib::Table<TIdx, TId> &lhs, const char(&rhs)[Len]){return lhs > String(rhs);}
	template <typename TIdx, typename TId, int Len> DSLib::Matrix<TIdx, TIdx> operator<(const DSLib::Table<TIdx, TId> &lhs, const char(&rhs)[Len]){return lhs < String(rhs);}
	template <typename TIdx, typename TId, int Len> DSLib::Matrix<TIdx, TIdx> operator>=(const DSLib::Table<TIdx, TId> &lhs, const char(&rhs)[Len]){return lhs >= String(rhs);}
	template <typename TIdx, typename TId, int Len> DSLib::Matrix<TIdx, TIdx> operator<=(const DSLib::Table<TIdx, TId> &lhs, const char(&rhs)[Len]){return lhs <= String(rhs);}

	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator<=(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs);

	template <typename TIdx, typename TId, typename T>
	DSLib::Matrix<TIdx, TIdx> operator<=(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs);

	template <typename T, typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator<=(const DSLib::Table<TIdx, TId> &lhs, const T &rhs);

	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator>=(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs);

	template <typename TIdx, typename TId, typename T>
	DSLib::Matrix<TIdx, TIdx> operator>=(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs);

	template <typename T, typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator>=(const DSLib::Table<TIdx, TId> &lhs, const T &rhs);

	//**************//
	//** Counting **//
	//**************//
	//Get the number of cells (slices give you the number of rows/cols
	template <typename TIdx, typename TId> TIdx operator~(const DSLib::Table<TIdx, TId> &rhs);

	//***********//
	//** Print **//
	//***********//
	//Print full Matrix
	template <typename TIdx, typename TId> DSLib::Table<TIdx, TId> & operator++(const DSLib::Table<TIdx, TId> &lhs, int);

	//Print Matrix meta information
	template <typename TIdx, typename TId> DSLib::Table<TIdx, TId> & operator--(const DSLib::Table<TIdx, TId> &lhs, int);

	//****************//
	//** Arithmetic **//
	//****************//
	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & operator+(DSLib::Table<TIdx, TId> &lhs, const T &rhs);

	template <typename TIdx, typename TId, typename T> 
	DSLib::Table<TIdx, TId> & operator-(DSLib::Table<TIdx, TId> &lhs, const T &rhs);

	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & operator+(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs);
	
	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & operator*(DSLib::Table<TIdx, TId> &lhs, const T &rhs);
	
	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & operator*(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs);

	template <typename TIdx, typename TId, typename T> 
	DSLib::Table<TIdx, TId> & operator/(DSLib::Table<TIdx, TId> &lhs, const T &rhs);
	
	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & operator/(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs);
	
	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & pow(DSLib::Table<TIdx, TId> &lhs, const T &rhs);
	
	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & pow(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs);

	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & operator%(DSLib::Table<TIdx, TId> &lhs, const T &rhs);

	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & operator%(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs);

	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator+(DSLib::Table<TIdx, TId> &&lhs, const T &rhs);
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator+(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Matrix<T, TIdx> &rhs);
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator-(DSLib::Table<TIdx, TId> &&lhs, const T &rhs);
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator-(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Matrix<T, TIdx> &rhs);
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator*(DSLib::Table<TIdx, TId> &&lhs, const T &rhs);
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator*(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Matrix<T, TIdx> &rhs);
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator/(DSLib::Table<TIdx, TId> &&lhs, const T &rhs);
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator/(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Matrix<T, TIdx> &rhs);
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator%(DSLib::Table<TIdx, TId> &&lhs, const T &rhs);
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator%(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Matrix<T, TIdx> &rhs);
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> pow(DSLib::Table<TIdx, TId> &&lhs, const T &rhs);
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> pow(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Matrix<T, TIdx> &rhs);

	//***************//
	//** Streaming **//
	//***************//
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator >> (DSLib::Table<TIdx, TId> &lhs, std::ostream &rhs);

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator >> (DSLib::Table<TIdx, TId> &&lhs, std::ostream &rhs);

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator<<(DSLib::Table<TIdx, TId> &lhs, std::istream &rhs);

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator<<(DSLib::Table<TIdx, TId> &&lhs, std::istream &rhs);

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator >> (DSLib::Table<TIdx, TId> &lhs, const DSTypes::String &rhs);

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator >> (DSLib::Table<TIdx, TId> &&lhs, const DSTypes::String &rhs);

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator<<(DSLib::Table<TIdx, TId> &lhs, const DSTypes::String &rhs);

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator<<(DSLib::Table<TIdx, TId> &&lhs, const DSTypes::String &rhs);

	//*************//
	//** Slicing **//
	//*************//
	//Defined as member functions

};

#ifdef MSVS_VER
	#pragma warning(pop)
#endif
