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

template <typename T, typename TIdx>
Matrix<T, TIdx> BILToTiled(const Matrix<T, TIdx> &bil, const TIdx tileCountX, const TIdx tileCountY) {
	//Matrix<T, TIdx> & bil2 = const_cast<Matrix<T, TIdx>&>(bil);
	
	const TIdx bandCount = tileCountX * tileCountY;
	const TIdx tileWidth = bil.cols.count();
	const TIdx tileHeight = bil.rows.count() / bandCount; 
	if (bil.rows.count() % bandCount != 0)
		throw Error(ecIncompatible, "BILToTiled", "Source image height is not dividable by the number of bands. Cannot deduce image height from tileCountX and tileCountY");

	Matrix<T, TIdx> tiled(tileHeight * tileCountY, tileWidth * tileCountX);
	TIdx srcy = 0;
	for (TIdx l=0;l<tileHeight;l++) {
		for (TIdx y=0;y<tileCountY;y++) {
			for (TIdx x=0;x<tileCountX;x++) {
				const TIdx tx = x * tileWidth;
				const TIdx ty = y * tileHeight + l;
				T * dstPtr = tiled.getData(ty, tx);
				const T * srcPtr = bil.getData(srcy, 0);
				memcpy((void*)dstPtr, (void*)srcPtr, tileWidth * sizeof(T));
				srcy++;
			}
		}
	}
	return tiled;
}

enum enviLayout {elBandSequential, elBandInterleaveByLine, elBandInterleaveByPixel};

template <typename T, typename TIdx>
void readRawENVI(Matrix<T, TIdx> &dst, const String &filename, const TIdx width, const TIdx bands, const enviLayout cubeLayout) {
	vector<T> buf;
	TIdx size = (TIdx)DSUtil::readToVector(filename, buf);
	if (size % (width * bands) != 0)
		throw Error(ecUnexpected, "readRawENVI", SS("Cannot deduce height form file size: " << size % (width * bands) << " elements too many"));
	TIdx height = size / (width * bands);
	if (cubeLayout == elBandInterleaveByLine) {
		dst.resize(bands * height, width);
		copy(buf.begin(), buf.end(), dst.rows->begin());
		
	} else //if (cubeLayout == elBandInterLeaveSequential) {
		throw Error(ecNotImplemented, "readRawENVI" , "Specified cubeLayout not supported");
}

template <typename T, typename TIdx>
void normalizeENVI(Matrix<T, TIdx> &cube, const Matrix<T, TIdx> &black, const Matrix<T, TIdx> &white, const TIdx bands, const enviLayout cubeLayout) {
	if (cubeLayout != elBandInterleaveByLine)
		throw Error(ecNotImplemented, "readRawENVI", "Specified cubeLayout not supported");
	const TIdx whiteHeight = white.rows.count() / bands;
	const TIdx blackHeight = black.rows.count() / bands;
	const TIdx height = cube.rows.count() / bands;
	Matrix<T, TIdx> whiteRef(bands, white.cols.count(), (T)0);
	Matrix<T, TIdx> blackRef(bands, black.cols.count(), (T)0);
	//Take average for white
	for (TIdx y=0;y<whiteHeight;y++) {
		for (TIdx b=0;b<bands;b++) {
			T * dstPtr = whiteRef.getData(b, 0);
			const T * srcPtr = white.getData(y * bands + b, 0);
			for (TIdx px=0;px<white.cols.count();px++,dstPtr++,srcPtr++)
				*dstPtr += *srcPtr;
		}
	}
	//Take average for black
	for (TIdx y = 0;y<blackHeight;y++) {
		for (TIdx b = 0;b<bands;b++) {
			T * dstPtr = blackRef.getData(b, 0);
			const T * srcPtr = black.getData(y * bands + b, 0);
			for (TIdx px = 0;px<black.cols.count();px++, dstPtr++, srcPtr++)
				*dstPtr += *srcPtr;
		}
	}
	//Average
	for (TIdx b = 0;b<bands;b++) {
		T * ptrWhite = whiteRef.getData(b, 0);
		T * ptrBlack = blackRef.getData(b, 0);
		for (TIdx px = 0;px<black.cols.count();px++, ptrWhite++, ptrBlack++) {
			*ptrWhite /= whiteHeight;
			*ptrBlack /= blackHeight;
		}
	}
	//Normalise
	for (TIdx y=0;y<height;y++) {
		for (TIdx b=0;b<bands;b++) {
			const T * ptrWhite = whiteRef.getData(b, 0);
			const T * ptrBlack = blackRef.getData(b, 0);
			T * ptrDst = cube.getData(y * bands + b, 0);
			for (TIdx x = 0;x<cube.cols.count();x++, ptrWhite++, ptrBlack++, ptrDst++)
				*ptrDst = (*ptrDst - *ptrBlack) / *ptrWhite;
		}
	}
}

int main(int argc, char *argv[]) {
	try {
		const String folder = "x:/DemosaicLarge/HLB/";
		const String file1 = folder + "downsample_to_original_crosstalk/16_m4_D4_D16_CAM_XIMEACamVIS03601.jl.png";
		const String file2 = folder + "crosstalk/CAM_XIMEACamVIS03601.jl.png";
		Matrix<Float> img = ImagePNG<Float>(file2, true).mat();
		Matrix<Float> ref = ImagePNG<Float>(file1, true).mat();
		Matrix<Float> dst;
		cout << SSIM(img, ref, dst, 1023.0f);
		
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
}
