#include "ddsl.hpp"
#include "chaiscript/chaiscript.hpp"

using namespace std;
using namespace DSLib;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSModel;

#define CHAI_FUN_MEMBER(FUN_NAME, RETURN_TYPE, CLASS_TYPE, ...) chaiscript::fun(static_cast<RETURN_TYPE(CLASS_TYPE::*)(__VA_ARGS__)>(&CLASS_TYPE::FUN_NAME)), #FUN_NAME);
#define CHAI_FUN_MEMBER_CONST(FUN_NAME, RETURN_TYPE, CLASS_TYPE, ...) chaiscript::fun(static_cast<RETURN_TYPE(CLASS_TYPE::*)(__VA_ARGS__)const>(&CLASS_TYPE::FUN_NAME)), #FUN_NAME);


//chai.add(chaiscript::fun(static_cast<std::string(Tm::*)()const> (&Tm::print)), "print");

template <typename T, typename TIdx>
void bindMatrixLib(chaiscript::ChaiScript &chai) {
    String type = SS("Matrix" << etos(dataType<T>()));
    typedef DSLib::Matrix<T, TIdx> Tm;

    chai.add(chaiscript::user_type<Tm>(), type);
	chai.add(chaiscript::constructor<Tm()>(), type);
    chai.add(CHAI_FUN_MEMBER_CONST(print, DSTypes::String, Tm);
    chai.add(CHAI_FUN_MEMBER_CONST(print, void, Tm, std::ostream &);
}

template <typename T, typename TIdx>
void bindMatrixFunc(chaiscript::ChaiScript &chai) {

}

template <typename T, typename TIdx>
void bindMatrixLang(chaiscript::ChaiScript &chai) {

}

void test(chaiscript::ChaiScript &chai) {
    chai.eval("var m = MatrixFloat();");
    chai.eval("print(m.print());");
}

int main(void) {
    chaiscript::ChaiScript chai;
    bindMatrixLib<Float, UInt32>(chai);
    bindMatrixLib<UInt32, UInt32>(chai);
    bindMatrixLib<Int32, UInt32>(chai);
    bindMatrixLib<String, UInt32>(chai);

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
                command = DSUtil::readToString(script);
                cout << "Executing: " << script << endl;
                chai.eval(command);
            } else {
                cout << "Executing: " << command << endl;
                chai.eval(command);
            }
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
