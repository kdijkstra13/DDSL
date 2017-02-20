#ifdef DDSL_NO_EXT_LIB
	#define DDSL_NO_EXT_LIB_PNG
    #define DDSL_NO_EXT_LIB_AF
	#define DDSL_NO_EXT_LIB_CAFFE
	#define DDSL_NO_EXT_LIB_SVM
#endif

//General functions
#include "h/DS_Types.h"
#include "h/DS_Util.h"

//Library
#include "h/lib/DS_Matrix.h"
#include "h/lib/DS_Table.h"
#include "h/lib/DS_MatrixFunc.h"
#include "h/lib/DS_TableFunc.h"
#include "h/lib/DS_Sampler.h"
#include "h/lib/DS_Evolution.h"

//Models
#include "h/model/DS_Model.h"
#include "h/model/DS_Transform.h"
#include "h/model/DS_Classify.h"
#include "h/model/DS_Evaluate.h"
#include "h/model/DS_Generate.h"
#include "h/model/DS_Write.h"
#include "h/model/DS_Read.h"

#ifndef DDSL_NO_EXT_LIB
	#ifndef DDSL_NO_EXT_LIB_CAFFE
		#include "h/model/DS_Classify_Caffe.hpp"
	#endif

	#ifndef DDSL_NO_EXT_LIB_SVM
		#include "h/model/DS_Classify_SVM.hpp"
	#endif

	#ifndef DDSL_NO_EXT_LIB_AF
		#include "h/lib/DS_MatrixFunc_AF.hpp"
		#include "h/model/DS_Transform_AF.hpp"
		#include "h/model/DS_Classify_AF.hpp"
	#endif

	#ifndef DDSL_NO_EXT_LIB_PNG
		#include "h/lib/DS_Image_PNG.hpp"
	#endif
#endif

//Language
#include "h/lang/DS_MatrixLang.h"
#include "h/lang/DS_TableLang.h"
#include "h/lang/DS_ModelLang.h"
