#include <DS_ChaiTable.h>

namespace DSChai {

	//Add library for Table
	template <typename TIdx, typename TId>
	void bindTableLibBasic(chaiscript::ModulePtr chai) {

		//Typedefs
		typedef Table<TIdx, TId> TTab;
		typedef Matrix<TIdx, TIdx> TTabIdx;

		//Add type and constructors
		CHAI_TYPE(chai, TTab, "Table");
		CHAI_CONSTRUCTOR(chai, TTab, "Table")
		CHAI_CONSTRUCTOR(chai, TTab, "Table", const TTab&);

		//Add conversions
		CHAI_CONVERSION(chai, TTab, bool);

		//Chai interop for converting to a string
		CHAI_MEMBER_CONST(chai, to_string, DSTypes::String, TTab);

		//Add class members
		CHAI_MEMBER_CONST(chai, print, DSTypes::String, TTab);
		CHAI_MEMBER_CONST(chai, print, void, TTab, std::ostream &);
	}

	//Add functions for Table
	template <typename TIdx, typename TId>
	void bindTableFuncBasic(chaiscript::ModulePtr chai) {

	}

	//Add language for Table
	template <typename T, typename TIdx, typename TId>
	void bindTableLangBasic(chaiscript::ModulePtr chai) {
		typedef Table<TIdx, TId> TTab;
		typedef Matrix<T, TIdx> TMat;
		
		//Add conversions to Matrix
		CHAI_CONVERSION(chai, TTab, TMat);
		
		//Streaming
		CHAI_OPER(chai, DSScript, >> , TTab &, TTab &, std::ostream &);
		CHAI_OPER(chai, DSScript, << , TTab &, TTab &, std::istream &);
		CHAI_OPER(chai, DSScript, >> , TTab &, TTab &, const DSTypes::String &);
		CHAI_OPER(chai, DSScript, << , TTab &, TTab &, const DSTypes::String &);
	}

	//Add language for Table
	template <typename T, typename TIdx, typename TId>
	void bindTableLangNumeric(chaiscript::ModulePtr chai) {
		typedef Table<TIdx, TId> TTab;
		typedef Matrix<T, TIdx> TMat;

		//Arithmetic
		CHAI_OPER(chai, DSScript, +, TTab &, TTab &, const T &);
		CHAI_OPER(chai, DSScript, -, TTab &, TTab &, const T &);
		CHAI_OPER(chai, DSScript, +, TTab &, TTab &, const TMat &);
		CHAI_OPER(chai, DSScript, -, TTab &, TTab &, const TMat &);
		CHAI_OPER(chai, DSScript, *, TTab &, TTab &, const T &);
		CHAI_OPER(chai, DSScript, *, TTab &, TTab &, const TMat &);
		CHAI_OPER(chai, DSScript, / , TTab &, TTab &, const T &);
		CHAI_OPER(chai, DSScript, / , TTab &, TTab &, const TMat &);
		CHAI_FUNC(chai, DSScript, pow, TTab &, TTab &, const T &);
		CHAI_FUNC(chai, DSScript, pow, TTab &, TTab &, const TMat &);
		CHAI_OPER(chai, DSScript, %, TTab &, TTab &, const T &);
		CHAI_OPER(chai, DSScript, %, TTab &, TTab &, const TMat &);
	}

	//Add basic functions
	void bindTableBasic(chaiscript::ModulePtr chai) {
		//Typedefs
		typedef UInt32 TIdx;
		typedef String TId;

		//Bind basic table
		bindTableLibBasic<TIdx, TId>(chai);
		bindTableFuncBasic<TIdx, TId>(chai);

		//Bind basic lang
		bindTableLangBasic<UInt8, TIdx, TId>(chai);
		bindTableLangBasic<UInt16, TIdx, TId>(chai);
		bindTableLangBasic<UInt32, TIdx, TId>(chai);
		bindTableLangBasic<UInt64, TIdx, TId>(chai);
		bindTableLangBasic<Int8, TIdx, TId>(chai);
		bindTableLangBasic<Int16, TIdx, TId>(chai);
		bindTableLangBasic<Int32, TIdx, TId>(chai);
		bindTableLangBasic<Int64, TIdx, TId>(chai);
		bindTableLangBasic<Float, TIdx, TId>(chai);
		bindTableLangBasic<Double, TIdx, TId>(chai);
		bindTableLangBasic<String, TIdx, TId>(chai);
		bindTableLangBasic<DataType, TIdx, TId>(chai);
		bindTableLangBasic<ContentType, TIdx, TId>(chai);
	}

	//Add basic functions
	void bindTableNumeric(chaiscript::ModulePtr chai) {
		//Typedefs
		typedef UInt32 TIdx;
		typedef String TId;

		//Bind basic table
		//bindTableLibNumeric<TIdx, TId>(chai);
		//bindTableFuncNumeric<TIdx, TId>(chai);

		//Bind basic lang
		bindTableLangNumeric<UInt8, TIdx, TId>(chai);
		bindTableLangNumeric<UInt16, TIdx, TId>(chai);
		bindTableLangNumeric<UInt32, TIdx, TId>(chai);
		bindTableLangNumeric<UInt64, TIdx, TId>(chai);
		bindTableLangNumeric<Int8, TIdx, TId>(chai);
		bindTableLangNumeric<Int16, TIdx, TId>(chai);
		bindTableLangNumeric<Int32, TIdx, TId>(chai);
		bindTableLangNumeric<Int64, TIdx, TId>(chai);
		bindTableLangNumeric<Float, TIdx, TId>(chai);
		bindTableLangNumeric<Double, TIdx, TId>(chai);
	}

	//Add all Table functions
	void bindTable(chaiscript::ModulePtr chai) {
		//Typedefs
		typedef UInt32 TIdx;
		typedef String TId;
		
		bindTableBasic(chai);
		bindTableNumeric(chai);
	}
}