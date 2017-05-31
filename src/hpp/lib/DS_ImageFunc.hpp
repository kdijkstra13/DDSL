#pragma once
#include "h/lib/DS_ImageFunc.h"
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

		TIdx y = tileIdx / tileCountX;
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
		TIdx tileSize = (TIdx)sqrt(~tiles2);
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


	/** General sliding window methods **/
	template <typename T, typename TIdx>
	T aggrMean(const T * const pixelPtr, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const TIdx imgStrideY, const TIdx imgStrideX) {
		T sum = 0;
		for (int winy = -(int)windowOriginY; winy < (int)windowHeight - (int)windowOriginY;winy++) {
			const int pre = (winy * imgStrideY);
			for (int winx = -(int)windowOriginX;winx < (int)windowWidth - (int)windowOriginX;winx++) {
				const T pxval = *(pixelPtr + winx + pre);
				sum += pxval;
			}
		}
		return sum / (windowWidth * windowHeight);
	}

	template <typename T, typename TIdx>
	T aggrSum(const T * const pixelPtr, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const TIdx imgStrideY, const TIdx imgStrideX) {
		T sum = 0;
		for (int winy = -(int)windowOriginY; winy < (int)windowHeight - (int)windowOriginY;winy++) {
			const int pre = (winy * imgStrideY);
			for (int winx = -(int)windowOriginX;winx < (int)windowWidth - (int)windowOriginX;winx++) {
				const T pxval = *(pixelPtr + winx + pre);
				sum += pxval;
			}
		}
		return sum;
	}

	template <typename T, typename TIdx>
	T aggrMin(const T * const pixelPtr, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const TIdx imgStrideY, const TIdx imgStrideX) {
		T min = *pixelPtr;
		for (int winy = -(int)windowOriginY;winy < windowHeight - windowOriginY;winy++) {
			const int pre = (winy * imgStrideY);
			for (int winx = -(int)windowOriginX;winx < windowWidth - windowOriginX;winx++) {
				const T pxval = *(pixelPtr + winx + pre);
				if (pxval < min)
					min = pxval;
			}
		}
		return min;
	}

	template <typename T, typename TIdx>
	T aggrConvolution(const T * const pixelPtr, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const TIdx imgStrideY, const TIdx imgStrideX, const DSLib::Matrix<T, TIdx> &mask) {
		T sum = 0;
		int masky = 0;
		for (int winy = -(int)windowOriginY; winy < (int)windowHeight - (int)windowOriginY;winy++, masky++) {
			int maskx = 0;
			const int pre = (winy * imgStrideY);
			for (int winx = -(int)windowOriginX;winx < (int)windowWidth - (int)windowOriginX;winx++, maskx++) {
				const T pxval = *(pixelPtr + winx + pre);
				sum += pxval * mask.val(masky, maskx);
			}
		}
		return sum;
	}

	template <typename T, typename TIdx>
	T aggrSSIM(const T * const pixelPtr1, const T * const pixelPtr2, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const TIdx imgStrideY1, const TIdx imgStrideX1, const TIdx imgStrideY2, const TIdx imgStrideX2, const DSLib::Matrix<T, TIdx> &mask, const T C1, const T C2, const T C3) {
		//mean
		T mean1 = 0;
		T mean2 = 0;
		int masky = 0;
		for (int winy = -(int)windowOriginY; winy < (int)windowHeight - (int)windowOriginY;winy++, masky++) {
			int maskx = 0;
			const int pre1 = (winy * imgStrideY1);
			const int pre2 = (winy * imgStrideY2);
			for (int winx = -(int)windowOriginX;winx < (int)windowWidth - (int)windowOriginX;winx++, maskx++) {
				const T pxval1 = *(pixelPtr1 + winx + pre1);
				const T pxval2 = *(pixelPtr2 + winx + pre2);
				const T w = mask.val(masky, maskx);
				mean1 += pxval1 * w;
				mean2 += pxval2 * w;
			}
		}

		//var and cov
		T var1 = 0;
		T var2 = 0;
		T cov = 0;
		masky = 0;
		for (int winy = -(int)windowOriginY; winy < (int)windowHeight - (int)windowOriginY;winy++, masky++) {
			int maskx = 0;
			const int pre1 = (winy * imgStrideY1);
			const int pre2 = (winy * imgStrideY2);
			for (int winx = -(int)windowOriginX;winx < (int)windowWidth - (int)windowOriginX;winx++, maskx++) {
				const T pxval1 = *(pixelPtr1 + winx + pre1);
				const T pxval2 = *(pixelPtr2 + winx + pre2);
				const T w = mask.val(masky, maskx);
				var1 += (pxval1 - mean1) * (pxval1 - mean1) * w;
				var2 += (pxval2 - mean2) * (pxval2 - mean2) * w;
				cov += (pxval1 - mean1) * (pxval2 - mean2) * w;
			}
		}
		//const T sd1 = sqrt(var1);
		//const T sd2 = sqrt(var2);

		//ssim
		const T two = (T)2;
		const T mean1sq = mean1 * mean1;
		const T mean2sq = mean2 * mean2;
		const T num = (two * mean1 * mean2 + C1) * (two * cov + C2);
		const T divi = (mean1sq + mean2sq + C1) * (var1 + var1 + C2);
		return (num / divi);
	}

	template <typename T, typename TIdx>
	std::function<T(const T * const, const TIdx, const TIdx)> makeAggrMean(const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX) {
		return [windowHeight, windowWidth, windowOriginY, windowOriginX](const T * const p, const TIdx imgStrideY, const TIdx imgStrideX) {
			return aggrMean(p, windowHeight, windowWidth, windowOriginY, windowOriginX, imgStrideY, imgStrideX);
		};
	}

	template <typename T, typename TIdx>
	std::function<T(const T * const, const TIdx, const TIdx)> makeAggrSum(const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX) {
		return [windowHeight, windowWidth, windowOriginY, windowOriginX](const T * const p, const TIdx imgStrideY, const TIdx imgStrideX) {
			return aggrSum(p, windowHeight, windowWidth, windowOriginY, windowOriginX, imgStrideY, imgStrideX);
		};
	}

	template <typename T, typename TIdx>
	void drawGaussian(DSLib::Matrix<T, TIdx> &mat, const TIdx meanY, const TIdx meanX, const T sigma) {
		//http://mathworld.wolfram.com/GaussianFunction.html
		T scale = T(1) / (T(2)*T(C_PI)*sigma*sigma);
		T mul = T(1) / (T(2)*sigma*sigma);
		T mnx = (T)meanX;
		T mny = (T)meanY;
		for (TIdx y = 0;y<mat.rows.count();y++) {
			for (TIdx x = 0;x<mat.cols.count();x++) {
				T parm = (x - mnx)*(x - mnx) + (y - mny)*(y - mny);
				mat.val(y, x) = scale * exp(-parm * mul);
			}
		}
	}

	template <typename T, typename TIdx>
	std::function<T(const T * const, const TIdx, const TIdx)> makeAggrConvolution(const DSLib::Matrix<T, TIdx> &mask, const TIdx windowOriginY, const TIdx windowOriginX) {
		TIdx windowHeight = mask.rows.count();
		TIdx windowWidth = mask.cols.count();
		return [windowHeight, windowWidth, windowOriginY, windowOriginX, mask](const T * const p, const TIdx imgStrideY, const TIdx imgStrideX) {
			return aggrConvolution(p, windowHeight, windowWidth, windowOriginY, windowOriginX, imgStrideY, imgStrideX, mask);
		};
	}

	template <typename T, typename TIdx>
	std::function<T(const T * const p1, const TIdx, const TIdx, const T * const, const TIdx, const TIdx)> makeAggrSSIM(const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const T sigma, const T C1, const T C2, const T C3) {
		DSLib::Matrix<T, TIdx> mask(windowHeight, windowWidth);
		drawGaussian(mask, windowHeight / 2, windowWidth / 2, sigma);
		return [windowHeight, windowWidth, windowOriginY, windowOriginX, mask, C1, C2, C3](const T * const p1, const TIdx imgStrideY1, const TIdx imgStrideX1, const T * const p2, const TIdx imgStrideY2, const TIdx imgStrideX2) {
			return aggrSSIM(p1, p2, windowHeight, windowWidth, windowOriginY, windowOriginX, imgStrideY1, imgStrideX1, imgStrideY2, imgStrideX2, mask, C1, C2, C3);
		};
	}

	template <typename T, typename TIdx>
	std::function<T(const T * const, const TIdx, const TIdx)> makeAggrGauss(const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX, const T sigma) {
		DSLib::Matrix<T, TIdx> mask(windowHeight, windowWidth);
		drawGaussian(mask, windowHeight / 2, windowWidth / 2, sigma);
		return [windowHeight, windowWidth, windowOriginY, windowOriginX, mask](const T * const p, const TIdx imgStrideY, const TIdx imgStrideX) {
			return aggrConvolution(p, windowHeight, windowWidth, windowOriginY, windowOriginX, imgStrideY, imgStrideX, mask);
		};
	}

	template <typename T, typename TIdx>
	std::function<T(const T * const, const TIdx, const TIdx)> makeAggrMin(const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginY, const TIdx windowOriginX) {
		return [windowHeight, windowWidth, windowOriginY, windowOriginX](const T * const p, const TIdx imgStrideY, const TIdx imgStrideX) {
			return aggrMin(p, windowHeight, windowWidth, windowOriginY, windowOriginX, imgStrideY, imgStrideX);
		};
	}

	template <typename T>
	std::function<T(const T)> makeFuncQuadratic(const T a, const T b, const T c) {
		return [a, b, c](const T x) {
			return a * x * x + b * x + c;
		};
	}

	template <typename T>
	std::function<T(const T)> makeFuncLinear(const T a, const T b) {
		return [a, b](const T x) {
			return a * x + b;
		};
	}

	template <typename T>
	std::function<T(const T, const T)> makeFuncDiffSq() {
		return [](const T x, const T y) {
			return (x - y) * (x - y);
		};
	}

	template <typename T>
	T getSlideArea(const T height, const T width, const T windowHeight, const T windowWidth) {
		return (height * width) - (windowHeight * width) - (windowWidth * height) + (windowWidth * windowHeight);
	}

	//Returns mean response
	template <typename T, typename TIdx, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginX, const TIdx windowOriginY>
	T slide(const DSLib::Matrix<T, TIdx> &a, const DSLib::Matrix<T, TIdx> &b, DSLib::Matrix<T, TIdx> &dst, std::function<T(const T, const T)> func) {
		if (a.order() != DSTypes::oRowMajor)
			throw DSTypes::Error(DSTypes::ecParameter, "slide", "Invalid order for a");
		if (b.order() != DSTypes::oRowMajor)
			throw DSTypes::Error(DSTypes::ecParameter, "slide", "Invalid order for b");
		if (dst.order() != DSTypes::oRowMajor)
			throw DSTypes::Error(DSTypes::ecParameter, "slide", "Invalid order for dst");
		if (!a.isSameSize(b))
			throw DSTypes::Error(DSTypes::ecParameter, "slide", SS("a and b have different sizes " << a.printSize() << " != " << b.printSize()));
		if (!dst.isSameSize(a))
			dst.resize(a.rows.count(), a.cols.count());

		const int startx = windowOriginX;
		const int imgw = a.cols.count();
		const int endx = a.cols.count() - (windowWidth - windowOriginX);
		const int starty = windowOriginY;
		const int endy = a.rows.count() - (windowHeight - windowOriginY);
		//const int dststridex = 1;
		const int dststridey = dst.getDataStride();
		const int astridex = 1;
		const int astridey = b.getDataStride();
		const int bstridex = 1;
		const int bstridey = b.getDataStride();

		const T * const aPtr = a.getData();
		const T * const bPtr = b.getData();
		T * const dstPtr = dst.getData();
		T sum = 0;

		//#pragma omp parallel for private(y) schedule(dynamic) reduction(+:sum)
		for (int y = starty;y<endy;y++) {
			const int prea = y * astridey;
			const int preb = y * bstridey;
			const int predst = y * dststridey;
			for (int x = startx;x<endx;x++) {
				const T * pixelPtra = aPtr + prea + x;
				const T * pixelPtrb = bPtr + preb + x;
				dstPtr[predst + x] = func(*pixelPtra, *pixelPtrb);
				sum += dstPtr[predst + x];
			}
		}
		return sum / getSlideArea(a.rows.count(), a.cols.count(), windowHeight, windowWidth);
	}

	template <typename T, typename TIdx, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginX, const TIdx windowOriginY>
	T slide(DSLib::Matrix<T, TIdx> &mat, std::function<T(const T)> func) {
		const int startx = windowOriginX;
		const int imgw = mat.cols.count();
		const int endx = mat.cols.count() - (windowWidth - windowOriginX);
		const int starty = windowOriginY;
		const int endy = mat.rows.count() - (windowHeight - windowOriginY);
		const int stridey = mat.getDataStride();
		T * const ptr = mat.getData();
		T sum = 0;

		//#pragma omp parallel for private(y) schedule(dynamic) reduction(+:sum)
		for (int y = starty;y<endy;y++) {
			const int pre = y * stridey;
			for (int x = startx;x<endx;x++) {
				ptr[pre + x] = func(ptr[pre + x]);
				sum += ptr[pre + x];
			}
		}
		return sum / getSlideArea(mat.rows.count(), mat.cols.count(), windowHeight, windowWidth);
	}

	template <typename T, typename TIdx, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginX, const TIdx windowOriginY>
	T slide(const DSLib::Matrix<T, TIdx> &src, DSLib::Matrix<T, TIdx> &dst, std::function<T(const T * const, const TIdx, const TIdx)> aggr, std::function<T(const T)> func) {
		if (src.order() != DSTypes::oRowMajor || dst.order() != DSTypes::oRowMajor)
			throw DSTypes::Error(DSTypes::ecParameter, "slide", "Invalid order for either source or destination");
		if (!dst.isSameSize(src))
			dst.resize(src.rows.count(), src.cols.count());

		const int startx = windowOriginX;
		const int imgw = src.cols.count();
		const int endx = src.cols.count() - (windowWidth - windowOriginX);
		const int starty = windowOriginY;
		const int endy = src.rows.count() - (windowHeight - windowOriginY);
		//const int dststridex = 1;
		const int dststridey = dst.getDataStride();
		const int srcstridex = 1;
		const int srcstridey = src.getDataStride();

		const T * const srcPtr = src.getData();
		T * const dstPtr = dst.getData();
		T sum = 0;

		//#pragma omp parallel for private(y) schedule(dynamic) reduction(+:sum)
		for (int y = starty;y<endy;y++) {
			const int presrc = y * srcstridey;
			const int predst = y * dststridey;
			for (int x = startx;x<endx;x++) {
				const T * pixelPtr = srcPtr + presrc + x;
				T val = aggr(pixelPtr, srcstridey, srcstridex);
				dstPtr[predst + x] = func(val);
				sum += dstPtr[predst + x];
			}
		}
		return sum / getSlideArea(src.rows.count(), src.cols.count(), windowHeight, windowWidth);
	}

	//Returns mean response
	template <typename T, typename TIdx, const TIdx windowHeight, const TIdx windowWidth, const TIdx windowOriginX, const TIdx windowOriginY>
	T slide(const DSLib::Matrix<T, TIdx> &a, const DSLib::Matrix<T, TIdx> &b, DSLib::Matrix<T, TIdx> &dst, std::function<T(const T * const p1, const TIdx, const TIdx, const T * const, const TIdx, const TIdx)> aggr, std::function<T(const T)> func) {
		if (a.order() != DSTypes::oRowMajor)
			throw DSTypes::Error(DSTypes::ecParameter, "slide", "Invalid order for a");
		if (b.order() != DSTypes::oRowMajor)
			throw DSTypes::Error(DSTypes::ecParameter, "slide", "Invalid order for b");
		if (dst.order() != DSTypes::oRowMajor)
			throw DSTypes::Error(DSTypes::ecParameter, "slide", "Invalid order for dst");
		if (!a.isSameSize(b))
			throw DSTypes::Error(DSTypes::ecParameter, "slide", SS("a and b have different sizes " << a.printSize() << " != " << b.printSize()));
		if (!dst.isSameSize(a))
			dst.resize(a.rows.count(), a.cols.count());

		const int startx = windowOriginX;
		const int imgw = a.cols.count();
		const int endx = a.cols.count() - (windowWidth - windowOriginX);
		const int starty = windowOriginY;
		const int endy = a.rows.count() - (windowHeight - windowOriginY);
		//const int dststridex = 1;
		const int dststridey = dst.getDataStride();
		const int astridex = 1;
		const int astridey = b.getDataStride();
		const int bstridex = 1;
		const int bstridey = b.getDataStride();

		const T * const aPtr = a.getData();
		const T * const bPtr = b.getData();
		T * const dstPtr = dst.getData();
		T sum = 0;

		//#pragma omp parallel for private(y) schedule(dynamic) reduction(+:sum)
		for (int y = starty;y<endy;y++) {
			const int prea = y * astridey;
			const int preb = y * bstridey;
			const int predst = y * dststridey;
			for (int x = startx;x<endx;x++) {
				const T * pixelPtra = aPtr + prea + x;
				const T * pixelPtrb = bPtr + preb + x;
				T val = aggr(pixelPtra, astridey, astridex, pixelPtrb, bstridey, bstridex);
				dstPtr[predst + x] = func(val);
				sum += dstPtr[predst + x];
			}
		}
		return sum / getSlideArea(a.rows.count(), a.cols.count(), windowHeight, windowWidth);
	}

	//Structural Simularity Index
	template <typename T, typename TIdx>
	T SSIM(const Matrix<T, TIdx> &a, const Matrix<T, TIdx> &b, Matrix<T, TIdx> &dst, T max) {
		//Wang et al. 2004
		const T sigma = (T)1.5;
		const TIdx w_w = 11;
		const TIdx w_h = 11;
		const TIdx w_ox = 5;
		const TIdx w_oy = 5;
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

	//Mean Squared Error
	template <typename T, typename TIdx>
	T MSE(const Matrix<T, TIdx> &a, const Matrix<T, TIdx> &b) {
		//https://en.wikipedia.org/wiki/Mean_squared_error
		auto func = makeFuncDiffSq<T>();
		Matrix<T, TIdx> dst;
		return slide<T, TIdx, 1, 1, 0, 0>(a, b, dst, func);				
	}

	//Peak Signal To Noise Ratio
	template <typename T, typename TIdx>
	T PSNR(const Matrix<T, TIdx> &a, const Matrix<T, TIdx> &b, T max) {
		//https://en.wikipedia.org/wiki/Peak_signal-to-noise_ratio
		T mse = MSE(a, b);
		return 20 * log10(max) - 10 * log10(mse);
	}

}
