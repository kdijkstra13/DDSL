#include "ddsl.hpp"
#include "chaiscript/chaiscript.hpp"

using namespace std;
using namespace DSLib;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSModel;

#define CHAI_TYPE(CHAI, CLASS_TYPE, TYPE_NAME) CHAI.add(chaiscript::user_type<CLASS_TYPE>(), TYPE_NAME);

#define CHAI_ENUM(CHAI, ENUM_TYPE, ENUM_NAME) CHAI.add(chaiscript::user_type<ENUM_TYPE>(), ENUM_NAME);
#define CHAI_ENUM_VALUE(CHAI, ENUM_VALUE, PREFIX) CHAI.add_global_const(chaiscript::const_var(ENUM_VALUE), SS(PREFIX << etos(ENUM_VALUE)));

#define CHAI_CONSTRUCTOR(CHAI, CLASS_TYPE, TYPE_NAME, ...) CHAI.add(chaiscript::constructor<CLASS_TYPE(__VA_ARGS__)>(), TYPE_NAME);

#define CHAI_MEMBER(CHAI, FUN_NAME, RETURN_TYPE, CLASS_TYPE, ...) CHAI.add(chaiscript::fun(static_cast<RETURN_TYPE(CLASS_TYPE::*)(__VA_ARGS__)>(&CLASS_TYPE::FUN_NAME)), #FUN_NAME);
#define CHAI_MEMBER_CONST(CHAI, FUN_NAME, RETURN_TYPE, CLASS_TYPE, ...) CHAI.add(chaiscript::fun(static_cast<RETURN_TYPE(CLASS_TYPE::*)(__VA_ARGS__)const>(&CLASS_TYPE::FUN_NAME)), #FUN_NAME);

#define CHAI_OPER(CHAI, OP_NAME, RETURN_TYPE, ...) CHAI.add(chaiscript::fun(static_cast<RETURN_TYPE(*)(__VA_ARGS__)>(operator OP_NAME)), #OP_NAME);

template<typename T>
void bindEnum(chaiscript::ChaiScript &chai, const String &name, const String &prefix) {
    CHAI_ENUM(chai, T, name);
	const int len = sizeof(enumStrings<T>::data) / sizeof(char*);
	for (int i=0;i<len;i++)
        CHAI_ENUM_VALUE(chai, (T)i, prefix);
}

void bindTypes(chaiscript::ChaiScript &chai) {
    bindEnum<DataType>(chai, "DataType", "dt");
    bindEnum<ContentType>(chai, "ContentType", "ct");
	bindEnum<PassThroughType>(chai, "PassThroughType", "ptt");
	bindEnum<ExecType>(chai, "ExecType", "et");
	bindEnum<Order>(chai, "Order", "o");
	bindEnum<ErrorCode>(chai, "ErrorCode", "ec");
	bindEnum<CellsIteratorType>(chai, "CellsIteratorType", "cit");
	bindEnum<ImageType>(chai, "ImageType", "it");
}

template <typename T, typename TIdx>
void bindMatrixLib(chaiscript::ChaiScript &chai) {
    //Matrix type
    String type = SS("Matrix" << etos(dataType<T>()));
    typedef DSLib::Matrix<T, TIdx> TMat;

    //Add type and constructors
    CHAI_TYPE(chai, TMat, type);
    CHAI_CONSTRUCTOR(chai, TMat, type)
    CHAI_CONSTRUCTOR(chai, TMat, type, const TMat&);

    //Add static init
    CHAI_OPER(chai, |, TMat, DataType, const T&);

    //Add class members
    CHAI_MEMBER_CONST(chai, print, DSTypes::String, TMat);
    CHAI_MEMBER_CONST(chai, print, void, TMat, std::ostream &);
}

template <typename T, typename TIdx>
void bindMatrixFunc(chaiscript::ChaiScript &chai) {

}

template <typename T, typename TIdx>
void bindMatrixLang(chaiscript::ChaiScript &chai) {
    typedef DSLib::Matrix<T, TIdx> TMat;
    //Printing
    //CHAI_OPER(chai, ++, TMat&, TMat&, int);
    CHAI_OPER(chai, ++, TMat&, TMat&);
    //CHAI_OPER(chai, --, TMat&, TMat&, int);
    CHAI_OPER(chai, --, TMat&, TMat&);

    //Concat
    CHAI_OPER(chai, |, TMat&, TMat&, const T&);
    CHAI_OPER(chai, |, TMat&, TMat&, const TMat&);
}

template <typename T, typename TIdx>
void bindMatrix(chaiscript::ChaiScript &chai) {
    bindMatrixLib<T, TIdx>(chai);
    bindMatrixLang<T, TIdx>(chai);
    bindMatrixFunc<T, TIdx>(chai);
}

void test(chaiscript::ChaiScript &chai) {
    chai.eval("var m = (dtFloat | 1 | 2);");
    chai.eval("--m;");
}

int main(void) {
    chaiscript::ChaiScript chai;

    bindTypes(chai);
    bindMatrix<UInt8, UInt32>(chai);
    bindMatrix<UInt16, UInt32>(chai);
    bindMatrix<UInt32, UInt32>(chai);
    bindMatrix<UInt64, UInt32>(chai);
    bindMatrix<Int8, UInt32>(chai);
    bindMatrix<Int16, UInt32>(chai);
    bindMatrix<Int32, UInt32>(chai);
    bindMatrix<Int64, UInt32>(chai);
    bindMatrix<Float, UInt32>(chai);
    bindMatrix<Double, UInt32>(chai);
    bindMatrix<String, UInt32>(chai);
    bindMatrix<DataType, UInt32>(chai);
    bindMatrix<ContentType, UInt32>(chai);

    string command;
    while (command != "q") {
        try {
            cout << "DDSL $";
            getline(cin, command);
            if (command == "t") {
                cout << "Executing standard test: ";
                test(chai);
            } else if (command == "q")
                break;
            else if (command == "r") {
                string script;
                cout << "Which script? ";
                getline(cin, script);
                cout << "Executing: " << script << endl;
                chai.eval_file(script);
            } else {
                cout << "Executing: " << command << endl;
                chai.eval(command);
            }
        } catch (const chaiscript::exception::eval_error &e) {
            std::cout << e.pretty_print() << endl;
        } catch (const double &e) {
            cout << "double: " << e << endl;
        } catch (int &e) {
            cout << "int: " << e << endl;
        } catch (float &e) {
            cout << "float: " << e << endl;
        } catch (const std::string &e) {
            cout << "string: " << e << endl;
        } catch (const Error &e) {
            cout << e.what() << endl;
        } catch (const std::exception &e) {
            cout << e.what() << endl;
        }
        cin.clear();
    }

}
