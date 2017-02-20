#include "hpp/model/DS_Classify_Caffe.hpp"
//Using Caffe in VS2015 with DDSL on Windows
//- Get Caffe Windows branch
//- Compile in VS2013
//- Install all required NuGet packages except Protocol Buffers (Because NuGet package for vc140 is too old)
//- Download and use Protocol Buffers from https://developers.google.com/protocol-buffers/

using namespace std;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSUtil;
using namespace DSLib;

namespace DSModel {

	template class CaffeMLP<UInt32, TableIdx, TableId>;
	template class CaffeMLP<String, TableIdx, TableId>;
}
