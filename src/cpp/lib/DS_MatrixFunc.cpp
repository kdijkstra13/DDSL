#include "hpp/lib/DS_MatrixFunc.hpp"

using namespace std;
using namespace DSLib;
using namespace DSTypes;
using namespace DSLang;

namespace DSFunc {
	//Convert
	template DSLib::Matrix<String, UInt32> & convert(DSLib::Matrix<String, UInt32> &dst, const DSLib::Matrix<UInt32, UInt32> &src);
	template DSLib::Matrix<String, UInt32> & convert(DSLib::Matrix<String, UInt32> &dst, const DSLib::Matrix<Int32, UInt32> &src);
	template DSLib::Matrix<String, UInt32> & convert(DSLib::Matrix<String, UInt32> &dst, const DSLib::Matrix<Double, UInt32> &src);
	template DSLib::Matrix<String, UInt32> & convert(DSLib::Matrix<String, UInt32> &dst, const DSLib::Matrix<DataType, UInt32> &src);
	template DSLib::Matrix<String, UInt32> & convert(DSLib::Matrix<String, UInt32> &dst, const DSLib::Matrix<ContentType, UInt32> &src);
	template DSLib::Matrix<String, UInt32> & convert(DSLib::Matrix<String, UInt32> &dst, const DSLib::Matrix<VoidPointer, UInt32> &src);
	template DSLib::Matrix<String, UInt32> & convert(DSLib::Matrix<String, UInt32> &dst, const DSLib::Matrix<MatrixDouble, UInt32> &src);
	template DSLib::Matrix<String, UInt32> & convert(DSLib::Matrix<String, UInt32> &dst, const DSLib::Matrix<MatrixInt32, UInt32> &src);
	template DSLib::Matrix<String, UInt32> & convert(DSLib::Matrix<String, UInt32> &dst, const DSLib::Matrix<MatrixUInt32, UInt32> &src);
	template DSLib::Matrix<String, UInt32> & convert(DSLib::Matrix<String, UInt32> &dst, const DSLib::Matrix<MatrixString, UInt32> &src);
	
	//Special converts (Double to UInt32)
	template DSLib::Matrix<Double, UInt32> & convert(DSLib::Matrix<Double, UInt32> &dst, const DSLib::Matrix<UInt32, UInt32> &src);

	//Convert using a DataType enum
	template DSLib::MatrixBase * convertFromDataType(DSTypes::DataType dstType, DSLib::Matrix<String, UInt32> &src);

	//Create using a DataType enum
	template DSLib::MatrixBase * createFromDataType(DSTypes::DataType dt, UInt32 tint);

	//T = Double, TInt = UInt32
	template std::ostream & operator<<(std::ostream & str, const DSLib::Matrix<Double, UInt32>& mat);
	template std::ostream && operator<<(std::ostream && str, const DSLib::Matrix<Double, UInt32>& mat);
	template std::istream & operator>>(std::istream & str, DSLib::Matrix<Double, UInt32>& mat);
	template std::istream && operator>>(std::istream && str, DSLib::Matrix<Double, UInt32>& mat);
	template DSLib::Matrix<Double, UInt32> & convert(DSLib::Matrix<Double, UInt32> &dst, const DSLib::Matrix<Double, UInt32> &src);
	template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<Double, UInt32> &lhs, DSLib::Matrix<Double, UInt32> &rhs, const std::function<bool(const Double&, const Double&)> &func);
	template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<Double, UInt32> &lhs, const Double &rhs, const std::function<bool(const Double&, const Double&)> &func);
	template void range(DSLib::Matrix<Double, UInt32> &dst, DSLib::Matrix<Double, UInt32> &lhs, const Double &rhs);
	template void transpose(DSLib::Matrix<Double, UInt32> &dst, DSLib::Matrix<Double, UInt32> &src);
	template void multiply(DSLib::Matrix<Double, UInt32> &dst, DSLib::Matrix<Double, UInt32> &src1, DSLib::Matrix<Double, UInt32> &src2);
	template void multiplyFast(Matrix<Double, UInt32>& dst, Matrix<Double, UInt32>& src1, Matrix<Double, UInt32>& src2);
	template void inverse(DSLib::Matrix<Double, UInt32> &dst, DSLib::Matrix<Double, UInt32> &src);
	template void determinant(Double &dst, DSLib::Matrix<Double, UInt32> &src);
	template void setUnion(Matrix<Double, UInt32> &dst, Matrix<Double, UInt32> &lhs, Matrix<Double, UInt32> &rhs);
	template void setIntersection(Matrix<Double, UInt32> &dst, Matrix<Double, UInt32> &lhs, Matrix<Double, UInt32> &rhs);
	template void setDifference(Matrix<Double, UInt32> &dst, Matrix<Double, UInt32> &lhs, Matrix<Double, UInt32> &rhs);
	template void apply(DSLib::Matrix<Double, UInt32> &dst, DSLib::Matrix<Double, UInt32> &lhs, DSLib::Matrix<Double, UInt32> &rhs, const std::function<Double(const Double&, const Double&)> &func);
	template void apply(DSLib::Matrix<Double, UInt32> &dst, DSLib::Matrix<Double, UInt32> &lhs, const Double &rhs, const std::function<Double(const Double&, const Double&)> &func);
	template void colMean(DSLib::Matrix<Double, UInt32> &dst, const DSLib::Matrix<Double, UInt32> &src);
	template void colSD(DSLib::Matrix<Double, UInt32> &dst, const DSLib::Matrix<Double, UInt32> &src, DSLib::Matrix<Double, UInt32> &mean);
	template void colVar(DSLib::Matrix<Double, UInt32> &dst, const DSLib::Matrix<Double, UInt32> &src, DSLib::Matrix<Double, UInt32> &mean);
	template void colMin(DSLib::Matrix<Double, UInt32> &dst, const DSLib::Matrix<Double, UInt32> &src);
	template void colMax(DSLib::Matrix<Double, UInt32> &dst, const DSLib::Matrix<Double, UInt32> &src);
	template Matrix<Double, UInt32> & diag(Matrix<Double, UInt32> &dst, const DSLib::Matrix<Double, UInt32>& v);
	template Matrix<Double, UInt32> & eye(Matrix<Double, UInt32> &dst, const UInt32 n);
	template Matrix<Double, UInt32> & shuffle(DSLib::Matrix<Double, UInt32> &mat, Double amount);

	//T = Int32, TInt = UInt32
	template std::ostream & operator<<(std::ostream & str, const DSLib::Matrix<Int32, UInt32>& mat);
	template std::ostream && operator<<(std::ostream && str, const DSLib::Matrix<Int32, UInt32>& mat);
	template std::istream & operator>>(std::istream & str, DSLib::Matrix<Int32, UInt32>& mat);
	template std::istream && operator>>(std::istream && str, DSLib::Matrix<Int32, UInt32>& mat);
	template DSLib::Matrix<Int32, UInt32> & convert(DSLib::Matrix<Int32, UInt32> &dst, const DSLib::Matrix<Int32, UInt32> &src);
	template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<Int32, UInt32> &lhs, DSLib::Matrix<Int32, UInt32> &rhs, const std::function<bool(const Int32&, const Int32&)> &func);
	template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<Int32, UInt32> &lhs, const Int32 &rhs, const std::function<bool(const Int32&, const Int32&)> &func);
	template void range(DSLib::Matrix<Int32, UInt32> &dst, DSLib::Matrix<Int32, UInt32> &lhs, const Int32 &rhs);
	template void transpose(DSLib::Matrix<Int32, UInt32> &dst, DSLib::Matrix<Int32, UInt32> &src);
	template void multiply(DSLib::Matrix<Int32, UInt32> &dst, DSLib::Matrix<Int32, UInt32> &src1, DSLib::Matrix<Int32, UInt32> &src2);
	//template void inverse(DSLib::Matrix<Int32, UInt32> &dst, DSLib::Matrix<Int32, UInt32> &src);
	template void determinant(Int32 &dst, DSLib::Matrix<Int32, UInt32> &src);
	template void setUnion(Matrix<Int32, UInt32> &dst, Matrix<Int32, UInt32> &lhs, Matrix<Int32, UInt32> &rhs);
	template void setIntersection(Matrix<Int32, UInt32> &dst, Matrix<Int32, UInt32> &lhs, Matrix<Int32, UInt32> &rhs);
	template void setDifference(Matrix<Int32, UInt32> &dst, Matrix<Int32, UInt32> &lhs, Matrix<Int32, UInt32> &rhs);
	template void apply(DSLib::Matrix<Int32, UInt32> &dst, DSLib::Matrix<Int32, UInt32> &lhs, DSLib::Matrix<Int32, UInt32> &rhs, const std::function<Int32(const Int32&, const Int32&)> &func);
	template void apply(DSLib::Matrix<Int32, UInt32> &dst, DSLib::Matrix<Int32, UInt32> &lhs, const Int32 &rhs, const std::function<Int32(const Int32&, const Int32&)> &func);

	//T = UInt32, TInt = UInt32
	template std::ostream & operator<<(std::ostream & str, const DSLib::Matrix<UInt32, UInt32>& mat);
	template std::ostream && operator<<(std::ostream && str, const DSLib::Matrix<UInt32, UInt32>& mat);
	template std::istream & operator>>(std::istream & str, DSLib::Matrix<UInt32, UInt32>& mat);
	template std::istream && operator>>(std::istream && str, DSLib::Matrix<UInt32, UInt32>& mat);
	template DSLib::Matrix<UInt32, UInt32> &  convert(DSLib::Matrix<UInt32, UInt32> &dst, const DSLib::Matrix<UInt32, UInt32> &src);
	template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<UInt32, UInt32> &lhs, DSLib::Matrix<UInt32, UInt32> &rhs, const std::function<bool(const UInt32&, const UInt32&)> &func);
	template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<UInt32, UInt32> &lhs, const UInt32 &rhs, const std::function<bool(const UInt32&, const UInt32&)> &func);
	template void range(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<UInt32, UInt32> &lhs, const UInt32 &rhs);
	template void transpose(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<UInt32, UInt32> &src);
	template void multiply(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<UInt32, UInt32> &src1, DSLib::Matrix<UInt32, UInt32> &src2);
	//template void inverse(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<UInt32, UInt32> &src);
	template void determinant(UInt32 &dst, DSLib::Matrix<UInt32, UInt32> &src);
	template void setUnion(Matrix<UInt32, UInt32> &dst, Matrix<UInt32, UInt32> &lhs, Matrix<UInt32, UInt32> &rhs);
	template void setIntersection(Matrix<UInt32, UInt32> &dst, Matrix<UInt32, UInt32> &lhs, Matrix<UInt32, UInt32> &rhs);
	template void setDifference(Matrix<UInt32, UInt32> &dst, Matrix<UInt32, UInt32> &lhs, Matrix<UInt32, UInt32> &rhs);
	template void apply(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<UInt32, UInt32> &lhs, DSLib::Matrix<UInt32, UInt32> &rhs, const std::function<UInt32(const UInt32&, const UInt32&)> &func);
	template void apply(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<UInt32, UInt32> &lhs, const UInt32 &rhs, const std::function<UInt32(const UInt32&, const UInt32&)> &func);
	template Matrix<UInt32, UInt32> random(UInt32 rows, UInt32 cols, UInt32 min, UInt32 max);
	template Matrix<UInt32, UInt32> random(UInt32 rows, UInt32 cols, const Matrix<UInt32> &vec);
	template Matrix<UInt32, UInt32> & shuffle(DSLib::Matrix<UInt32, UInt32> &mat, Double amount);

	//T = String, TInt = UInt32
	template std::ostream & operator<<(std::ostream & str, const DSLib::Matrix<String, UInt32>& mat);
	template std::ostream && operator<<(std::ostream && str, const DSLib::Matrix<String, UInt32>& mat);
	template std::istream & operator>>(std::istream & str, DSLib::Matrix<String, UInt32>& mat);
	template std::istream && operator>>(std::istream && str, DSLib::Matrix<String, UInt32>& mat);
	template DSLib::Matrix<String, UInt32> &  convert(DSLib::Matrix<String, UInt32> &dst, const DSLib::Matrix<String, UInt32> &src);
	template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<String, UInt32> &lhs, DSLib::Matrix<String, UInt32> &rhs, const std::function<bool(const String&, const String&)> &func);
	template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<String, UInt32> &lhs, const String &rhs, const std::function<bool(const String&, const String&)> &func);
	//template void range(DSLib::Matrix<Double, UInt32> &dst, DSLib::Matrix<Double, UInt32> &lhs, const Double &rhs);
	template void transpose(DSLib::Matrix<String, UInt32> &dst, DSLib::Matrix<String, UInt32> &src);
	template void setUnion(Matrix<String, UInt32> &dst, Matrix<String, UInt32> &lhs, Matrix<String, UInt32> &rhs);
	template void setIntersection(Matrix<String, UInt32> &dst, Matrix<String, UInt32> &lhs, Matrix<String, UInt32> &rhs);
	template void setDifference(Matrix<String, UInt32> &dst, Matrix<String, UInt32> &lhs, Matrix<String, UInt32> &rhs);
	template void apply(DSLib::Matrix<String, UInt32> &dst, DSLib::Matrix<String, UInt32> &lhs, DSLib::Matrix<String, UInt32> &rhs, const std::function<String(const String&, const String&)> &func);
	template void apply(DSLib::Matrix<String, UInt32> &dst, DSLib::Matrix<String, UInt32> &lhs, const String &rhs, const std::function<String(const String&, const String&)> &func);
	template Matrix<String, UInt32> & shuffle(DSLib::Matrix<String, UInt32> &mat, Double amount);

	//T = DataType, TInt = UInt32
	template std::ostream & operator<<(std::ostream & str, const DSLib::Matrix<DataType, UInt32>& mat);
	template std::ostream && operator<<(std::ostream && str, const DSLib::Matrix<DataType, UInt32>& mat);
	template std::istream & operator>>(std::istream & str, DSLib::Matrix<DataType, UInt32>& mat);
	template std::istream && operator>>(std::istream && str, DSLib::Matrix<DataType, UInt32>& mat);
	template DSLib::Matrix<DataType, UInt32> & convert(DSLib::Matrix<DataType, UInt32> &dst, const DSLib::Matrix<DataType, UInt32> &src);
	template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<DataType, UInt32> &lhs, DSLib::Matrix<DataType, UInt32> &rhs, const std::function<bool(const DataType&, const DataType&)> &func);
	template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<DataType, UInt32> &lhs, const DataType &rhs, const std::function<bool(const DataType&, const DataType&)> &func);
	template void transpose(DSLib::Matrix<DataType, UInt32> &dst, DSLib::Matrix<DataType, UInt32> &src);

	//T = ContentType, TInt = UInt32
	template std::ostream & operator<<(std::ostream & str, const DSLib::Matrix<ContentType, UInt32>& mat);
	template std::ostream && operator<<(std::ostream && str, const DSLib::Matrix<ContentType, UInt32>& mat);
	template std::istream & operator>>(std::istream & str, DSLib::Matrix<ContentType, UInt32>& mat);
	template std::istream && operator>>(std::istream && str, DSLib::Matrix<ContentType, UInt32>& mat);
	template DSLib::Matrix<ContentType, UInt32> &  convert(DSLib::Matrix<ContentType, UInt32> &dst, const  DSLib::Matrix<ContentType, UInt32> &src);
	template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<ContentType, UInt32> &lhs, DSLib::Matrix<ContentType, UInt32> &rhs, const std::function<bool(const ContentType&, const ContentType&)> &func);
	template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<ContentType, UInt32> &lhs, const ContentType &rhs, const std::function<bool(const ContentType&, const ContentType&)> &func);
	template void transpose(DSLib::Matrix<ContentType, UInt32> &dst, DSLib::Matrix<ContentType, UInt32> &src);

	//T = MatrixDouble, TInt = UInt32
	template std::ostream & operator<<(std::ostream & str, const DSLib::Matrix<MatrixDouble, UInt32>& mat);
	template std::ostream && operator<<(std::ostream && str, const DSLib::Matrix<MatrixDouble, UInt32>& mat);
	template std::istream & operator>>(std::istream & str, DSLib::Matrix<MatrixDouble, UInt32>& mat);
	template std::istream && operator>>(std::istream && str, DSLib::Matrix<MatrixDouble, UInt32>& mat);

	template DSLib::Matrix<MatrixDouble, UInt32> &  convert(DSLib::Matrix<MatrixDouble, UInt32> &dst, const DSLib::Matrix<MatrixDouble, UInt32> &src);
	//template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<MatrixDouble, UInt32> &lhs, DSLib::Matrix<MatrixDouble, UInt32> &rhs, const std::function<bool(const MatrixDouble&, const MatrixDouble&)> &func);
	//template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<MatrixDouble, UInt32> &lhs, const MatrixDouble &rhs, const std::function<bool(const MatrixDouble&, const MatrixDouble&)> &func);
	template void transpose(DSLib::Matrix<MatrixDouble, UInt32> &dst, DSLib::Matrix<MatrixDouble, UInt32> &src);

	//T = MatrixInt32, TInt = UInt32
	template std::ostream & operator<<(std::ostream & str, const DSLib::Matrix<MatrixInt32, UInt32>& mat);
	template std::ostream && operator<<(std::ostream && str, const DSLib::Matrix<MatrixInt32, UInt32>& mat);
	template std::istream & operator>>(std::istream & str, DSLib::Matrix<MatrixInt32, UInt32>& mat);
	template std::istream && operator>>(std::istream && str, DSLib::Matrix<MatrixInt32, UInt32>& mat);
	template DSLib::Matrix<MatrixInt32, UInt32> &  convert(DSLib::Matrix<MatrixInt32, UInt32> &dst, const DSLib::Matrix<MatrixInt32, UInt32> &src);
	//template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<MatrixInt32, UInt32> &lhs, DSLib::Matrix<MatrixInt32, UInt32> &rhs, const std::function<bool(const MatrixInt32&, const MatrixInt32&)> &func);
	//template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<MatrixInt32, UInt32> &lhs, const MatrixInt32 &rhs, const std::function<bool(const MatrixInt32&, const MatrixInt32&)> &func);
	template void transpose(DSLib::Matrix<MatrixInt32, UInt32> &dst, DSLib::Matrix<MatrixInt32, UInt32> &src);

	//T = MatrixUInt32, TInt = UInt32
	template std::ostream & operator<<(std::ostream & str, const DSLib::Matrix<MatrixUInt32, UInt32>& mat);
	template std::ostream && operator<<(std::ostream && str, const DSLib::Matrix<MatrixUInt32, UInt32>& mat);
	template std::istream & operator>>(std::istream & str, DSLib::Matrix<MatrixUInt32, UInt32>& mat);
	template std::istream && operator>>(std::istream && str, DSLib::Matrix<MatrixUInt32, UInt32>& mat);
	template DSLib::Matrix<MatrixUInt32, UInt32> & convert(DSLib::Matrix<MatrixUInt32, UInt32> &dst, const DSLib::Matrix<MatrixUInt32, UInt32> &src);
	//template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<MatrixUInt32, UInt32> &lhs, DSLib::Matrix<MatrixUInt32, UInt32> &rhs, const std::function<bool(const MatrixUInt32&, const MatrixUInt32&)> &func);
	//template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<MatrixUInt32, UInt32> &lhs, const MatrixUInt32 &rhs, const std::function<bool(const MatrixUInt32&, const MatrixUInt32&)> &func);
	template void transpose(DSLib::Matrix<MatrixUInt32, UInt32> &dst, DSLib::Matrix<MatrixUInt32, UInt32> &src);

	//T = MatrixString, TInt = UInt32
	template std::ostream & operator<<(std::ostream & str, const DSLib::Matrix<MatrixString, UInt32>& mat);
	template std::ostream && operator<<(std::ostream && str, const DSLib::Matrix<MatrixString, UInt32>& mat);
	template std::istream & operator>>(std::istream & str, DSLib::Matrix<MatrixString, UInt32>& mat);
	template std::istream && operator>>(std::istream && str, DSLib::Matrix<MatrixString, UInt32>& mat);
	template DSLib::Matrix<MatrixString, UInt32> & convert(DSLib::Matrix<MatrixString, UInt32> &dst, const DSLib::Matrix<MatrixString, UInt32> &src);
	//template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<MatrixString, UInt32> &lhs, DSLib::Matrix<MatrixString, UInt32> &rhs, const std::function<bool(const MatrixString&, const MatrixString&)> &func);
	//template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<MatrixString, UInt32> &lhs, const MatrixString &rhs, const std::function<bool(const MatrixString&, const MatrixString&)> &func);
	template void transpose(DSLib::Matrix<MatrixString, UInt32> &dst, DSLib::Matrix<MatrixString, UInt32> &src);

	//T = VoidPointer, TInt = UInt32
	template std::ostream & operator<<(std::ostream & str, const DSLib::Matrix<VoidPointer, UInt32>& mat);
	template std::ostream && operator<<(std::ostream && str, const DSLib::Matrix<VoidPointer, UInt32>& mat);
	template std::istream & operator>>(std::istream & str, DSLib::Matrix<VoidPointer, UInt32>& mat);
	template std::istream && operator>>(std::istream && str, DSLib::Matrix<VoidPointer, UInt32>& mat);
	template DSLib::Matrix<VoidPointer, UInt32> &  convert(DSLib::Matrix<VoidPointer, UInt32> &dst, const DSLib::Matrix<VoidPointer, UInt32> &src);
	template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<VoidPointer, UInt32> &lhs, DSLib::Matrix<VoidPointer, UInt32> &rhs, const std::function<bool(const VoidPointer&, const VoidPointer&)> &func);
	template void test(DSLib::Matrix<UInt32, UInt32> &dst, DSLib::Matrix<VoidPointer, UInt32> &lhs, const VoidPointer &rhs, const std::function<bool(const VoidPointer&, const VoidPointer&)> &func);
	template void transpose(DSLib::Matrix<VoidPointer, UInt32> &dst, DSLib::Matrix<VoidPointer, UInt32> &src);

}
