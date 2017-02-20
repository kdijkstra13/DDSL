#include "ArrayFireDDSL.h"

using namespace std;
using namespace af;

namespace ArrayFireDDSL {
	void setDevice(int device) {
		af::setDevice(0);
		af::info();
	}

	af::array zScore(af::array data, const unsigned int dim) {
		af::array means = af::mean(data, dim);
		af::array stdvs = af::stdev(data, dim);
		unsigned int n = (unsigned int)data.dims(dim);
		af::array tiledmeans = tile(means, (dim==0U)?n:1, (dim==1)?n:1);
		af::array tiledstdvs = tile(stdvs, (dim==0U)?n:1, (dim==1)?n:1);
		data = (data - tiledmeans) / tiledstdvs;
		return data;
	}

	void PCA(const af::array &data, af::array &vectors, af::array &values) {		
		af::array u;
		af::svd(u, values, vectors, data);
		values = sqrt((values * values) / data.dims(0));
		af::transposeInPlace(vectors); //ArrayFire's SVD returns the vectors transposed..
	}

	void LDA(const af::array &data, af::array &targets, af::array &classes, af::array &vectors, af::array &values) {		
		//Get between class scatter
		af::array classMeans(classes.dims(0), data.dims(1));
		for (unsigned int i = 0; i < classes.dims(0);i++) {
			af::array c = data(targets == tile(classes(i), (unsigned int)targets.dims(0)), af::span);
			classMeans(i, span) = af::mean(c);
		}
		af::array Sb = af::matmul(classMeans, classMeans, AF_MAT_TRANS) / (classMeans.dims(0));

		//Get within class scatter
		af::array Sw = af::matmul(data, data, AF_MAT_TRANS) / (data.dims(0));

		//Solve system
		af::array u;
		af::array ratio = af::matmul(af::inverse(Sw), Sb);
		af::svd(u, values, vectors, ratio);
		af::transposeInPlace(vectors); //ArrayFire's SVD returns the vectors transposed..
		vectors = vectors.cols(0, (int)classes.dims(0) - 2);
		values = values.rows(0, (int)classes.dims(0) - 2);
	}

	af::array project(af::array &data, af::array &vectors, unsigned int keepDims) {
		if (keepDims == 0)
			return af::matmul(data, vectors);
		else
			return af::matmul(data, vectors.cols(0, keepDims - 1));
	}
}