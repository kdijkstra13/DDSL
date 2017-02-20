#include "hpp/DS_Util.hpp"

using namespace std;
using namespace DSTypes;
using namespace DSLib;

namespace DSUtil {
	template void write(ostream &s, const Double &data);
	template void write(ostream &s, const UInt32 &data);
	template void write(ostream &s, const Int32 &data);
	template void write(ostream &s, const MatrixDouble &data);
	template void write(ostream &s, const MatrixInt32 &data);
	template void write(ostream &s, const MatrixString &data);
	template void write(ostream &s, const MatrixUInt32 &data);
	template void write(ostream &s, const VoidPointer &data);

	template void read(istream &s, Double &data);
	template void read(istream &s, UInt32 &data);
	template void read(istream &s, Int32 &data);
	template void read(istream &s, MatrixDouble &data);
	template void read(istream &s, MatrixInt32 &data);
	template void read(istream &s, MatrixString &data);
	template void read(istream &s, MatrixUInt32 &data);
	template void read(istream &s, VoidPointer &data);
}
