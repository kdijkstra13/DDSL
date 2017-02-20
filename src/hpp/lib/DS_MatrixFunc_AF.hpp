#pragma once
#include "h/lib/DS_MatrixFunc_AF.h"
#include "hpp/lang/DS_MatrixLang.hpp"

using namespace std;
using namespace DSLib;
using namespace DSTypes;
using namespace DSLang;
using namespace DSLib::ArrayFire;
using namespace DSFunc::ArrayFire;

namespace DSLib {
	namespace ArrayFire {
		template <typename T, typename TInt>
		Array<T, TInt>::Array(const Order order) {
			order_ = order;
		}

		template <typename T, typename TInt>
		Array<T, TInt>::Array() {
			order_ = oRowMajor;
		}

		template <typename T, typename TInt>
		Array<T, TInt>::Array(const Matrix<T, TInt> &mat) {
			this->mat(mat);
		}

		template <typename T, typename TInt>
		Array<T, TInt>::Array(const Matrix<T, TInt> &mat, const Order order) {
			this->mat(mat);
			convertOrder(order);
		}

		template <typename T, typename TInt>
		Array<T, TInt>::Array(af::array &arr, const Order order) {
			arr_ = arr;
			order_ = order;
		}

		template <typename T, typename TInt>
		Array<T, TInt> & Array<T, TInt>::operator=(const Matrix<T, TInt> &mat) {
			mat(mat);
			return *this;
		}

		template <typename T, typename TInt>
		Array<T, TInt> & Array<T, TInt>::operator=(af::array &arr) {
			arr_ = arr;
			return *this;
		}

		template <typename T, typename TInt>
		Array<T, TInt> & Array<T, TInt>::operator=(af::array &&arr) {
			arr_ = std::move(arr);
			return *this;
		}

		template <typename T, typename TInt>
		Matrix<T, TInt> Array<T, TInt>::mat() {
			Matrix<T, TInt> m;
			arrayToMatrix(arr_, m, order_);
			return m;
		}

		template <typename T, typename TInt>
		af::array & Array<T, TInt>::arr() {
			return arr_;
		}

		template <typename T, typename TInt>
		Array<T, TInt> & Array<T, TInt>::mat(const Matrix<T, TInt> &mat) {
			order_ = matrixToArray(mat, arr_);
			return *this;
		}
		
		template <typename T, typename TInt>
		Array<T, TInt> & Array<T, TInt>::arr(af::array arr, const Order order) {
			order_ = order;
			arr_ = arr;
			return *this;
		}

		template <typename T, typename TInt>
		af::dtype Array<T, TInt>::convert(af::dtype type) {
			af::dtype old = arr_.type();
			arr_ = arr_.as(type);
			return old;
		};

		template <typename T, typename TInt>
		DSTypes::Order Array<T, TInt>::convertOrder(DSTypes::Order o) {
			Order old = order_;
			if (o != order_) {
				arr_ = af::transpose(arr_);
				order_ = o;
			}
			return old;
		}

		//Implicit cast
		template <typename T, typename TInt>
		Array<T, TInt>::operator af::array() {
			return arr();
		}

		//Implicit cast
		template <typename T, typename TInt>
		Array<T, TInt>::operator Matrix<T, TInt>() {			
			return mat();
		}
		
		template <typename T, typename TInt>
		Matrix<T, TInt> Array<T, TInt>::operator()() {
			return mat();
		}
	}
}

namespace DSFunc {
	namespace ArrayFire {

		inline void setDevice(int device) {
			af::setDevice(device);
			af::info();
		}

		template <typename T, typename TInt>
		DSTypes::Order matrixToArray(const Matrix<T, TInt> &mat, af::array &dst) {
			Matrix<T, TInt> mat2 = mat;
			if (!mat2.isFullSlice()) 
				!mat2; //DDSL
			if (mat2.order() == oRowMajor)
				dst = af::array(mat.cols.count(), mat.rows.count(), mat2.getData());
			else
				dst = af::array(mat.rows.count(), mat.cols.count(), mat2.getData());
			return mat2.order();
		}
		
		template <typename T, typename TInt>
		void arrayToMatrix(const af::array &arr, Matrix<T, TInt> &dst, DSTypes::Order order) {
			if (arr.numdims() > 2)
				throw Error(ecIncompatible, "AFArrayToMatrix", "Cannot convert an array of more than 2 dimensions");
			Matrix<T, TInt> mat2(order);
			if (order == oRowMajor) {
				mat2.resize((TInt)arr.dims(1), (TInt)arr.dims(0));
			} else {
				mat2.resize((TInt)arr.dims(0), (TInt)arr.dims(1));
			}
			arr.host(mat2.getMem());
			dst = mat2; //DSA if dst has been preinitialized
		}		

		template <typename T, typename TInt>
		void zScore(const DSLib::Matrix<T, TInt> &mat, DSLib::Matrix<T, TInt> &dst) {
			Array<T, TInt> arr = mat;
			dst = Array<T, TInt>(ArrayFireDDSL::zScore(arr, arr.colDim()), arr.order());
		}

		template <typename T, typename TInt>
		void colMean(const DSLib::Matrix<T, TInt> &mat, DSLib::Matrix<T, TInt> &dst) {
			dst = Array<T, TInt>(af::mean(Array<T, TInt>(mat), mat.order()==oColMajor?0:1), mat.order());
		}

		template <typename T, typename TInt>
		void PCA(const DSLib::Matrix<T, TInt> &features, DSLib::Matrix<T, TInt> &projected, DSLib::Matrix<T, TInt> &vectors, DSLib::Matrix<T, TInt> &values, DSLib::Matrix<T, TInt> &mean, DSLib::Matrix<T, TInt> &sd, const bool center, const bool scale, const int keepDims) {
			try {	
				Array<T, TInt> feat(features, oColMajor);
				Array<T, TInt> vec(oColMajor), val(oColMajor), proj(oColMajor);
				//only colMajor
				if (center)  {
					Array<T, TInt> means(oColMajor);
					if (~mean > 0) {
						means.mat() = mean;
					} else {
						means.arr() = af::mean(feat.arr(), 0);
						mean = means.mat();
					}
					af::array tiledmeans = tile(means.arr(), (unsigned int)feat.arr().dims(0), 1);
					feat.arr() = feat.arr() - tiledmeans;
				} if (scale) {
					Array<T, TInt> stdvs(oColMajor);
					if (~sd > 0) {
						stdvs.mat() = sd;
					} else {
						stdvs.arr() = af::stdev(feat.arr(), 0);
						sd = stdvs.mat();
					}
					af::array tiledstdvs = tile(stdvs.arr(), (unsigned int)feat.arr().dims(0), 1);
					feat.arr() = feat.arr() / tiledstdvs;
				}
				ArrayFireDDSL::PCA(feat.arr(), vec.arr(), val.arr());
				proj.arr() = ArrayFireDDSL::project(feat.arr(), vec.arr(), keepDims);
				proj.convertOrder(features.order());
				projected = proj;
				values = val;
				vectors = vec;
			} catch (const af::exception &e) {
				throw Error(ecGeneral, "ArrayFire::PCA", e.what());
			}
		}

		template <typename T, typename TInt>		
		void LDA(const DSLib::Matrix<T, TInt> &features, const DSLib::Matrix<T, TInt> &target, const DSLib::Matrix<T, TInt> &classes, DSLib::Matrix<T, TInt> &projected, DSLib::Matrix<T, TInt> &vectors, DSLib::Matrix<T, TInt> &values, DSLib::Matrix<T, TInt> &mean, DSLib::Matrix<T, TInt> &sd, const bool center, const bool scale, const int keepDims) {
			try {
				Array<T, TInt> feat(features, oColMajor); //force features to colMajor
				Array<T, TInt> tar(target, oColMajor);			
				Matrix<T, TInt> classes2;
				if (classes.cols.count() > 1 && classes.rows.count() == 1)
					classes2 += classes; //Need: (dtT^x^x^x), so transpose
				else
					classes2 = classes;
				Array<T, TInt> cls(classes2, oColMajor);
				Array<T, TInt> vec(oColMajor), val(oColMajor), proj(oColMajor);

				//only colMajor
				if (center)  {
					Array<T, TInt> means(oColMajor);
					if (~mean > 0) {
						means.mat() = mean;
					} else {
						means.arr() = af::mean(feat.arr(), 0);
						mean = means.mat();
					}
					af::array tiledmeans = tile(means.arr(), (unsigned int)feat.arr().dims(0), 1);
					feat.arr() = feat.arr() - tiledmeans;
				} if (scale) {
					Array<T, TInt> stdvs(oColMajor);
					if (~sd > 0) {
						stdvs.mat() = sd;
					} else {
						stdvs.arr() = af::stdev(feat.arr(), 0);
						sd = stdvs.mat();
					}
					af::array tiledstdvs = tile(stdvs.arr(), (unsigned int)feat.arr().dims(0), 1);
					feat.arr() = feat.arr() / tiledstdvs;
				}
				ArrayFireDDSL::LDA(feat.arr(), tar.arr(), cls.arr(), vec.arr(), val.arr());
				proj.arr() = ArrayFireDDSL::project(feat.arr(), vec.arr(), keepDims);
				proj.convertOrder(features.order());
				projected = proj;
				values = val;
				vectors = vec;
			} catch (const af::exception &e) {
				throw Error(ecGeneral, "ArrayFire::LDA", e.what());
			}
		}

		template <typename T, typename TInt>
		void project(DSLib::Matrix<T, TInt> &in, DSLib::Matrix<T, TInt> &out, DSLib::Matrix<T, TInt> &vectors, DSLib::Matrix<T, TInt> &mean, DSLib::Matrix<T, TInt> &sd, UInt32 keepDims) {
			try {
				Array<T, TInt> data(in, oColMajor); //Force to colMajor
				Array<T, TInt> vec(vectors, oColMajor); //Force to colMajor
				Array<T, TInt> ret(oColMajor);

				if (~mean > 0) {
					Array<T, TInt> means(mean, oColMajor);
					af::array tiledmeans = af::tile(means.arr(), (UInt32)data.arr().dims(0), 1);
					data.arr() = data.arr() - tiledmeans;
				}

				if (~sd > 0) {
					Array<T, TInt> sds(sd, oColMajor);
					af::array tiledsvds = tile(sds.arr(), (UInt32)data.arr().dims(0), 1);
					data.arr() = data.arr() / tiledsvds;
				}

				ret.arr() = ArrayFireDDSL::project(data.arr(), vec.arr(), keepDims);
				ret.convertOrder(in.order());
				out = ret;			
			} catch (const af::exception &e) {
				throw Error(ecGeneral, "ArrayFire::project", e.what());
			}
		}

		template <typename T, typename TInt>
		void project(DSLib::Matrix<T, TInt> &in, DSLib::Matrix<T, TInt> &out, DSLib::Matrix<T, TInt> &vectors, int keepDims) {
			try {
				Array<T, TInt> data(in, oColMajor); //Force to colMajor
				Array<T, TInt> vec(vectors, oColMajor); //Force to colMajor
				Array<T, TInt> ret(oColMajor);
				ret.arr() = ArrayFireDDSL::project(data.arr(), vec.arr(), keepDims);
				ret.convertOrder(in.order());
				out = ret;
			} catch (const af::exception &e) {
				throw Error(ecGeneral, "ArrayFire::project", e.what());
			}
		}
	}
}
