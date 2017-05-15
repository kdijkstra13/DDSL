#pragma once
#include "h/lib/DS_Image.h"

namespace DSFunc {
	//********************//
	//** Convert mosaic **//
	//********************//

	template <typename T, typename TIdx>
	Matrix<T, TIdx> mosaicToTile(const Matrix<T, TIdx> &mosaic, const TIdx mosaicCountX, const TIdx mosaicCountY, const TIdx tileIdx);

	template <typename T, typename TIdx>
	Matrix<T, TIdx> mosaicToTiled(const Matrix<T, TIdx> &mosaic, const TIdx mosaicCountX, const TIdx mosaicCountY);

	template <typename T, typename TIdx>
	Matrix<Matrix<T, TIdx>> mosaicToTiles(const Matrix<T, TIdx> &mosaic, const TIdx mosaicCountX, const TIdx mosaicCountY);

	//********************//
	//** Convert tiled **//
	//********************//

	template <typename T, typename TIdx>
	Matrix<T, TIdx> tiledToTile(const Matrix<T, TIdx> &tiled, const TIdx tileCountX, const TIdx tileCountY, const TIdx tileIdx);
		
	template <typename T, typename TIdx>
	Matrix<T, TIdx> tiledToMosaic(const Matrix<T, TIdx> &tiled, const TIdx tileCountX, const TIdx tileCountY);

	template <typename T, typename TIdx>
	Matrix<Matrix<T, TIdx>> tiledToTiles(const Matrix<T, TIdx> &tiled, const TIdx tileCountX, const TIdx tileCountY);

	//*******************//
	//** Convert tiles **//
	//*******************//

	template <typename T, typename TIdx>
	Matrix<T, TIdx> tilesToSmallMosaic(const Matrix<Matrix<T, TIdx>, TIdx> &tiles);

	template <typename T, typename TIdx>
	Matrix<T, TIdx> tilesToMosaic(const Matrix<Matrix<T, TIdx>> &tiles, const TIdx tileCountX, const TIdx tileCountY);

	template <typename T, typename TIdx>
	Matrix<T, TIdx> tilesToTiled(const Matrix<Matrix<T, TIdx>, TIdx> &tiles, const TIdx tileCountX, const TIdx tileCountY);
	
	//**
	//** Aggregation functions (note: Use helper functions to use them)
	//**

	template <typename T, typename TIdx>
	T aggrMean(const T * const pixelPtr, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const TIdx imgStrideY, const TIdx imgStrideX);

	template <typename T, typename TIdx>
	T aggrSum(const T * const pixelPtr, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const TIdx imgStrideY, const TIdx imgStrideX);

	template <typename T, typename TIdx>
	T aggrMin(const T * const pixelPtr, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const TIdx imgStrideY, const TIdx imgStrideX);

	template <typename T, typename TIdx>
	T aggrConvolution(const T * const pixelPtr, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const TIdx imgStrideY, const TIdx imgStrideX, const DSLib::Matrix<T, TIdx> &mask);

	template <typename T, typename TIdx>
	T aggrSSIM(const T * const pixelPtr1, const T * const pixelPtr2, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const TIdx imgStrideY1, const TIdx imgStrideX1, const TIdx imgStrideY2, const TIdx imgStrideX2, const DSLib::Matrix<T, TIdx> &mask, const T C1, const T C2, const T C3);
	
	//**
	//Helper functions for creating lambdas
	//**

	template <typename T, typename TIdx>
	void drawGaussian(DSLib::Matrix<T, TIdx> &mat, const TIdx meanY, const TIdx meanX, const T sigma);

	template <typename T, typename TIdx>
	std::function<T(const T * const, const TIdx, const TIdx)> makeAggrMean(const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX);

	template <typename T, typename TIdx>
	std::function<T(const T * const, const TIdx, const TIdx)> makeAggrSum(const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX);

	template <typename T, typename TIdx>
	std::function<T(const T * const, const TIdx, const TIdx)> makeAggrConvolution(const DSLib::Matrix<T, TIdx> &mask, const TIdx windowOriginY, const TIdx windowOriginX);

	template <typename T, typename TIdx>
	std::function<T(const T * const p1, const TIdx, const TIdx, const T * const, const TIdx, const TIdx)> makeAggrSSIM(const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const T sigma, const T C1, const T C2, const T C3);

	template <typename T, typename TIdx>
	std::function<T(const T * const, const TIdx, const TIdx)> makeAggrGauss(const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const T sigma);

	template <typename T, typename TIdx>
	std::function<T(const T * const, const TIdx, const TIdx)> makeAggrMin(const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX);

	template <typename T>
	std::function<T(const T)> makeFuncQuadratic(const T a, const T b, const T c);

	template <typename T>
	std::function<T(const T)> makeFuncLinear(const T a, const T b);

	template <typename T>
	std::function<T(const T, const T)> makeFuncDiffSq();

	template <typename T>
	T getSlideArea(const T height, const T width, const T windowHeight, const T windowWidth);

	//Slide two images with function
	template <typename T, typename TIdx, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginX, const TIdx windowOriginY>
	T slide(const DSLib::Matrix<T, TIdx> &a, const DSLib::Matrix<T, TIdx> &b, DSLib::Matrix<T, TIdx> &dst, std::function<T(const T, const T)> func);

	//Slide one image with function
	template <typename T, typename TIdx, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginX, const TIdx windowOriginY>
	T slide(DSLib::Matrix<T, TIdx> &mat, std::function<T(const T)> func);

	//Slide one image with aggregation and function
	template <typename T, typename TIdx, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginX, const TIdx windowOriginY>
	T slide(const DSLib::Matrix<T, TIdx> &src, DSLib::Matrix<T, TIdx> &dst, std::function<T(const T * const, const TIdx, const TIdx)> aggr, std::function<T(const T)> func);

	//Slide two images with aggregation and function
	template <typename T, typename TIdx, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginX, const TIdx windowOriginY>
	T slide(const DSLib::Matrix<T, TIdx> &a, const DSLib::Matrix<T, TIdx> &b, DSLib::Matrix<T, TIdx> &dst, std::function<T(const T * const p1, const TIdx, const TIdx, const T * const, const TIdx, const TIdx)> aggr, std::function<T(const T)> func);
	
	//*********************//
	//** Image functions **//
	//*********************//

	//Structural Simularity Index
	template <typename T, typename TIdx>
	T SSIM(const Matrix<T, TIdx> &a, const Matrix<T, TIdx> &b, Matrix<T, TIdx> &dst, T max = numeric_limits<T>().max());

	//Mean Squared Error
	template <typename T, typename TIdx>
	T MSE(const Matrix<T, TIdx> &a, const Matrix<T, TIdx> &b);

	//Peak signal-to-noise Index
	template <typename T, typename TIdx>
	T PSNR(const Matrix<T, TIdx> &a, const Matrix<T, TIdx> &b, T max = numeric_limits<T>().max());

}
