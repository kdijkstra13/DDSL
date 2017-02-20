#pragma once

#ifdef MSVC_VER
#pragma warning(push)
//#pragma warning(disable: 4244)
#endif

#include "hpp/model/DS_Model.hpp"

namespace DSLang {
	//***********//
	//** Join **//
	//**********//
	template <typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> & operator!(DSModel::Model<TIdx, TId> &rhs);

	template <typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> && operator!(DSModel::Model<TIdx, TId> &&rhs);

	//*******************//
	//** Concatenation **//
	//*******************//
	
	//Concat synchronous
	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator|(DSModel::Model<TIdx, TId> &lhs, const DSModel::Model<TIdx, TId>& rhs);

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator|(DSModel::Model<TIdx, TId> &lhs, DSModel::Model<TIdx, TId>&& rhs);

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator|(DSModel::Model<TIdx, TId> &&lhs, DSModel::Model<TIdx, TId>& rhs);

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>&& operator|(DSModel::Model<TIdx, TId> &&lhs, DSModel::Model<TIdx, TId>&& rhs);

	//Concat asynchronous
	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator^(DSModel::Model<TIdx, TId> &lhs, const DSModel::Model<TIdx, TId>& rhs);

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator^(DSModel::Model<TIdx, TId> &lhs, DSModel::Model<TIdx, TId>&& rhs);

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator^(DSModel::Model<TIdx, TId> &&lhs, DSModel::Model<TIdx, TId>& rhs);

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>&& operator^(DSModel::Model<TIdx, TId> &&lhs, DSModel::Model<TIdx, TId>&& rhs);

	//*****************//
	//** PassThrough **//
	//*****************//
	
	//pttNone
	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> & operator-(DSModel::Model<TIdx, TId> &m);

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> && operator-(DSModel::Model<TIdx, TId> &&m);

	//pttAll
	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> & operator+(DSModel::Model<TIdx, TId> &m);

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> && operator+(DSModel::Model<TIdx, TId> &&m);
	
	//******************/
	//** Combination **//
	//*****************//
	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator-(DSModel::Model<TIdx, TId> &lhs, const DSModel::Model<TIdx, TId>& rhs);

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator-(DSModel::Model<TIdx, TId> &lhs, DSModel::Model<TIdx, TId>&& rhs);

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator-(DSModel::Model<TIdx, TId> &&lhs, DSModel::Model<TIdx, TId>& rhs);

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>&& operator-(DSModel::Model<TIdx, TId> &&lhs, DSModel::Model<TIdx, TId>&& rhs);

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator+(DSModel::Model<TIdx, TId> &lhs, const DSModel::Model<TIdx, TId>& rhs);

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator+(DSModel::Model<TIdx, TId> &lhs, DSModel::Model<TIdx, TId>&& rhs);

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator+(DSModel::Model<TIdx, TId> &&lhs, DSModel::Model<TIdx, TId>& rhs);

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>&& operator+(DSModel::Model<TIdx, TId> &&lhs, DSModel::Model<TIdx, TId>&& rhs);

	//***************//
	//** Execution **//
	//**************//	

	//Train pipeline
	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator>(const DSLib::Table<TIdx, TId> &t, DSModel::Model<TIdx, TId> &m);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator>(const DSLib::Table<TIdx, TId> &&t, DSModel::Model<TIdx, TId> &m);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator>(const DSLib::Table<TIdx, TId> &&t, DSModel::Model<TIdx, TId> &&m);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator>(const DSLib::Table<TIdx, TId> &t, DSModel::Model<TIdx, TId> &&m);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator>>(const DSLib::Table<TIdx, TId> &t, DSModel::Model<TIdx, TId> &m);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator>>(const DSLib::Table<TIdx, TId> &&t, DSModel::Model<TIdx, TId> &m);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator>>(const DSLib::Table<TIdx, TId> &&t, DSModel::Model<TIdx, TId> &&m);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator>>(const DSLib::Table<TIdx, TId> &t, DSModel::Model<TIdx, TId> &&m);

	//Train from first model to current model
	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator<(const DSLib::Table<TIdx, TId> &t, DSModel::Model<TIdx, TId> &m);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator<(const DSLib::Table<TIdx, TId> &&t, DSModel::Model<TIdx, TId> &m);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator<(const DSLib::Table<TIdx, TId> &&t, DSModel::Model<TIdx, TId> &&m);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator<(const DSLib::Table<TIdx, TId> &t, DSModel::Model<TIdx, TId> &&m);

	//Apply from first model to current model
	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator << (const DSLib::Table<TIdx, TId> &t, DSModel::Model<TIdx, TId> &m);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator << (const DSLib::Table<TIdx, TId> &&t, DSModel::Model<TIdx, TId> &m);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator << (const DSLib::Table<TIdx, TId> &&t, DSModel::Model<TIdx, TId> &&m);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator << (const DSLib::Table<TIdx, TId> &t, DSModel::Model<TIdx, TId> &&m);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator<(DSModel::Model<TIdx, TId> &m, const DSLib::Table<TIdx, TId> &t);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator<(DSModel::Model<TIdx, TId> &m, DSLib::Table<TIdx, TId> &&t);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator<(DSModel::Model<TIdx, TId> &&m, DSLib::Table<TIdx, TId> &&t);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator<(DSModel::Model<TIdx, TId> &&m, const DSLib::Table<TIdx, TId> &t);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator<<(DSModel::Model<TIdx, TId> &m, const DSLib::Table<TIdx, TId> &t);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator<<(DSModel::Model<TIdx, TId> &m, DSLib::Table<TIdx, TId> &&t);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator<<(DSModel::Model<TIdx, TId> &&m, DSLib::Table<TIdx, TId> &&t);

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator<<(DSModel::Model<TIdx, TId> &&m, const DSLib::Table<TIdx, TId> &t);

	//**************//
	//** Counting **//
	//**************//
	
	//Count models in pipeline
	template <typename TIdx, typename TId>
	TIdx operator~(const DSModel::Model<TIdx, TId> &rhs);

	//***********//
	//** Print **//
	//***********//	
	
	//Print pipeline
	template <typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> & operator++(DSModel::Model<TIdx, TId> &m, int);

	template <typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> && operator++(DSModel::Model<TIdx, TId> &&m, int);

	template <typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> & operator--(DSModel::Model<TIdx, TId> &m, int);

	template <typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> && operator--(DSModel::Model<TIdx, TId> &&m, int);

	//*************//
	//** Writing **//
	//*************//
	template <typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> & operator>>(DSModel::Model<TIdx, TId> &lhs, const DSTypes::String &rhs);

	template <typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> && operator>>(DSModel::Model<TIdx, TId> &&lhs, const DSTypes::String &rhs);

	template <typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> & operator<<(DSModel::Model<TIdx, TId> &lhs, const DSTypes::String &rhs);

	template <typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> && operator<<(DSModel::Model<TIdx, TId> &&lhs, const DSTypes::String &rhs);

	//*************//
	//** Slicing **//
	//*************//
	//Defined as member functions

};

#ifdef MSVS_VER
#pragma warning(pop)
#endif
