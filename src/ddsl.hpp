#ifdef DDSL_NO_EXT_LIB
	#define DDSL_NO_EXT_LIB_PNG
    #define DDSL_NO_EXT_LIB_AF
	#define DDSL_NO_EXT_LIB_CAFFE
	#define DDSL_NO_EXT_LIB_SVM
#endif

#ifdef DDSL_TESTER
	//#define DDSL_NO_EXT_LIB_PNG
    //#define DDSL_NO_EXT_LIB_AF
	//#define DDSL_NO_EXT_LIB_CAFFE
	//#define DDSL_NO_EXT_LIB_SVM
#endif

//General functions
#include "hpp/DS_Types.hpp"
#include "hpp/DS_Util.hpp"

//Library
#include "hpp/lib/DS_Matrix.hpp"
#include "hpp/lib/DS_Table.hpp"
#include "hpp/lib/DS_MatrixFunc.hpp"
#include "hpp/lib/DS_TableFunc.hpp"
#include "hpp/lib/DS_Image.hpp"
#include "hpp/lib/DS_Sampler.hpp"
#include "hpp/lib/DS_Evolution.hpp"

//Models
#include "hpp/model/DS_Model.hpp"
#include "hpp/model/DS_Transform.hpp"
#include "hpp/model/DS_Classify.hpp"
#include "hpp/model/DS_Evaluate.hpp"
#include "hpp/model/DS_Generate.hpp"
#include "hpp/model/DS_Write.hpp"
#include "hpp/model/DS_Read.hpp"
#include "hpp/model/DS_Image.hpp"

//Language
#include "hpp/lang/DS_MatrixLang.hpp"
#include "hpp/lang/DS_TableLang.hpp"
#include "hpp/lang/DS_ModelLang.hpp"

#ifndef DDSL_NO_EXT_LIB
	#ifndef DDSL_NO_EXT_LIB_CAFFE
		#include "hpp/model/DS_Classify_Caffe.hpp"
	#endif

	#ifndef DDSL_NO_EXT_LIB_SVM
		#include "hpp/model/DS_Classify_SVM.hpp"
	#endif

	#ifndef DDSL_NO_EXT_LIB_AF
		#include "hpp/lib/DS_MatrixFunc_AF.hpp"
		#include "hpp/model/DS_Transform_AF.hpp"
		#include "hpp/model/DS_Classify_AF.hpp"
	#endif

	#ifndef DDSL_NO_EXT_LIB_PNG
		#include "hpp/lib/DS_Image_PNG.hpp"
	#endif
#endif