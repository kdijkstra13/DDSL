#include "hpp/lang/DS_ModelLang.hpp"

#ifdef MSVC_VER
#pragma warning(push)
//#pragma warning(disable: 4244)
#endif

#include "hpp/model/DS_Model.hpp"

namespace DSLang {
	//DSTypes::UInt32 = DSTypes::UInt32, DSTypes::String=DSTypes::UInt32
	template DSModel::Model<DSTypes::UInt32, DSTypes::String> & operator!(DSModel::Model<DSTypes::UInt32, DSTypes::String> &rhs);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String> && operator!(DSModel::Model<DSTypes::UInt32, DSTypes::String> &&rhs);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String>& operator|(DSModel::Model<DSTypes::UInt32, DSTypes::String> &lhs, const DSModel::Model<DSTypes::UInt32, DSTypes::String>& rhs);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String>& operator|(DSModel::Model<DSTypes::UInt32, DSTypes::String> &lhs, DSModel::Model<DSTypes::UInt32, DSTypes::String>&& rhs);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String>& operator|(DSModel::Model<DSTypes::UInt32, DSTypes::String> &&lhs, DSModel::Model<DSTypes::UInt32, DSTypes::String>& rhs);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String>&& operator|(DSModel::Model<DSTypes::UInt32, DSTypes::String> &&lhs, DSModel::Model<DSTypes::UInt32, DSTypes::String>&& rhs);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String>& operator^(DSModel::Model<DSTypes::UInt32, DSTypes::String> &lhs, const DSModel::Model<DSTypes::UInt32, DSTypes::String>& rhs);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String>& operator^(DSModel::Model<DSTypes::UInt32, DSTypes::String> &lhs, DSModel::Model<DSTypes::UInt32, DSTypes::String>&& rhs);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String>& operator^(DSModel::Model<DSTypes::UInt32, DSTypes::String> &&lhs, DSModel::Model<DSTypes::UInt32, DSTypes::String>& rhs);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String>&& operator^(DSModel::Model<DSTypes::UInt32, DSTypes::String> &&lhs, DSModel::Model<DSTypes::UInt32, DSTypes::String>&& rhs);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String> & operator-(DSModel::Model<DSTypes::UInt32, DSTypes::String> &m);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String> && operator-(DSModel::Model<DSTypes::UInt32, DSTypes::String> &&m);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String> & operator+(DSModel::Model<DSTypes::UInt32, DSTypes::String> &m);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String> && operator+(DSModel::Model<DSTypes::UInt32, DSTypes::String> &&m);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String>& operator-(DSModel::Model<DSTypes::UInt32, DSTypes::String> &lhs, const DSModel::Model<DSTypes::UInt32, DSTypes::String>& rhs);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String>& operator-(DSModel::Model<DSTypes::UInt32, DSTypes::String> &lhs, DSModel::Model<DSTypes::UInt32, DSTypes::String>&& rhs);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String>& operator-(DSModel::Model<DSTypes::UInt32, DSTypes::String> &&lhs, DSModel::Model<DSTypes::UInt32, DSTypes::String>& rhs);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String>&& operator-(DSModel::Model<DSTypes::UInt32, DSTypes::String> &&lhs, DSModel::Model<DSTypes::UInt32, DSTypes::String>&& rhs);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String>& operator+(DSModel::Model<DSTypes::UInt32, DSTypes::String> &lhs, const DSModel::Model<DSTypes::UInt32, DSTypes::String>& rhs);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String>& operator+(DSModel::Model<DSTypes::UInt32, DSTypes::String> &lhs, DSModel::Model<DSTypes::UInt32, DSTypes::String>&& rhs);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String>& operator+(DSModel::Model<DSTypes::UInt32, DSTypes::String> &&lhs, DSModel::Model<DSTypes::UInt32, DSTypes::String>& rhs);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String>&& operator+(DSModel::Model<DSTypes::UInt32, DSTypes::String> &&lhs, DSModel::Model<DSTypes::UInt32, DSTypes::String>&& rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator>(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &t, DSModel::Model<DSTypes::UInt32, DSTypes::String> &m);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator>(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &&t, DSModel::Model<DSTypes::UInt32, DSTypes::String> &m);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> && operator>(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &&t, DSModel::Model<DSTypes::UInt32, DSTypes::String> &&m);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> && operator>(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &t, DSModel::Model<DSTypes::UInt32, DSTypes::String> &&m);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator>>(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &t, DSModel::Model<DSTypes::UInt32, DSTypes::String> &m);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator>>(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &&t, DSModel::Model<DSTypes::UInt32, DSTypes::String> &m);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> && operator>>(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &&t, DSModel::Model<DSTypes::UInt32, DSTypes::String> &&m);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> && operator>>(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &t, DSModel::Model<DSTypes::UInt32, DSTypes::String> &&m);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator<(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &t, DSModel::Model<DSTypes::UInt32, DSTypes::String> &m);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator<(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &&t, DSModel::Model<DSTypes::UInt32, DSTypes::String> &m);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> && operator<(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &&t, DSModel::Model<DSTypes::UInt32, DSTypes::String> &&m);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> && operator<(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &t, DSModel::Model<DSTypes::UInt32, DSTypes::String> &&m);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator<<(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &t, DSModel::Model<DSTypes::UInt32, DSTypes::String> &m);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator<<(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &&t, DSModel::Model<DSTypes::UInt32, DSTypes::String> &m);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> && operator<<(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &&t, DSModel::Model<DSTypes::UInt32, DSTypes::String> &&m);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> && operator<<(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &t, DSModel::Model<DSTypes::UInt32, DSTypes::String> &&m);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator<(DSModel::Model<DSTypes::UInt32, DSTypes::String> &m, const DSLib::Table<DSTypes::UInt32, DSTypes::String> &t);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator<(DSModel::Model<DSTypes::UInt32, DSTypes::String> &m, DSLib::Table<DSTypes::UInt32, DSTypes::String> &&t);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> && operator<(DSModel::Model<DSTypes::UInt32, DSTypes::String> &&m, DSLib::Table<DSTypes::UInt32, DSTypes::String> &&t);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> && operator<(DSModel::Model<DSTypes::UInt32, DSTypes::String> &&m, const DSLib::Table<DSTypes::UInt32, DSTypes::String> &t);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator<<(DSModel::Model<DSTypes::UInt32, DSTypes::String> &m, const DSLib::Table<DSTypes::UInt32, DSTypes::String> &t);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator<<(DSModel::Model<DSTypes::UInt32, DSTypes::String> &m, DSLib::Table<DSTypes::UInt32, DSTypes::String> &&t);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> && operator<<(DSModel::Model<DSTypes::UInt32, DSTypes::String> &&m, DSLib::Table<DSTypes::UInt32, DSTypes::String> &&t);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> && operator<<(DSModel::Model<DSTypes::UInt32, DSTypes::String> &&m, const DSLib::Table<DSTypes::UInt32, DSTypes::String> &t);
	template  DSTypes::UInt32 operator~(const DSModel::Model<DSTypes::UInt32, DSTypes::String> &rhs);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String> & operator++(DSModel::Model<DSTypes::UInt32, DSTypes::String> &m, int);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String> && operator++(DSModel::Model<DSTypes::UInt32, DSTypes::String> &&m, int);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String> & operator--(DSModel::Model<DSTypes::UInt32, DSTypes::String> &m, int);
	template DSModel::Model<DSTypes::UInt32, DSTypes::String> && operator--(DSModel::Model<DSTypes::UInt32, DSTypes::String> &&m, int);
};

#ifdef MSVS_VER
#pragma warning(pop)
#endif
