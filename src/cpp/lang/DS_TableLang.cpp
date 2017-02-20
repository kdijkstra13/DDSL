#include "hpp/lang/DS_TableLang.hpp"

#ifdef MSVC_VER
	#pragma warning(push)
	//#pragma warning(disable: 4244)
#endif

#include "hpp/DS_Types.hpp"
#include "hpp/lib/DS_Table.hpp"

namespace DSLang {

	//TIdx = DSTypes::UInt32, TId = DSTypes::String, T = DSTypes::String
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator!(DSLib::Table<DSTypes::UInt32, DSTypes::String> &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> && operator!(DSLib::Table<DSTypes::UInt32, DSTypes::String> &&rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> operator*(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &rhs);
	template DSLib::Table<DSTypes::UInt32> operator|(DSTypes::ContentType lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);
	template DSLib::Table<DSTypes::UInt32> operator^(DSTypes::ContentType lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator|(DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Table<DSTypes::UInt32, DSTypes::String> &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator^(DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Table<DSTypes::UInt32, DSTypes::String> &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator|(DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator^(DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> operator|(DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, const DSLib::Table<DSTypes::UInt32, DSTypes::String> &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> operator^(DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, const DSLib::Table<DSTypes::UInt32, DSTypes::String> &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> operator|(DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> operator^(DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);

	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator|(const DSLib::Matrix<DSTypes::String, DSTypes::UInt32> &lhs, DSLib::Table<DSTypes::UInt32, DSTypes::String> &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator^(const DSLib::Matrix<DSTypes::String, DSTypes::UInt32> &lhs, DSLib::Table<DSTypes::UInt32, DSTypes::String> &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator^(const DSLib::Matrix<DSTypes::ContentType, DSTypes::UInt32> &lhs, DSLib::Table<DSTypes::UInt32, DSTypes::String> &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> operator|(const DSLib::Matrix<DSTypes::String, DSTypes::UInt32> &lhs, DSLib::Table<DSTypes::UInt32, DSTypes::String> &&rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> operator^(const DSLib::Matrix<DSTypes::String, DSTypes::UInt32> &lhs, DSLib::Table<DSTypes::UInt32, DSTypes::String> &&rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> operator^(const DSLib::Matrix<DSTypes::ContentType, DSTypes::UInt32> &lhs, DSLib::Table<DSTypes::UInt32, DSTypes::String> &&rhs);

	template DSLib::Matrix<DSTypes::UInt32, DSTypes::UInt32> operator==(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Table<DSTypes::UInt32, DSTypes::String> &rhs);
	template DSLib::Matrix<DSTypes::UInt32, DSTypes::UInt32> operator==(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);
	template DSLib::Matrix<DSTypes::UInt32, DSTypes::UInt32> operator==(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const Double &rhs);
	template DSLib::Matrix<DSTypes::UInt32, DSTypes::UInt32> operator!=(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Table<DSTypes::UInt32, DSTypes::String> &rhs);
	template DSLib::Matrix<DSTypes::UInt32, DSTypes::UInt32> operator!=(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);
	template DSLib::Matrix<DSTypes::UInt32, DSTypes::UInt32> operator!=(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const Double &rhs);
	template DSLib::Matrix<DSTypes::UInt32, DSTypes::UInt32> operator<(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Table<DSTypes::UInt32, DSTypes::String> &rhs);
	template DSLib::Matrix<DSTypes::UInt32, DSTypes::UInt32> operator<(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);
	template DSLib::Matrix<DSTypes::UInt32, DSTypes::UInt32> operator<(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const Double &rhs);
	template DSLib::Matrix<DSTypes::UInt32, DSTypes::UInt32> operator>(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Table<DSTypes::UInt32, DSTypes::String> &rhs);
	template DSLib::Matrix<DSTypes::UInt32, DSTypes::UInt32> operator>(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);	
	/* Should work?? template <typename DSTypes::Double, typename DSTypes::UInt32, typename DSTypes::String> typename std::enable_if<!std::is_same<DSTypes::Double, DSLib::Model<DSTypes::UInt32, DSTypes::String>>::value, DSLib::Matrix<DSTypes::UInt32, DSTypes::UInt32>>::type operator>(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const T &rhs);*/
	template DSLib::Matrix<DSTypes::UInt32, DSTypes::UInt32> operator<=(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Table<DSTypes::UInt32, DSTypes::String> &rhs);
	template DSLib::Matrix<DSTypes::UInt32, DSTypes::UInt32> operator<=(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);
	template DSLib::Matrix<DSTypes::UInt32, DSTypes::UInt32> operator<=(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSTypes::Double &rhs);
	template DSLib::Matrix<DSTypes::UInt32, DSTypes::UInt32> operator>=(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Table<DSTypes::UInt32, DSTypes::String> &rhs);
	template DSLib::Matrix<DSTypes::UInt32, DSTypes::UInt32> operator>=(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);
	template DSLib::Matrix<DSTypes::UInt32, DSTypes::UInt32> operator>=(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSTypes::Double &rhs);

	template DSLib::Matrix<DSTypes::UInt32, DSTypes::UInt32> operator~(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &rhs);

	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator++(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, int);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator--(const DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, int);

	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator+(DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSTypes::Double &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator-(DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSTypes::Double &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator+(DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);
	
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator*(DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSTypes::Double &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator*(DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator/(DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSTypes::Double &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator/(DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & pow(DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSTypes::Double &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & pow(DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);
	//template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator%(DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSTypes::Double &rhs);
	//template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator%(DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> operator+(DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, const DSTypes::Double &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> operator+(DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> operator-(DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, const DSTypes::Double &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> operator-(DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);

	template DSLib::Table<DSTypes::UInt32, DSTypes::String> operator*(DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, const DSTypes::Double &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> operator*(DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> operator/(DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, const DSTypes::Double &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> operator/(DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);
	//template DSLib::Table<DSTypes::UInt32, DSTypes::String> operator%(DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, const DSTypes::Double &rhs);
	//template DSLib::Table<DSTypes::UInt32, DSTypes::String> operator%(DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> pow(DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, const DSTypes::Double &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> pow(DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, const DSLib::Matrix<DSTypes::Double, DSTypes::UInt32> &rhs);

	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator >> (DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, std::ostream &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> && operator >> (DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, std::ostream &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator<<(DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, std::istream &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> && operator<<(DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, std::istream &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator >> (DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSTypes::String &rhs);

	template DSLib::Table<DSTypes::UInt32, DSTypes::String> && operator >> (DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, const DSTypes::String &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> & operator<<(DSLib::Table<DSTypes::UInt32, DSTypes::String> &lhs, const DSTypes::String &rhs);
	template DSLib::Table<DSTypes::UInt32, DSTypes::String> && operator<<(DSLib::Table<DSTypes::UInt32, DSTypes::String> &&lhs, const DSTypes::String &rhs);

};

#ifdef MSVS_VER
	#pragma warning(pop)
#endif
