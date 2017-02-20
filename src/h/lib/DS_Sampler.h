#pragma once

namespace DSLib {
	//********************************//
	//** Single image to patches    **//
	//********************************//

	//Convert an image with a per-pixel target to patches and targets
	template <typename T, typename TIdx, template <typename, typename> class TMat> 
	void matrixToPatches(const TMat<T, TIdx> &mat, const TMat<T, TIdx> &matTargets, const Matrix<TIdx, TIdx> &coords, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<T, TIdx> &patchTargets, const TIdx patchHeight=3, const TIdx patchWidth=3);

	//Convert an image to patches
	template <typename T, typename TIdx, template <typename, typename> class TMat> 
	void matrixToPatches(const TMat<T, TIdx> &mat, const Matrix<TIdx, TIdx> &coords, Matrix<TMat<T, TIdx>, TIdx> &patches, const TIdx patchHeight=3, const TIdx patchWidth=3);

	//********************************//
	//** Multiple images to patches **//
	//********************************//

	//Convert images and per-pixel targets to n random patches and targets
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToUniformPatches(const Matrix<TMat<T, TIdx>, TIdx> &mats, const Matrix<TMat<T, TIdx>> &targets, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<T, TIdx> &patchTargets, Matrix<TIdx, TIdx> &coords, const TIdx n, const TIdx patchHeight=3, const TIdx patchWidth=3, const bool targetAsLabels=true);

	//Convert images and per-image targets to n random patches and targets
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToUniformPatches(const Matrix<TMat<T, TIdx>, TIdx> &mats, const Matrix<T, TIdx> &targets, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<T, TIdx> &patchTargets, Matrix<TIdx, TIdx> &coords, const TIdx n, const TIdx patchHeight=3, const TIdx patchWidth=3);

	//Convert images and per-pixel targets and coordinates to nRow, nCol random patches and targets (data augmentation based on coordsIn)
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToUniformPatches(const Matrix<TMat<T, TIdx>, TIdx> &mats, const Matrix<TMat<T, TIdx>> &targets, const Matrix<Matrix<TIdx, TIdx>, TIdx> &coordsIn, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<T, TIdx> &patchTargets, Matrix<Matrix<TIdx, TIdx>, TIdx> &coords, const TIdx n, const TIdx negativeX, const TIdx positiveX, const TIdx negativeY, const TIdx positiveY, const TIdx patchHeight=3, const TIdx patchWidth=3, const bool randomChannels=false, const bool targetAsLabels=true);

	//Convert images and per-image targets and coordinates to nRow, nCol random patches and targets (data augmentation based on coordsIn)
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToUniformPatches(const Matrix<TMat<T, TIdx>, TIdx> &mats, const Matrix<T, TIdx> &targets, const Matrix<Matrix<TIdx, TIdx>, TIdx> &coordsIn, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<T, TIdx> &patchTargets, Matrix<Matrix<TIdx, TIdx>, TIdx> &coords, const TIdx n, const TIdx negativeY, const TIdx positiveY, const TIdx negativeX, const TIdx positiveX, const TIdx patchHeight=3, const TIdx patchWidth=3, const bool randomChannels=false, const bool targetAsLabels=true);

	//Convert images and per-coordinate targets and coordinates to nRow, nCol random patches and targets (data augmentation based on coordsIn)
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToUniformPatches(const Matrix<TMat<T, TIdx>, TIdx> &mats, const Matrix<Matrix<TIdx, TIdx>, TIdx> &coordsIn, const Matrix<Matrix<T, TIdx>, TIdx> &coordsTargets, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<T, TIdx> &patchTargets, Matrix<Matrix<TIdx, TIdx>, TIdx> &coords, const TIdx n, const TIdx negativeY, const TIdx positiveY, const TIdx negativeX, const TIdx positiveX, const TIdx patchHeight=3, const TIdx patchWidth=3, const bool randomChannels=false, const bool targetAsLabels=true);

	//Convert images to uniform patches
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToUniformPatches(const Matrix<TMat<T, TIdx>, TIdx> &mats, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<TIdx, TIdx> &coords, const TIdx n, const TIdx patchHeight=3U, const TIdx patchWidth=3U);

	//Convert images and per-pixel targets to dense patches and targets
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToPatches(const Matrix<TMat<T, TIdx>, TIdx> &mats, const Matrix<TMat<T, TIdx>> &targets, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<T, TIdx> &patchTargets, Matrix<TIdx, TIdx> &coords, const TIdx patchHeight=3, const TIdx patchWidth=3);

	//Convert images and per-image targets to dense patches and targets
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToPatches(const Matrix<TMat<T, TIdx>, TIdx> &mats, const Matrix<T, TIdx> &targets, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<T, TIdx> &patchTargets, Matrix<TIdx, TIdx> &coords, const TIdx patchHeight=3, const TIdx patchWidth=3);

	//Convert images to dense patches
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToPatches(const Matrix<TMat<T, TIdx>, TIdx> &mats, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<TIdx, TIdx> &coords, const TIdx patchHeight=3, const TIdx patchWidth=3);

	//******************************//
	//** Single image to samples **//
	//******************************//

	//Convert an image with and per-pixel target to samples and targets
	template <typename T, typename TIdx>
	void matrixToSamples(const Matrix<T, TIdx> &mat, const Matrix<T, TIdx> &matTargets, const Matrix<TIdx, TIdx> &coords, Matrix<T, TIdx> &samples, Matrix<T, TIdx> &sampleTargets, const TIdx windowHeight=1, const TIdx windowWidth=1);


	//Convert an image to samples
	template <typename T, typename TIdx>
	void matrixToSamples(const Matrix<T, TIdx> &mat, const Matrix<TIdx, TIdx> &coords, Matrix<T, TIdx> &samples, const TIdx windowHeight=1,const  TIdx windowWidth=1);

	//Convert samples to a images
	template <typename T, typename TIdx>
	void samplesToMatrix(const Matrix<T, TIdx> &samples, const Matrix<TIdx, TIdx> &coords, Matrix<T, TIdx> &mat, const TIdx width, const TIdx height);

	//*********************************//
	//** Multiple images to samples **//
	//********************************//

	//Convert images and per-pixel targets to n random samples and targets
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToUniformSamples(const Matrix<TMat<T, TIdx>, TIdx> &featuresIn, const Matrix<TMat<T, TIdx>> &targetsIn, Matrix<T, TIdx> &featuresOut, Matrix<T, TIdx> &targetsOut, Matrix<TIdx, TIdx> &coords, const TIdx n, const TIdx windowHeight=1, const TIdx windowWidth=1, const bool targetAsLabels=true);

	//Convert images and per-image targets to n random samples and targets per image
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToUniformSamples(const Matrix<TMat<T, TIdx>, TIdx> &featuresIn, const Matrix<T, TIdx> &targetsIn, Matrix<T, TIdx> &featuresOut, Matrix<T, TIdx> &targetsOut, Matrix<TIdx, TIdx> &coords, const TIdx n, const TIdx windowHeight=1, const TIdx windowWidth=1);

	//Convert images and per-pixel targets to dense samples and targets
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToSamples(const Matrix<TMat<T, TIdx>, TIdx> &featuresIn, const Matrix<TMat<T, TIdx>> &targetsIn, Matrix<T, TIdx> &featuresOut, Matrix<T, TIdx> &targetsOut, Matrix<TIdx, TIdx> &coords, const TIdx windowHeight=1, const TIdx windowWidth=1);

	//Convert images and per-image targets to dense samples and targets
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToSamples(const Matrix<TMat<T, TIdx>, TIdx> &featuresIn, const Matrix<T, TIdx> &targetsIn, Matrix<T, TIdx> &featuresOut, Matrix<T, TIdx> &targetsOut, Matrix<TIdx, TIdx> &coords, const TIdx windowHeight=1, const TIdx windowWidth=1);

	//Convert images to dense samples
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToSamples(const Matrix<TMat<T, TIdx>, TIdx> &featuresIn, Matrix<T, TIdx> &featuresOut, Matrix<TIdx, TIdx> &coords, const TIdx windowHeight=1, const TIdx windowWidth=1);

	//Convert images to dense samples
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void samplesToMatrices(const Matrix<T, TIdx> &samples, const Matrix<TIdx, TIdx> &coords, const TIdx height, const TIdx width, const TIdx n, Matrix<TMat<T, TIdx>, TIdx> &mats);

	//Convert images to dense samples
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void samplesToMatrices(const Matrix<T, TIdx> &samples, const Matrix<Matrix<TIdx, TIdx>, TIdx> &coords, const TIdx height, const TIdx width, Matrix<TMat<T, TIdx>, TIdx> &mats);

	//***************************//
	//** Coordinate generation **//
	//***************************//

	//Generate n coordinates balanced on per-pixel targets
	template <typename T, typename TIdx>
	void genUniform2DCoords(const Matrix<T, TIdx> &targets, const TIdx n, const TIdx borderHeight, const TIdx borderWidth, Matrix<TIdx, TIdx> &coords, const bool targetAsLabels=true);

	//Generate n coordinates
	template <typename T, typename TIdx>
	void genUniform2DCoords(const TIdx height, const TIdx width, const TIdx n, const TIdx borderHeight, const TIdx borderWidth, Matrix<T, TIdx> &coords);

	//Generate coordinates from an image
	template <typename T, typename TIdx>
	void genDense2DCoords(const TIdx height, const TIdx width, const TIdx borderHeight, const TIdx borderWidth, Matrix<T, TIdx> &coords);

	//Generate additional random samples (used for data augmentation)
	//Source 2D coordinates are randomized in nRows, nCols additional coords. If nCols > 0 the source coord is copied for all nCols (only for the first row). The randomization starts for additional rows.
	template <typename TIdx>
	void addUniform2DCoords(const Matrix<TIdx, TIdx> &srcCoords, const TIdx nRows, const TIdx nCols, const TIdx negativeX, const TIdx positiveX, const TIdx negativeY, const TIdx positiveY, Matrix<TIdx, TIdx> &coords);

	//**********//
	//** Misc **//
	//**********//

	//Get area of an image excluding border caused by patches
	template <typename TIdx>
	TIdx areaOf(TIdx height, TIdx width, TIdx patchHeight, TIdx patchWidth);

	//Flatten a matrix into a vector
	template <typename T, typename TIdx>
	void matrixToVector(const Matrix<T, TIdx> &in, Matrix<T, TIdx> &out, Order o);
}