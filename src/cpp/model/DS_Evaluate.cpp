#include "hpp/model/DS_Evaluate.hpp"

using namespace std;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSUtil;
using namespace DSLib;

namespace DSModel {

	template class Confusion<UInt32, TableIdx, TableId>;
	template class Confusion<String, TableIdx, TableId>;
}