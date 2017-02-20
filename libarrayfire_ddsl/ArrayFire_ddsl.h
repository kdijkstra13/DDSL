#pragma once
#include <memory>
#include "arrayfire.h"

namespace ArrayFireDDSL {
	//Wrappers for ArrayFire functions to force linking

	void setDevice(int device);

	af::array zScore(af::array data, const unsigned int dim = 0);

	void PCA(const af::array &data, af::array &vectors, af::array &values);
	
	void LDA(const af::array &data, af::array &targets, af::array &classes, af::array &vectors, af::array &values);	

	af::array project(af::array &data, af::array &vectors, unsigned int keepDims=0);

/*	af::array colMean(const af::array &in);

	af::array colSDev(const af::array &in);

	af::array correlation(const af::array &in);

	af::array covariance(const af::array &in);

	af::array multiplyMatrix(const af::array &dst, af::array &mat);

	//New High-level functions based on ArrayFire
	af::array zScore(af::array &data, int tileSize=1) {

	void PCA(const af::array &data, af::array &vectors, af::array &values);

	af::array project(af::array &data, af::array &vectors, af::array &dst);
*/
}