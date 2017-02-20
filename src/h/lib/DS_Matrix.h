#pragma once

#ifdef MSC_VER
  #pragma warning(push)
  #pragma warning(disable: 4521)
  #pragma warning(disable: 4522)
#endif

#include "h/DS_Types.h"
//CellsContinuous is more efficient than CellsRegion, but is not region/slice aware
//Cell iterators traverse over cells
//
//Matrix.rows[r][c] //get CellsContinuous Cell iterator from row begin to row end
//Matrix.cols[c][r] //get CellsContinuous Cell iterator from col begin to col end

//Matrix.rows[r]->begin() & Matrix.rows[r]->end() //get CellsContinuous Cell iterator from row begin to row end
//Matrix.col[r]->begin() & Matrix.col[r]->end() //get CellsContinuous Cell iterator from col begin to col end

//Matrix.rows->begin() & Matrix.rows.end() //get CellsRegion Cell iterator from first row begin to last row end. Iterator traverses rows
//Matrix.cols->begin() & Matrix.cols.end() //get CellsRegion Cell iterator from first col begin to last col end. Iterator traverses cols

//Index iterators traverse of indices (rows or cols)
//
//Matrix.rows.begin() & Matrix.rows.end() //get Index iterator from first row to last row
//Matrix.cols.begin() & Matrix.cols.end() //get Index iterator from first col to last col

//Matrix.rows.begin()->begin() //same as Matrix.rows[0]->begin()
//Matrix.rows.end()->end() //same as Matrix.rows[0]->end()

//Possible pitfall
//
//Matrix.rows[0]->begin() & Matrix.rows[5]->end() //Traversing will not always give the desired effect (CellsContinuous iterators are not region aware)
//Matrix.rows[0].cell(citRegion).begin() & Matrix.rows[5].cell(citRegion).end() //Traversing will give the desired effect (CellsRegion iterators are region aware)

namespace DSLib {
	template <typename T, typename TInt = DSTypes::MatrixIdx>
	class CellsBase {
	public:
		class IteratorBase : public std::iterator<std::forward_iterator_tag, T>{
		friend class CellsBase<T, TInt>;
		public:
			IteratorBase();
			IteratorBase(const TInt stride, const TInt contraStride, const TInt count, const TInt contraCount, const typename std::vector<T>::iterator &cursor);
			~IteratorBase();

			IteratorBase& operator=(const IteratorBase& other);			
			bool operator==(const IteratorBase& other) const;
			bool operator!=(const IteratorBase& other) const;

			T& operator*();
			T* operator->();
		protected:
			TInt stride_; //copy from Index
			TInt contraStride_; //copy from Index
			TInt count_; //copy from Index
			TInt contraCount_; //copy from Index
			TInt counter_; //major counter
			TInt counter2_; //minor counter
			typename std::vector<T>::iterator cursor_;
		};
		void setValues(const typename std::vector<T>::iterator &begin, const typename std::vector<T>::iterator &end, const TInt stride, const TInt contraStride, const TInt count, const TInt contraCount);
		TInt count();		
		TInt contraCount();

		CellsBase();
		CellsBase(const typename std::vector<T>::iterator &begin, const typename std::vector<T>::iterator &end, const TInt stride, const TInt contraStride, const TInt count, const TInt contraCount);
		virtual ~CellsBase(){};
	protected:						
		typename std::vector<T>::iterator begin_; //begin of data
		typename std::vector<T>::iterator end_; //end of of data
		TInt stride_; //stride when ++
		TInt contraStride_; //stride to skip a row/column
		TInt count_; //end_ - begin_
		TInt contraCount_;
	};	
	
	template <typename T, typename TInt = DSTypes::MatrixIdx>
	class CellsRegion : public CellsBase<T, TInt> {
	public:
		class Iterator : public CellsBase<T, TInt>::IteratorBase {
		friend class CellsRegion<T, TInt>;
		public:				
			Iterator();
			Iterator(const Iterator &other);

			Iterator(const TInt stride, const TInt contraStride, const TInt count, const TInt contraCount, const typename std::vector<T>::iterator &cursor);
			~Iterator();
			
			Iterator& operator++();
			Iterator operator++(int);
		};
		CellsRegion();
		CellsRegion(const typename std::vector<T>::iterator &begin, const typename std::vector<T>::iterator &end, const TInt stride, const TInt contraStride, const TInt count, const TInt contraCount);

		T & val(const TInt index) { return *((*this)[index]); }
		T & operator()(const TInt index) { return val(index); }
		T * operator->() { return &(val(0)); }
		T & operator*() { return val(0); }

		Iterator operator[](const TInt index);
		Iterator begin();
		Iterator end();
	};

	template <typename T, typename TInt = DSTypes::MatrixIdx>
	class CellsContinuous : public CellsBase<T, TInt> {
	public:
		class Iterator : public CellsBase<T, TInt>::IteratorBase {
		friend class CellsContinuous<T, TInt>;
		public:				
			Iterator();
			Iterator(const Iterator &other);

			Iterator(const TInt stride, const TInt contraStride, const TInt count, const TInt contraCount, const typename std::vector<T>::iterator &cursor);
			~Iterator();

			Iterator& operator++();
			Iterator operator++(int);
		};
		CellsContinuous();
		CellsContinuous(const typename std::vector<T>::iterator &begin, const typename std::vector<T>::iterator &end, const TInt stride, const TInt contraStride, const TInt count, const TInt contraCount);		
		
		T & val(const TInt index) { return *((*this)[index]); }
		T & operator()(const TInt index) { return val(index); }
		T * operator->() { return &(val(0)); }
		T & operator*() { return val(0); }

		Iterator operator[](const TInt index);
		Iterator begin();
		Iterator end();
	};

	class MatrixBase {
	public:
		class IndexBase {
		friend class MatrixBase;
		public: //IndexBase
			virtual bool isAligned() const = 0;
			virtual bool isRow() const = 0;
			virtual bool isCol() const = 0;
			virtual void add(const MatrixBase &mat) = 0;
			virtual void take(const MatrixBase &mat) = 0;
			virtual bool sort(const MatrixBase &indices) = 0;
			virtual ~IndexBase(){}; //virtual destructor for deleting derived classes.
		private:
			virtual MatrixBase & matrixBase_() = 0;
			virtual IndexBase & contraBase_() = 0;
		};

		virtual DSTypes::DataType getDataType() const = 0;
		virtual const void * getMem() const = 0;
		virtual void * getMem() = 0;
		virtual DSTypes::UInt getMemStride() const = 0;
		virtual DSTypes::UInt getMemLineSize() const = 0;
		virtual DSTypes::UInt getMemLineCount() const = 0;
		virtual DSTypes::UInt getMemBytesPerCell() const = 0;

		virtual void setOrder(DSTypes::Order order) = 0;
		virtual DSTypes::Order order() const = 0;
		virtual void clear() = 0;
		virtual void clearAll() = 0; //Clear all children, parents, brothers and sisters.
		virtual void breakUp(const bool copy=true) = 0;
		virtual void breakDown(const bool copy=true) = 0;
		virtual void breakUpDown(MatrixBase &mat) = 0;
		virtual void resize2(DSTypes::UInt rowCount, DSTypes::UInt colCount) = 0;

		virtual bool isInitialized() const = 0;
		virtual bool isEmpty() const = 0;
		virtual bool isRegion() const = 0;
		virtual bool isContinuous() const = 0;
		virtual bool isSlice() const = 0;
		virtual bool isFullSlice() const = 0;
		virtual bool isVector() const = 0;
		virtual bool isColVector() const = 0;
		virtual bool isRowVector() const = 0;
		virtual bool isSameData(const MatrixBase &other) const = 0;
		virtual bool isSquare() const = 0;
		virtual bool isSameSize(const MatrixBase &other) const = 0;

		virtual IndexBase & rowsBase() = 0;
		virtual IndexBase & colsBase() = 0;

		virtual void write(std::ostream &output) const = 0;
		virtual void write(const DSTypes::String filename) const = 0;

		virtual void read(std::istream &input) = 0;
		virtual void read(const DSTypes::String filename) = 0;

		//Print output
		virtual void print(std::ostream &output) const = 0;
		virtual std::string print() const = 0;
		virtual void printLess(std::ostream &output) const = 0;
		virtual std::string printLess() const = 0;
		virtual std::string printSize() const = 0;

		virtual void slice(MatrixBase &dst, MatrixBase &rowFirst, MatrixBase &rowLast, MatrixBase &colFirst, MatrixBase &colLast) = 0;
		virtual void slice(MatrixBase &dst, MatrixBase &rowIndices, MatrixBase &colIndices) = 0;
		virtual void slice(MatrixBase &dst, MatrixBase &indices, MatrixBase &first, MatrixBase &last, bool rowIndices=true) = 0;
		virtual MatrixBase * create() const = 0;
		virtual MatrixBase * copy() const = 0;
		virtual MatrixBase * move() const = 0;
		virtual MatrixBase * take() const = 0;
				
		virtual void toStringMatrix(MatrixBase &dst) = 0;

		virtual void isEqual(MatrixBase &dst, const MatrixBase &other) const = 0;
		virtual void isNotEqual(MatrixBase &dst, const MatrixBase &other) const = 0;
		virtual void isGreater(MatrixBase &dst, const MatrixBase &other) const = 0;
		virtual void isSmaller(MatrixBase &dst, const MatrixBase &other) const = 0;
		virtual void isGreaterEqual(MatrixBase &dst, const MatrixBase &other) const = 0;
		virtual void isSmallerEqual(MatrixBase &dst, const MatrixBase &other) const = 0;

		virtual MatrixBase &operator=(const MatrixBase &other) = 0;
		virtual MatrixBase &operator=(MatrixBase &&other) = 0;

		virtual ~MatrixBase(){}; //virtual destructor for deleting derived classes.
	};

	template <typename T, typename TInt = DSTypes::MatrixIdx>
	class Matrix : public MatrixBase {
	public:
		class Index : public MatrixBase::IndexBase {
		friend class Matrix;
		public: //Index
			class Iterator : public std::iterator<std::forward_iterator_tag, CellsContinuous<T, TInt>> {
			friend class Matrix<T, TInt>::Index;
			public: //Iterator
				Iterator();
				Iterator(const Iterator &other);
				Iterator(Index &index, const typename std::vector<T>::iterator &cursor);
				~Iterator();
				
				Iterator& operator=(const Iterator& other);
				bool operator==(const Iterator& other) const;
				bool operator!=(const Iterator& other) const;
				Iterator& operator++();
				Iterator operator++(int);
				
				CellsBase<T, TInt> & cells(DSTypes::CellsIteratorType cellsIteratorType); //Either CellsRegion or CellContinuous (from beginning to end of row/col)
				CellsContinuous<T, TInt> * operator->(); //returns the CellContinuous (from beginning to end of row/col)
				CellsContinuous<T, TInt> & operator*(); //returns the CellContinuous (from beginning to end of row/col)

				typename CellsContinuous<T, TInt>::Iterator operator[](const TInt index); //returns the CellsContinuous of a specific cell
			private: //Iterator
				TInt stride_;
				TInt count_;
				Index * index_;
				typename std::vector<T>::iterator cursor_;
			};
			Index();
			Index(Matrix &matrix, const TInt first, const TInt last, const TInt stride, Index *contra);

			CellsBase<T, TInt> &cells(DSTypes::CellsIteratorType cellsIteratorType); //Either CellsRegion or CellContinuous (from beginning to end of matrix)
			CellsRegion<T, TInt> * operator->(); //returns the CellsRegion (from beginning to end of matrix)
			CellsRegion<T, TInt> & operator*(); //returns the CellsRegion (from beginning to end of matrix)

			Iterator operator[](const TInt index);
			Iterator begin();
			Iterator end();			
			
			TInt count() const;
			TInt stride() const;
			TInt first() const;
			TInt last() const;

			//Overridden
			bool isAligned() const;
			bool isRow() const;
			bool isCol() const;			

			void add(const T &val);
			void add(const std::vector<T> &vec);
			void add(const Matrix<T, TInt> &mat);
			void add(const MatrixBase &mat) {add(dynamic_cast<const Matrix<T, TInt> &>(mat));};
			void take(const Matrix<T, TInt> &mat);
			void take(const MatrixBase &mat) { take(dynamic_cast<const Matrix<T, TInt> &>(mat)); };

			bool sort(const Matrix<TInt, TInt> &indices);
			bool sort(const MatrixBase &indices) {return sort(dynamic_cast<const Matrix<TInt, TInt> &>(indices));};

		private: //Index
			typename std::vector<T>::iterator begin_;
			typename std::vector<T>::iterator end_;
			Matrix * matrix_;
			MatrixBase & matrixBase_() {return *matrix_;}
			TInt first_;
			TInt last_;
			TInt stride_; //Where to find the next ++
			TInt count_;
			Index * contra_; //If this == row then contra == columns
			IndexBase & contraBase_() {return *contra_;}
			bool isRow_;
			CellsContinuous<T, TInt> cellsContinuous_;
			CellsRegion<T, TInt> cellsRegion_;			
		};
		
		Matrix();
		Matrix(DSTypes::Order order);
		Matrix(const TInt rowCount, const TInt colCount, DSTypes::Order order = DSTypes::oRowMajor);
		Matrix(const TInt rowCount, const TInt colCount, const T &initVal, DSTypes::Order order = DSTypes::oRowMajor);
		Matrix(const TInt rowCount, const TInt colCount, const T *initArr, DSTypes::Order order = DSTypes::oRowMajor);
		Matrix(const TInt rowCount, const TInt colCount, const std::vector<T> initVec, DSTypes::Order order = DSTypes::oRowMajor);
		Matrix(const Matrix &mat);
		Matrix(const std::vector<T> &vec, DSTypes::Order order = DSTypes::oRowMajor);
		Matrix(Matrix &&mat);
		Matrix(const MatrixBase &mat);
		Matrix(MatrixBase &&mat);

		MatrixBase * create() const {
			Matrix<T, TInt> * mat = new Matrix<T, TInt>();
			return mat;
		}
		MatrixBase * copy() const {
			Matrix<T, TInt> * mat = new Matrix<T, TInt>(order_);
			mat->clone(*this);
			return mat;
		}
		MatrixBase * move() const {
			Matrix<T, TInt> * mat = new Matrix<T, TInt>(order_);
			mat->clone(std::move(*this));
			return mat;
		}
		MatrixBase * take() const {
			Matrix<T, TInt> * mat = new Matrix<T, TInt>(order_);
			mat->cols.take(*this);			
			return mat;
		}

		virtual ~Matrix();
		
		Index rows;
		IndexBase & rowsBase() {return rows;};
		Index cols;
		IndexBase & colsBase() {return cols;};

		CellsContinuous<T, TInt> & vec();
		CellsContinuous<T, TInt> & vec() const;
		
		T & vec(const TInt index);
		T & val(const TInt row, const TInt col);

		const T & vec(const TInt index) const;
		const T & val(const TInt row, const TInt col) const;
	
		TInt getContraCount(const Index &idx) const; //if idx == row return cols.count()
		TInt getCount(const Index &idx) const; //if idx == row return rows.count()
		TInt getStride(const Index &idx) const; //if idx == row return cols.stride()
		TInt getContraStride(const Index &idx) const; //if idx == row return rows.stride()		

		//Get data vector
		std::vector<T> & data() {return (up_ == nullptr)?data_:up_->data_;};

		//Functions for low level data access. Higher performance compared to iterators
		T* getData() { return (up_ == nullptr) ? data_.data() : up_->data_.data() + rows.first_ * rows.stride_ + cols.first_ * cols.stride_; };
		const T* getData() const { return (up_ == nullptr) ? data_.data() : up_->data_.data() + rows.first_ * rows.stride_ + cols.first_ * cols.stride_; };
		TInt getDataStride() const {return (order_ == DSTypes::oRowMajor)?rows.stride_ :cols.stride_;};
		TInt getDataLineSize() const {return (order_ == DSTypes::oRowMajor)?cols.count():rows.count();};
		TInt getDataLineCount() const {return (order_ == DSTypes::oRowMajor)?rows.count():cols.count();};
		TInt getDataBytesPerCell() const {return sizeof(T);};
		DSTypes::DataType getDataType() const {T t; return DSTypes::dataType(t);};
		DSTypes::DataType getIntDataType() const { TInt t; return DSTypes::dataType(t); };

		//Functions for lowest level data access (from MatrixBase).
		void * getMem() {return (void*) getData();};
		const void * getMem() const { return (void*)getData(); };
		DSTypes::UInt getMemStride() const {return getDataStride();};
		DSTypes::UInt getMemLineSize() const {return getDataLineSize();};
		DSTypes::UInt getMemLineCount() const {return getDataLineCount();};
		DSTypes::UInt getMemBytesPerCell() const {return sizeof(T);};

		//Non virtual functions
		void resize(TInt rowCount, TInt colCount);
		void clone(const Matrix &src); //clone Matrix, will point data of src, so not a "real" clone.
		void clone(Matrix &&src); //clone Matrix, will move data of src
		virtual Matrix dup() const; //Copy Matrix. Performs a deep copy if T inherits from MatrixBase (e.g. Matrix<> or ImagePNG<>)
		bool isSlicable(TInt &first, TInt &last) const; //Determines if slicing can be used using the indices vector, and returns the slice indices.
		void breakUpDown(MatrixBase &mat); //Break all ties with specified matrix

		//overridden functions
		void setOrder(DSTypes::Order order); //sets the order, but also clear the Matrix.
		DSTypes::Order order() const;
		void clear();
		void clearAll();
		void breakUp(const bool copy=true); //break ties with up_
		void breakDown(const bool copy=true); //Force all down_ to copy() or clear()
		void resize2(DSTypes::UInt rowCount, DSTypes::UInt colCount) {resize((TInt)rowCount, (TInt)colCount);}

		bool isVector() const;
		bool isColVector() const;
		bool isRowVector() const;
		bool isInitialized() const;
		bool isEmpty() const;
		bool isRegion() const;
		bool isContinuous() const;
		bool isSlice() const;
		bool isFullSlice() const;
		bool isSameData(const Matrix<T, TInt> &other) const;
		bool isSameData(const MatrixBase &other) const {return isSameData(dynamic_cast<const Matrix<T, TInt>&>(other));}
		bool isSquare() const {return cols.count() == rows.count();};
		bool isSameSize(const Matrix<T, TInt> &other) const { return cols.count() == other.cols.count() && rows.count() == other.rows.count();};
		bool isSameSize(const MatrixBase &other) const { return isSameSize(dynamic_cast<const Matrix<T, TInt>&>(other));};

		void toStringMatrix(Matrix<DSTypes::String, TInt> &dst);
		void toStringMatrix(MatrixBase &dst) {toStringMatrix(dynamic_cast<Matrix<DSTypes::String, TInt>&>(dst));};

		//Reading and writing
		virtual void write(std::ostream &output) const;
		virtual void write(const DSTypes::String filename) const;
		void read(std::istream &input);
		void read(const DSTypes::String filename);

		//Printing output
		void print(std::ostream &output) const;
		std::string print() const;
		void printLess(std::ostream &output) const;
		std::string printLess() const;
		std::string printSize() const;

		//Slicing
		void slice(Matrix<T, TInt> &dst, const TInt rowFirst, const TInt rowLast, const TInt colFirst, const TInt colLast);
		void slice(Matrix<T, TInt> &dst, const Matrix<TInt, TInt> &rowIndices, const Matrix<TInt, TInt> &colIndices);		
		void slice(MatrixBase &dst, MatrixBase &rowFirst, MatrixBase &rowLast, MatrixBase &colFirst, MatrixBase &colLast);
		void slice(MatrixBase &dst, MatrixBase &rowIndices, MatrixBase &colIndices);
		void slice(MatrixBase &dst, MatrixBase &indices, MatrixBase &first, MatrixBase &last, bool rowIndices=true); //TODO: Remove bool: Should be split in two (rowIndices, colFirst, colLast) and (rowFirst, rowLast, colIndices)

		//Manipulation as a MatrixBase
		void isEqual(MatrixBase &dst, const MatrixBase &other) const;
		void isNotEqual(MatrixBase &dst, const MatrixBase &other) const;
		void isGreater(MatrixBase &dst, const MatrixBase &other) const;
		void isSmaller(MatrixBase &dst, const MatrixBase &other) const;
		void isGreaterEqual(MatrixBase &dst, const MatrixBase &other) const;
		void isSmallerEqual(MatrixBase &dst, const MatrixBase &other) const;

		//Assign one to the other (referenced copy, or direct slice assignment)
		Matrix<T, TInt> &operator=(const Matrix<T, TInt> &other);
		MatrixBase &operator=(const MatrixBase &other) {return operator=(dynamic_cast<const Matrix<T, TInt>&>(other));}
		//Assign one to the other (move copy, or direct slice assignment)
		Matrix<T, TInt> &operator=(Matrix<T, TInt> &&other);
		MatrixBase &operator=(MatrixBase &&other) { return operator=(dynamic_cast<Matrix<T, TInt>&&>(other));}
		//Assign constant to all elements
		Matrix<T, TInt> &operator=(const T &val);
		#ifndef DS_DISABLE_IMPLICIT_CONVERSIONS
		operator T();
		#endif
		//Implicit convert to bool for tests ==, >=, <=, !=
		operator bool() {return isInitialized();}
		//Return full slice
		Matrix<T, TInt> operator()(){return *this;};
		//Slice Matrix single row
		Matrix<T, TInt> operator()(const TInt row);
		//Slice Matrix by row indices
		Matrix<T, TInt> operator()(const Matrix<TInt, TInt> &rowIndices);
		//Slice Matrix single
		Matrix<T, TInt> operator()(const TInt rowBegin, const TInt rowLength);
		//Slice Matrix region
		Matrix<T, TInt> operator()(const TInt rowBegin, const TInt rowLength, const TInt colBegin, const TInt colLength);
		//Slice Matrix by indices
		Matrix<T, TInt> operator()(const Matrix<TInt, TInt> &rowIndices, const Matrix<TInt, TInt> &colIndices);
		//Slice Matrix by col
		Matrix<T, TInt> operator[](const TInt col);
		//Slice Matrix by col indices
		Matrix<T, TInt> operator[](const Matrix<TInt, TInt> &colIndices);
		//Slice combinations
		Matrix<T, TInt> operator()(const Matrix<TInt, TInt> &rowIndices, const TInt col);
		Matrix<T, TInt> operator()(const Matrix<TInt, TInt> &rowIndices, const TInt colBegin, const TInt colLength);
		Matrix<T, TInt> operator()(const TInt row, const Matrix<TInt, TInt> &colIndices);
		Matrix<T, TInt> operator()(const TInt rowBegin, const TInt rowLength, const Matrix<TInt, TInt> &colIndices);
		Matrix<T, TInt> operator()(const TInt rowBegin, const TInt rowLength, const TInt col);
	protected:
		typename std::vector<T>::iterator end();
		typename std::vector<T>::iterator begin();
		typename std::vector<T>::iterator at(const TInt row, const TInt col); //Get iterator at position row, column. Should be done after invalidation of *down_

		typename std::vector<T>::const_iterator begin() const;
		typename std::vector<T>::const_iterator end() const;
		typename std::vector<T>::const_iterator at(const TInt row, const TInt col) const; 
	private:
		std::vector<T> data_;

		DSTypes::Order order_;
		std::vector<Matrix<T, TInt> *> down_;
		void updateIt_(); //Update the iterators of cols_ and rows_. Convert first_ and last_ to begin_ and end_		
		void updateStride_(); //Stride can change if up_ adds a col or row
		Matrix<T, TInt> *up_;
		
		void invalidate_(); //update iterators
		void init_();		

		std::mutex lockRefRel_;
		Matrix& ref_(Matrix<T, TInt> &mat); //I am *down with *data
		void rel_(const Matrix<T, TInt> &mat); //Called when *down goed out of scope
	};
};

namespace DSTypes{
	//Type definitions for Matrix put into the DSTypes namespace after definition of Matrix<>. Allows for Matrix<MatrixDouble>
	typedef DSLib::Matrix<DSTypes::Float, DSTypes::MatrixIdx> MatrixDouble;
	typedef DSLib::Matrix<DSTypes::Double, DSTypes::MatrixIdx> MatrixFloat;
	typedef DSLib::Matrix<DSTypes::Int32, DSTypes::MatrixIdx> MatrixInt32;
	typedef DSLib::Matrix<DSTypes::UInt32, DSTypes::MatrixIdx> MatrixUInt32;
	typedef DSLib::Matrix<DSTypes::String, DSTypes::MatrixIdx> MatrixString;
}

#ifdef MSC_VER
	#pragma warning(pop)
#endif
