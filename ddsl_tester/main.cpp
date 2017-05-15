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
		Matrix<Float> a = ImagePNG<Float>("c:/temp/simul/original.png", true).getChannel(1);
		Matrix<Float> b = ImagePNG<Float>("c:/temp/simul/upsampled.png", true).getChannel(1);


		Matrix<Float> dst;
		cout << SSIM(a, b, dst, 255.0f) << endl;
		cout << PSNR(a, b, 255.0f) << endl;

		ImagePNG<Float>("c:/temp/simul/SSIM.png", dst * 100.0f).saveImage();

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
