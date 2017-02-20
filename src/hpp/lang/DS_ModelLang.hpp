#include "h/lang/DS_ModelLang.h"

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
	DSModel::Model<TIdx, TId> & operator!(DSModel::Model<TIdx, TId> &rhs) {
		if (rhs.last().execType() == DSTypes::etSynchronous)
			rhs.last().execType(DSTypes::etAsynchronous);
		else {
			rhs.last().execType(DSTypes::etSynchronous);
			rhs.last().wait();
		}		
		return rhs;
	}

	template <typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> && operator!(DSModel::Model<TIdx, TId> &&rhs) {
		if (rhs.last().execType() == DSTypes::etSynchronous)
			rhs.last().execType(DSTypes::etAsynchronous);
		else {
			rhs.last().execType(DSTypes::etSynchronous);
			rhs.last().wait();
		}
		return std::move(rhs);
	}

	//*******************//
	//** Concatenation **//
	//*******************//

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator|(DSModel::Model<TIdx, TId> &lhs, const DSModel::Model<TIdx, TId>& rhs) {		
		lhs.last().execType(DSTypes::etSynchronous);
		lhs.last().next(rhs);
		return lhs;
	}

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator|(DSModel::Model<TIdx, TId> &lhs, DSModel::Model<TIdx, TId>&& rhs) {		
		lhs.last().execType(DSTypes::etSynchronous);
		lhs.last().next(std::move(rhs));
		return lhs;
	}

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator|(DSModel::Model<TIdx, TId> &&lhs, DSModel::Model<TIdx, TId>& rhs) {
		rhs.first().prev(std::move(lhs));
		rhs.first().execType(DSTypes::etSynchronous);
		return rhs.first();
	}

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>&& operator|(DSModel::Model<TIdx, TId> &&lhs, DSModel::Model<TIdx, TId>&& rhs) {		
		lhs.last().execType(DSTypes::etSynchronous);
		lhs.last().next(std::move(rhs));
		return std::move(lhs);
	}

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator^(DSModel::Model<TIdx, TId> &lhs, const DSModel::Model<TIdx, TId>& rhs) {		
		lhs.last().execType(DSTypes::etAsynchronous);
		lhs.last().next(rhs);
		return lhs;
	}

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator^(DSModel::Model<TIdx, TId> &lhs, DSModel::Model<TIdx, TId>&& rhs) {				
		lhs.last().execType(DSTypes::etAsynchronous);
		lhs.last().next(std::move(rhs));
		return lhs;
	}

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator^(DSModel::Model<TIdx, TId> &&lhs, DSModel::Model<TIdx, TId>& rhs) {		
		rhs.first().execType(DSTypes::etAsynchronous);
		rhs.first().prev(std::move(lhs));
		return rhs.first();
	}

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>&& operator^(DSModel::Model<TIdx, TId> &&lhs, DSModel::Model<TIdx, TId>&& rhs) {
		lhs.last().execType(DSTypes::etAsynchronous);
		lhs.last().next(std::move(rhs));
		return std::move(lhs);
	}

	//*****************//
	//** PassThrough **//
	//*****************//
	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> & operator-(DSModel::Model<TIdx, TId> &m) {
		m.passThroughType(DSTypes::pttNone);
		return m;
	}

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> && operator-(DSModel::Model<TIdx, TId> &&m) {
		m.passThroughType(DSTypes::pttNone);
		return std::move(m);
	}

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> & operator+(DSModel::Model<TIdx, TId> &m) {
		m.passThroughType(DSTypes::pttAll);
		return m;
	}

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> && operator+(DSModel::Model<TIdx, TId> &&m) {
		m.passThroughType(DSTypes::pttAll);
		return std::move(m);
	}
	
	//******************/
	//** Combination **//
	//*****************//
	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator-(DSModel::Model<TIdx, TId> &lhs, const DSModel::Model<TIdx, TId>& rhs) {
		lhs.last().execType(DSTypes::etSynchronous);
		lhs.last().next(rhs);
		lhs.last().passThroughType(DSTypes::pttNone);
		return lhs;
	}

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator-(DSModel::Model<TIdx, TId> &lhs, DSModel::Model<TIdx, TId>&& rhs) {				
		lhs.last().execType(DSTypes::etSynchronous);
		lhs.last().next(std::move(rhs));
		lhs.last().passThroughType(DSTypes::pttNone);
		return lhs;
	}

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator-(DSModel::Model<TIdx, TId> &&lhs, DSModel::Model<TIdx, TId>& rhs) {		
		rhs.first().execType(DSTypes::etSynchronous);
		rhs.first().prev(std::move(lhs));
		rhs.first().passThroughType(DSTypes::pttNone);
		return rhs.first();
	}

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>&& operator-(DSModel::Model<TIdx, TId> &&lhs, DSModel::Model<TIdx, TId>&& rhs) {		
		lhs.last().execType(DSTypes::etSynchronous);
		lhs.last().next(std::move(rhs));
		lhs.last().passThroughType(DSTypes::pttNone);
		return std::move(lhs);
	}

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator+(DSModel::Model<TIdx, TId> &lhs, const DSModel::Model<TIdx, TId>& rhs) {		
		lhs.last().execType(DSTypes::etSynchronous);
		lhs.last().next(rhs);
		lhs.last().passThroughType(DSTypes::pttAll);
		return lhs;
	}

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator+(DSModel::Model<TIdx, TId> &lhs, DSModel::Model<TIdx, TId>&& rhs) {		
		lhs.last().execType(DSTypes::etSynchronous);
		lhs.last().next(std::move(rhs));
		lhs.last().passThroughType(DSTypes::pttAll);
		return lhs;
	}

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>& operator+(DSModel::Model<TIdx, TId> &&lhs, DSModel::Model<TIdx, TId>& rhs) {		
		rhs.first().execType(DSTypes::etSynchronous);
		rhs.first().prev(std::move(lhs));
		rhs.first().passThroughType(DSTypes::pttAll);
		return rhs.first();
	}

	template<typename TIdx, typename TId>
	DSModel::Model<TIdx, TId>&& operator+(DSModel::Model<TIdx, TId> &&lhs, DSModel::Model<TIdx, TId>&& rhs) {		
		lhs.last().execType(DSTypes::etSynchronous);
		lhs.last().next(std::move(rhs));
		lhs.last().passThroughType(DSTypes::pttAll);
		return std::move(lhs);
	}

	//***************//
	//** Execution **//
	//**************//	

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator>(const DSLib::Table<TIdx, TId> &t, DSModel::Model<TIdx, TId> &m) {
		return m.train(t);
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator>(const DSLib::Table<TIdx, TId> &&t, DSModel::Model<TIdx, TId> &m) {
		return m.train(std::move(t));
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator>(const DSLib::Table<TIdx, TId> &&t, DSModel::Model<TIdx, TId> &&m) {
		return std::move(m.train(std::move(t)));
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator>(const DSLib::Table<TIdx, TId> &t, DSModel::Model<TIdx, TId> &&m) {
		return std::move(m.train(t));
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator>>(const DSLib::Table<TIdx, TId> &t, DSModel::Model<TIdx, TId> &m) {
		return m.apply(t);
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator>>(const DSLib::Table<TIdx, TId> &&t, DSModel::Model<TIdx, TId> &m) {
		return m.apply(std::move(t));
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator>>(const DSLib::Table<TIdx, TId> &&t, DSModel::Model<TIdx, TId> &&m) {
		return std::move(m.apply(std::move(t)));
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator>>(const DSLib::Table<TIdx, TId> &t, DSModel::Model<TIdx, TId> &&m) {
		return std::move(m.apply(t));
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator<(const DSLib::Table<TIdx, TId> &t, DSModel::Model<TIdx, TId> &m) {
		return m.first().train(t, m);
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator<(const DSLib::Table<TIdx, TId> &&t, DSModel::Model<TIdx, TId> &m) {
		return m.first().train(std::move(t), m);
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator<(const DSLib::Table<TIdx, TId> &&t, DSModel::Model<TIdx, TId> &&m) {
		return std::move(m.first().train(std::move(t), std::move(m)));
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator<(const DSLib::Table<TIdx, TId> &t, DSModel::Model<TIdx, TId> &&m) {
		return std::move(m.first().train(t, std::move(m)));
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator << (const DSLib::Table<TIdx, TId> &t, DSModel::Model<TIdx, TId> &m) {
		return m.first().apply(t, m);
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator << (const DSLib::Table<TIdx, TId> &&t, DSModel::Model<TIdx, TId> &m) {
		return m.first().apply(std::move(t), m);
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator << (const DSLib::Table<TIdx, TId> &&t, DSModel::Model<TIdx, TId> &&m) {
		return std::move(m.first().apply(std::move(t), std::move(m)));
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator << (const DSLib::Table<TIdx, TId> &t, DSModel::Model<TIdx, TId> &&m) {
		return std::move(m.first().apply(t, std::move(m)));
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator<(DSModel::Model<TIdx, TId> &m, const DSLib::Table<TIdx, TId> &t) {
		return m.first().train(t, m);
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator<(DSModel::Model<TIdx, TId> &m, DSLib::Table<TIdx, TId> &&t) {
		return m.first().train(std::move(t), m);
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator<(DSModel::Model<TIdx, TId> &&m, DSLib::Table<TIdx, TId> &&t) {
		return std::move(m.first().train(std::move(t), m));
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator<(DSModel::Model<TIdx, TId> &&m, const DSLib::Table<TIdx, TId> &t) {
		return std::move(m.first().train(t, m));
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator<<(DSModel::Model<TIdx, TId> &m, const DSLib::Table<TIdx, TId> &t) {
		return m.first().apply(t, m);
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator<<(DSModel::Model<TIdx, TId> &m, DSLib::Table<TIdx, TId> &&t) {
		return m.first().apply(std::move(t), m);
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator<<(DSModel::Model<TIdx, TId> &&m, DSLib::Table<TIdx, TId> &&t) {
		return std::move(m.first().apply(std::move(t), m));
	}

	template<typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator<<(DSModel::Model<TIdx, TId> &&m, const DSLib::Table<TIdx, TId> &t) {
		return std::move(m.first().apply(t, m));
	}

	//**************//
	//** Counting **//
	//**************//
	template <typename TIdx, typename TId>
	TIdx operator~(const DSModel::Model<TIdx, TId> &rhs) {
		return rhs.count();
	}

	//***********//
	//** Print **//
	//***********//	
	template <typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> & operator++(DSModel::Model<TIdx, TId> &m, int){
		m.print(std::cout);
		return m;
	}

	template <typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> && operator++(DSModel::Model<TIdx, TId> &&m, int) {
		m.print(std::cout);
		return std::move(m);
	}

	template <typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> & operator--(DSModel::Model<TIdx, TId> &m, int){
		m.printLess(std::cout);
		return m;
	}

	template <typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> && operator--(DSModel::Model<TIdx, TId> &&m, int) {
		m.printLess(std::cout);
		return std::move(m);
	}

	//*************//
	//** Writing **//
	//*************//
	template <typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> & operator>>(DSModel::Model<TIdx, TId> &lhs, const DSTypes::String &rhs) {
		lhs.write(rhs);
		return(lhs);
	}

	template <typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> && operator>>(DSModel::Model<TIdx, TId> &&lhs, const DSTypes::String &rhs) {
		lhs.write(rhs);
		return std::move(lhs);
	}

	template <typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> & operator<<(DSModel::Model<TIdx, TId> &lhs, const DSTypes::String &rhs) {
		lhs.read(rhs);
		return lhs;
	}

	template <typename TIdx, typename TId>
	DSModel::Model<TIdx, TId> && operator<<(DSModel::Model<TIdx, TId> &&lhs, const DSTypes::String &rhs) {
		lhs.read(rhs);
		return std::move(lhs);
	}

	//*************//
	//** Slicing **//
	//*************//
	//Defined as member functions

};

#ifdef MSVS_VER
#pragma warning(pop)
#endif
