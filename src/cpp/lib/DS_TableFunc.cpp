#include "hpp/lib/DS_TableFunc.hpp"

using namespace std;
using namespace DSLib;
using namespace DSTypes;
using namespace DSLang;

namespace DSFunc {
	//TIdx = UInt32, TId = String
	template bool sortById(DSLib::MatrixBase::IndexBase &index, const DSLib::Matrix<String, UInt32> &srcIdxToId, map<String, UInt32> &dstIdToIdx);
	template void test(DSLib::Matrix<UInt32, UInt32> &dst, const DSLib::Table<UInt32, String> &lhs, const DSLib::Table<UInt32, String> &rhs, const std::function<void(MatrixBase &dst, const MatrixBase &lhs, const MatrixBase &rhs)> &func);
	template Matrix<String, UInt32> genIds(const String & prefix, const UInt32 from, const UInt32 count);
	template std::ostream & operator<<(std::ostream & str, const DSLib::Table<UInt32, String>& tab);
	template std::ostream && operator<<(std::ostream && str, const DSLib::Table<UInt32, String>& tab);
	template std::istream & operator>>(std::istream & str, DSLib::Table<UInt32, String>& tab);
	template std::istream && operator>>(std::istream && str, DSLib::Table<UInt32, String>& tab);

}
