//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include "ddsl.hpp"

using namespace std;
using namespace DSLib;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSModel;
using namespace DSImage;


int main(void) {
	try {
		Matrix<Float> m = ImagePNG<Float>("C:/temp/resultA.png", true).mat();
		Matrix<Float> m2;
		auto mn = makeAggrMean<Float, UInt32>(11, 11, 5, 5);
		auto lin = makeFuncLinear(1.0f, 50.0f);
		slide<Float, UInt32, 11U, 11U, 5U, 5U>(m, m2, mn, lin);
		ImagePNG<Float>("c:/temp/!!.png", m2).saveImage();
		m2++;

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
