#include "hpp/model/DS_Classify.hpp"

using namespace std;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSUtil;
using namespace DSLib;

namespace DSModel {

	template class Random<UInt32, TableIdx, TableId>;
	template class Random<String, TableIdx, TableId>;

	template class NaiveBayes<UInt32, TableIdx, TableId>;
	template class NaiveBayes<String, TableIdx, TableId>;

}
