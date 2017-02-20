#include "ddsl.hpp"

using namespace std;
using namespace DSLib;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSModel;


int main(void) {

	try {
		Matrix<UInt32> pid = (dtUInt32 | (1U ^ 2U ^ 3U ^ 4U));
		Matrix<UInt32> sid = (dtUInt32 | (1U ^ 1U ^ 2U ^ 2U));
		Table<> tab = (ctParameter | pid | sid);
		tab++;

	} catch (Error &e) {
		cout << e.what();
	}
	cout << endl << "Say something: ";
	char a;
	cin >> a;
}
