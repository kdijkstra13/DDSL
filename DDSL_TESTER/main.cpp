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

void initGlog() {
	//Start logging for caffe
	cout << "Init google log" << endl;
	google::InitGoogleLogging("DDSL");
	google::SetLogDestination(google::GLOG_INFO, "/tmp/DDSL");
}

void initCaffe(UInt32 ngpus) {
	cout << "Init caffe" << endl;
	caffe::Caffe::set_mode(caffe::Caffe::GPU);
	caffe::Caffe::set_solver_count(ngpus);
}

int main(void) {
	try {
		initGlog();
		UInt32 ngpus = 4;
		initCaffe(ngpus);
		cout << "Start" << endl;

	} catch (const char * e) {
		cout << e;
	} catch (const Error &e) {
		cout << e.what();
	}
	cout << endl << "Say something: ";
	char a;
	cin >> a;
}
