#include "hpp/DS_Types.hpp"
#include "hpp/lib/DS_Matrix.hpp"

using namespace std;

namespace DSTypes {
	template std::ostream& operator<<(std::ostream& str, enumConstRef<ContentType> const&);
	template std::ostream& operator<<(std::ostream& str, enumConstRef<DataType> const&);
	template std::ostream& operator<<(std::ostream& str, enumConstRef<PassThroughType> const&);
	template std::ostream& operator<<(std::ostream& str, enumConstRef<ExecType> const&);
	template std::ostream& operator<<(std::ostream& str, enumConstRef<Order> const&);
	template std::ostream& operator<<(std::ostream& str, enumConstRef<ErrorCode> const&);
	template std::ostream& operator<<(std::ostream& str, enumConstRef<CellsIteratorType> const&);

	template std::istream& operator>>(std::istream& str, enumRef<ContentType> &);
	template std::istream& operator>>(std::istream& str, enumRef<DataType> &);
	template std::istream& operator>>(std::istream& str, enumRef<PassThroughType> &);
	template std::istream& operator>>(std::istream& str, enumRef<ExecType> &);
	template std::istream& operator>>(std::istream& str, enumRef<Order> &);
	template std::istream& operator>>(std::istream& str, enumRef<ErrorCode> &);
	template std::istream& operator>>(std::istream& str, enumRef<CellsIteratorType> &);

	template std::istream& operator>>(std::istream& str, enumRef<ContentType> &&);
	template std::istream& operator>>(std::istream& str, enumRef<DataType> &&);
	template std::istream& operator>>(std::istream& str, enumRef<PassThroughType> &&);
	template std::istream& operator>>(std::istream& str, enumRef<ExecType> &&);
	template std::istream& operator>>(std::istream& str, enumRef<Order> &&);
	template std::istream& operator>>(std::istream& str, enumRef<ErrorCode> &&);
	template std::istream& operator>>(std::istream& str, enumRef<CellsIteratorType> &&);

	template bool dataTypeEqual(const DataType dt, const Double &tp);
	template bool dataTypeEqual(const DataType dt, const UInt32 &tp);
	template bool dataTypeEqual(const DataType dt, const Int32 &tp);
	template bool dataTypeEqual(const DataType dt, const String &tp);
	template bool dataTypeEqual(const DataType dt, const DataType &tp);
	template bool dataTypeEqual(const DataType dt, const ContentType &tp);
	template bool dataTypeEqual(const DataType dt, const MatrixDouble &tp);
	template bool dataTypeEqual(const DataType dt, const MatrixInt32 &tp);
	template bool dataTypeEqual(const DataType dt, const MatrixUInt32 &tp);
	template bool dataTypeEqual(const DataType dt, const MatrixString &tp);
	template bool dataTypeEqual(const DataType dt, const VoidPointer &tp);

	template String typeName(const Double &);
	template String typeName(const UInt32 &);
	template String typeName(const Int32 &);
	template String typeName(const String &);
	template String typeName(const DataType &);
	template String typeName(const ContentType &);
	template String typeName(const MatrixDouble &);
	template String typeName(const MatrixInt32 &);
	template String typeName(const MatrixUInt32 &);
	template String typeName(const MatrixString &);
	template String typeName(const VoidPointer &);

	template DataType dataType(const Double &);
	template DataType dataType(const UInt32 &);
	template DataType dataType(const Int32 &);
	template DataType dataType(const String &);
	template DataType dataType(const DataType &);
	template DataType dataType(const ContentType &);
	template DataType dataType(const MatrixDouble &);
	template DataType dataType(const MatrixInt32 &);
	template DataType dataType(const MatrixUInt32 &);
	template DataType dataType(const MatrixString &);
	template DataType dataType(const VoidPointer &);
};
