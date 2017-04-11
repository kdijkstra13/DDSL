#pragma once
#include "h/lib/DS_MatrixFunc.h"
#include "hpp/lang/DS_MatrixLang.hpp"
#include "hpp/lib/DS_Image.hpp"

#ifndef DDSL_NO_EXT_LIB_PNG
#include "h/lib/DS_Image_PNG.h"
#endif

using namespace std;
using namespace DSLib;
using namespace DSTypes;
using namespace DSLang;
using namespace DSImage;

namespace DSFunc {
	//********************//
	//** Convert mosaic **//
	//********************//

	template <typename T, typename TIdx>
	Matrix<T, TIdx> mosaicToTile(const Matrix<T, TIdx> &mosaic, const TIdx mosaicCountX, const TIdx mosaicCountY, const TIdx tileIdx) {
		Matrix<T, TIdx> & mosaic2 = const_cast<Matrix<T, TIdx>&>(mosaic);

		TIdx yo = tileIdx / mosaicCountY;
		TIdx xo = tileIdx % mosaicCountX;
		TIdx tileWidth = (mosaic2.cols.count() / mosaicCountX);
		TIdx tileHeight = (mosaic2.rows.count() / mosaicCountY);

		Matrix<T, TIdx> ret(tileHeight, tileWidth);
		auto dst_it = ret.rows->begin();
		for (TIdx y = 0;y<mosaic2.rows.count();y += mosaicCountY) {
			for (TIdx x = 0;x<mosaic2.cols.count();x += mosaicCountX) {
				*dst_it = mosaic2.val(y + yo, x + xo);
				dst_it++;
			}
		}
		return ret;
	}

	template <typename T, typename TIdx>
	Matrix<T, TIdx> mosaicToTiled(const Matrix<T, TIdx> &mosaic, const TIdx mosaicCountX, const TIdx mosaicCountY) {
		Matrix<T, TIdx> & mosaic2 = const_cast<Matrix<T, TIdx>&>(mosaic);
		TIdx tileWidth = mosaic2.cols.count() / mosaicCountX;
		TIdx tileHeight = mosaic2.rows.count() / mosaicCountY;
		Matrix<T, TIdx> tiled(mosaic2.rows.count(), mosaic2.cols.count());
		for (auto y = tiled.rows->begin(); y != tiled.rows.end();y += tileHeight) {
			for (auto x = tiled.cols->begin(); x != tiled.cols->end();x += tileWidth) {
				for (auto yo = 0;yo<tileHeight;yo++) {
					for (auto xo = 0;xo<tileWidth;xo++) {
						tiled.val(y + yo, x + xo) = mosaic2.val((y / tileHeight) + (yo*mosaicCountY), (x / tileWidth) + (xo*mosaicCountX));
					}
				}
			}
		}
		return tiled;
	}

	template <typename T, typename TIdx>
	Matrix<Matrix<T, TIdx>> mosaicToTiles(const Matrix<T, TIdx> &mosaic, const TIdx mosaicCountX, const TIdx mosaicCountY) {
		Matrix<Matrix<T, TIdx>> mats(1U, mosaicCountX * mosaicCountY);
		TIdx idx = 0;
		for (auto it = mats.vec().begin(); it != mats.vec().end(); it++, idx++)
			*it = mosaicToTile(mosaic, mosaicCountX, mosaicCountY, idx);
		return mats;
	}

	//********************//
	//** Convert tiled **//
	//********************//

	template <typename T, typename TIdx>
	Matrix<T, TIdx> tiledToTile(const Matrix<T, TIdx> &tiled, const TIdx tileCountX, const TIdx tileCountY, const TIdx tileIdx) {
		Matrix<T, TIdx> & tiled2 = const_cast<Matrix<T, TIdx>&>(tiled);

		TIdx y = tileIdx / tileCountY;
		TIdx x = tileIdx % tileCountX;
		TIdx tileWidth = (tiled2.cols.count() / tileCountX);
		TIdx tileHeight = (tiled2.rows.count() / tileCountY);
		return tiled2(y * tileHeight, tileHeight, x * tileWidth, tileWidth);
	}

	template <typename T, typename TIdx>
	Matrix<T, TIdx> tiledToMosaic(const Matrix<T, TIdx> &tiled, const TIdx tileCountX, const TIdx tileCountY) {
		Matrix<T, TIdx> & tiled2 = const_cast<Matrix<T, TIdx>&>(tiled);

		TIdx tileWidth = tiled2.cols.count() / tileCountX;
		TIdx tileHeight = tiled2.rows.count() / tileCountY;
		Matrix<T, TIdx> mosaic(tiled2.rows.count(), tiled2.cols.count());
		for (TIdx y = 0; y != tiled2.rows.count();y += tileHeight) {
			for (TIdx x = 0; x != tiled2.cols.count();x += tileWidth) {
				for (auto yo = 0;yo<tileHeight;yo++) {
					for (auto xo = 0;xo<tileWidth;xo++) {
						mosaic.val((y / tileHeight) + (yo*tileCountY), (x / tileWidth) + (xo*tileCountX)) = tiled2.val(y + yo, x + xo);
					}
				}
			}
		}
		return mosaic;
	}

	template <typename T, typename TIdx>
	Matrix<Matrix<T, TIdx>> tiledToTiles(const Matrix<T, TIdx> &tiled, const TIdx tileCountX, const TIdx tileCountY) {
		Matrix<Matrix<T, TIdx>> mats(1U, tileCountX * tileCountY);
		TIdx idx = 0;
		for (auto it = mats.vec().begin(); it != mats.vec().end(); it++, idx++)
			*it = *tiledToTile(tiled, tileCountX, tileCountY, idx);
		return mats;
	}

	//*******************//
	//** Convert tiles **//
	//*******************//

	template <typename T, typename TIdx>
	Matrix<T, TIdx> tilesToSmallMosaic(const Matrix<Matrix<T, TIdx>, TIdx> &tiles) {
		Matrix<Matrix<T, TIdx>, TIdx> & tiles2 = const_cast<Matrix<Matrix<T, TIdx>, TIdx>&>(tiles);
		Matrix<T, TIdx> ret(tiles2.val(0, 0).rows.count(), tiles2.val(0, 0).cols.count());
		TIdx tileSize = sqrt(~tiles2);
		for (TIdx y = 0;y<ret.rows.count();y += tileSize) {
			for (TIdx x = 0;x<ret.cols.count();x += tileSize) {
				Matrix<T, TIdx> part = ret(y, tileSize, x, tileSize);
				auto partit = part.rows->begin();
				TIdx idx = 0;
				for (TIdx yy = 0;yy<tileSize;yy++) {
					for (TIdx xx = 0;xx<tileSize;xx++) {
						*partit = tiles2.vec(idx).val(y + yy, x + xx);
						idx++;
						partit++;
					}
				}
			}
		}
		return ret;
	}

	template <typename T, typename TIdx>
	Matrix<T, TIdx> tilesToMosaic(const Matrix<Matrix<T, TIdx>> &tiles, const TIdx tileCountX, const TIdx tileCountY) {
		Matrix<Matrix<T, TIdx>, TIdx> &tiles2 = const_cast<Matrix<Matrix<T, TIdx>, TIdx> &>(tiles);
		if (~tiles2 != tileCountX * tileCountY)
			throw Error(ecRangeError, "tilesToMosaic", SS(~tiles << " != " << tileCountX << "x" << tileCountY));
		Matrix<T, TIdx> mosaic(tileCountY * tiles.vec(0).rows.count(), tileCountX * tiles.vec(0).cols.count());
		TIdx tileIdx = 0;
		for (auto tile = tiles.vec().begin(); tile != tiles.vec().end(); tile++, tileIdx++) {
			TIdx yo = tileIdx / tileCountY;
			TIdx xo = tileIdx % tileCountX;
			for (TIdx y = 0;y<mosaic.rows.count();y += tileCountY) {
				for (TIdx x = 0;x<mosaic.cols.count();x += tileCountX) {
					mosaic.val(y + yo, x + xo) = tile->val(y / tileCountY, x / tileCountY);
				}
			}
		}
		return mosaic;
	}

	template <typename T, typename TIdx>
	Matrix<T, TIdx> tilesToTiled(const Matrix<Matrix<T, TIdx>, TIdx> &tiles, const TIdx tileCountX, const TIdx tileCountY) {
		Matrix<Matrix<T, TIdx>, TIdx> &tiles2 = const_cast<Matrix<Matrix<T, TIdx>, TIdx> &>(tiles);
		if (~tiles2 != tileCountX * tileCountY)
			throw Error(ecRangeError, "tilesToTiled", SS(~tiles << " != " << tileCountX << "x" << tileCountY));
		TIdx tileSizeY = tiles2.vec(0).rows.count();
		TIdx tileSizeX = tiles2.vec(0).cols.count();
		TIdx i = 0;
		Matrix<T, TIdx> dst;
		dst.resize(tiles2.vec(0).rows.count() * tileCountY, tiles2.vec(0).cols.count() * tileCountX);
		for (TIdx y = 0;y<tileCountY;y++) {
			for (TIdx x = 0;x<tileCountX;x++, i++) {
				Matrix<T, TIdx> part = dst(y*tileSizeY, tileSizeY, x*tileSizeX, tileSizeX);
				if (!part.isSameSize(tiles.vec(i)))
					throw Error(ecInternal, "tilesToTiled", "Not the same size. Cannot perform direct slice assignment.");
				part = tiles.vec(i);
			}
		}
		return dst;
	}

	/*
	float aggrMean(const float * const pixelPtr, const int radius, const int imgWidth) {
	float sum = 0;
	for (int winy=-radius;winy<=radius;winy++) {
	const int pre = (winy * imgWidth);
	for (int winx=-radius;winx<=radius;winx++) {
	const float pxval = *(pixelPtr + winx + pre);
	sum += pxval;
	}
	}
	return sum / ((2*radius+1)*(2*radius+1));
	}

	float aggrMin(const float * const pixelPtr, const int radius, const int imgWidth) {
	float min = *pixelPtr;
	for (int winy = -radius;winy <= radius;winy++) {
	const int pre = (winy * imgWidth);
	for (int winx = -radius;winx <= radius;winx++) {
	const float pxval = *(pixelPtr + winx + pre);
	if (pxval < min)
	min = pxval;
	}
	}
	return min;
	}


	function<float(const float)> makeModelQuadratic(const float a, const float b, const float c) {
	return [a,b,c](const float x) {
	return a * x * x + b * x + c;
	};
	}

	function<float(const float)> makeModelLinear(const float a, const float b) {
	return [a, b](const float x) {
	return a * x + b;
	};
	}

	template <typename T, const int radius>
	void model_tpl(const RealImage<T> &flowSrc, RealImage<T> &dst, function<T(const T * const, const int, const int)> aggr, function<T(const T)> model) {
	dst.Resize(HeightWidth(flowSrc.GetHeight(), flowSrc.GetWidth()));
	SetAllPixels(dst, 0.0f);

	const int startx = radius;
	const int imgw = flowSrc.GetWidth();
	const int endx = flowSrc.GetWidth() - radius;
	const int starty = radius;
	const int endy = flowSrc.GetHeight() - radius;

	const T * const flowSrcPtr = flowSrc.GetFirstPixelPtr();
	T * const dstPtr = dst.GetFirstPixelPtr();

	#pragma omp parallel for
	for (int y = starty;y<endy;y++) {
	const int pre = y * imgw;
	for (int x = startx;x<endx;x++) {
	const T * pixelPtr = flowSrcPtr + pre + x;
	T val = aggr(pixelPtr, radius, imgw);
	dstPtr[pre + x] = model(val);
	}
	}
	}*/
}
