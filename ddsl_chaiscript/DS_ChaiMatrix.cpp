#include <DS_ChaiMatrix.h>

namespace DSChai {
	
	//Add library for Matrix
	template <typename T, typename TIdx>
	void bindMatrixLibBasic(chaiscript::ModulePtr chai) {

		//Typedefs
		typedef Matrix<T, TIdx> TMat;
		typedef Matrix<TIdx, TIdx> TMatIdx;
		String name = SS("Matrix" << etos(dataType<T>()));

		//Add type and constructors
		CHAI_TYPE(chai, TMat, name);
		CHAI_CONSTRUCTOR(chai, TMat, name)
		CHAI_CONSTRUCTOR(chai, TMat, name, const TMat&);

		//Add conversions
		CHAI_CONVERSION(chai, TMat, bool);
		CHAI_CONVERSION(chai, TMat, T);

		//Chai interop for converting to a string
		CHAI_MEMBER_CONST(chai, to_string, DSTypes::String, TMat);

		//Add class members
		CHAI_MEMBER_CONST(chai, print, DSTypes::String, TMat);
		CHAI_MEMBER_CONST(chai, print, void, TMat, std::ostream &);
	}

	//Add functions for Matrix
	template <typename T, typename TIdx>
	void bindMatrixFuncBasic(chaiscript::ModulePtr chai) {

	}

	//Add language for Matrix
	template <typename T, typename TIdx>
	void bindMatrixLangBasic(chaiscript::ModulePtr chai) {
		//Typedefs
		typedef Matrix<T, TIdx> TMat;
		typedef Matrix<TIdx, TIdx> TMatIdx;

		//Printing
		CHAI_OPER(chai, DSScript, ++, TMat, const TMat&);
		CHAI_OPER(chai, DSScript, --, TMat, const TMat&);

		//Static init
		CHAI_OPER(chai, DSScript, |, TMat, DSTypes::DataType, const T&);
		CHAI_OPER(chai, DSScript, ^, TMat, DSTypes::DataType, const T&);

		//Assignment
		CHAI_OPER(chai, DSScript, |=, TMat, TMat&, const TMat&);
		CHAI_OPER(chai, DSScript, ^=, TMat, TMat&, const TMat&);
		CHAI_OPER(chai, DSScript, +=, TMat, TMat&, const TMat&);

		//Breakup
		CHAI_OPER(chai, DSScript, !, TMat, TMat&);
		CHAI_OPER(chai, DSScript, *, TMat, const TMat&);

		//Concat
		CHAI_OPER(chai, DSScript, | , TMat, TMat&, const T&);
		CHAI_OPER(chai, DSScript, | , TMat, TMat&, const TMat&);
		CHAI_OPER(chai, DSScript, ^, TMat, TMat&, const T&);
		CHAI_OPER(chai, DSScript, ^, TMat, TMat&, const TMat&);

		//Comparing
		CHAI_OPER(chai, DSScript, == , TMatIdx, const TMat&, const TMat&);
		CHAI_OPER(chai, DSScript, == , TMatIdx, const TMat&, const T&);
		CHAI_OPER(chai, DSScript, != , TMatIdx, const TMat&, const TMat&);
		CHAI_OPER(chai, DSScript, != , TMatIdx, const TMat&, const T&);

		//Members
		CHAI_MEMBER_OPER(chai, =, TMat&, TMat, const T&);
		CHAI_MEMBER_OPER(chai, =, TMat&, TMat, const TMat&);
		CHAI_MEMBER_OPER(chai, [], TMat, TMat, const TIdx);
		CHAI_MEMBER_OPER(chai, [], TMat, TMat, const TMatIdx &);

		CHAI_MEMBER_OPER_AS(chai, (), "s", TMat, TMat);
		CHAI_MEMBER_OPER_AS(chai, (), "s", TMat, TMat, const TIdx);
		CHAI_MEMBER_OPER_AS(chai, (), "s", TMat, TMat, const TMatIdx&);
		CHAI_MEMBER_OPER_AS(chai, (), "s", TMat, TMat, const TIdx, const TIdx);
		CHAI_MEMBER_OPER_AS(chai, (), "s", TMat, TMat, const TIdx, const TIdx, const TIdx, const TIdx);
		CHAI_MEMBER_OPER_AS(chai, (), "s", TMat, TMat, const TMatIdx &, const TMatIdx &);
		CHAI_MEMBER_OPER_AS(chai, (), "s", TMat, TMat, const TMatIdx &, const TIdx);
		CHAI_MEMBER_OPER_AS(chai, (), "s", TMat, TMat, const TMatIdx &, const TIdx, const TIdx);
		CHAI_MEMBER_OPER_AS(chai, (), "s", TMat, TMat, const TIdx, const TMatIdx &);
		CHAI_MEMBER_OPER_AS(chai, (), "s", TMat, TMat, const TIdx, const TIdx, const TMatIdx &);
		CHAI_MEMBER_OPER_AS(chai, (), "s", TMat, TMat, const TIdx, const TIdx, const TIdx);
	}

	//Add numerical library for Matrix
	template <typename T, typename TIdx>
	void bindMatrixLibNumeric(chaiscript::ModulePtr chai) {
	}

	//Add numerical operators for Matrix
	template <typename T, typename TIdx>
	void bindMatrixLangNumeric(chaiscript::ModulePtr chai) {
		//Typedefs
		typedef Matrix<T, TIdx> TMat;
		typedef Matrix<TIdx, TIdx> TMatIdx;

		//Add operators
		CHAI_OPER(chai, DSScript, *=, TMat, TMat&, const TMat&);
		CHAI_OPER(chai, DSScript, >, TMatIdx, const TMat&, const TMat&);
		CHAI_OPER(chai, DSScript, >, TMatIdx, const TMat&, const T&);
		CHAI_OPER(chai, DSScript, <, TMatIdx, const TMat&, const TMat&);
		CHAI_OPER(chai, DSScript, <, TMatIdx, const TMat&, const T&);
		CHAI_OPER(chai, DSScript, >= , TMatIdx, const TMat&, const TMat&);
		CHAI_OPER(chai, DSScript, >= , TMatIdx, const TMat&, const T&);
		CHAI_OPER(chai, DSScript, <= , TMatIdx, const TMat&, const TMat&);
		CHAI_OPER(chai, DSScript, <= , TMatIdx, const TMat&, const T&);
	}

	//Add numerical functions for Matrix
	template <typename T, typename TIdx>
	void bindMatrixFuncNumeric(chaiscript::ModulePtr chai) {
	}

	//Add basic operators for Matrix
	template <typename T, typename TIdx>
	void bindMatrixBasic(chaiscript::ModulePtr chai) {
		bindMatrixLibBasic<T, TIdx>(chai);
		bindMatrixLangBasic<T, TIdx>(chai);
		bindMatrixFuncBasic<T, TIdx>(chai);
	}

	template <typename T, typename TIdx>
	void bindMatrixNumeric(chaiscript::ModulePtr chai) {
		bindMatrixLibNumeric<T, TIdx>(chai);
		bindMatrixLangNumeric<T, TIdx>(chai);
		bindMatrixFuncNumeric<T, TIdx>(chai);
	}

	void bindMatrix(chaiscript::ModulePtr chai) {
		//Typedefs
		typedef UInt32 TIdx;

		//Bind basic operators for basic types
		bindMatrixBasic<UInt8, TIdx>(chai);
		bindMatrixBasic<UInt16, TIdx>(chai);
		bindMatrixBasic<UInt32, TIdx>(chai);
		bindMatrixBasic<UInt64, TIdx>(chai);
		bindMatrixBasic<Int8, TIdx>(chai);
		bindMatrixBasic<Int16, TIdx>(chai);
		bindMatrixBasic<Int32, TIdx>(chai);
		bindMatrixBasic<Int64, TIdx>(chai);
		bindMatrixBasic<Float, TIdx>(chai);
		bindMatrixBasic<Double, TIdx>(chai);
		bindMatrixBasic<String, TIdx>(chai);
		bindMatrixBasic<DataType, TIdx>(chai);
		bindMatrixBasic<ContentType, TIdx>(chai);

		//Bind basic operators for Matrix of Matrix types
		bindMatrixBasic<Matrix<UInt32, TIdx>, TIdx>(chai);
		bindMatrixBasic<Matrix<Int32, TIdx>, TIdx>(chai);
		bindMatrixBasic<Matrix<Float, TIdx>, TIdx>(chai);
		bindMatrixBasic<Matrix<Double, TIdx>, TIdx>(chai);
		bindMatrixBasic<Matrix<String, TIdx>, TIdx>(chai);

		//Bind numeric operators
		bindMatrixNumeric<UInt8, TIdx>(chai);
		bindMatrixNumeric<UInt16, TIdx>(chai);
		bindMatrixNumeric<UInt32, TIdx>(chai);
		bindMatrixNumeric<UInt64, TIdx>(chai);
		bindMatrixNumeric<Int8, TIdx>(chai);
		bindMatrixNumeric<Int16, TIdx>(chai);
		bindMatrixNumeric<Int32, TIdx>(chai);
		bindMatrixNumeric<Int64, TIdx>(chai);
		bindMatrixNumeric<Float, TIdx>(chai);
		bindMatrixNumeric<Double, TIdx>(chai);
	}
}