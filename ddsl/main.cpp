#include "stdafx.h"
#include <direct.h>

using namespace std;
using namespace DSLib;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSModel;

void initGlog() {
	//Start logging for caffe
	google::InitGoogleLogging("DDSL");
	google::SetLogDestination(google::GLOG_INFO, "c:/temp/DDSL");
}

int main(int argc, char **) {
	try {		
		initGlog();
		//LDA test
		const UInt32 n = 10000;
		DSFunc::ArrayFire::setDevice(0);
		GenGauss<Float> p1 = GenGauss<Float>(dtFloat | 1.0f | 2.0f | 1.0f | 1.0f, dtFloat | 10.0f | 12.0f | 14.0f | 10.0f);
		GenGauss<Float> p2 = GenGauss<Float>(dtFloat | 3.0f | 3.0f | 2.0f | 1.0f, dtFloat | 10.0f | 12.0f | 14.0f | 20.0f);
		GenGauss<Float> p3 = GenGauss<Float>(dtFloat | 1.0f | 1.0f | 3.0f | 1.0f, dtFloat | 10.0f | 12.0f | 14.0f | 30.0f);
		Table<> c1 = (Table<>(n) > !p1) | (ctTarget | Matrix<String>(n, 1U, "A", oColMajor));
		Table<> c2 = (Table<>(n) > !p2) | (ctTarget | Matrix<String>(n, 1U, "B", oColMajor));
		Table<> c3 = (Table<>(n) > !p3) | (ctTarget | Matrix<String>(n, 1U, "C", oColMajor));
		p1.progressLoop();
		p2.progressLoop();
		p3.progressLoop();
		Matrix<String> classes = (dtString|"A"|"B"|"C");
		cout << "Merge" << endl;
		Table<> c = (dtString|"F1"|"F2"|"F3"|"F4"|"Target") ^ (c1 ^ c2 ^ c3);
		cout << "Big pipeline" << endl;
		/*^ DSModel::ArrayFire::LDA<String>(classes, true, false) ^ Paste<>(c[ctTarget])*/
		DSModel::Export<> p =	+Export<>("c:\\temp\\DDSL\\export.txt") ^ DSModel::ArrayFire::PCA<>(true, true, 3) | 
								+Export<>("c:\\temp\\DDSL\\proj.txt") ^ DSModel::ArrayFire::Correlation<String>(classes) ^ Paste<>(c[ctTarget]) |  
								+Export<>("c:\\temp\\DDSL\\result.txt") ^ -DSModel::Confusion<String>(classes);
		for (unsigned int i=0;i<100;i++) {
			Table<> out = c > !p;
			p.progressLoop();
			out++;
		}
	}
	catch (const char * e) {
		cout << e;
	} catch (const Error &e) {
		cout << e.what();
	}
	cout << endl << "Say something: ";
	char a;
	cin >> a;
}
