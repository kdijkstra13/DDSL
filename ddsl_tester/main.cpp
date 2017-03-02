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
//		Table<>() > DSModel::Caffe<String>(dtString|"a"|"b");
		ImagePNG<Float> png("C:\\DATASETS\\PlastcisSWIR2\\0000_0.png" , true);
		png(0U, 10U, 0U, 10U)++;

	} catch (const double &e) {
		cout << "double: " << e << endl;
	} catch (int &e) {
		cout << "int: " << e << endl;
	} catch (float &e) {
		cout << "float: " << e << endl;
	} catch (const std::string &e) {
		cout << "string: " << e << endl;
	} catch (const std::exception &e) {
		cout << e.what();
  	} catch (const Error &e) {
		cout << e.what();
	}
	cout << endl << "Say something: ";
	char a;
	cin >> a;
}
