#ifdef MSVC
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "ddsl.hpp"

using namespace std;
using namespace DSLib;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSModel;
using namespace DSImage;


int main(int argc, char *argv[]) {
	//_CrtSetBreakAlloc(243);
	try {
		String f = "c:\\temp\\CT.png";
		//while (true) {
			Matrix<Double> m;
			readPNG(m, String(f));
			cout << "Next" << endl;
		//}
		
		/*
		unsigned int h, w;
		unsigned char c, bits;
		readPNGInfo(&h, &w, &c, &bits, f.c_str());
		m.clear();
		m.setOrder(oRowMajor);
		m.resize(h, w);
		readRawPNG(m.getMem(), f.c_str());
		*/

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
	cout << endl << "Done.";
	cin.get();
    #ifdef MSVC
	_CrtDumpMemoryLeaks();
    #endif
}
