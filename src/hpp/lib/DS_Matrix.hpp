#pragma once

#include "h/lib/DS_Matrix.h"

#include "hpp/lib/DS_MatrixFunc.hpp"
#include "hpp/lang/DS_MatrixLang.hpp"
#include "hpp/DS_Util.hpp"

using namespace std;
using namespace DSTypes;
using namespace DSLang;
using namespace DSFunc;
using namespace DSUtil;

namespace DSLib {

	//Specializations
	template <> inline bool Matrix<MatrixDouble, MatrixIdx>::isSlicable(MatrixIdx &first, MatrixIdx &) const { return false; }
	template <> inline void Matrix<MatrixDouble, MatrixIdx>::isSmaller(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <> inline void Matrix<MatrixDouble, MatrixIdx>::isSmallerEqual(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <> inline void Matrix<MatrixDouble, MatrixIdx>::isGreaterEqual(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <> inline void Matrix<MatrixDouble, MatrixIdx>::isGreater(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <> inline void Matrix<MatrixDouble, MatrixIdx>::isEqual(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <> inline void Matrix<MatrixDouble, MatrixIdx>::isNotEqual(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };

	template <> inline bool Matrix<MatrixInt32, MatrixIdx>::isSlicable(MatrixIdx &first, MatrixIdx &) const { return false; }
	template <>	inline void Matrix<MatrixInt32, MatrixIdx>::isSmaller(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <>	inline void Matrix<MatrixInt32, MatrixIdx>::isSmallerEqual(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <>	inline void Matrix<MatrixInt32, MatrixIdx>::isGreaterEqual(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <>	inline void Matrix<MatrixInt32, MatrixIdx>::isGreater(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <>	inline void Matrix<MatrixInt32, MatrixIdx>::isEqual(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <>	inline void Matrix<MatrixInt32, MatrixIdx>::isNotEqual(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };

	template <> inline bool Matrix<MatrixUInt32, MatrixIdx>::isSlicable(MatrixIdx &first, MatrixIdx &) const { return false; }
	template <>	inline void Matrix<MatrixUInt32, MatrixIdx>::isSmallerEqual(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <>	inline void Matrix<MatrixUInt32, MatrixIdx>::isSmaller(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <>	inline void Matrix<MatrixUInt32, MatrixIdx>::isGreaterEqual(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <>	inline void Matrix<MatrixUInt32, MatrixIdx>::isGreater(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <>	inline void Matrix<MatrixUInt32, MatrixIdx>::isEqual(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <>	inline void Matrix<MatrixUInt32, MatrixIdx>::isNotEqual(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };

	template <>	inline bool Matrix<MatrixString, MatrixIdx>::isSlicable(MatrixIdx &first, MatrixIdx &) const { return false; }
	template <>	inline void Matrix<MatrixString, MatrixIdx>::isSmallerEqual(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <>	inline void Matrix<MatrixString, MatrixIdx>::isSmaller(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <>	inline void Matrix<MatrixString, MatrixIdx>::isGreaterEqual(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <>	inline void Matrix<MatrixString, MatrixIdx>::isGreater(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <>	inline void Matrix<MatrixString, MatrixIdx>::isEqual(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };
	template <>	inline void Matrix<MatrixString, MatrixIdx>::isNotEqual(MatrixBase &dst, const MatrixBase &) const { dst.clear(); };

	template <> inline bool Matrix<String, UInt32>::isSlicable(UInt32 &first, UInt32 &) const { return false; }
	template <> inline bool Matrix<VoidPointer, UInt32>::isSlicable(UInt32 &first, UInt32 &) const { return false; }

	//CellsBase::IteratorBase
	template <typename T, typename TInt>
	CellsBase<T, TInt>::IteratorBase::IteratorBase() : stride_(0), contraStride_(0), count_(0), contraCount_(0), counter_(0), counter2_(0) {};

	template <typename T, typename TInt>
	CellsBase<T, TInt>::IteratorBase::IteratorBase(const TInt stride, const TInt contraStride, const TInt count, const TInt contraCount, const typename std::vector<T>::iterator &cursor) : stride_(stride), contraStride_(contraStride), count_(count), contraCount_(contraCount), counter_(0), counter2_(0), cursor_(cursor) {};

	template <typename T, typename TInt>
	CellsBase<T, TInt>::IteratorBase::~IteratorBase() {};

	template <typename T, typename TInt>
	typename CellsBase<T, TInt>::IteratorBase& CellsBase<T, TInt>::IteratorBase::operator=(const typename CellsBase<T, TInt>::IteratorBase& other) {
		stride_ = other.stride_;
		count_ = other.count_;
		contraStride_ = other.contraStride_;
		contraCount_ = other.contraCount_;
		counter_ = other.counter_;
		counter2_ = other.counter2_;
		cursor_ = other.cursor_;
		return (*this);
	};

	template <typename T, typename TInt>
	bool CellsBase<T, TInt>::IteratorBase::operator==(const typename CellsBase<T, TInt>::IteratorBase& other) const {
		return (cursor_ == other.cursor_);
	};

	template <typename T, typename TInt>
	bool CellsBase<T, TInt>::IteratorBase::operator!=(const typename CellsBase<T, TInt>::IteratorBase& other) const {
		return (cursor_ != other.cursor_);
	};

	template <typename T, typename TInt>
	T& CellsBase<T, TInt>::IteratorBase::operator*() {
		return *cursor_;
	};

	template <typename T, typename TInt>
	T* CellsBase<T, TInt>::IteratorBase::operator->() {
		return &(*cursor_);
	};

	//CellsBase
	template <typename T, typename TInt>
	TInt CellsBase<T, TInt>::count() {
		return count_;
	}

	template <typename T, typename TInt>
	TInt CellsBase<T, TInt>::contraCount() {
		return contraCount_;
	}

	template <typename T, typename TInt>
	void CellsBase<T, TInt>::setValues(const typename std::vector<T>::iterator &begin, const typename std::vector<T>::iterator &end, const TInt stride, const TInt contraStride, const TInt count, const TInt contraCount) {
		begin_ = begin;
		end_ = end;
		stride_ = stride;
		contraStride_ = contraStride;
		count_ = count;
		contraCount_= contraCount;
	}

	template <typename T, typename TInt>
	CellsBase<T, TInt>::CellsBase() : stride_(0), contraStride_(0), count_(0), contraCount_(0) {}

	template <typename T, typename TInt>
	CellsBase<T, TInt>::CellsBase(const typename std::vector<T>::iterator &begin, const typename std::vector<T>::iterator &end, const TInt stride, const TInt contraStride, const TInt count, const TInt contraCount) : begin_(begin), end_(end), stride_(stride), contraStride_(contraStride), count_(count), contraCount_(contraCount) {};

	//CellsRegion::Iterator
	template <typename T, typename TInt>
	CellsRegion<T, TInt>::Iterator::Iterator() {
		this->stride_ = 0;
		this->count_ = 0;
		this->contraStride_ = 0;
		this->contraCount_ = 0;
		this->counter_ = 0;
		this->counter2_ = 0;
	}

	template <typename T, typename TInt>
	CellsRegion<T, TInt>::Iterator::Iterator(const typename CellsRegion<T, TInt>::Iterator &other) {
		this->stride_ = other.stride_;
		this->count_ = other.count_;
		this->contraStride_ = other.contraStride_;
		this->contraCount_ = other.contraCount_;
		this->counter_ = other.counter_;
		this->counter2_ = other.counter2_;
		this->cursor_ = other.cursor_;
	}

	template <typename T, typename TInt>
	CellsRegion<T, TInt>::Iterator::Iterator(const TInt stride, const TInt contraStride, const TInt count, const TInt contraCount, const typename std::vector<T>::iterator &cursor) : CellsBase<T, TInt>::IteratorBase(stride, contraStride, count, contraCount, cursor) {}

	template <typename T, typename TInt>
	CellsRegion<T, TInt>::Iterator::~Iterator() {
	}

	template <typename T, typename TInt>
	inline typename CellsRegion<T, TInt>::Iterator& CellsRegion<T, TInt>::Iterator::operator++() {
		if (this->counter_ >= this->count_-1) {
			if (this->counter2_ >= this->contraCount_-1) {
				//cursor_ += this->contraStride_;
				this->cursor_ ++; //end reached
				return *this;
			}
			if (this->contraStride_ >= (this->counter_ * this->stride_)) {
				this->cursor_ += this->contraStride_ - (this->counter_ * this->stride_); //lf+cr
			} else {
				this->cursor_ -= (this->counter_ * this->stride_) - this->contraStride_; //lf+cr
			}
			this->counter_ = 0;
			this->counter2_++;
		} else {
			this->cursor_ += this->stride_;
			this->counter_++;
		}
		return *this;
	}

	template <typename T, typename TInt>
	inline typename CellsRegion<T, TInt>::Iterator CellsRegion<T, TInt>::Iterator::operator++(int) {
		CellsRegion<T, TInt>::Iterator it(*this);
		++(*this);
		return it;
	}

	//CellsRegion
	template <typename T, typename TInt>
	CellsRegion<T, TInt>::CellsRegion(const typename std::vector<T>::iterator &begin, const typename std::vector<T>::iterator &end, const TInt stride, const TInt contraStride, const TInt count, const TInt contraCount) : CellsBase<T, TInt>(begin, end, stride, contraStride, count, contraCount) {}

	template <typename T, typename TInt>
	CellsRegion<T, TInt>::CellsRegion() : CellsBase<T, TInt>() {};

	template <typename T, typename TInt>
	typename CellsRegion<T, TInt>::Iterator CellsRegion<T, TInt>::operator[](const TInt index) {
		return CellsRegion<T, TInt>::Iterator(this->stride_, this->contraStride_, this->count_, this->contraCount_, this->begin_ + index * this->stride_);
	}

	template <typename T, typename TInt>
	typename CellsRegion<T, TInt>::Iterator CellsRegion<T, TInt>::begin() {
		return CellsRegion<T, TInt>::Iterator(this->stride_, this->contraStride_, this->count_, this->contraCount_, this->begin_);
	}

	template <typename T, typename TInt>
	typename CellsRegion<T, TInt>::Iterator CellsRegion<T, TInt>::end() {
		return CellsRegion<T, TInt>::Iterator(this->stride_, this->contraStride_, this->count_, this->contraCount_, this->end_);
	}

	//CellsContinuous::Iterator
	template <typename T, typename TInt>
	CellsContinuous<T, TInt>::Iterator::Iterator() {
		this->stride_ = 0;
		this->count_ = 0;
		this->contraStride_ = 0;
		this->contraCount_ = 0;
		this->counter_ = 0;
		this->counter2_ = 0;
	}

	template <typename T, typename TInt>
	CellsContinuous<T, TInt>::Iterator::Iterator(const typename CellsContinuous<T, TInt>::Iterator &other) {
		this->stride_ = other.stride_;
		this->count_ = other.count_;
		this->contraStride_ = other.contraStride_;
		this->contraCount_ = other.contraCount_;
		this->counter_ = other.counter_;
		this->counter2_ = other.counter2_;
		this->cursor_ = other.cursor_;
	}

	template <typename T, typename TInt>
	CellsContinuous<T, TInt>::Iterator::Iterator(const TInt stride, const TInt contraStride, const TInt count, const TInt contraCount, const typename std::vector<T>::iterator &cursor) : CellsBase<T, TInt>::IteratorBase(stride, contraStride, count, contraCount, cursor) {}

	template <typename T, typename TInt>
	CellsContinuous<T, TInt>::Iterator::~Iterator() {
	}

	template <typename T, typename TInt>
	inline typename CellsContinuous<T, TInt>::Iterator& CellsContinuous<T, TInt>::Iterator::operator++() {
		this->cursor_ += this->stride_;
		return *this;
	}

	template <typename T, typename TInt>
	inline typename CellsContinuous<T, TInt>::Iterator CellsContinuous<T, TInt>::Iterator::operator++(int) {
		Iterator it(*this);
		++(*this);
		return it;
	}

	//CellsContinuous
	template <typename T, typename TInt>
	CellsContinuous<T, TInt>::CellsContinuous(const typename std::vector<T>::iterator &begin, const typename std::vector<T>::iterator &end, const TInt stride, const TInt contraStride, const TInt count, const TInt contraCount) : CellsBase<T, TInt>(begin, end, stride, contraStride, count, contraCount) {}

	template <typename T, typename TInt>
	CellsContinuous<T, TInt>::CellsContinuous() : CellsBase<T, TInt>() {};

	template <typename T, typename TInt>
	typename CellsContinuous<T, TInt>::Iterator CellsContinuous<T, TInt>::operator[](const TInt index) {
		return Iterator(this->stride_, this->contraStride_, this->count_, this->contraCount_, this->begin_ + index * this->stride_);
	}

	template <typename T, typename TInt>
	typename CellsContinuous<T, TInt>::Iterator CellsContinuous<T, TInt>::begin() {
		return Iterator(this->stride_, this->contraStride_, this->count_, this->contraCount_, this->begin_);
	}

	template <typename T, typename TInt>
	typename CellsContinuous<T, TInt>::Iterator CellsContinuous<T, TInt>::end() {
		return Iterator(this->stride_, this->contraStride_, this->count_, this->contraCount_, this->end_);
	}

	//Matrix::Index::Iterator
	template <typename T, typename TInt>
	Matrix<T, TInt>::Index::Iterator::Iterator(typename Matrix<T, TInt>::Index &index, const typename std::vector<T>::iterator &cursor) : index_(&index), cursor_(cursor) {
		stride_ = index.stride();
		count_ = index.count();
	};

	template<typename T, typename TInt>
	Matrix<T, TInt>::Index::Iterator::Iterator() {
		stride_ = 0;
		count_ = 0;
	}

	template<typename T, typename TInt>
	Matrix<T, TInt>::Index::Iterator::Iterator(const Iterator & other) {
		index_ = other.index_;
		cursor_ = other.cursor_;
		stride_ = other.stride_;
		count_ = other.count_;
	}

	template <typename T, typename TInt>
	Matrix<T, TInt>::Index::Iterator::~Iterator() {};

	template <typename T, typename TInt>
	CellsBase<T, TInt> &Matrix<T, TInt>::Index::Iterator::cells(CellsIteratorType cellsIteratorType) {
		TInt stride = index_->contra_->stride_;
		TInt contraStride = index_->stride_;
		TInt count = index_->contra_->count_;
		TInt contraCount = index_->count_;
		switch (cellsIteratorType) {
			case citContinuous:	{
				index_->cellsContinuous_.setValues(cursor_, cursor_ + count * stride, stride, contraStride, count, contraCount);
				return index_->cellsContinuous_;
			}
			case citRegion:	{
				index_->cellsRegion_.setValues(cursor_, cursor_ + 1 * contraStride, stride, contraStride, count, contraCount);
				return index_->cellsRegion_;
			}
			default:
				throw Error(ecInvalidEnum, "Matrix<T, TInt>::Index::Iterator::cells()", SS(etos(cellsIteratorType)));
		}
	}

	template <typename T, typename TInt>
	CellsContinuous<T, TInt> * Matrix<T, TInt>::Index::Iterator::operator->() {
		return &(dynamic_cast<CellsContinuous<T, TInt> &> (cells(citContinuous)));
	}

	template<typename T, typename TInt>
	CellsContinuous<T, TInt>& Matrix<T, TInt>::Index::Iterator::operator*() {
		return dynamic_cast<CellsContinuous<T, TInt> &> (cells(citContinuous));
	}

	template <typename T, typename TInt>
	typename Matrix<T, TInt>::Index::Iterator& Matrix<T, TInt>::Index::Iterator::operator=(const typename Matrix<T, TInt>::Index::Iterator& other) {
		index_ = other.index_;
		cursor_ = other.cursor_;
		stride_ = other.stride_;
		count_ = other.count_;
		return(*this);
	}

	template <typename T, typename TInt>
	bool Matrix<T, TInt>::Index::Iterator::operator==(const typename Matrix<T, TInt>::Index::Iterator& other) const {
		return cursor_ == other.cursor_;
	}

	template <typename T, typename TInt>
	bool Matrix<T, TInt>::Index::Iterator::operator!=(const typename Matrix<T, TInt>::Index::Iterator& other) const {
		return cursor_ != other.cursor_;
	}

	template <typename T, typename TInt>
	typename Matrix<T, TInt>::Index::Iterator& Matrix<T, TInt>::Index::Iterator::operator++() {
		cursor_ += stride_;
		return(*this);
	}

	template <typename T, typename TInt>
	typename Matrix<T, TInt>::Index::Iterator Matrix<T, TInt>::Index::Iterator::operator++(int) {
		Iterator tmp(*this);
		++(*this);
		return tmp;
	}

	template <typename T, typename TInt>
	typename CellsContinuous<T, TInt>::Iterator Matrix<T, TInt>::Index::Iterator::operator[](const TInt index) {
		return typename CellsContinuous<T, TInt>::Iterator(index_->contra_->stride_, index_->stride_, index_->contra_->count_, index_->count_, cursor_ + index * index_->contra_->stride_);
	}

	//Matrix::Index
	template <typename T, typename TInt>
	Matrix<T, TInt>::Index::Index() : matrix_(nullptr), first_(0), last_(0), stride_(0), count_(0), contra_(nullptr), isRow_(false) {
		//begin_ = matrix_->data_.end();
		//end_ = matrix_->data_.end();
	}

	template <typename T, typename TInt>
	Matrix<T, TInt>::Index::Index(Matrix &matrix, const TInt first, const TInt last, const TInt stride, Index *contra) : matrix_(&matrix), first_(first), last_(last), stride_(stride), count_(0), contra_(contra), isRow_(false) {
		if (stride_ != 0) {
			matrix_->updateIt_();
			count_ = (TInt) (end_ - begin_) / stride;
		} else {
			//begin_ = matrix_->data_.end();
   			//end_ = matrix_->data.end();
			count_ = 0;
		}
	};

	template <typename T, typename TInt>
	typename Matrix<T, TInt>::Index::Iterator Matrix<T, TInt>::Index::operator[](const TInt index) {
		return Iterator(*this, begin_ + index * stride_);
	}

	template <typename T, typename TInt>
	typename Matrix<T, TInt>::Index::Iterator Matrix<T, TInt>::Index::begin() {
		return Iterator(*this, begin_);
	}

	template <typename T, typename TInt>
	typename Matrix<T, TInt>::Index::Iterator Matrix<T, TInt>::Index::end() {
		return Iterator(*this, end_);
	}

	template <typename T, typename TInt>
	TInt Matrix<T, TInt>::Index::count() const {return count_;};

	template <typename T, typename TInt>
	TInt Matrix<T, TInt>::Index::stride() const {return stride_;};

	template <typename T, typename TInt>
	TInt Matrix<T, TInt>::Index::first() const {return first_;};

	template <typename T, typename TInt>
	TInt Matrix<T, TInt>::Index::last() const {return last_;};

	template <typename T, typename TInt>
	bool Matrix<T, TInt>::Index::isAligned() const {
		return (isRow() && matrix_->order_ == oRowMajor) || (isCol() && matrix_->order_ == oColMajor);
	}

	template <typename T, typename TInt>
	bool Matrix<T, TInt>::Index::isRow() const {
		return (this == &(matrix_->rows));
	}

	template <typename T, typename TInt>
	bool Matrix<T, TInt>::Index::isCol() const {
		return (this == &(matrix_->cols));
	}

	template <typename T, typename TInt>
	CellsBase<T, TInt> &Matrix<T, TInt>::Index::cells(CellsIteratorType cellsIteratorType) {
		TInt stride = contra_->stride_;
		TInt contraStride = stride_;
		TInt count = contra_->count_;
		TInt contraCount = count_;
		if (cellsIteratorType == citContinuous && matrix_->isRegion() && !isAligned())
				throw Error(ecIncompatible, "Matrix<T, TInt>::Index::cells()", "Cannot get an instance of CellsContinuous when data isRegion() or not isAligned()");
		switch (cellsIteratorType) {
			case citRegion: {
				//cellsRegion_.setValues(matrix_->begin(), matrix_->begin() + (contraCount * contraStride), stride, contraStride, count, contraCount);
				//if (matrix_->order_ == oRowMajor)
					//cellsRegion_.setValues(matrix_->begin(), matrix_->at(matrix_->rows.count_, 0), stride, contraStride, count, contraCount);
					if (contraCount == 0)
						cellsRegion_.setValues(matrix_->begin(), matrix_->begin(), stride, contraStride, count, contraCount);
					else
						cellsRegion_.setValues(matrix_->begin(), matrix_->at(matrix_->rows.count_-1, matrix_->cols.count_-1)+1, stride, contraStride, count, contraCount);
				//else
					//cellsRegion_.setValues(matrix_->begin(), matrix_->at(matrix_->rows.count_-1, matrix_->cols.count_-1)+1, stride, contraStride, count, contraCount);
				return cellsRegion_;
			}
			case citContinuous: {
				if (contraCount == 0)
					cellsContinuous_.setValues(matrix_->begin(), matrix_->begin(), stride, contraStride, count, contraCount);
				else
					cellsContinuous_.setValues(matrix_->begin(), matrix_->begin() + (contraCount * contraStride), stride, contraStride, count, contraCount);
				//cellsContinuous_.setValues(matrix_->begin(), matrix_->end(), stride, contraStride, count, contraCount);
				return cellsContinuous_;
			}
			default:
				throw Error(ecInvalidEnum, "Matrix<T, TInt>::Index::cells()", SS(etos(cellsIteratorType)));
		}
	}

	template <typename T, typename TInt>
	CellsRegion<T, TInt> * Matrix<T, TInt>::Index::operator->() {
		return &(dynamic_cast<CellsRegion<T, TInt> &> (cells(citRegion)));
	}

	template<typename T, typename TInt>
	CellsRegion<T, TInt>& Matrix<T, TInt>::Index::operator*() {
		return dynamic_cast<CellsRegion<T, TInt> &> (cells(citRegion));
	}

	//Matrix
	template <typename T, typename TInt>
	void Matrix<T, TInt>::updateIt_() {
		rows.begin_ = at(0, 0);
		rows.end_ = at(rows.count_, 0);
		cols.begin_ = rows.begin_;
		cols.end_ = at(0, cols.count_);

		//traverser crisis
		/*rows.begin_ = at(0, 0);
		cols.begin_ = at(0, 0);
		if (order_ == oColMajor) {
			rows.end_ = at(0, cols.count_);
			cols.end_ = at(0, cols.count_);
		} else if (order_ == oRowMajor) {
			rows.end_ = at(rows.count_, 0);
			cols.end_ = at(rows.count_, 0);
		}*/
	}

	template <typename T, typename TInt>
	void Matrix<T, TInt>::updateStride_() {
		if (up_ != nullptr) {
			rows.stride_ = up_->rows.stride_;
			cols.stride_ = up_->cols.stride_;
		}
	}

	template <typename T, typename TInt>
	Matrix<T, TInt>& Matrix<T, TInt>::ref_(Matrix<T, TInt> &mat) {
		std::lock_guard<std::mutex> lock(lockRefRel_);
		if (up_ != nullptr) {
			return (up_->ref_(mat));
		} else {
			down_.push_back(&mat);
			return *this;
		}
	}

	template <typename T, typename TInt>
	void Matrix<T, TInt>::rel_(const Matrix<T, TInt> &mat){
		std::lock_guard<std::mutex> lock(lockRefRel_);
		for (auto it=down_.begin();it!=down_.end();it++) {
			if (*it == &mat) {
				down_.erase(it);
				return;
			}
		}
		throw Error(ecNotFound, "Matrix::rel_()", "Release Matrix not found.");
	}

	template <typename T, typename TInt>
	typename vector<T>::iterator Matrix<T, TInt>::begin() {
		return at(0, 0);
	}

	template <typename T, typename TInt>
	typename vector<T>::iterator Matrix<T, TInt>::end() {
		//return rows.end_; //traverser crisis
		return at(rows.count_-1, cols.count_-1)+1;
	}

	template <typename T, typename TInt>
	typename std::vector<T>::const_iterator Matrix<T, TInt>::begin() const {
		return at(0, 0);
	}

	template <typename T, typename TInt>
	typename std::vector<T>::const_iterator Matrix<T, TInt>::end() const {
		//return rows.end_; //traverser crisis
		return at(rows.count_-1, cols.count_-1)+1;
	}

	template <typename T, typename TInt>
	typename vector<T>::iterator Matrix<T, TInt>::at(const TInt row, const TInt col) {
		if (up_ != nullptr && up_ == this)
			throw Error(ecInternal, "Matrix::at()", "up_ == this");
		if (up_ != nullptr) {
			return up_->at(row + rows.first_, col + cols.first_);
		} else
			return (data_.begin() + row * rows.stride_ + col * cols.stride_);
	}

	template <typename T, typename TInt>
	typename vector<T>::const_iterator Matrix<T, TInt>::at(const TInt row, const TInt col) const {
		if (up_ != nullptr && up_ == this)
			throw Error(ecInternal, "Matrix::at()", "up_ == this");
		if (up_ != nullptr) {
			return up_->at(row + rows.first_, col + cols.first_);
		} else
			return (data_.begin() + row * rows.stride_ + col * cols.stride_);
	}

	template<typename T, typename TInt>
	void Matrix<T, TInt>::invalidate_() {
		updateStride_();
		updateIt_();
		for (auto it=down_.begin();it!=down_.end();it++)
			(*it)->invalidate_();
	}

	template<typename T, typename TInt>
	void Matrix<T, TInt>::init_() {
		rows.matrix_ = this;
		rows.contra_ = &cols;
		cols.matrix_ = this;
		cols.contra_ = &rows;
		up_ = nullptr;
	}

	template <typename T, typename TInt>
	void Matrix<T, TInt>::slice(Matrix<T, TInt> &dst, const Matrix<TInt, TInt> &rowIndices, const Matrix<TInt, TInt> &colIndices) {
		TInt rowFirst, rowLast, colFirst, colLast;
		bool rowSlicable, colSlicable;
		dst.setOrder(order()); //Set order flag

		if (rowIndices.vec().count() == 0 && colIndices.vec().count() == 0) {
			dst.clear();
			return;
		}

		if (!rowIndices.isInitialized()) {
			rowSlicable = true;
			rowFirst = 0;
			rowLast = rows.count()-1;
		} else
			rowSlicable = rowIndices.isSlicable(rowFirst, rowLast);

		if (!colIndices.isInitialized()) {
			colSlicable = true;
			colFirst = 0;
			colLast = cols.count()-1;
		} else
			colSlicable = colIndices.isSlicable(colFirst, colLast);

		if (rowLast > rows.count() || colLast > cols.count())
			throw Error(ecRangeError, "Matrix::slice(rowIndices, colIndices)", SS("Slice row (" << rowLast << ") exceeds row count (" << rows.count() << ") OR slice col (" << colLast << ") exceeds col count (" << cols.count() << ")."));

		if (rowSlicable && colSlicable) {
			slice(dst, rowFirst, rowLast+1, colFirst, colLast+1);
		} else {
			if (colSlicable) {
				dst.resize(rowIndices.vec().count(), colLast-colFirst+1);
				UInt32 r=0;
				for (auto r_it=rowIndices.vec().begin();r_it!=rowIndices.vec().end();r_it++) {
					std::copy(rows[*r_it][colFirst], rows[*r_it][colLast+1], dst.rows[r][0]);
					r++;
				}
			} else if (rowSlicable) {
				dst.resize(rowLast-rowFirst+1, colIndices.vec().count());
				UInt32 c=0;
				for (auto c_it=colIndices.vec().begin();c_it!=colIndices.vec().end();c_it++) {
					std::copy(cols[*c_it][rowFirst], cols[*c_it][rowLast+1], dst.cols[c][0]);
					c++;
				}
			} else {
				dst.resize(rowIndices.vec().count(), colIndices.vec().count());
				UInt32 r=0;
				UInt32 c;
				if (order() == oRowMajor) {
					for (auto r_it=rowIndices.vec().begin();r_it!=rowIndices.vec().end();r_it++) {
						c=0;
						for (auto c_it=colIndices.vec().begin();c_it!=colIndices.vec().end();c_it++) {
							*(dst.rows[r][c]) = *(rows[*r_it][*c_it]);
							c++;
						}
						r++;
					}
				} else if (order() == oColMajor) {
					UInt32 r;
					UInt32 c=0;
					for (auto c_it=colIndices.vec().begin();c_it!=colIndices.vec().end();c_it++) {
						r=0;
						for (auto r_it=rowIndices.vec().begin();r_it!=rowIndices.vec().end();r_it++) {
							*(dst.rows[r][c]) = *(rows[*r_it][*c_it]);
							r++;
						}
						c++;
					}
				}
			}
		}
	}

	template<typename T, typename TInt>
	void Matrix<T, TInt>::slice(Matrix<T, TInt> &dst, const TInt rowFirst, const TInt rowLast, const TInt colFirst, const TInt colLast) {
		if (rowLast > rows.count_ || colLast > cols.count_)
			throw Error(ecRangeError, "Matrix::Slice(rowFirst, rowLast, colFirst, colLast)", SS("Slice row (" << rowLast << ") exceeds row count (" << rows.count_ << ") OR slice col (" << colLast << ") exceeds col count (" << cols.count_ << ")."));

		if (up_ == nullptr) {
			//Create reference
			dst.up_ = &(ref_(dst));

			//Copy relevant stuff
			dst.order_ = order_;
			dst.cols.first_ = colFirst;
			dst.cols.last_ = colLast;
			dst.rows.first_ = rowFirst;
			dst.rows.last_ = rowLast;
			dst.rows.count_ = dst.rows.last_ - dst.rows.first_;
			dst.cols.count_ = dst.cols.last_ - dst.cols.first_;
			dst.rows.stride_ = rows.stride_;
			dst.cols.stride_ = cols.stride_;

			//Update begin_ and end_ iterators
			dst.invalidate_();
		} else {
			up_->slice(dst, rowFirst + rows.first_, rowLast + rows.first_, colFirst + cols.first_, colLast + cols.first_);
		}
	}

	//Reference clone for non const Matrices
	template<typename T, typename TInt>
	void Matrix<T, TInt>::clone(const Matrix<T, TInt> &src) {
		breakUp(false);
		breakDown();

		data_.resize(0);

		//Reference this and get the root Matrix
		up_ = &(const_cast<Matrix<T, TInt>&>(src).ref_(*this));

		//Copy other relevant stuff from mat
		order_ = src.order_;
		cols.first_ = src.cols.first_;
		cols.last_ = src.cols.last_;
		rows.first_ = src.rows.first_;
		rows.last_ = src.rows.last_;
		rows.count_ = src.rows.count_;
		cols.count_ = src.cols.count_;
		rows.stride_ = src.rows.stride_;
		cols.stride_ = src.cols.stride_;

		//force this to update the begin_ and end_ iterators.
		invalidate_();
	}

	template<typename T, typename TInt>
	void Matrix<T, TInt>::clone(Matrix<T, TInt> && src) {
		breakUp(false);
		breakDown();

		if (src.up_ == nullptr)
			data_ = std::move(src.data());
		else {
			data_.resize(0);
			up_ = &(const_cast<Matrix<T, TInt>&>(src).ref_(*this));
		}

		//Copy other relevant stuff from mat
		order_ = src.order_;
		cols.first_ = src.cols.first_;
		cols.last_ = src.cols.last_;
		rows.first_ = src.rows.first_;
		rows.last_ = src.rows.last_;
		rows.count_ = src.rows.count_;
		cols.count_ = src.cols.count_;
		rows.stride_ = src.rows.stride_;
		cols.stride_ = src.cols.stride_;

		//force this to update the begin_ and end_ iterators.
		invalidate_();
	}

	template<typename T, typename TInt>
	Matrix<T, TInt> Matrix<T, TInt>::dup() const {
		Matrix<T, TInt> m = const_cast<Matrix<T, TInt>&>(*this);
		m.breakUp();
		if (~m > 0) {
			//Specific deep copy if T inherits from MatrixBase
			if (std::is_base_of<MatrixBase, T>::value) {
				for (auto it=m.rows->begin();it!=m.rows->end();it++) {
					reinterpret_cast<MatrixBase&>(*it).breakUp();
				}
			}
		}
		return m;
	}

	template <typename T, typename TInt>
	bool Matrix<T, TInt>::isSlicable(TInt &first, TInt &last) const {
		if (!isVector())
			return false;
		if (rows.count() == 1 && cols.count() == 1) {
			first = (TInt) *(vec().begin());
			last = first;
			return true;
		}
		bool slicable = true;
		typename vector<T>::const_iterator it = begin();
		auto itPrev = it;
		it++;
		first = (TInt) *itPrev;
		while (it != end()) {
			if (*it - *itPrev != 1)
				slicable = false;
			itPrev = it;
			it++;
		};
		last = (TInt) *itPrev;
		return slicable;
	}

	template <typename T, typename TInt>
	bool Matrix<T, TInt>::isVector() const {
		return (isRowVector() || isColVector());
	}

	template <typename T, typename TInt>
	bool Matrix<T, TInt>::isColVector() const {
		return cols.count_ == 1;
	}

	template <typename T, typename TInt>
	bool  Matrix<T, TInt>::isRowVector() const {
		return rows.count_ == 1;
	}

	template <typename T, typename TInt>
	bool Matrix<T, TInt>::isInitialized() const {
		return (rows.stride_ != 0 && cols.stride_ != 0);
	}

	template <typename T, typename TInt>
	bool Matrix<T, TInt>::isEmpty() const {
		return (rows.count_ == 0 || cols.count_ == 0);
	}

	template <typename T, typename TInt>
	bool Matrix<T, TInt>::isRegion() const {
		if (up_ == nullptr)
			return false;
		else
			return (cols.stride_ != rows.count_ && rows.stride_ != cols.count_);
	}

	template <typename T, typename TInt>
	bool Matrix<T, TInt>::isContinuous() const {
		return !isRegion();
	}

	template <typename T, typename TInt>
	bool Matrix<T, TInt>::isSlice() const {
		return (up_ != nullptr);
	}

	template<typename T, typename TInt>
	bool Matrix<T, TInt>::isFullSlice() const {
		return isSlice() && isSameData(*up_);

	}

	template<typename T, typename TInt>
	bool Matrix<T, TInt>::isSameData(const Matrix<T, TInt> &other) const {
		//Check memory address in case of incompatible iterators.
		return (&(*(this->at(0,0))) == &(*(other.at(0,0))) &&
				&(*(this->at(this->rows.count()-1, this->cols.count()-1))) == &(*(other.at(other.rows.count()-1, other.cols.count()-1))));
	}

	template <typename T, typename TInt>
	void Matrix<T, TInt>::breakUpDown(MatrixBase &mat) {
		Matrix<T, TInt> &matt = dynamic_cast<Matrix<T, TInt>&>(mat);
		if (this == matt.up_)
			matt.breakUp(true);
		if (up_ == &matt)
			breakUp(true);
	}

	template <typename T, typename TInt>
	void Matrix<T, TInt>::breakUp(const bool copy) {
		//static const char * f = "Matrix<T, TInt>::breakUp()";
		if (up_ != nullptr) {
			if (copy) {
				//Should only get a slice, if first != 0 or last != count
				if (rows.first_ == 0 && rows.last_ == up_->rows.count_ && cols.first_ == 0 && cols.last_ == up_->cols.count_) {
					data_ = up_->data_;
				} else {
					if (order_ == oRowMajor) {
						//CellsRegion<T, TInt> cells =(rows.cells(citRegion));
						data_.resize(rows.count_ * cols.count_ + cols.count_); //including DW
						std::copy(rows->begin(), rows->end(), data_.begin());
						rows.stride_ = cols.count_;
						cols.stride_ = 1;
					} else if (order_ == oColMajor) {
						//CellsRegion<T, TInt> cells = static_cast<CellsRegion<T, TInt>&>(cols[0].cells(citRegionFromStart));
						data_.resize(rows.count_ * cols.count_ + rows.count_); //including DW
						std::copy(cols->begin(), cols->end(), data_.begin());
						rows.stride_ = 1;
						cols.stride_ = rows.count_;
					} else {
						rows.count_ = 0;
						cols.count_ = 0;
						rows.stride_ = 0;
						cols.stride_ = 0;
					}
					cols.first_ = 0;
					cols.last_ = cols.count_;
					rows.first_ = 0;
					rows.last_ = rows.count_;
				}
			} else {
				rows.count_ = 0;
				cols.count_ = 0;
				rows.stride_ = 0;
				cols.stride_ = 0;
				rows.first_ = 0;
				cols.first_ = 0;
				rows.last_ = 0;
				rows.last_ = 0;
			}
			up_->rel_(*this);
			up_ = nullptr;
			updateIt_();
		}
	}

	template <typename T, typename TInt>
	void Matrix<T, TInt>::breakDown(const bool copy) {
		while (down_.begin() != down_.end())
			(*down_.begin())->breakUp(copy);
	}


	template <typename T, typename TInt>
	void Matrix<T, TInt>::resize(TInt rowCount, TInt colCount) {
		if (rowCount == 0 || colCount == 0)
			clear();
		if (colCount < cols.count_ || rowCount < rows.count_ ||
			(order_ == oRowMajor && colCount != cols.count_) ||
			(order_ == oColMajor && rowCount != rows.count_)) //Only break ties if data structure is smaller or 1-d data gets misaligned.
			breakDown();
		breakUp();

		cols.count_ = colCount;
		rows.count_ = rowCount;
		cols.first_ = 0;
		cols.last_ = colCount;
		rows.first_ = 0;
		rows.last_ = rowCount;

		if (order_ == oRowMajor) {
			cols.stride_ = 1;
			rows.stride_ = colCount;
		} else {
			cols.stride_ = rowCount;
			rows.stride_ = 1;
		}
		if (order_ == oRowMajor)
			data_.resize(cols.count_ * rows.count_ + cols.count_);
		else if (order_ == oColMajor)
			data_.resize(cols.count_ * rows.count_ + rows.count_);
		updateIt_();
	}

	template <typename T, typename TInt>
	void Matrix<T, TInt>::Index::add(const T &value) {

		if (!matrix_->isVector() && matrix_->isInitialized())
			throw Error(ecIncompatible, "Matrix<T>::Index::add()", "Cannot add single value to a non-vector.");

		if (stride_ == 0) { //Initialization
			stride_ = 1;
			contra_->stride_ = 1;
			contra_->count_ = 1;
			contra_->last_ = contra_->count_;
		} else
			matrix_->breakUp();

		if (contra_->count_ != 1)
			throw Error(ecSizeMismatch, "Matrix<T>::Index::add()", "Cannot add a single value to the row of a col vector, or a col of a row vector.");

		matrix_->data_.insert(matrix_->data_.begin() + count_, value);

		if (isAligned() || count_ == 0) {
			if (count_ == 0)
				matrix_->data_.resize(2); //deadweight
			else
				contra_->stride_ = count_+1; //Added for (dtInt32|10)^20^30^40; 12jan2017
		} else {
			contra_->stride_ = count_+1;
			matrix_->data_.resize(matrix_->data_.size()+1); //add deadweight
		}
		//Update iterator administration
		count_++;
		last_++;

		matrix_->invalidate_();
	};

	template <typename T, typename TInt>
	void Matrix<T, TInt>::Index::add(const vector<T> &vec) {
		if (vec.size() == 0)
			return;
		if (stride_ == 0) { //Initialization
			if (isAligned()) {
				stride_ = (TInt)vec.size();
				contra_->stride_ = 1;
			} else {
				stride_ = 1;
				contra_->stride_ = 1;
			}
			contra_->count_ = (TInt) vec.size();
			contra_->last_ = contra_->count_;
		}

		if (vec.size() != contra_->count_)
			throw Error(ecSizeMismatch, "Matrix::Index::add()", SS("Invalid vector length: " << vec.size() << " and " << count_));

		matrix_->breakUp();

		if (isAligned() || count_ == 0) {
			matrix_->data_.insert(matrix_->data_.begin() + (count_ * contra_->count_), vec.begin(), vec.end());
			//initialize dead weight
			if (count_ == 0) {
				if (isAligned()) {
					matrix_->data_.resize(vec.size() * 2);
				} else {
					matrix_->data_.resize(vec.size() + 1);
				}
			}
		} else {
			//Add an element after each "row"
			typename vector<T>::const_iterator itfrom = vec.begin();
			typename vector<T>::iterator itto = matrix_->data_.begin();
			while (itfrom != vec.end()) {
				itto+=contra_->stride_;
				itto=matrix_->data_.insert(itto, *itfrom);
				itto++;
				itfrom++;
			}
			contra_->stride_ = count_+1;
			//Add deadWeight
			matrix_->data_.resize(matrix_->data_.size()+1);
		}
		//Update iterator administration
		count_++;
		last_++;
		matrix_->invalidate_();
	};


	template <typename T, typename TInt>
	void Matrix<T, TInt>::Index::add(const Matrix<T, TInt> &mat) {
		static const char * f = "Index::add(Matrix)";

		if (mat.order_ != matrix_->order_)
			throw Error(ecIncompatible, f, SS("Incompatible order:" << etos(matrix_->order_) << " and " << etos(mat.order())));

		if (!mat.isInitialized())
			return;

		if (!matrix_->isInitialized()) {//Uninitialized matrix
			matrix_->clone(mat);
			//*matrix_ = mat.dup();
			return;
		}

		if (isRow() && matrix_->cols.count_ != mat.cols.count_)
			throw Error(ecSizeMismatch, f, SS("Column count mismatch:" << matrix_->cols.count_ << " and " << mat.cols.count_));

		if (isCol() && matrix_->rows.count_ != mat.rows.count_)
			throw Error(ecSizeMismatch, f, SS("Row count mismatch:" << matrix_->rows.count_ << " and " << mat.rows.count_));

		this->matrix_->breakUpDown(const_cast<Matrix<T, TInt>&>(mat));
		matrix_->breakUp();

		if (isAligned()) {
			Matrix<T, TInt> matcpy = mat.dup();
			matrix_->data_.insert(matrix_->data_.begin() + count_ * contra_->count_, matcpy.begin(), matcpy.end());
			count_ += mat.getCount(*this);
			last_ = count_;
			matrix_->invalidate_();
		} else {
			TInt contraCount = mat.getContraCount(*this);
			TInt count = mat.getCount(*this);
			TInt contraStride = mat.getContraStride(*this);
			TInt myCount = count_;
			TInt myContraCount = contra_->count_;

			Matrix<T, TInt> org(matrix_->rows.count(), matrix_->cols.count());
			org = *matrix_;

			if (this->isCol()) {
				matrix_->clear();
				matrix_->resize(myContraCount, myCount + count);
				Matrix<T, TInt> slice1;
				matrix_->slice(slice1, 0, myContraCount, 0, myCount);
				slice1 = org; //direct slice assignment
				Matrix<T, TInt> slice2;
				matrix_->slice(slice2, 0, myContraCount, myCount, myCount + count);
				slice2 = mat;//.dup(); //direct slice assignment
				matrix_->invalidate_();
			} else {
				//cout << "TODO: Speedup, just like this->isCol()" ;
				TInt dstIdx = contra_->stride_;
				typename vector<T>::const_iterator srcIt = mat.begin();
				for (TInt i=0;i<contraCount;i++) {
					matrix_->data_.insert(matrix_->begin() + dstIdx, srcIt, srcIt + count);
					srcIt += contraStride;
					dstIdx += contra_->stride_ + count;
				}
				contra_->stride_ += count;

				//add deadWeight
				matrix_->data_.resize(matrix_->data_.size() + count);

				//Generic final step
				count_ += mat.getCount(*this);
				last_ = count_;
				matrix_->invalidate_();
			}
		}
	}

	template<typename T, typename TInt>
	void Matrix<T, TInt>::Index::take(const Matrix<T, TInt>& mat) {
		TInt cnt = count_;
		add(mat);
		if ((matrix_->up_ == nullptr) && (mat.up_ == nullptr || mat.isFullSlice())) {
			Matrix<T, TInt> * mat2;
			if (mat.isFullSlice())
				mat2 = const_cast<Matrix<T, TInt>&>(mat).up_; //In this case mat is handled by mat.up_.down_
			else
				mat2 = &(const_cast<Matrix<T, TInt>&>(mat));

			//move each down slice to this
			for (auto m = mat2->down_.begin();m!=mat2->down_.end();m++) {
				(*m)->up_ = this->matrix_;
				if (isCol())
					(*m)->cols.first_ += cnt;
				else
					(*m)->rows.first_ += cnt;
				matrix_->down_.push_back(*m);
				(*m)->invalidate_();
			}

			//move mat to this
			mat2->down_.clear();
			mat2->up_ = this->matrix_;
			if (isCol())
				mat2->cols.first_ += cnt;
			else
				mat2->rows.first_ += cnt;
			matrix_->down_.push_back(mat2);
			mat2->invalidate_();
		}
	}

	template<typename T, typename TInt>
	bool Matrix<T, TInt>::Index::sort(const Matrix<TInt, TInt> &indices) {
		//Check if sorting is necessary
		if (!indices.isVector())
			throw Error(ecIncompatible, "Index::sort()", "indices should be a vector");
		if (indices.vec().count() != count())
			throw Error(ecIncompatible, "Index::sort()", SS("indices count and Index count mismatch (" << indices.vec().count() << " != " << count() << ")"));
		for (TInt i=0;i<indices.vec().count()-1;i++) {
			if (*(indices.vec()[i]) != *(indices.vec()[i+1])-1)
				break;
			return false;
		}

		//If this matrix is already a slice, this will be faster
		Matrix<T, TInt> srcMat(*(this->matrix_)); //Make a reference. Will only take over root reference (fast)
		this->matrix_->breakUp(false); //Break ties without copying (fast)
		this->matrix_->breakDown(); //If this is a root matrix, the actual copy to srcMat will be made here (slow)
		this->matrix_->resize(srcMat.rows.count(), srcMat.cols.count());
		TInt dstIdx =0;
		//TODO: Should be tested changed to vec()
		if (isRow()) {
			for (auto srcIdx=indices.vec().begin();srcIdx!=indices.vec().end();srcIdx++,dstIdx++) {
				std::copy(srcMat.rows[*srcIdx]->begin(), srcMat.rows[*srcIdx]->end(), this->matrix_->rows[dstIdx]->begin());
			}
		} else if (isCol()) {
			for (auto srcIdx=indices.vec().begin();srcIdx!=indices.vec().end();srcIdx++,dstIdx++) {
				std::copy(srcMat.cols[*srcIdx]->begin(), srcMat.cols[*srcIdx]->end(), this->matrix_->cols[dstIdx]->begin());
			}
		}
		return true;
	}

	//Matrix
	template<typename T, typename TInt>
	Matrix<T, TInt>::Matrix() {
		init_();
		order_ = oRowMajor;
		updateIt_();
	}

	template<typename T, typename TInt>
	Matrix<T, TInt>::Matrix(Order order) {
		init_();
		order_ = order;
		updateIt_();
	}

	template<typename T, typename TInt>
	Matrix<T, TInt>::Matrix(const std::vector<T> &vec, DSTypes::Order order) {
		init_();
		order_ = order;
		updateIt_();
		if (order_ == oRowMajor)
			rows.add(vec);
		else
			cols.add(vec);
	}

	template<typename T, typename TInt>
	Matrix<T, TInt>::Matrix(const TInt rowCount, const TInt colCount, DSTypes::Order order) {
		init_();
		order_ = order;
		resize(rowCount, colCount);
	}

	template<typename T, typename TInt>
	Matrix<T, TInt>::Matrix(const TInt rowCount, const TInt colCount, const T & initVal, DSTypes::Order order) {
		init_();
		order_ = order;
		resize(rowCount, colCount);
		for (auto it = data_.begin(); it != data_.end(); it++)
			*it = initVal;
	}

	template<typename T, typename TInt>
	Matrix<T, TInt>::Matrix(const TInt rowCount, const TInt colCount, const T* initArr, DSTypes::Order order) {
		init_();
		order_ = order;
		resize(rowCount, colCount);
        std::copy(std::begin(initArr), std::end(initArr), data_.begin());
	}

	template<typename T, typename TInt>
	Matrix<T, TInt>::Matrix(const TInt rowCount, const TInt colCount, const vector<T> initVec, DSTypes::Order order) {
		init_();
		order_ = order;
		resize(rowCount, colCount);
        std::copy(initVec.begin(), initVec.end(), data_.begin());
	}

	template<typename T, typename TInt>
	Matrix<T, TInt>::Matrix(const Matrix &mat) {
		init_();
		clone(mat);
	}

	template<typename T, typename TInt>
	Matrix<T, TInt>::Matrix(Matrix && mat) {
		init_();
		clone(std::move(mat));
	}

	template<typename T, typename TInt>
	Matrix<T, TInt>::Matrix(const MatrixBase & matbase) {
		try {
			const Matrix<T, TInt> & mat = dynamic_cast<const Matrix<T, TInt> &>(matbase);
			init_();
			clone(mat);
		} catch (const std::bad_cast &) {
			throw Error(ecIncompatible, "Matrix(MatrixBase &)", SS("Bad cast while casting " << typeid(matbase).name() << " to " << typeid(*this).name()));
		}
	}

	template<typename T, typename TInt>
	Matrix<T, TInt>::Matrix(MatrixBase && matbase) {
		try {
			Matrix<T, TInt> && mat = dynamic_cast<Matrix<T, TInt> &&>(matbase);
			init_();
			clone(std::move(mat));
		}
		catch (const std::bad_cast &) {
			throw Error(ecIncompatible, "Matrix(MatrixBase &)", SS("Bad cast while casting " << typeid(matbase).name() << " to " << typeid(*this).name()));
		}
	}

	template <typename T, typename TInt>
	void Matrix<T, TInt>::clear() {
		breakDown();
		breakUp(false);
		//Zero all rows and columns
		rows.count_ = 0;
		rows.stride_ = 0;
		cols.count_ = 0;
		cols.stride_ = 0;
		rows.first_ = 0;
		rows.last_ = 0;
		cols.first_ = 0;
		cols.last_ = 0;
		//Clear actual data
		data_.clear();
		//Update iterators
		updateIt_();
	}

	template <typename T, typename TInt>
	void Matrix<T, TInt>::clearAll() {
		//Clear entire shallow tree and don't copy any data
		if (up_ != nullptr)
			up_->clearAll();
		else {
			breakDown(false);
			//Zero all rows and columns
			rows.count_ = 0;
			rows.stride_ = 0;
			cols.count_ = 0;
			cols.stride_ = 0;
			rows.first_ = 0;
			rows.last_ = 0;
			cols.first_ = 0;
			cols.last_ = 0;
			//Clear actual data
			/*
			if (std::is_base_of<MatrixBase, T>::value) {
				for (auto it=data_.begin();it!=data_.end();it++) {
					reinterpret_cast<MatrixBase&>(*it).clearAll();
				}
			}*/
			data_.clear();
			//Update iterators
			updateIt_();
		}
	}

	template<typename T, typename TInt>
	Matrix<T, TInt>::~Matrix() {
		breakDown();
		breakUp(false);
		data_.clear();
	}

	template<typename T, typename TInt>
	CellsContinuous<T, TInt> &Matrix<T, TInt>::vec() {
		if (rows.count_ > 1 && cols.count_ > 1)
			throw Error(ecIncompatible, "Matrix<T, TInt>::vector()", "Matrix contains multiple vectors.");

		if (!isInitialized() || rows.count() == 0 || cols.count() == 0) {
			rows.cellsContinuous_.setValues(data_.end(), data_.end(), 0, 0, 0, 0);
		} else if (order_ == oRowMajor) {
			if (rows.count_ == 1) {
				cols.cellsContinuous_.setValues(at(0, 0), at(rows.count_ - 1, cols.count_ - 1) + cols.stride_, cols.stride_, rows.stride_, cols.count_, rows.count_);
				return cols.cellsContinuous_;
			} else if (cols.count_ == 1) {
				rows.cellsContinuous_.setValues(at(0, 0), at(rows.count_ - 1, cols.count_ - 1) + rows.stride_, rows.stride_, cols.stride_, rows.count_, cols.count_);
				return rows.cellsContinuous_;
			}
		} else if (order_ == oColMajor) {
			if (rows.count_ == 1) {
				cols.cellsContinuous_.setValues(at(0, 0), at(rows.count_ - 1, cols.count_ - 1) + cols.stride_, cols.stride_, rows.stride_, cols.count_, rows.count_);
				return cols.cellsContinuous_;
			} else if (cols.count_ == 1) {
				rows.cellsContinuous_.setValues(at(0, 0), at(rows.count_ - 1, cols.count_ - 1) + rows.stride_, rows.stride_, cols.stride_, rows.count_, cols.count_);
				return rows.cellsContinuous_;
			}
		}
		return rows.cellsContinuous_;
	}

	template<typename T, typename TInt>
	CellsContinuous<T, TInt> &Matrix<T, TInt>::vec() const {
		//ugly but effective. Saves many const_casts along the way
		return const_cast<Matrix<T, TInt>*>(this)->vec();
	}

	template<typename T, typename TInt>
	const T & Matrix<T, TInt>::vec(const TInt index) const {
		if (isRowVector())
			return *(getData() + (index * cols.stride_));
		else if (isColVector())
			return *(getData() + (index * rows.stride_));
		else
			throw Error(ecIncompatible, "vec(TInt)", "This is not a vector");
	}

	template<typename T, typename TInt>
	T & DSLib::Matrix<T, TInt>::vec(const TInt index) {
		if (isRowVector())
			return *(getData() + (index * cols.stride_));
		else if (isColVector())
			return *(getData() + (index * rows.stride_));
		else
			throw Error(ecIncompatible, "vec(TInt)", "This is not a vector");
	}

	template<typename T, typename TInt>
	const T & Matrix<T, TInt>::val(const TInt row, const TInt col) const {
		return *(getData() + (row * rows.stride_ + col * cols.stride_));
	}

	template<typename T, typename TInt>
	T & Matrix<T, TInt>::val(const TInt row, const TInt col) {
		return *(getData() + (row * rows.stride_ + col * cols.stride_));
	}

	template<typename T, typename TInt>
	void Matrix<T, TInt>::slice(MatrixBase &dst, MatrixBase &rowFirst, MatrixBase &rowLast, MatrixBase &colFirst, MatrixBase &colLast) {
		Matrix<TInt, TInt> * rowfirst = &(dynamic_cast<Matrix<TInt, TInt> &> (rowFirst));
		Matrix<TInt, TInt> * rowlast = &(dynamic_cast<Matrix<TInt, TInt> &> (rowLast));
		Matrix<TInt, TInt> * colfirst = &(dynamic_cast<Matrix<TInt, TInt> &> (colFirst));
		Matrix<TInt, TInt> * collast = &(dynamic_cast<Matrix<TInt, TInt> &> (colLast));
		Matrix<T, TInt> * dest =  &(dynamic_cast<Matrix<T, TInt> &> (dst));
		if (rowfirst == nullptr || rowlast == nullptr || colfirst == nullptr || collast == nullptr)
			throw Error(ecIncompatible, "slice(MatrixBase)", "Incompatible types");
		slice(*dest, *(rowfirst->vec()[0]), *(rowlast->vec()[0]), *(colfirst->vec()[0]), *(collast->vec()[0]));
	}

	template<typename T, typename TInt>
	void Matrix<T, TInt>::slice(MatrixBase &dst, MatrixBase &rowIndices, MatrixBase &colIndices) {
		Matrix<TInt, TInt> * rowi = &(dynamic_cast<Matrix<TInt, TInt> &> (rowIndices));
		Matrix<TInt, TInt> * coli = &(dynamic_cast<Matrix<TInt, TInt> &> (colIndices));
		Matrix<T, TInt> * dest = &(dynamic_cast<Matrix<T, TInt> &> (dst));
		if (coli == nullptr || rowi == nullptr)
			throw Error(ecIncompatible, "slice(MatrixBase, MatrixBase, MatrixBase)", "Incompatible types");
		slice(*dest, *rowi, *coli);
	}

	template<typename T, typename TInt>
	void Matrix<T, TInt>::slice(MatrixBase &dst, MatrixBase &indices, MatrixBase &first, MatrixBase &last, bool rowIndices) {
		Matrix<TInt, TInt> * i = &(dynamic_cast<Matrix<TInt, TInt> &> (indices));
		Matrix<TInt, TInt> * f = &(dynamic_cast<Matrix<TInt, TInt> &> (first));
		Matrix<TInt, TInt> * l = &(dynamic_cast<Matrix<TInt, TInt> &> (last));
		Matrix<T, TInt> * d = &(dynamic_cast<Matrix<T, TInt> &> (dst));
		if (i == nullptr || f == nullptr || l == nullptr)
			throw Error(ecIncompatible, "slice(MatrixBase, MatrixBase, bool)", "Incompatible types");
		if (rowIndices)
			*d = (*this)(*i, *(f->vec()[0]), *(l->vec()[0]) - *(f->vec()[0]));
		else
			*d = (*this)(*(f->vec()[0]), *(l->vec()[0]) - *(f->vec()[0]), *i);
	}

	template <typename T, typename TInt>
	TInt Matrix<T, TInt>::getContraCount(const typename Matrix<T, TInt>::Index &idx) const {
		if (idx.isRow())
			return (cols.count_);
		else
			return (rows.count_);
	}

	template <typename T, typename TInt>
	TInt Matrix<T, TInt>::getCount(const typename Matrix<T, TInt>::Index &idx) const {
		if (idx.isRow())
			return (rows.count_);
		else
			return (cols.count_);
	}

	template <typename T, typename TInt>
	TInt Matrix<T, TInt>::getStride(const typename Matrix<T, TInt>::Index &idx) const {
		if (idx.isRow())
			return (rows.stride_);
		else
			return (cols.stride_);
	}

	template <typename T, typename TInt>
	TInt Matrix<T, TInt>::getContraStride(const typename Matrix<T, TInt>::Index &idx) const {
		if (idx.isRow())
			return (cols.stride_);
		else
			return (rows.stride_);
	}

	template <typename T, typename TInt>
	void Matrix<T, TInt>::setOrder(Order order) {
		clear();
		order_ = order;
	}

	template <typename T, typename TInt>
	Order Matrix<T, TInt>::order() const {return order_;}


	template<typename T, typename TInt>
	void Matrix<T, TInt>::toStringMatrix(Matrix<DSTypes::String, TInt>& dst) {
		convert(dst, *this);
	}

	template<typename T, typename TInt>
	void Matrix<T, TInt>::write(const String filename) const {
		ofstream f(filename, fstream::binary);
		if (!f)
			throw Error(ecGeneral, "Matrix::write", "Unable to open file for writing " + filename);
		write(f);
	}

	template<typename T, typename TInt>
	void  Matrix<T, TInt>::write(std::ostream & output) const {
		DSUtil::write(output, (String)"Matrix");
		DSUtil::write(output, this->getDataType());
		DSUtil::write(output, this->getIntDataType());
		DSUtil::write(output, this->order_);
		DSUtil::write(output, this->rows.count_);
		DSUtil::write(output, this->cols.count_);
		Matrix<T, TInt> * t = const_cast<Matrix<T, TInt>*>(this);
		if (order_ == oRowMajor) {
			for (auto it=t->rows->begin();it!=t->rows->end();it++)
				DSUtil::write(output, *it);
		} else {
			for (auto it = t->cols->begin();it != t->cols->end();it++)
				DSUtil::write(output, *it);
		}
	}

	template<typename T, typename TInt>
	void Matrix<T, TInt>::read(const String filename) {
		ifstream f(filename, fstream::binary);
		if (!f)
			throw Error(ecGeneral, "Matrix::write", "Unable to open file for reading " + filename);
		read(f);
	}

	template<typename T, typename TInt>
	void Matrix<T, TInt>::read(std::istream & input) {
		if (!input)
			throw (Error(ecNotFound, "Matrix::read", "stream not open or no data left"));
		String type;
		DataType dt, dt2;
		DSUtil::read(input, type);
		if (type != "Matrix")
			throw Error(ecIncompatible, "Matrix::read", SS("File does not contain a matrix but a: " << type));
		DSUtil::read(input, dt);
		DSUtil::read(input, dt2);
		T d1;
		TInt d2;
		if (!dataTypeEqual(dt, d1))
			throw Error(ecIncompatible, "Matrix::read", SS("Wrong DataType in file. Matrix type is " << typeName(d1) << " while file contains " << etos(dt)));
		if (!dataTypeEqual(dt2, d2))
			throw Error(ecIncompatible, "Matrix::read", SS("Wrong DataType in file. TInt type of Matrix is " << typeName(d2) << " while contains " << etos(dt2)));
		clear();
		DSUtil::read(input, order_);
		TInt rc, cc;
		DSUtil::read(input, rc);
		DSUtil::read(input, cc);
		resize(rc, cc);
		if (order_ == oRowMajor) {
			for (auto it = rows->begin();it != rows->end();it++)
				DSUtil::read(input, *it);
		} else {
			for (auto it = cols->begin();it != cols->end();it++)
				DSUtil::read(input, *it);
		}
	}

	template <typename T, typename TInt>
	void Matrix<T, TInt>::print(ostream &output) const {
		output << "/*Matrix*/" << endl;
		Matrix<T, TInt> & m = const_cast<Matrix<T, TInt>&>(*this);
		TInt cnt = 0;
		if (order_ == oRowMajor) {
			for (auto row = m.rows.begin(); row != m.rows.end(); row++,cnt++) {
				output << "(dt" << etos(getDataType());
				for (auto cell=row->begin(); cell!=row->end();cell++) {
					output << " | ";
					DSUtil::fmt(output, *cell);
				}
				output << ")";
				if (cnt != m.rows.count()-1)
					output << " ^ " << endl;
				else
					output << endl;
			}
		} else if (order_ == oColMajor) {
			for (auto col = m.cols.begin(); col != m.cols.end(); col++,cnt++) {
				output << ((cols.count() != 1) ? "(" : "");
				output << "dt" << etos(getDataType());
				for (auto cell = col->begin(); cell != col->end();cell++) {
					output << " ^ ";
					DSUtil::fmt(output, *cell);
				}
				output << ((cols.count() != 1) ? ")" : "");
				if (cnt != m.cols.count()-1)
					output << " | " << endl;
				else
					output << endl;
			}
		}
	}

	template <typename T, typename TInt>
	std::string Matrix<T, TInt>::print() const {
		stringstream ss;
		print(ss);
		return ss.str();
	}

	template <typename T, typename TInt>
	void Matrix<T, TInt>::printLess(ostream &output)  const {
		stringstream ss;
		T t;
		TInt tInt;
		output << "DataType: " << typeName(t) << "(" << typeName(tInt) << ")" << endl;
		output << "Order: "  << etos(order_) << endl;
		output << "Rows x Cols: "  << rows.count_ << " x " << cols.count_ << endl;
		if (up_ != nullptr)
			output << "up.Rows x up.Cols: "  << up_->rows.count_ << " x " << up_->cols.count_ << endl;
	}

	template <typename T, typename TInt>
	std::string Matrix<T, TInt>::printLess()  const {
		stringstream ss;
		printLess(ss);
		return ss.str();
	}

	template <typename T, typename TInt>
	std::string Matrix<T, TInt>::printSize() const {
		return SS("(" << rows.count() << "x" << cols.count() << ")");
	}

	//** DSLang **//
	template <typename T, typename TInt>
	void Matrix<T, TInt>::isEqual(MatrixBase &dst, const MatrixBase &other) const {
		DSLib::Matrix<T, TInt> * lhs = const_cast<DSLib::Matrix<T, TInt> *> (this);
		DSLib::Matrix<T, TInt> * rhs = static_cast<DSLib::Matrix<T, TInt> *>(const_cast<MatrixBase *> (&other));
		static_cast<Matrix<TInt, TInt> &>(dst) = *lhs == *rhs;
	}

	template <typename T, typename TInt>
	void Matrix<T, TInt>::isNotEqual(MatrixBase &dst, const MatrixBase &other) const {
		DSLib::Matrix<T, TInt> * lhs = const_cast<DSLib::Matrix<T, TInt> *> (this);
		DSLib::Matrix<T, TInt> * rhs = static_cast<DSLib::Matrix<T, TInt> *>(const_cast<MatrixBase *> (&other));
		static_cast<Matrix<TInt, TInt> &>(dst) = *lhs != *rhs;
	}

	template <typename T, typename TInt>
	void Matrix<T, TInt>::isGreater(MatrixBase &dst, const MatrixBase &other) const {
		DSLib::Matrix<T, TInt> * lhs = const_cast<DSLib::Matrix<T, TInt> *> (this);
		DSLib::Matrix<T, TInt> * rhs = static_cast<DSLib::Matrix<T, TInt> *>(const_cast<MatrixBase *> (&other));
		static_cast<Matrix<TInt, TInt> &>(dst) = *lhs > *rhs;
	}

	template <typename T, typename TInt>
	void Matrix<T, TInt>::isSmaller(MatrixBase &dst, const MatrixBase &other) const {
		DSLib::Matrix<T, TInt> * lhs = const_cast<DSLib::Matrix<T, TInt> *> (this);
		DSLib::Matrix<T, TInt> * rhs = static_cast<DSLib::Matrix<T, TInt> *>(const_cast<MatrixBase *> (&other));
		static_cast<Matrix<TInt, TInt> &>(dst) = *lhs < *rhs;
	}

	template <typename T, typename TInt>
	void Matrix<T, TInt>::isGreaterEqual(MatrixBase &dst, const MatrixBase &other) const {
		DSLib::Matrix<T, TInt> * lhs = const_cast<DSLib::Matrix<T, TInt> *> (this);
		DSLib::Matrix<T, TInt> * rhs = static_cast<DSLib::Matrix<T, TInt> *>(const_cast<MatrixBase *> (&other));
		static_cast<Matrix<TInt, TInt> &>(dst) = *lhs >= *rhs;
	}

	template <typename T, typename TInt>
	void Matrix<T, TInt>::isSmallerEqual(MatrixBase &dst, const MatrixBase &other) const {
		DSLib::Matrix<T, TInt> * lhs = const_cast<DSLib::Matrix<T, TInt> *> (this);
		DSLib::Matrix<T, TInt> * rhs = static_cast<DSLib::Matrix<T, TInt> *>(const_cast<MatrixBase *> (&other));
		static_cast<Matrix<TInt, TInt> &>(dst) = *lhs <= *rhs;
	}

	template <typename T, typename TInt>
	Matrix<T, TInt> & Matrix<T, TInt>::operator=(const T &val) {
		//Can be more efficient, by checking for CellsContinuous
		if (order() == oRowMajor) {
			for (auto c = rows->begin(); c != rows->end(); c++) {
				*c = val;
			}
		} else {
			for (auto c = cols->begin(); c != cols->end(); c++) {
				*c = val;
			}
		}
		return *this;
	}

	template <typename T, typename TInt>
	Matrix<T, TInt> & Matrix<T, TInt>::operator=(const Matrix<T, TInt> &other) {
		if (this == &other)
			return *this;
		//Direct slice assignment
		if (isInitialized() && this->rows.count() == other.rows.count() && this->cols.count() == other.cols.count()) {
			Matrix<T, TInt> * matp = const_cast<Matrix<T, TInt> *>(&other);
			if (order_ == oRowMajor)
				std::copy(matp->rows->begin(), matp->rows->end(), this->rows->begin());
			else
				std::copy(matp->cols->begin(), matp->cols->end(), this->cols->begin());
			return *this;
		}
		clone(other);
		return *this;
	}

	template<typename T, typename TInt>
	Matrix<T, TInt>& Matrix<T, TInt>::operator=(Matrix<T, TInt>&& other) {
		if (this == &other)
			return *this;
		//Direct slice assignment (TODO: move data when appropriate)
		if (isInitialized() && this->rows.count() == other.rows.count() && this->cols.count() == other.cols.count()) {
			Matrix<T, TInt> * matp = const_cast<Matrix<T, TInt> *>(&other);
			if (order_ == oRowMajor)
				std::copy(matp->rows->begin(), matp->rows->end(), this->rows->begin());
			else
				std::copy(matp->cols->begin(), matp->cols->end(), this->cols->begin());
			return *this;
		}

		clone(std::move(other));
		return *this;
	}

	template <typename T, typename TInt>
	Matrix<T, TInt> Matrix<T, TInt>::operator()(const TInt row) {
		Matrix<T, TInt> dst(order_);
		slice(dst, row, row+1, 0, cols.count_);
		return dst;
	}

	template <typename T, typename TInt>
	Matrix<T, TInt> Matrix<T, TInt>::operator()(const TInt rowBegin, const TInt rowLength) {
		Matrix<T, TInt> dst(order_);
		slice(dst, rowBegin, rowBegin + rowLength, 0, cols.count_);
		return dst;
	}

	template <typename T, typename TInt>
	Matrix<T, TInt> Matrix<T, TInt>::operator()(const TInt rowBegin, const TInt rowLength, const TInt col) {
		Matrix<T, TInt> dst(order_);
		slice(dst, rowBegin, rowBegin + rowLength, col, col+1);
		return dst;
	}

	template <typename T, typename TInt>
	Matrix<T, TInt> Matrix<T, TInt>::operator()(const TInt rowBegin, const TInt rowLength, const TInt colBegin, const TInt colLength) {
		Matrix<T, TInt> dst(order_);
		slice(dst, rowBegin, rowBegin + rowLength, colBegin, colBegin + colLength);
		return dst;
	}

	template <typename T, typename TInt>
	Matrix<T, TInt> Matrix<T, TInt>::operator()(const Matrix<TInt, TInt> &rowIndices, const Matrix<TInt, TInt> &colIndices) {
		Matrix<T, TInt> dst(order_);
		slice(dst, rowIndices, colIndices);
		return dst;
	}

	template <typename T, typename TInt>
	Matrix<T, TInt> Matrix<T, TInt>::operator[](const TInt col) {
		Matrix<T, TInt> dst(order_);
		slice(dst, 0, rows.count_, col, col+1);
		return dst;
	}

	template <typename T, typename TInt>
	Matrix<T, TInt> Matrix<T, TInt>::operator[](const Matrix<TInt, TInt> &colIndices) {
		Matrix<T, TInt> dst(order_);
		slice(dst, Matrix<TInt, TInt>(), colIndices);
		return dst;
	}

	template <typename T, typename TInt>
	Matrix<T, TInt> Matrix<T, TInt>::operator()(const Matrix<TInt, TInt> &rowIndices) {
		Matrix<T, TInt> dst(order_);
		slice(dst, rowIndices, Matrix<TInt, TInt>());
		return dst;
	}

	template <typename T, typename TInt>
	Matrix<T, TInt> Matrix<T, TInt>::operator()(const Matrix<TInt, TInt> &rowIndices, const TInt col) {
		if (!rowIndices.isVector())
			throw Error(ecIncompatible, "Matrix<T>::operator()", "!indices.isVector()");
		Matrix<T, TInt> dst(order_);
		TInt first, last;
		if (rowIndices.isSlicable(first, last)) {
			slice(dst, first, last+1, col, col+1);
		} else {
			for (auto ri = rowIndices.vec().begin(); ri != rowIndices.vec().end();ri++) {
				dst.rows.add((*this)(*ri, col));
			}
		}
		return dst;
	}

	template <typename T, typename TInt>
	Matrix<T, TInt> Matrix<T, TInt>::operator()(const Matrix<TInt, TInt> &rowIndices, const TInt colBegin, const TInt colLength) {
		if (!rowIndices.isVector())
			throw Error(ecIncompatible, "Matrix<T>::operator()", "!indices.isVector()");
		Matrix<T, TInt> dst(order_);
		TInt first, last;
		if (rowIndices.isSlicable(first, last)) {
			slice(dst, first, last+1, colBegin, colBegin+colLength);
		} else {
			dst.resize(rowIndices.vec().count(), colLength - colBegin);
			auto dstrow = dst.rows.begin();
			for (auto ri = rowIndices.vec().begin(); ri != rowIndices.vec().end();ri++, dstrow++) {
				std::copy(this->rows[*ri][colBegin], this->rows[*ri][colBegin+colLength], dstrow->begin());
				//dst.rows.add((*this)(*ri, 1, colBegin, colLength));
			}
		}
		return dst;
	}

	template <typename T, typename TInt>
	Matrix<T, TInt> Matrix<T, TInt>::operator()(const TInt row, const Matrix<TInt, TInt> &colIndices) {
		if (!colIndices.isVector())
			throw Error(ecIncompatible, "Matrix<T>::operator()", "!indices.isVector()");
		Matrix<T, TInt> dst(order_);
		TInt first, last;
		if (colIndices.isSlicable(first, last)) {
			slice(dst, row, row+1, first, last+1);
		} else {
			for (auto ci = colIndices.vec().begin(); ci != colIndices.vec().end();ci++) {
				dst.cols.add((*this)(row, 1, *ci));
			}
		}
		return dst;
	}

	template <typename T, typename TInt>
	Matrix<T, TInt> Matrix<T, TInt>::operator()(const TInt rowBegin, const TInt rowLength, const Matrix<TInt, TInt> &colIndices) {
		if (!colIndices.isVector())
			throw Error(ecIncompatible, "Matrix<T>::operator()", "!indices.isVector()");
		Matrix<T, TInt> dst(order_);
		TInt first, last;
		if (colIndices.isSlicable(first, last)) {
			slice(dst, rowBegin, rowBegin+rowLength, first, last+1);
		} else {
			for (auto ci = colIndices.vec().begin(); ci != colIndices.vec().end();ci++) {
				dst.cols.add((*this)(rowBegin, rowLength, *ci));
			}
		}
		return dst;
	}

	//Implicit cast to scalar
	#ifndef DS_DISABLE_IMPLICIT_CONVERSIONS
	template<typename T, typename TInt>
	Matrix<T, TInt>::operator T() {
		if (rows.count() == 1 || cols.count() == 1) {
			#ifdef _DEBUG
				T d;
				cout << "Note: Implicit conversion from DSLib::Matrix to " << typeName(d) << endl;
			#endif
			return *(rows[0][0]);
		} else {
			T dummy;
			throw (Error(ecRangeError, SS("operator (" << typeName(dummy) << ")"), "Invalid implicit convert: Wrong type or not a micro slice / scalar."));
		}
	}
	#endif


};

