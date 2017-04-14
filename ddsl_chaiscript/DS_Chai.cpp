#include <DS_Chai.h>
#include <DS_ChaiMatrix.h>
#include <DS_ChaiTable.h>
#include <DS_ChaiModel.h>

namespace DSChai {
	//Special function to add all enums of a type
	template<typename T>
	void bindEnum(chaiscript::ModulePtr chai, const String &name, const String &prefix) {
		CHAI_ENUM(chai, T, name);
		const int len = sizeof(enumStrings<T>::data) / sizeof(char*);
		for (int i = 0;i<len;i++)
			CHAI_ENUM_VALUE(chai, (T)i, prefix);
	}

	//Add all types
	void bindTypes(chaiscript::ModulePtr chai) {
		bindEnum<DataType>(chai, "DataType", "dt");
		bindEnum<ContentType>(chai, "ContentType", "ct");
		bindEnum<PassThroughType>(chai, "PassThroughType", "ptt");
		bindEnum<ExecType>(chai, "ExecType", "et");
		bindEnum<Order>(chai, "Order", "o");
		bindEnum<ErrorCode>(chai, "ErrorCode", "ec");
		bindEnum<CellsIteratorType>(chai, "CellsIteratorType", "cit");
		bindEnum<ImageType>(chai, "ImageType", "it");

		//These interfere with chai's build in types
		/* CHAI_TYPE(chai, DSTypes::UInt8, "UInt8");
		CHAI_TYPE(chai, DSTypes::UInt16, "UInt16");
		CHAI_TYPE(chai, DSTypes::UInt32, "UInt32");
		CHAI_TYPE(chai, DSTypes::UInt64, "UInt64");
		CHAI_TYPE(chai, DSTypes::Int8, "Int8");
		CHAI_TYPE(chai, DSTypes::Int16, "Int16");
		CHAI_TYPE(chai, DSTypes::Int32, "Int32");
		CHAI_TYPE(chai, DSTypes::Int64, "Int64");
		CHAI_TYPE(chai, DSTypes::Float, "Float");
		CHAI_TYPE(chai, DSTypes::Double, "Double");
		CHAI_TYPE(chai, DSTypes::String, "String");*/
	}

	CHAISCRIPT_MODULE_EXPORT chaiscript::ModulePtr create_chaiscript_module_chaiscript_ddsl() {
		chaiscript::ModulePtr chai(new chaiscript::Module());
		bindTypes(chai);
		bindMatrix(chai);
		bindTable(chai);
		return chai;
	}
}