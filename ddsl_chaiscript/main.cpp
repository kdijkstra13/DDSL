#include "ddsl.hpp"
#include "chaiscript/chaiscript.hpp"

using namespace std;
using namespace DSLib;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSModel;



int main(void) {
	try {
		chaiscript::ChaiScript chai;
		chai.add(chaiscript::user_type<Matrix<Float, UInt32>>(), "MatrixFloat");
		chai.add(chaiscript::constructor<Matrix<Float, UInt32>()>(), "MatrixFloat");
		//chai.add(chaiscript::fun(&(operator++(Matrix<Float, UInt32> &, int))), "!");

		chai.eval("var m = MatrixFloat(); print(\"Hello World\");");
		//chai.add(chaiscript::fun(&Matrix<Float>::rows, Class_instance_ptr), "rows");

	} catch (const double &e) {
		cout << "double: " << e << endl;
	} catch (int &e) {
		cout << "int: " << e << endl;
	} catch (float &e) {
		cout << "float: " << e << endl;
	} catch (const std::string &e) {
		cout << "string: " << e << endl;
  	} catch (const Error &e) {
		cout << e.what();
	} catch (const std::exception &e) {
		cout << e.what();
	}
	cout << endl << "Say something: ";
	char a;
	cin >> a;
}