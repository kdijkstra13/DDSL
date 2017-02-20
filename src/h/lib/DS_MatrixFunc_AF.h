#pragma once
#include "h/DS_Types.h"
#include "h/lib/DS_Matrix.h"

#include "ArrayFire_ddsl.h"

namespace DSLib {
	namespace ArrayFire {
		template <typename T, typename TInt = DSTypes::MatrixIdx>
		class Array {
		private:
			af::array arr_;
			DSTypes::Order order_;
		public:
			Matrix<T, TInt> mat();
			af::array & arr();
			Array<T, TInt> & mat(const Matrix<T, TInt> &mat);
			Array<T, TInt> & arr(af::array arr, const Order order);

			Array();
			Array(const Order order);
			Array(const Matrix<T, TInt> &mat);
			Array(const Matrix<T, TInt> &mat, const Order order);
			Array(af::array &arr, const Order order);
			Array<T, TInt> & operator=(const Matrix<T, TInt> &mat);
			Array<T, TInt> & operator=(af::array &arr);
			Array<T, TInt> & operator=(af::array &&arr);
			Matrix<T, TInt> operator()();
			operator af::array();
			operator Matrix<T, TInt>();

			TInt rowDim() {return order_ == oRowMajor?0:1;};
			TInt colDim() {return order_ == oColMajor?0:1;};
			Order order() {return order_;};
			af::dtype convert(af::dtype type);
			DSTypes::Order convertOrder(DSTypes::Order o);
		};
	}
}

namespace DSFunc {
	namespace ArrayFire {

		void setDevice(int device);

		template <typename T, typename TInt>
		DSTypes::Order matrixToArray(const Matrix<T, TInt> &mat, af::array &dst);		
		
		template <typename T, typename TInt>
		void arrayToMatrix(const af::array &arr, Matrix<T, TInt> &dst, DSTypes::Order order = oRowMajor);

		template <typename T, typename TInt>
		void zScore(const DSLib::Matrix<T, TInt> &mat, DSLib::Matrix<T, TInt> &dst);

		template <typename T, typename TInt>
		void colMean(const DSLib::Matrix<T, TInt> &mat, DSLib::Matrix<T, TInt> &dst);

		template <typename T, typename TInt>		
		void PCA(const DSLib::Matrix<T, TInt> &features, DSLib::Matrix<T, TInt> &projected, DSLib::Matrix<T, TInt> &vectors, DSLib::Matrix<T, TInt> &values, DSLib::Matrix<T, TInt> &mean, DSLib::Matrix<T, TInt> &sd, const bool center = true, const bool scale = true, const int keepDims=-1);

		template <typename T, typename TInt>
		void LDA(const DSLib::Matrix<T, TInt> &features, const DSLib::Matrix<T, TInt> &target, const DSLib::Matrix<T, TInt> &classes, DSLib::Matrix<T, TInt> &projected, DSLib::Matrix<T, TInt> &vectors, DSLib::Matrix<T, TInt> &values, DSLib::Matrix<T, TInt> &mean, DSLib::Matrix<T, TInt> &sd, const bool center = true, const bool scale = true, const int keepDims=-1);
	
		template <typename T, typename TInt>
		void project(DSLib::Matrix<T, TInt> &in, DSLib::Matrix<T, TInt> &out, DSLib::Matrix<T, TInt> &vectors, int keepDims=-1);
	}
}
