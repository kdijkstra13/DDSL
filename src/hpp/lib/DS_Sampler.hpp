#pragma once

#include "h/lib/DS_Sampler.h"

namespace DSLib {
	//********************************//
	//** Single image to patches    **//
	//********************************//

	//Convert an image with a per-pixel target to patches and targets
	template <typename T, typename TIdx, template <typename, typename> class TMat> 
	void matrixToPatches(const TMat<T, TIdx> &mat, const TMat<T, TIdx> &matTargets, const Matrix<TIdx, TIdx> &coords, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<T, TIdx> &patchTargets, const TIdx patchHeight, const TIdx patchWidth) {
		Matrix<TIdx, TIdx> &coordsCast = const_cast<Matrix<TIdx, TIdx>&>(coords);
		TMat<T, TIdx> &matCast = const_cast<TMat<T, TIdx>&>(mat);

		TIdx patchWidth2 = patchWidth / 2;
		TIdx patchHeight2 = patchHeight / 2;
		if (~patchTargets == 0)
			patchTargets.resize(coords.rows.count(), 1);
		else if (patchTargets.rows.count() != coords.rows.count() || patchTargets.cols.count() != 1)
			throw Error(ecSizeMismatch, "matrixToPatches", SS("patchTargets matrix is initialized to the wrong size: (" << patchTargets.rows.count() << " x " << patchTargets.cols.count() << ") != (" << coords.rows.count() << " x 1)"));

		if (~patches == 0)
			patches.resize(coords.rows.count(), 1);
		else if (patches.rows.count() != coords.rows.count() || patches.cols.count() != 1)
			throw Error(ecSizeMismatch, "matrixToPatches", SS("patches matrix is initialized to the wrong size: (" << patches.rows.count() << " x " << patches.cols.count() << ") != (" << coords.rows.count() << " x 1)"));

		auto patch = patches.rows->begin();
		auto patchTarget = patchTargets.rows->begin();
		for (auto coordsRow = coordsCast.rows.begin(); coordsRow != coordsCast.rows.end(); coordsRow++, patch++, patchTarget++) {
				TIdx y = coordsRow->val(0);
				TIdx x = coordsRow->val(1);
				*patchTarget = matTargets.val(y, x);
				matCast.slice(*patch, y - patchHeight2, y - patchHeight2 + patchHeight, x - patchWidth2, x - patchWidth2 + patchWidth);
				//patch->breakUp();
				//*patch = matCast(y - patchHeight2, patchHeight, x - patchWidth2, patchWidth);
		}
	}

	//Convert an image to patches
	template <typename T, typename TIdx, template <typename, typename> class TMat> 
	void matrixToPatches(const TMat<T, TIdx> &mat, const Matrix<TIdx, TIdx> &coords, Matrix<TMat<T, TIdx>, TIdx> &patches, const TIdx patchHeight, const TIdx patchWidth) {
		Matrix<TIdx, TIdx> &coordsCast = const_cast<Matrix<TIdx, TIdx>&>(coords);
		TMat<T, TIdx> &matCast = const_cast<TMat<T, TIdx>&>(mat);
		TIdx patchWidth2 = patchWidth / 2;
		TIdx patchHeight2 = patchHeight / 2;

		if (~patches == 0)
			patches.resize(coords.rows.count(), 1);
		else if (patches.rows.count() != coords.rows.count() || patches.cols.count() != 1)
			throw Error(ecSizeMismatch, "matrixToPatches", SS("patches matrix is initialized to the wrong size: (" << patches.rows.count() << " x " << patches.cols.count() << ") != (" << coords.rows.count() << " x 1)"));

		auto patch = patches.rows->begin();
		for (auto coordsRow = coordsCast.rows.begin(); coordsRow != coordsCast.rows.end(); coordsRow++, patch++) {
				TIdx y = coordsRow->val(0);
				TIdx x = coordsRow->val(1);
				matCast.slice(*patch, y - patchHeight2, y - patchHeight2 + patchHeight, x - patchWidth2, x - patchWidth2 + patchWidth);
				//patch->breakUp();
				//*patch = matCast(y - patchHeight2, patchHeight, x - patchWidth2, patchWidth);
		}
	}

	//********************************//
	//** Multiple images to patches **//
	//********************************//

	//Convert images and per-pixel targets to n random patches and targets
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToUniformPatches(const Matrix<TMat<T, TIdx>, TIdx> &mats, const Matrix<TMat<T, TIdx>> &targets, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<T, TIdx> &patchTargets, Matrix<TIdx, TIdx> &coords, const TIdx n, const TIdx patchHeight, const TIdx patchWidth, const bool targetAsLabels) {
		const String f = "matricesToUniformPatches";
		if (~mats == 0 || ~targets == 0)
			throw Error(ecParameter, f, "Matrices or targets empty");
		Matrix<TMat<T, TIdx>, TIdx> &matsCast = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(mats);
		Matrix<TMat<T, TIdx>, TIdx> &targetsCast = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(targets);	
		if (mats.rows.count() != targets.rows.count())
			throw Error(ecSizeMismatch, f, SS("Number of matrices is not equal to number of targets: " << mats.rows.count() << " != " << targets.rows.count()));

		coords.clear();
		patches.clear();
		patchTargets.clear();
		auto row = matsCast.rows.begin();
		for (auto tar = targetsCast.rows->begin();tar != targetsCast.rows->end(); tar++, row++) {
			Matrix<TMat<T, TIdx>, TIdx> patchesTemp;
			Matrix<T, TIdx> patchTargetsTemp;
			Matrix<TIdx, TIdx> coordsTemp;
			genUniform2DCoords(*tar, n, patchHeight/2, patchWidth/2, coordsTemp);
			int c=0;
			for (auto cell = row->begin(); cell != row->end(); cell++) {
				Matrix<TMat<T, TIdx>, TIdx> patchesChannelTemp;
				if (cell->rows.count() != tar->rows.count() || cell->cols.count() != tar->cols.count())
					throw Error(ecSizeMismatch, f, SS("Matrix and target sizes differ: " << cell->printSize() << " != " << tar->printSize()));
				matrixToPatches(*cell, *tar, coordsTemp, patchesChannelTemp, patchTargetsTemp, patchHeight, patchWidth);
				patchesTemp.cols.add(patchesChannelTemp);
				//patchesTemp | patchesChannelTemp;
			}
			patches ^ patchesTemp;
			patchTargets ^ patchTargetsTemp;
			coords ^ coordsTemp;
		}
	}

	//Convert images and per-image targets to n random patches and targets
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToUniformPatches(const Matrix<TMat<T, TIdx>, TIdx> &mats, const Matrix<T, TIdx> &targets, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<T, TIdx> &patchTargets, Matrix<TIdx, TIdx> &coords, const TIdx n, const TIdx patchHeight, const TIdx patchWidth) {
		const String f = "matricesToUniformPatches";
		if (~mats == 0 || ~targets == 0)
			throw Error(ecParameter, f, "Matrices or targets empty");
		Matrix<TMat<T, TIdx>, TIdx> &matsCast = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(mats);
		Matrix<T, TIdx> &targetsCast = const_cast<Matrix<T, TIdx>&>(targets);	
		if (mats.rows.count() != targets.rows.count())
			throw Error(ecSizeMismatch, f, SS("Number of matrices is not equal to number of targets: " << mats.rows.count() << " != " << targets.rows.count()));
		TIdx height = mats.val(0,0).rows.count();
		TIdx width = mats.val(0,0).cols.count();
		coords.clear();
		patches.clear();
		patchTargets.clear();
		auto row = matsCast.rows.begin();
		for (auto tar = targetsCast.rows->begin();tar != targetsCast.rows->end(); tar++, row++) {
			Matrix<TMat<T, TIdx>, TIdx> patchesTemp;
			Matrix<T, TIdx> patchTargetsTemp(n, 1U, *tar);
			Matrix<TIdx, TIdx> coordsTemp;
			genUniform2DCoords(height, width, n, patchHeight/2, patchWidth/2, coordsTemp);		
			for (auto cell = row->begin(); cell != row->end(); cell++) {
				Matrix<TMat<T, TIdx>, TIdx> patchesChannelTemp;
				matrixToPatches(*cell, coordsTemp, patchesChannelTemp, patchHeight, patchWidth);
				patchesTemp | patchesChannelTemp;
			}
			patches ^ patchesTemp;
			patchTargets ^ patchTargetsTemp;
			coords ^ coordsTemp;
		}
	}

	//Convert images and per-pixel targets and coordinates to nRow, nCol random patches and targets (data augmentation based on coordsIn)
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToUniformPatches(const Matrix<TMat<T, TIdx>, TIdx> &mats, const Matrix<TMat<T, TIdx>> &targets, const Matrix<Matrix<TIdx, TIdx>, TIdx> &coordsIn, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<T, TIdx> &patchTargets, Matrix<Matrix<TIdx, TIdx>, TIdx> &coords, const TIdx n, const TIdx negativeX, const TIdx positiveX, const TIdx negativeY, const TIdx positiveY, const TIdx patchHeight, const TIdx patchWidth, const bool randomChannels, const bool targetAsLabels) {
		const String f = "matricesToUniformPatches";
		if (~mats == 0 || ~targets == 0)
			throw Error(ecParameter, f, "Matrices or targets empty");
		Matrix<TMat<T, TIdx>, TIdx> &matsCast = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(mats);
		Matrix<TMat<T, TIdx>, TIdx> &targetsCast = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(targets);	
		Matrix<TMat<TIdx, TIdx>, TIdx> &coordsInCast = const_cast<Matrix<TMat<TIdx, TIdx>, TIdx>&>(coordsIn);	
		if (mats.rows.count() != targets.rows.count())
			throw Error(ecSizeMismatch, f, SS("Number of matrices is not equal to number of targets: " << mats.rows.count() << " != " << targets.rows.count()));

		coords.clear();
		coords.resize(mats.rows.count(), 1U);
		patches.clear();
		patchTargets.clear();
		auto coordsInMat = coordsInCast.rows->begin();
		auto coordsMat = coords.rows->begin();
		auto matRow = matsCast.rows.begin();
		for (auto tar = targetsCast.rows->begin(); tar != targetsCast.rows->end(); tar++, matRow++, coordsInMat++, coordsMat++) {
			addUniform2DCoords(*coordsInMat, n, randomChannels?matsCast.cols.count():1U, negativeX, positiveX, negativeY, positiveY, *coordsMat);
			Matrix<TMat<T, TIdx>, TIdx> patchesTemp;
			Matrix<T, TIdx> patchTargetsTemp;
			if (randomChannels) {
				TIdx c=0;
				for (auto mat=matRow->begin(); mat!=matRow->end(); mat++, c+=2) {
					if (mat->rows.count() != tar->rows.count() || mat->cols.count() != tar->cols.count())
						throw Error(ecSizeMismatch, f, SS("Matrix and target sizes differ: " << mat->printSize() << " != " << tar->printSize()));
					Matrix<Matrix<T, TIdx>, TIdx> patchesChannelTemp;
					Matrix<TIdx, TIdx> crds = (*coordsMat)(0U, coordsMat->rows.count(), c, 2);				
					matrixToPatches(*mat, *tar, crds, patchesChannelTemp, patchTargetsTemp, patchHeight, patchWidth); 			
					patchesTemp | patchesChannelTemp;
				}
			} else {
				for (auto mat=matRow->begin(); mat!=matRow->end(); mat++) {
					if (mat->rows.count() != tar->rows.count() || mat->cols.count() != tar->cols.count())
						throw Error(ecSizeMismatch, f, SS("Matrix and target sizes differ: " << mat->printSize() << " != " << tar->printSize()));
					Matrix<Matrix<T, TIdx>, TIdx> patchesChannelTemp;		
					matrixToPatches(*mat, *tar, *coordsMat, patchesChannelTemp, patchTargetsTemp, patchHeight, patchWidth); 			
					patchesTemp | patchesChannelTemp;
				}
			}
			patches ^ patchesTemp;
			//Get targets before data augmentation (target could change not after translation augmentation)
			patchTargetsTemp.clear();
			patchTargetsTemp.resize(coordsInMat->rows.count() * n, 1);		
			for (TIdx c=0;c<coordsInMat->rows.count();c++) {
				TIdx y = coordsInMat->val(c, 0);
				TIdx x = coordsInMat->val(c, 1);
				for (TIdx i=0;i<n;i++)
					patchTargetsTemp.val(c*n+i, 0) = tar->val(y, x);
			}
			patchTargets ^ patchTargetsTemp;
		}
	}

	//Convert images and per-image targets and coordinates to nRow, nCol random patches and targets (data augmentation based on coordsIn)
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToUniformPatches(const Matrix<TMat<T, TIdx>, TIdx> &mats, const Matrix<T, TIdx> &targets, const Matrix<Matrix<TIdx, TIdx>, TIdx> &coordsIn, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<T, TIdx> &patchTargets, Matrix<Matrix<TIdx, TIdx>, TIdx> &coords, const TIdx n, const TIdx negativeY, const TIdx positiveY, const TIdx negativeX, const TIdx positiveX, const TIdx patchHeight, const TIdx patchWidth, const bool randomChannels, const bool targetAsLabels) {
			const String f = "matricesToUniformPatches";
		if (~mats == 0 || ~targets == 0)
			throw Error(ecParameter, f, "Matrices or targets empty");
		Matrix<TMat<T, TIdx>, TIdx> &matsCast = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(mats);
		Matrix<T, TIdx> &targetsCast = const_cast<Matrix<T, TIdx>&>(targets);	
		Matrix<TMat<TIdx, TIdx>, TIdx> &coordsInCast = const_cast<Matrix<TMat<TIdx, TIdx>, TIdx>&>(coordsIn);	
		if (mats.rows.count() != targets.rows.count())
			throw Error(ecSizeMismatch, f, SS("Number of matrices is not equal to number of targets: " << mats.rows.count() << " != " << targets.rows.count()));

		coords.clear();
		coords.resize(mats.rows.count(), 1U);
		patches.clear();
		patchTargets.clear();
		auto coordsInMat = coordsInCast.rows->begin();
		auto coordsMat = coords.rows->begin();
		auto matRow = matsCast.rows.begin();
		for (auto tar = targetsCast.rows->begin(); tar != targetsCast.rows->end(); tar++, matRow++, coordsInMat++, coordsMat++) {
			addUniform2DCoords(*coordsInMat, n, randomChannels?matsCast.cols.count():1U, negativeX, positiveX, negativeY, positiveY, *coordsMat);
			Matrix<TMat<T, TIdx>, TIdx> patchesTemp;
			Matrix<T, TIdx> patchTargetsTemp(coordsMat->rows.count(), 1U, *tar);
			if (randomChannels) {
				TIdx c=0;
				for (auto mat=matRow->begin(); mat!=matRow->end(); mat++, c+=2) {
					Matrix<Matrix<T, TIdx>, TIdx> patchesChannelTemp;
					Matrix<TIdx, TIdx> crds = (*coordsMat)(0U, coordsMat->rows.count(), c, 2);
					matrixToPatches(*mat, crds, patchesChannelTemp, patchHeight, patchWidth); 
					patchesTemp | patchesChannelTemp;
				}
			} else {
				for (auto mat=matRow->begin(); mat!=matRow->end(); mat++) {
					Matrix<Matrix<T, TIdx>, TIdx> patchesChannelTemp;		
					matrixToPatches(*mat, *coordsMat, patchesChannelTemp, patchHeight, patchWidth); 
					patchesTemp | patchesChannelTemp;
				}
			}
			patches ^ patchesTemp;
			patchTargets ^ patchTargetsTemp;
		}
	}

	//Convert images and per-coordinate targets and coordinates to nRow, nCol random patches and targets (data augmentation based on coordsIn)
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToUniformPatches(const Matrix<TMat<T, TIdx>, TIdx> &mats, const Matrix<Matrix<TIdx, TIdx>, TIdx> &coordsIn, const Matrix<Matrix<T, TIdx>, TIdx> &coordsTargets, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<T, TIdx> &patchTargets, Matrix<Matrix<TIdx, TIdx>, TIdx> &coords, const TIdx n, const TIdx negativeY, const TIdx positiveY, const TIdx negativeX, const TIdx positiveX, const TIdx patchHeight, const TIdx patchWidth, const bool randomChannels, const bool targetAsLabels) {
		const String f = "matricesToUniformPatches";
		if (~mats == 0 || ~coordsTargets == 0)
			throw Error(ecParameter, f, "Matrices or targets empty");
		Matrix<TMat<T, TIdx>, TIdx> &matsCast = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(mats);
		Matrix<Matrix<T, TIdx>, TIdx> &targetsCast = const_cast<Matrix<Matrix<T, TIdx>, TIdx>&>(coordsTargets);	
		Matrix<TMat<TIdx, TIdx>, TIdx> &coordsInCast = const_cast<Matrix<TMat<TIdx, TIdx>, TIdx>&>(coordsIn);	
		if (mats.rows.count() != coordsTargets.rows.count())
			throw Error(ecSizeMismatch, f, SS("Number of matrices is not equal to number of targets: " << mats.rows.count() << " != " << coordsTargets.rows.count()));

		coords.clear();
		coords.resize(mats.rows.count(), 1U);
		patches.clear();
		patchTargets.clear();
		auto coordsInMat = coordsInCast.rows->begin();
		auto coordsMat = coords.rows->begin();
		auto matRow = matsCast.rows.begin();
		for (auto tar = targetsCast.rows->begin(); tar != targetsCast.rows->end(); tar++, matRow++, coordsInMat++, coordsMat++) {
			addUniform2DCoords(*coordsInMat, n, randomChannels?matsCast.cols.count():1U, negativeX, positiveX, negativeY, positiveY, *coordsMat);
			Matrix<TMat<T, TIdx>, TIdx> patchesTemp;
			if (randomChannels) {
				TIdx c=0;
				for (auto mat=matRow->begin(); mat!=matRow->end(); mat++, c+=2) {
					Matrix<Matrix<T, TIdx>, TIdx> patchesChannelTemp;
					Matrix<TIdx, TIdx> crds = (*coordsMat)(0U, coordsMat->rows.count(), c, 2);
					matrixToPatches(*mat, crds, patchesChannelTemp, patchHeight, patchWidth); 
					patchesTemp | patchesChannelTemp;
				}
			} else {
				for (auto mat=matRow->begin(); mat!=matRow->end(); mat++) {
					Matrix<Matrix<T, TIdx>, TIdx> patchesChannelTemp;		
					matrixToPatches(*mat, *coordsMat, patchesChannelTemp, patchHeight, patchWidth); 
					patchesTemp | patchesChannelTemp;
				}
			}

			Matrix<T, TIdx> patchTargetsTemp;
			patchTargetsTemp.resize(coordsInMat->rows.count() * n, 1);		
			for (TIdx c=0;c<coordsInMat->rows.count();c++) {
				for (TIdx i=0;i<n;i++)
					patchTargetsTemp.val(c*n+i, 0) = tar->val(c, 0U);
			}

			patches ^ patchesTemp;
			patchTargets ^ patchTargetsTemp;
		}
	}

	//Convert images and per-pixel targets to dense patches and targets
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToPatches(const Matrix<TMat<T, TIdx>, TIdx> &mats, const Matrix<TMat<T, TIdx>> &targets, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<T, TIdx> &patchTargets, Matrix<TIdx, TIdx> &coords, const TIdx patchHeight, const TIdx patchWidth) {
		const String f = "matricesToPatches";
		if (~mats == 0 || ~targets == 0)
			throw Error(ecParameter, f, "Matrices or targets empty");
		Matrix<TMat<T, TIdx>, TIdx> &matsCast = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(mats);
		Matrix<TMat<T, TIdx>, TIdx> &targetsCast = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(targets);	
		if (mats.rows.count() != targets.rows.count())
			throw Error(ecSizeMismatch, f, SS("Number of matrices is not equal to number of targets: " << mats.rows.count() << " != " << targets.rows.count()));
		TIdx h = mats.val(0,0).rows.count();
		TIdx w = mats.val(0,0).cols.count();

		coords.clear();
		patches.clear();
		patchTargets.clear();
		auto row = matsCast.rows.begin();
		for (auto tar = targetsCast.rows->begin();tar != targetsCast.rows->end(); tar++, row++) {
			Matrix<TIdx, TIdx> coordsTemp;
			Matrix<TMat<T, TIdx>, TIdx> patchesTemp;
			Matrix<T, TIdx> patchTargetsTemp;
			genDense2DCoords(h, w, patchHeight/2, patchWidth/2, coordsTemp);
			int c=0;
			for (auto cell = row->begin(); cell != row->end(); cell++) {
				Matrix<TMat<T, TIdx>, TIdx> patchesChannelTemp;
				if (cell->rows.count() != tar->rows.count() || cell->cols.count() != tar->cols.count())
					throw Error(ecSizeMismatch, f, SS("Matrix and target sizes differ: " << cell->printSize() << " != " << tar->printSize()));
				matrixToPatches(*cell, *tar, coordsTemp, patchesChannelTemp, patchTargetsTemp, patchHeight, patchWidth);
				patchesTemp | patchesChannelTemp;
			}
			patches ^ patchesTemp;
			patchTargets ^ patchTargetsTemp;
			coords ^ coordsTemp;
		}
	}

	//Convert images and per-image targets to dense patches and targets
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToPatches(const Matrix<TMat<T, TIdx>, TIdx> &mats, const Matrix<T, TIdx> &targets, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<T, TIdx> &patchTargets, Matrix<TIdx, TIdx> &coords, const TIdx patchHeight, const TIdx patchWidth) {
		const String f = "matricesToPatches";
		if (~mats == 0 || ~targets == 0)
			throw Error(ecParameter, f, "Matrices or targets empty");
		Matrix<TMat<T, TIdx>, TIdx> &matsCast = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(mats);
		Matrix<T, TIdx> &targetsCast = const_cast<Matrix<T, TIdx>&>(targets);	
		if (mats.rows.count() != targets.rows.count())
			throw Error(ecSizeMismatch, f, SS("Number of matrices is not equal to number of targets: " << mats.rows.count() << " != " << targets.rows.count()));
		TIdx height = mats.val(0,0).rows.count();
		TIdx width = mats.val(0,0).cols.count();
		coords.clear();
		patches.clear();
		patchTargets.clear();
		auto row = matsCast.rows.begin();
		for (auto tar = targetsCast.rows->begin();tar != targetsCast.rows->end(); tar++, row++) {
			Matrix<TIdx, TIdx> coordsTemp;
			genDense2DCoords(height, width, patchHeight/2, patchWidth/2, coordsTemp);
			Matrix<TMat<T, TIdx>, TIdx> patchesTemp;
			Matrix<T, TIdx> patchTargetsTemp(coordsTemp.rows.count(), 1U, *tar);
			for (auto cell = row->begin(); cell != row->end(); cell++) {
				Matrix<TMat<T, TIdx>, TIdx> patchesChannelTemp;
				matrixToPatches(*cell, coordsTemp, patchesChannelTemp, patchHeight, patchWidth);
				patchesTemp | patchesChannelTemp;
			}
			patches ^ patchesTemp;
			patchTargets ^ patchTargetsTemp;
			coords ^ coordsTemp;
		}
	}

	//Convert images to uniform patches
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToUniformPatches(const Matrix<TMat<T, TIdx>, TIdx> &mats, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<TIdx, TIdx> &coords, const TIdx n, const TIdx patchHeight, const TIdx patchWidth) {
		const String f = "matricesToPatches";
		if (~mats == 0)
			throw Error(ecParameter, f, "Matrices are empty");
		Matrix<TMat<T, TIdx>, TIdx> &matsCast = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(mats);
		TIdx height = mats.val(0,0).rows.count();
		TIdx width = mats.val(0,0).cols.count();
		coords.clear();
		patches.clear();
		for (auto row = matsCast.rows.begin(); row != matsCast.rows.end(); row++) {
			Matrix<TIdx, TIdx> coordsTemp;
			genUniform2DCoords(height, width, n, patchHeight/2, patchWidth/2, coordsTemp);
			Matrix<TMat<T, TIdx>, TIdx> patchesTemp;
			for (auto cell = row->begin(); cell != row->end(); cell++) {
				Matrix<TMat<T, TIdx>, TIdx> patchesChannelTemp;
				matrixToPatches(*cell, coordsTemp, patchesChannelTemp, patchHeight, patchWidth);
				patchesTemp | patchesChannelTemp;
			}
			patches ^ patchesTemp;
			coords ^ coordsTemp;
		}
	}

	//Convert images to dense patches
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToPatches(const Matrix<TMat<T, TIdx>, TIdx> &mats, Matrix<TMat<T, TIdx>, TIdx> &patches, Matrix<TIdx, TIdx> &coords, const TIdx patchHeight, const TIdx patchWidth) {
		/*const String f = "matricesToPatches";
		if (~mats == 0)
			throw Error(ecParameter, f, "Matrices are empty");
		Matrix<TMat<T, TIdx>, TIdx> &matsCast = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(mats);
		TIdx height = mats.val(0,0).rows.count();
		TIdx width = mats.val(0,0).cols.count();
		coords.clear();
		patches.clear();
		for (auto row = matsCast.rows.begin(); row != matsCast.rows.end(); row++) {
			Matrix<TIdx, TIdx> coordsTemp;
			genDense2DCoords(height, width, patchHeight/2, patchWidth/2, coordsTemp);
			Matrix<TMat<T, TIdx>, TIdx> patchesTemp;
			for (auto cell = row->begin(); cell != row->end(); cell++) {
				Matrix<TMat<T, TIdx>, TIdx> patchesChannelTemp;
				matrixToPatches(*cell, coordsTemp, patchesChannelTemp, patchHeight, patchWidth);
				patchesTemp | patchesChannelTemp;
			}
			patches ^ patchesTemp;
			coords ^ coordsTemp;
		}*/
		//Faster
		const String f = "matricesToPatches";
		if (~mats == 0)
			throw Error(ecParameter, f, "Matrices are empty");
		Matrix<TMat<T, TIdx>, TIdx> &matsCast = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(mats);
		TIdx height = mats.val(0,0).rows.count();
		TIdx width = mats.val(0,0).cols.count();
		coords.clear();
		patches.resize(areaOf(height, width, patchHeight, patchWidth) * mats.rows.count(), mats.cols.count());
		//coords.resize(areaOf(height, width, patchHeight, patchWidth), 1);
		TIdx borderx = patchWidth / 2;
		TIdx bordery = patchHeight / 2;
		for (auto row = matsCast.rows.begin(); row != matsCast.rows.end(); row++) {
			TIdx chan=0;
			for (auto cell = row->begin(); cell != row->end(); cell++, chan++) {
				TIdx i = 0;
				for (TIdx y = bordery; y < height-bordery;y++) {
					for (TIdx x = borderx; x < width-borderx;x++) {
						cell->slice(patches.val(i, chan), y - bordery, y - bordery + patchHeight, x - borderx, x - borderx + patchWidth);
						patches.val(i, chan).breakUp();
						i++;
					}
				}
			}
		}

		for (auto i=0;i<mats.rows.count();i++) {
			Matrix<TIdx, TIdx> coordsTemp;
			genDense2DCoords(height, width, bordery, borderx, coordsTemp);
			coords ^ coordsTemp;
		}
	}

	//******************************//
	//** Single image to samples **//
	//******************************//

	//Convert an image with and per-pixel target to samples and targets
	template <typename T, typename TIdx>
	void matrixToSamples(const Matrix<T, TIdx> &mat, const Matrix<T, TIdx> &matTargets, const Matrix<TIdx, TIdx> &coords, Matrix<T, TIdx> &samples, Matrix<T, TIdx> &sampleTargets, const TIdx windowHeight, const TIdx windowWidth) {
		Matrix<TIdx, TIdx> coordsCast = const_cast<Matrix<TIdx, TIdx>&>(coords);

		TIdx windowWidth2 = windowWidth / 2;
		TIdx windowHeight2 = windowHeight / 2;
		if (~sampleTargets == 0)
			sampleTargets.resize(coords.rows.count(), 1);
		else if (sampleTargets.rows.count() != coords.rows.count() || sampleTargets.cols.count() != 1)
			throw Error(ecSizeMismatch, "matrixToSample", SS("sampleTargets matrix is initialized to the wrong size: (" << sampleTargets.rows.count() << " x " << sampleTargets.cols.count() << ") != (" << coords.rows.count() << " x 1)"));

		if (~samples == 0)
			samples.resize(coords.rows.count(), windowWidth * windowHeight);
		else if (samples.rows.count() != coords.rows.count() || samples.cols.count() != windowWidth * windowHeight)
			throw Error(ecSizeMismatch, "matrixToSample", SS("samples matrix is initialized to the wrong size: (" << samples.rows.count() << " x " << samples.cols.count() << ") != (" << coords.rows.count() << " x " << windowWidth * windowHeight << ")"));

		if (windowWidth == 1 && windowHeight == 1) {
			auto sample = samples.rows->begin();
			auto sampleTarget = sampleTargets.rows->begin();
			for (auto coordsRow = coordsCast.rows.begin(); coordsRow != coordsCast.rows.end(); coordsRow++, sample++, sampleTarget++) {
				 TIdx y = coordsRow->val(0);
				 TIdx x = coordsRow->val(1);
				 *sampleTarget = matTargets.val(y, x);
				 *sample = mat.val(y, x);
			}
		} else {
			auto sampleRow = samples.rows.begin();
			auto sampleTarget = sampleTargets.rows->begin();
			for (auto coordsRow = coordsCast.rows.begin(); coordsRow != coordsCast.rows.end(); coordsRow++, sampleRow++, sampleTarget++) {
				TIdx y = coordsRow->val(0);		 
				TIdx x = coordsRow->val(1);
				auto sample = sampleRow->begin();
				*sampleTarget = matTargets.val(y, x);
				for (TIdx wy = y - windowHeight2; wy < y + windowHeight; wy++) {
					for (TIdx wx = x - windowWidth2; wx < x + windowWidth; wx++) {
						*sample = mat.val(wy, wx);
						sample++;
					}
				}
			}
		}
	}

	//Convert an image with and per-coordinate target to samples and targets
	//This works but is not needed
	/*template <typename T, typename TIdx>
	void matrixToSamples(const Matrix<T, TIdx> &mat, const Matrix<TIdx, TIdx> &coords, const Matrix<T, TIdx> &coordTargets, Matrix<T, TIdx> &samples, const TIdx windowHeight, const TIdx windowWidth) {
		Matrix<TIdx, TIdx> coordsCast = const_cast<Matrix<TIdx, TIdx>&>(coords);
		Matrix<TIdx, TIdx> coordTargetsCast = const_cast<Matrix<TI, TIdx>&>(coordTargets);

		TIdx windowWidth2 = windowWidth / 2;
		TIdx windowHeight2 = windowHeight / 2;
		if (~sampleTargets == 0)
			sampleTargets.resize(coords.rows.count(), 1);
		else if (sampleTargets.rows.count() != coords.rows.count() || sampleTargets.cols.count() != 1)
			throw Error(ecSizeMismatch, "matrixToSample", SS("sampleTargets matrix is initialized to the wrong size: (" << sampleTargets.rows.count() << " x " << sampleTargets.cols.count() << ") != (" << coords.rows.count() << " x 1)"));

		if (~samples == 0)
			samples.resize(coords.rows.count(), windowWidth * windowHeight);
		else if (samples.rows.count() != coords.rows.count() || samples.cols.count() != windowWidth * windowHeight)
			throw Error(ecSizeMismatch, "matrixToSample", SS("samples matrix is initialized to the wrong size: (" << samples.rows.count() << " x " << samples.cols.count() << ") != (" << coords.rows.count() << " x " << windowWidth * windowHeight << ")"));

		if (windowWidth == 1 && windowHeight == 1) {
			auto sample = samples.rows->begin();
			auto sampleTarget = sampleTargets.rows->begin();
			auto coordTarget = coordTargetsCast.rows->begin();
			for (auto coordsRow = coordsCast.rows.begin(); coordsRow != coordsCast.rows.end(); coordsRow++, sample++, sampleTarget++, coordTarget++) {
				 TIdx y = coordsRow->val(0);
				 TIdx x = coordsRow->val(1);
				 *sampleTarget = *coordTarget;
				 *sample = mat.val(y, x);
			}
		} else {
			auto sampleRow = samples.rows.begin();
			auto sampleTarget = sampleTargets.rows->begin();
			auto coordTarget = coordTargetsCast.rows->begin();
			for (auto coordsRow = coordsCast.rows.begin(); coordsRow != coordsCast.rows.end(); coordsRow++, sampleRow++, sampleTarget++, coordTarget++) {
				TIdx y = coordsRow->val(0);		 
				TIdx x = coordsRow->val(1);
				auto sample = sampleRow->begin();
				*sampleTarget = *coordTarget;
				for (TIdx wy = y - windowHeight2; wy < y + windowHeight; wy++) {
					for (TIdx wx = x - windowWidth2; wx < x + windowWidth; wx++) {
						*sample = mat.val(wy, wx);
						sample++;
					}
				}
			}
		}
	}*/

	//Convert an image to samples
	template <typename T, typename TIdx>
	void matrixToSamples(const Matrix<T, TIdx> &mat, const Matrix<TIdx, TIdx> &coords, Matrix<T, TIdx> &samples, const TIdx windowHeight,const  TIdx windowWidth) {
		Matrix<TIdx, TIdx> coordsCast = const_cast<Matrix<TIdx, TIdx>&>(coords);

		TIdx windowWidth2 = windowWidth / 2;
		TIdx windowHeight2 = windowHeight / 2;

		if (~samples == 0)
			samples.resize(coords.rows.count(), windowWidth * windowHeight);
		else if (samples.rows.count() != coords.rows.count() || samples.cols.count() != windowWidth * windowHeight)
			throw Error(ecSizeMismatch, "matrixToSample", SS("samples matrix is initialized to the wrong size: (" << samples.rows.count() << " x " << samples.cols.count() << ") != (" << coords.rows.count() << " x " << windowWidth * windowHeight << ")"));

		if (windowWidth == 1 && windowHeight == 1) {
			auto sample = samples.rows->begin();
			for (auto coordsRow = coordsCast.rows.begin(); coordsRow != coordsCast.rows.end(); coordsRow++, sample++) {
				TIdx y = coordsRow->val(0);		 
				TIdx x = coordsRow->val(1);
				 *sample = mat.val(y, x);
			}
		} else {
			auto sampleRow = samples.rows.begin();
			for (auto coordsRow = coordsCast.rows.begin(); coordsRow != coordsCast.rows.end(); coordsRow++, sampleRow++) {
				TIdx y = coordsRow->val(0);		 
				TIdx x = coordsRow->val(1);
				auto sample = sampleRow->begin();
				for (TIdx wy = y - windowHeight2; wy < y - windowHeight2 + windowHeight; wy++) {
					for (TIdx wx = x - windowWidth2; wx < x - windowWidth2 + windowWidth; wx++) {
						*sample = mat.val(wy, wx);
						sample++;
					}
				}
			}
		}
	}

	//Convert samples to a images
	template <typename T, typename TIdx>
	void samplesToMatrix(const Matrix<T, TIdx> &samples, const Matrix<TIdx, TIdx> &coords, Matrix<T, TIdx> &mat, const TIdx width, const TIdx height) {
		Matrix<TIdx, TIdx> coordsCast = const_cast<Matrix<TIdx, TIdx>&>(coords);
		Matrix<T, TIdx> samplesCast = const_cast<Matrix<T, TIdx>&>(samples);
	
		mat.resize(height, width);
		auto sample = samplesCast.rows->begin();
		for (auto coordsRow = coordsCast.rows.begin(); coordsRow != coordsCast.rows.end(); coordsRow++, sample++) {		 
			 TIdx y = coordsRow->val(0);		 
			 TIdx x = coordsRow->val(1);
			 mat.val(y, x) = *sample;
		}
	}

	//*********************************//
	//** Multiple images to samples **//
	//********************************//

	//Convert images and per-pixel targets to n random samples and targets
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToUniformSamples(const Matrix<TMat<T, TIdx>, TIdx> &featuresIn, const Matrix<TMat<T, TIdx>> &targetsIn, Matrix<T, TIdx> &featuresOut, Matrix<T, TIdx> &targetsOut, Matrix<TIdx, TIdx> &coords, const TIdx n, const TIdx windowHeight, const TIdx windowWidth, const bool targetAsLabels) {
		Matrix<TMat<T, TIdx>, TIdx> &featuresIn2 = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(featuresIn);
		Matrix<TMat<T, TIdx>, TIdx> &targetsIn2 = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(targetsIn);	
		coords.clear();
		featuresOut.clear();
		targetsOut.clear();
		UInt32 dbg=3;
		auto row = featuresIn2.rows.begin();
		for (auto tar = targetsIn2.rows->begin();tar != targetsIn2.rows->end(); tar++, row++) {
			Matrix<T, TIdx> featuresTemp(n, featuresIn.cols.count() * windowWidth * windowHeight);
			Matrix<T, TIdx> targetsTemp(n, 1);
			Matrix<TIdx, TIdx> coordsTemp;		
			genUniform2DCoords(*tar, n, windowHeight/2, windowWidth/2, coordsTemp);
			TIdx col = 0;
			for (auto cell = row->begin(); cell != row->end(); cell++, col+=(windowWidth * windowHeight)) {
				if (!cell->isInitialized())
					throw Error(ecIncompatible, "matricesToUniformSamples", "Source Matrix is empty");
				if (!cell->isInitialized())
					throw Error(ecIncompatible, "matricesToUniformSamples", "Target Matrix is empty");
				auto featuresSlice = featuresTemp(0, featuresTemp.rows.count(), col, windowWidth * windowHeight);
				matrixToSamples(*cell, *tar, coordsTemp, featuresSlice, targetsTemp, windowHeight, windowWidth);
			}
			featuresOut ^ featuresTemp;
			targetsOut ^ targetsTemp;
			coords ^ coordsTemp;
		}
	}

	//Convert images and per-image targets to n random samples and targets per image
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToUniformSamples(const Matrix<TMat<T, TIdx>, TIdx> &featuresIn, const Matrix<T, TIdx> &targetsIn, Matrix<T, TIdx> &featuresOut, Matrix<T, TIdx> &targetsOut, Matrix<TIdx, TIdx> &coords, const TIdx n, const TIdx windowHeight, const TIdx windowWidth) {
		Matrix<TMat<T, TIdx>, TIdx> &featuresIn2 = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(featuresIn);
		Matrix<T, TIdx> &targetsIn2 = const_cast<Matrix<T, TIdx>&>(targetsIn);	
		coords.clear();
		featuresOut.clear();
		targetsOut.clear();
		auto row = featuresIn2.rows.begin();
		for (auto tar = targetsIn2.rows->begin();tar != targetsIn2.rows->end(); tar++, row++) {
			TIdx rc = row->begin()->rows.count();
			TIdx cc = row->begin()->cols.count();
			Matrix<T, TIdx> featuresTemp(n, featuresIn.cols.count() * windowWidth * windowHeight);
			Matrix<T, TIdx> targetsTemp(n, 1, *tar);
			Matrix<TIdx, TIdx> coordsTemp;
			genUniform2DCoords(rc, cc, n, windowHeight/2, windowWidth/2, coordsTemp);
			TIdx col = 0;
			for (auto cell = row->begin(); cell != row->end(); cell++, col+=(windowWidth * windowHeight)) {
				auto featuresSlice = featuresTemp(0, featuresTemp.rows.count(), col, windowWidth * windowHeight);
				matrixToSamples(*cell, coordsTemp, featuresSlice, windowHeight, windowWidth);
			}
			featuresOut ^ featuresTemp;
			targetsOut ^ targetsTemp;
			coords ^ coordsTemp;
		}
	}

	//Convert images and per-pixel targets to dense samples and targets
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToSamples(const Matrix<TMat<T, TIdx>, TIdx> &featuresIn, const Matrix<TMat<T, TIdx>> &targetsIn, Matrix<T, TIdx> &featuresOut, Matrix<T, TIdx> &targetsOut, Matrix<TIdx, TIdx> &coords, const TIdx windowHeight, const TIdx windowWidth) {
		Matrix<TMat<T, TIdx>, TIdx> &featuresIn2 = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(featuresIn);
		Matrix<TMat<T, TIdx>, TIdx> &targetsIn2 = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(targetsIn);
		coords.clear();
		featuresOut.clear();
		targetsOut.clear();
		auto row = featuresIn2.rows.begin();
		for (auto tar = targetsIn2.rows->begin();tar != targetsIn2.rows->end(); tar++, row++) {
			TIdx borderWidth = windowWidth/2;
			TIdx borderHeight = windowHeight/2;
			TIdx n = (tar->rows.count() - (borderHeight*2)) * (tar->cols.count() - (borderWidth*2));
			Matrix<T, TIdx> featuresTemp(n, featuresIn.cols.count() * windowWidth * windowHeight);
			Matrix<T, TIdx> targetsTemp(n, 1);
			Matrix<TIdx, TIdx> coordsTemp;
			genDense2DCoords(tar->rows.count(), tar->cols.count(), borderHeight, borderWidth, coordsTemp);
			TIdx col = 0;
			for (auto cell = row->begin(); cell != row->end(); cell++, col+=(windowWidth*windowHeight)) {
				auto featuresSlice = featuresTemp(0, featuresTemp.rows.count(), col, windowWidth * windowHeight);
				matrixToSamples(*cell, *tar, coordsTemp, featuresSlice, targetsTemp, windowHeight, windowWidth);
			}
			featuresOut ^ featuresTemp;
			targetsOut ^ targetsTemp;
			coords ^ coordsTemp;
		}
	}

	//Convert images and per-image targets to dense samples and targets
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToSamples(const Matrix<TMat<T, TIdx>, TIdx> &featuresIn, const Matrix<T, TIdx> &targetsIn, Matrix<T, TIdx> &featuresOut, Matrix<T, TIdx> &targetsOut, Matrix<TIdx, TIdx> &coords, const TIdx windowHeight, const TIdx windowWidth) {
		Matrix<TMat<T, TIdx>, TIdx> &featuresIn2 = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(featuresIn);
		Matrix<TMat<T, TIdx>, TIdx> &targetsIn2 = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(targetsIn);
		coords.clear();
		featuresOut.clear();
		targetsOut.clear();
		auto row = featuresIn2.rows.begin();
		for (auto tar = targetsIn2.rows->begin();tar != targetsIn2.rows->end(); tar++, row++) {
			TIdx rc = row->begin()->rows.count();
			TIdx cc = row->begin()->cols.count();
			TIdx borderWidth = windowWidth/2;
			TIdx borderHeight = windowHeight/2;
			TIdx n = (rc - (borderHeight*2)) * (cc - (borderWidth*2));
			Matrix<T, TIdx> featuresTemp(n, featuresIn.cols.count() * windowWidth * windowHeight);
			Matrix<T, TIdx> targetsTemp(n, 1);
			Matrix<TIdx, TIdx> coordsTemp;
			//genDense2DCoords(tar->rows.count(), tar->cols.count(), borderHeight, borderWidth, coordsTemp);
			genDense2DCoords(rc, cc, borderHeight, borderWidth, coordsTemp);
			TIdx col = 0;
			for (auto cell = row->begin(); cell != row->end(); cell++, col+=(windowWidth*windowHeight)) {
				auto featuresSlice = featuresTemp(0, featuresTemp.rows.count(), col, windowWidth * windowHeight);
				matrixToSamples(*cell, coordsTemp, *tar, featuresSlice, targetsTemp, windowHeight, windowWidth);
			}
			featuresOut ^ featuresTemp;
			targetsOut ^ targetsTemp;
			coords ^ coordsTemp;
		}
	}

	//Convert images to dense samples
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void matricesToSamples(const Matrix<TMat<T, TIdx>, TIdx> &featuresIn, Matrix<T, TIdx> &featuresOut, Matrix<TIdx, TIdx> &coords, const TIdx windowHeight, const TIdx windowWidth) {
		Matrix<TMat<T, TIdx>, TIdx> &featuresIn2 = const_cast<Matrix<TMat<T, TIdx>, TIdx>&>(featuresIn);
		coords.clear();
		featuresOut.clear();	
		for (auto row = featuresIn2.rows.begin();row != featuresIn2.rows.end(); row++) {
			TIdx rc = row->begin()->rows.count();
			TIdx cc = row->begin()->cols.count();
			TIdx borderWidth = windowWidth/2;
			TIdx borderHeight = windowHeight/2;
			TIdx n = (rc - (borderHeight*2)) * (cc - (borderWidth*2));
			Matrix<T, TIdx> featuresTemp(n, featuresIn.cols.count() * windowWidth * windowHeight);
			Matrix<TIdx, TIdx> coordsTemp;
			//genDense2DCoords(tar->rows.count(), tar->cols.count(), borderHeight, borderWidth, coordsTemp);
			genDense2DCoords(rc, cc, borderHeight, borderWidth, coordsTemp);
			TIdx col = 0;
			for (auto cell = row->begin(); cell != row->end(); cell++, col+=(windowWidth*windowHeight)) {
				auto featuresSlice = featuresTemp(0, featuresTemp.rows.count(), col, windowWidth * windowHeight);
				matrixToSamples(*cell, coordsTemp, featuresSlice, windowHeight, windowWidth);
			}
			featuresOut ^ featuresTemp;
			coords ^ coordsTemp;
		}
	}

	//Convert images to dense samples
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void samplesToMatrices(const Matrix<T, TIdx> &samples, const Matrix<TIdx, TIdx> &coords, const TIdx height, const TIdx width, const TIdx n, Matrix<TMat<T, TIdx>, TIdx> &mats) {	
		Matrix<TIdx, TIdx> & coords2 = const_cast<Matrix<TIdx, TIdx>&>(coords);
		Matrix<T, TIdx> & samples2 = const_cast<Matrix<T, TIdx>&>(samples);
	
		if (coords.rows.count() != samples.rows.count())
			throw Error(ecParameter, "samplesToMatrices", "The number of provided samples is not equal to the number of coordinates");
		if (coords.rows.count() % n != 0)
			throw Error(ecParameter, "samplesToMatrices", SS("The number of provided coords is not a multiple of count " << coords.rows.count() << " % " << n << " != 0"));
		mats.resize(n, samples.cols.count());
		auto sampleRow = samples2.rows.begin();
		auto coordsRow = coords2.rows.begin();
		for (auto featureRow = mats.rows.begin(); featureRow != mats.rows.end(); featureRow++) {
			for (auto matIt = featureRow->begin(); matIt != featureRow->end(); matIt++) {
				matIt->resize(height, width);
			}
			for (TIdx i=0; i<coords.rows.count() / n; i++) {
				auto sample = sampleRow->begin();
				for (auto matIt = featureRow->begin(); matIt != featureRow->end(); matIt++, sample++) {
					matIt->val(coordsRow->val(0), coordsRow->val(1)) = *sample;
				}
				sampleRow++;
				coordsRow++;
			}
		}	
	}

	//Convert images to dense samples
	template <typename T, typename TIdx, template <typename, typename> class TMat>
	void samplesToMatrices(const Matrix<T, TIdx> &samples, const Matrix<Matrix<TIdx, TIdx>, TIdx> &coords, const TIdx height, const TIdx width, Matrix<TMat<T, TIdx>, TIdx> &mats) {	
		Matrix<Matrix<TIdx, TIdx>, TIdx> & coordsCast = const_cast<Matrix<Matrix<TIdx, TIdx>, TIdx>&>(coords);
		Matrix<T, TIdx> & samplesCast = const_cast<Matrix<T, TIdx>&>(samples);

		mats.resize(coords.rows.count(), samples.cols.count());	
		auto sampleRow = samplesCast.rows.begin();
		auto coordsIt = coordsCast.rows->begin();
		for (auto matsRow = mats.rows.begin(); matsRow != mats.rows.end(); matsRow++, coordsIt++) {
			for (auto matIt = matsRow->begin(); matIt != matsRow->end(); matIt++) {
				matIt->resize(height, width);
			}
			for (auto coordsRow = coordsIt->rows.begin(); coordsRow != coordsIt->rows.end(); coordsRow++) {
				auto sample = sampleRow->begin();
				for (auto matIt = matsRow->begin(); matIt != matsRow->end(); matIt++, sample++) {
					matIt->val(coordsRow->val(0), coordsRow->val(1)) = *sample;
				}
				sampleRow++;
			}
		}
	}

	//***************************//
	//** Coordinate generation **//
	//***************************//

	//Generate n coordinates balanced on per-pixel targets
	template <typename T, typename TIdx>
	void genUniform2DCoords(const Matrix<T, TIdx> &targets, const TIdx n, const TIdx borderHeight, const TIdx borderWidth, Matrix<TIdx, TIdx> &coords, const bool targetAsLabels) {
		if (~targets == 0)
			throw Error(ecRangeError, "genUniform2DCoords", "Targets are empty");
		coords.clear();
		Matrix<T, TIdx> ut;
		if (targetAsLabels)
			ut = uniqueValues(targets, T(0)); //Start from 1?
		else
			ut = uniqueValues(targets);

		if (~ut == 0)
			throw Error(ecRangeError, "genUniform2DCoords", "No appropriate targets found");	
		if (n % ~ut != 0)
			throw Error(ecIncompatible, "genUniform2DCoords", SS("Cannot sample " << n << " samples from " << ~ut << " classes because, " << n << " is not divisable by " << ~ut));
		TIdx cnt = n / ~ut;

		for (auto t = ut.rows->begin(); t != ut.rows->end(); t++) {
			Matrix<TIdx, TIdx> coordinates = (targets == *t);
			if (borderHeight != 0 || borderWidth != 0)
				coordinates = coordinates((coordinates >= (Matrix<TIdx, TIdx>() | borderHeight | borderWidth)) && (coordinates <  (Matrix<TIdx, TIdx>() | targets.rows.count() - borderHeight | targets.cols.count() - borderWidth)));
			coordinates = coordinates(shuffle(Matrix<TIdx, TIdx>(TIdx(1),TIdx(1),TIdx(0)) || coordinates.rows.count()-1, 3));
			if (cnt < coordinates.rows.count())
				coords ^ coordinates(0, cnt, 0, 2);
			else
				throw Error(ecRangeError, "genUniform2DCoords", SS("Target " << *t << " only contains " << coordinates.rows.count() << " samples, which is smaller than the required " << cnt << " samples per class."));
		}
	}

	//Generate n coordinates
	template <typename T, typename TIdx>
	void genUniform2DCoords(const TIdx height, const TIdx width, const TIdx n, const TIdx borderHeight, const TIdx borderWidth, Matrix<T, TIdx> &coords) {
		 coords = (random(n, 1U, (Matrix<TIdx, TIdx>() | borderHeight) || (height-borderHeight-1)) | random(n, 1U, (Matrix<TIdx, TIdx>() | borderWidth) || (width-borderWidth-1)));
	}

	//Generate coordinates from an image
	template <typename T, typename TIdx>
	void genDense2DCoords(const TIdx height, const TIdx width, const TIdx borderHeight, const TIdx borderWidth, Matrix<T, TIdx> &coords) {
		coords.resize((height - (borderHeight*2)) * (width - (borderWidth*2)), 2);	
		auto c=coords.rows.begin();
		for (TIdx y = borderHeight; y<height-borderHeight; y++) {
			for (TIdx x = borderWidth; x<width-borderWidth; x++) {
				c->val(0) = y;
				c->val(1) = x;
				c++;
			}		
		}
	}

	//Generate additional random samples (used for data augmentation)
	//Source 2D coordinates are randomized in nRows, nCols additional coords. If nCols > 0 the source coord is copied for all nCols (only for the first row). The randomization starts for additional rows.
	template <typename TIdx>
	void addUniform2DCoords(const Matrix<TIdx, TIdx> &srcCoords, const TIdx nRows, const TIdx nCols, const TIdx negativeX, const TIdx positiveX, const TIdx negativeY, const TIdx positiveY, Matrix<TIdx, TIdx> &coords) {	
		Matrix<TIdx, TIdx> srcCoords2 = const_cast<Matrix<TIdx, TIdx>&>(srcCoords);
		coords.resize(nRows * srcCoords.rows.count(), 2 * nCols);
		for (TIdx x=0;x<nCols;x++) {
			coords[x*2] = random(nRows * srcCoords.rows.count(), 1U, 0U, negativeX + positiveX);
			coords[x*2+1] = random(nRows * srcCoords.rows.count(), 1U, 0U, negativeY + positiveX);
		}
		auto rdst = coords.rows.begin();
		for (auto r = srcCoords2.rows.begin(); r != srcCoords2.rows.end(); r++) {
			//Copy the first coord to the whole row
			for (TIdx x=0;x<nCols;x++) {
				rdst->val(x*2) = r->val(0);
				rdst->val(x*2+1) = r->val(1);
			}
			rdst++;
			//Generate additional rows (y=1 because the first row is a copy)
			for (TIdx y=1;y<nRows;y++,rdst++) {
				for (TIdx x=0;x<nCols;x++) {
					rdst->val(x*2)   = (r->val(0) + rdst->val(x*2)) - negativeX;
					rdst->val(x*2+1) = (r->val(1) + rdst->val(x*2+1)) - negativeY;
				}
			}
		}
	}

	template <typename TIdx>
	inline TIdx areaOf(TIdx height, TIdx width, TIdx patchHeight, TIdx patchWidth) {
		TIdx borderWidth = patchWidth / 2;
		TIdx borderHeight = patchHeight / 2;
		return (height - borderHeight*2) * (width - borderWidth*2);
	}

	//Flatten a matrix into a vector
	template <typename T, typename TIdx>
	void matrixToVector(const Matrix<T, TIdx> &in, Matrix<T, TIdx> &out, Order o) {
		Matrix<T, TIdx> & in2 = const_cast<Matrix<T, TIdx> &>(in);
		if (o == oRowMajor) {
			out.resize(1, ~in);
			copy(in2.rows->begin(), in2.rows->end(), out.vec().begin());
		} else if (o == oColMajor) {
			out.resize(~in, 1);
			copy(in2.cols->begin(), in2.cols->end(), out.vec().begin());
		}
	}
}