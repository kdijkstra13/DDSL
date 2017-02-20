#include "hpp/model/DS_Classify_SVM.hpp"

using namespace std;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSUtil;
using namespace DSLib;

namespace DSModel {

	template class SVM<UInt32, TableIdx, TableId>;
	template class SVM<String, TableIdx, TableId>;
}