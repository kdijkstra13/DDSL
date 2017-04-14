#include "chaiscript/chaiscript.hpp"

//Add a type
#define CHAI_TYPE(CHAI, CLASS_TYPE, TYPE_NAME) CHAI->add(chaiscript::user_type<CLASS_TYPE>(), TYPE_NAME);

//Add a enum and enum values
#define CHAI_ENUM(CHAI, ENUM_TYPE, ENUM_NAME) CHAI->add(chaiscript::user_type<ENUM_TYPE>(), ENUM_NAME);
#define CHAI_ENUM_VALUE(CHAI, ENUM_VALUE, PREFIX) CHAI->add_global_const(chaiscript::const_var(ENUM_VALUE), SS(PREFIX << etos(ENUM_VALUE)));

//Add constructor
#define CHAI_CONSTRUCTOR(CHAI, CLASS_TYPE, TYPE_NAME, ...) CHAI->add(chaiscript::constructor<CLASS_TYPE(__VA_ARGS__)>(), TYPE_NAME);

//Add member functions
#define CHAI_MEMBER(CHAI, FUN_NAME, RETURN_TYPE, CLASS_TYPE, ...) CHAI->add(chaiscript::fun(static_cast<RETURN_TYPE(CLASS_TYPE::*)(__VA_ARGS__)>(&CLASS_TYPE::FUN_NAME)), #FUN_NAME);
#define CHAI_MEMBER_CONST(CHAI, FUN_NAME, RETURN_TYPE, CLASS_TYPE, ...) CHAI->add(chaiscript::fun(static_cast<RETURN_TYPE(CLASS_TYPE::*)(__VA_ARGS__)const>(&CLASS_TYPE::FUN_NAME)), #FUN_NAME);

//Add member operator
#define CHAI_MEMBER_OPER(CHAI, OP_NAME, RETURN_TYPE, CLASS_TYPE, ...) CHAI->add(chaiscript::fun(static_cast<RETURN_TYPE(CLASS_TYPE::*)(__VA_ARGS__)>(&CLASS_TYPE::operator OP_NAME)), #OP_NAME);
#define CHAI_MEMBER_OPER_CONST(CHAI, OP_NAME, RETURN_TYPE, CLASS_TYPE, ...) CHAI->add(chaiscript::fun(static_cast<RETURN_TYPE(CLASS_TYPE::*)(__VA_ARGS__)const>(&CLASS_TYPE::operator OP_NAME)), #OP_NAME);

//Add member operator
#define CHAI_MEMBER_OPER_AS(CHAI, OP_NAME, OP_NAME_AS, RETURN_TYPE, CLASS_TYPE, ...) CHAI->add(chaiscript::fun(static_cast<RETURN_TYPE(CLASS_TYPE::*)(__VA_ARGS__)>(&CLASS_TYPE::operator OP_NAME)), OP_NAME_AS);
#define CHAI_MEMBER_OPER_AS_CONST(CHAI, OP_NAME, OP_NAME_AS, RETURN_TYPE, CLASS_TYPE, ...) CHAI->add(chaiscript::fun(static_cast<RETURN_TYPE(CLASS_TYPE::*)(__VA_ARGS__)const>(&CLASS_TYPE::operator OP_NAME)), OP_NAME_AS);

//Add external operator
#define CHAI_OPER(CHAI, NS, OP_NAME, RETURN_TYPE, ...) CHAI->add(chaiscript::fun(static_cast<RETURN_TYPE(*)(__VA_ARGS__)>(NS::operator OP_NAME)), #OP_NAME);

//Add function
#define CHAI_FUNC(CHAI, NS, FUNC_NAME, RETURN_TYPE, ...) CHAI->add(chaiscript::fun(static_cast<RETURN_TYPE(*)(__VA_ARGS__)>(NS::FUNC_NAME)), #FUNC_NAME);

//Add type conversion
#define CHAI_CONVERSION(CHAI, TYPE_FROM, TYPE_TO) CHAI->add(chaiscript::type_conversion<TYPE_FROM, TYPE_TO>());


//#ifdef DDSL_CHAI_MODULE
//	#define DDSL_CHAISCRIPT_MODULE_EXPORT CHAISCRIPT_MODULE_EXPORT
//#else
//	#define DDSL_CHAISCRIPT_MODULE_EXPORT
//#endif

namespace DSChai {
	CHAISCRIPT_MODULE_EXPORT chaiscript::ModulePtr create_chaiscript_module_chaiscript_ddsl();
}
