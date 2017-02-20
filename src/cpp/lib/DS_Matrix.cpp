#include "hpp/lib/DS_Matrix.hpp"
#include "hpp/lib/DS_MatrixFunc.hpp"
#include "hpp/lang/DS_MatrixLang.hpp"
#include "hpp/DS_Util.hpp"

using namespace DSTypes;

namespace DSLib {
	template class Matrix<Double, UInt32>;
	template class CellsBase<Double, UInt32>;
	template class CellsRegion<Double, UInt32>;
	template class CellsContinuous<Double, UInt32>;

	template class Matrix<Int32, UInt32>;
	template class CellsBase<Int32, UInt32>;
	template class CellsRegion<Int32, UInt32>;
	template class CellsContinuous<Int32, UInt32>;

	template class Matrix<UInt32, UInt32>;
	template class CellsBase<UInt32, UInt32>;
	template class CellsRegion<UInt32, UInt32>;
	template class CellsContinuous<UInt32, UInt32>;

	template class Matrix<String, UInt32>;
	template class CellsBase<String, UInt32>;
	template class CellsRegion<String, UInt32>;
	template class CellsContinuous<String, UInt32>;

	template class Matrix<ContentType, UInt32>;
	template class CellsBase<ContentType, UInt32>;
	template class CellsRegion<ContentType, UInt32>;
	template class CellsContinuous<ContentType, UInt32>;

	template class Matrix<DataType, UInt32>;
	template class CellsBase<DataType, UInt32>;
	template class CellsRegion<DataType, UInt32>;
	template class CellsContinuous<DataType, UInt32>;

	template class Matrix<MatrixDouble, MatrixIdx>;
	template class CellsBase<MatrixDouble, MatrixIdx>;
	template class CellsRegion<MatrixDouble, MatrixIdx>;
	template class CellsContinuous<MatrixDouble, MatrixIdx>;

	template class Matrix<MatrixInt32, MatrixIdx>;
	template class CellsBase<MatrixInt32, MatrixIdx>;
	template class CellsRegion<MatrixInt32, MatrixIdx>;
	template class CellsContinuous<MatrixInt32, MatrixIdx>;

	template class Matrix<MatrixUInt32, MatrixIdx>;
	template class CellsBase<MatrixUInt32, MatrixIdx>;
	template class CellsRegion<MatrixUInt32, MatrixIdx>;
	template class CellsContinuous<MatrixUInt32, MatrixIdx>;

	template class Matrix<MatrixString, MatrixIdx>;
	template class CellsBase<MatrixString, MatrixIdx>;
	template class CellsRegion<MatrixString, MatrixIdx>;
	template class CellsContinuous<MatrixString, MatrixIdx>;

	template class Matrix<VoidPointer, UInt32>;
	template class CellsBase<VoidPointer, UInt32>;
	template class CellsRegion<VoidPointer, UInt32>;
	template class CellsContinuous<VoidPointer, UInt32>;
};

