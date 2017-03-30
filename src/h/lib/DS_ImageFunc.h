#pragma once

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

}
