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

//Peak Signal To Noise Ratio
template <typename T, typename TIdx>
T PSNR(const Matrix<T, TIdx> &a, const Matrix<T, TIdx> &b) {

}

//Structural Simularity Index
template <typename T, typename TIdx>
T SSIM(const Matrix<T, TIdx> &a, const Matrix<T, TIdx> &b, Matrix<T, TIdx> &dst, T max = numeric_limits<T>().max()) {
	//Wang et al. 2004
	const T sigma = (T)1.5;
	const TIdx w_w=11;
	const TIdx w_h=11;
	const TIdx w_ox=5; 
	const TIdx w_oy=5;
	const T K1 = (T)0.01;
	const T K2 = (T)0.03;
	const T L = max;
	const T C1 = (K1 * L) * (K1 * L);
	const T C2 = (K2 * L) * (K2 * L);
	const T C3 = (C2 / T(2));

	auto aggr = makeAggrSSIM(w_h, w_w, w_oy, w_ox, sigma, C1, C2, C3);
	auto func = makeFuncLinear(T(1), T(0));
	return slide<T, TIdx, w_h, w_w, w_ox, w_oy>(a, b, dst, aggr, func);
}

int main(void) {
	try {		
		Matrix<Float> m =ImagePNG<Float>("c:/temp/bear.png", true).getChannel(1);

		Matrix<Float> m2;
		auto aggr = makeAggrGauss(11, 11, 5, 5, 1.5f);
		auto func = makeFuncLinear(1.0f, 1.0f);
		slide<Float, UInt32, 11, 11, 5, 5>(m, m2, aggr, func);

		//Matrix<Float, UInt32> m2 = randomReal(m.rows.count(), m.cols.count(), 0.0f, 255.0f);
		Matrix<Float> m3;
		cout << SSIM(m, m2, m3, 255.0f);

		ImagePNG<Float>("c:/temp/bear2.png", m3 * 255.0f).saveImage();
		ImagePNG<Float>("c:/temp/bear3.png", m2).saveImage();
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
