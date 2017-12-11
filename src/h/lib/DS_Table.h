#pragma once

#include "h/DS_Types.h"
#include "h/lib/DS_Matrix.h"
#include "h/lib/DS_MatrixFunc.h"
#include "h/lang/DS_MatrixLang.h"

namespace DSLib {
	template<typename TIdx = DSTypes::TableIdx, typename TId = DSTypes::TableId>
	class Table {
	public:
		class TableMatrix {
		friend class Table<TIdx, TId>;
		private:
			TIdx colOffset_;
			TIdx colCount_;
			TIdx rowCount_;
			DSTypes::DataType dataType_;
			DSTypes::ContentType contentType_;
			DSLib::MatrixBase * data_;
		public:
			TableMatrix(DSLib::MatrixBase * mat) {
				data_ = mat;
				colOffset_ = 0;
				colCount_ = 0;
				rowCount_ = 0;
				contentType_ = DSTypes::ctUnknown;
				dataType_ = DSTypes::dtDataType;
			}

			~TableMatrix() {
				delete data_;
			}
			DSTypes::DataType dataType() const {return dataType_;};
			DSTypes::ContentType contentType() const {return contentType_;};
			TIdx colOffset() const {return colOffset_;};
			TIdx colCount() const {return colCount_;};
			TIdx rowCount() const {return rowCount_;};
			DSLib::MatrixBase & data() {return *data_;};
		};

		class Index {
			friend class Table<TIdx, TId>;
		private:
			std::vector<TId> idxToId_;
			std::map<TId, TIdx> idToIdx_;
			TIdx count_;
			Table * table_;
			Index * contra_;

			void findIds(DSLib::Matrix<TId, TIdx> &result, const DSLib::Matrix<TId, TIdx> &ids);			
			void findIds(DSLib::Matrix<TId, TIdx> &result, const std::vector<TId> &ids);

			void clear();

		public:
			Index() {
				count_ = 0;
				contra_ = nullptr;
				table_ = nullptr;
			}
			void addIds(const DSLib::Matrix<TId, TIdx> &ids, TIdx atInsertAt);

			DSLib::Matrix<TIdx, TIdx> add(const Table &tab); //returns the indices of the newly added tab
			DSLib::Matrix<TIdx, TIdx> take(const Table &tab); //returns the indices of the newly added tab
			DSLib::Matrix<TIdx, TIdx> add(const DSLib::MatrixBase &mat, const TIdx rowCount, const TIdx colCount, const DSTypes::ContentType ct, const DSTypes::DataType dt, const DSLib::Matrix<TId, TIdx> &rowIds, const DSLib::Matrix<TId, TIdx> &colIds);
			DSLib::Matrix<TIdx, TIdx> take(const DSLib::MatrixBase &mat, const TIdx rowCount, const TIdx colCount, const DSTypes::ContentType ct, const DSTypes::DataType dt, const DSLib::Matrix<TId, TIdx> &rowIds, const DSLib::Matrix<TId, TIdx> &colIds);

			void setIds(const DSLib::Matrix<TId, TIdx> & ids);
		
			bool isRow() const {return (this == &(table_->rows));}
			bool isCol() const {return (this == &(table_->cols));}
			bool hasIds() const {return idxToId_.begin() != idxToId_.end();};			
			bool hasId(const TId &id);

			TIdx idToIdx(const TId id);
			std::map<TId, TIdx> idToIdxAsMap(const TIdx start, const TIdx count) const;
			
			TId idxToId(const TIdx idx);			
			std::vector<TId> idxToIdAsVec(const TIdx start, const TIdx count) const;
			DSLib::Matrix<TId, TIdx> idxToId(const TIdx start=0, const TIdx count=0) const;
			DSLib::Matrix<TId, TIdx> ids() const {return idxToId(0,0);};
			DSLib::Matrix<TIdx, TIdx> idxs() const;
			
			const std::vector<TId> & idxToIdAsVec() const {return idxToId_;};
			
			TIdx count() const;
			void count(TIdx count) {count_ = count;};
		};

		class Slicer {
		private:
			enum SliceState {Start, RowIndex, RowBegin, RowLength, ColIndex, ColBegin, ColLength, DataType, ContentType, Stop}; //Last written value
			bool useRowRegion_, useColRegion_;
			SliceState state_;
			TIdx rowBegin_, rowLength_, colBegin_, colLength_;
			DSLib::Matrix<TIdx> colIndices_, rowIndices_;
			std::vector<DSTypes::DataType> dataTypes_;
			std::vector<DSTypes::ContentType> contentTypes_;
			Table * src_;
			Table * dst_;
			int remainingCount_;
		public:
			Slicer();
			Slicer(Table<TIdx, TId> &dst, Table<TIdx, TId> &src, int valueCount);
			void setValueCount(int valueCount);
			void setSource(Table<TIdx, TId> *src);
			void setDestination(Table<TIdx, TId> *dst);
			bool isInitialized();
			void clear();
			void next(const TIdx &idx);
			void next(const TId &id);
			void next(const DSLib::Matrix<TIdx> &mat);
			void next(const DSLib::Matrix<TId> &mat);
			void next(const DSTypes::DataType dt);
			void next(const DSTypes::ContentType ct);
			void next(const DSLib::Matrix<DSTypes::DataType> &mat);
			void next(const DSLib::Matrix<DSTypes::ContentType> &mat);
			DSLib::Matrix<TIdx, TIdx> slice();
		};

	private:
		//Pointers are owned by *ByIdx;
		std::vector<TableMatrix *> matrixByIdx_;		

		std::map<std::pair<DSTypes::ContentType, DSTypes::DataType>, TableMatrix *> matrixByType_;

		void updateOffsets();

		template<typename T>
		DSLib::Matrix<TIdx, TIdx> slice(Slicer &slicer, const T &selector) {
			slicer.next(selector);
			return slicer.slice();
		}
		
		template<typename T, typename... Tn>
		DSLib::Matrix<TIdx, TIdx> slice(Slicer &slicer, const T &selector, const Tn... selectors) {
			slicer.next(selector);
			return slice(slicer, selectors...);
		}

		void TypesToColIndices_(DSLib::Matrix<TIdx, TIdx> &colIndices, const std::vector<DSTypes::DataType> dataTypes, const std::vector<DSTypes::ContentType> contentTypes);
	public:
		TableMatrix * findMatrix(DSTypes::ContentType ct, DSTypes::DataType dt);
		std::vector<TableMatrix *> findMatrices(DSTypes::ContentType ct);
		std::vector<TableMatrix *> findMatrices(DSTypes::DataType dt);

		TableMatrix * addMatrix(const DSLib::MatrixBase &mat, DSTypes::ContentType ct, DSTypes::DataType dt, TIdx rowcount, TIdx colCount, TIdx colOffset);
		TableMatrix * takeMatrix(const DSLib::MatrixBase &mat, DSTypes::ContentType ct, DSTypes::DataType dt, TIdx rowcount, TIdx colCount, TIdx colOffset);
		TableMatrix * addMatrix(const DSLib::MatrixBase &&mat, DSTypes::ContentType ct, DSTypes::DataType dt, TIdx rowcount, TIdx colCount, TIdx colOffset);

		void write_(std::ostream &output) const;
		Table<TIdx, TId> & read_(std::istream &input);

		//Reading and writing
		void write(std::ostream &output) const;
		void write(const DSTypes::String filename) const;
		Table<TIdx, TId> & read(std::istream &input);
		Table<TIdx, TId> & read(const DSTypes::String filename);

		DSTypes::DataType getIdType() const { TId t; return DSTypes::dataType(t); };
		DSTypes::DataType getIdxType() const { TIdx t; return DSTypes::dataType(t); };

		Index rows;
		Index cols;
		
		Table() {
			cols.table_ = this;
			rows.table_ = this;
			cols.contra_ = &rows;
			rows.contra_ = &cols;
		}
		
		Table(const TIdx rowCount) {
			cols.table_ = this;
			rows.table_ = this;
			cols.contra_ = &rows;
			rows.contra_ = &cols;
			resizeRows(rowCount);
		}
	
		Table(const Table &other) {clone(other);}
		Table(Table &&other) {clone(std::move(other));}

		~Table() {clear();}

		#ifndef DS_DISABLE_IMPLICIT_CONVERSIONS
		template <typename T>
		operator DSLib::Matrix<T, TIdx>() {
			if (matrixByIdx_.size() != 1)
				throw DSTypes::Error(DSTypes::ecIncompatible, "Implicit cast from Table to Matrix", SS("Table should only contain one Matrix not " << matrixByIdx_.size()));
			return mat<T>(0);
		}
		#endif

		bool isRowTuple() const {return rows.count() == 1;}
		bool isColTuple() const {return cols.count() == 1;}
		bool isTuple() const {return isRowTuple() | isColTuple();}
		bool isInitialized() const {return rows.count() != 0 || cols.count() != 0;};				
		bool isSameData(const Table<TIdx, TId> &other) const;
		bool isSameStructure(const Table<TIdx, TId> &other) const;

		DSTypes::ContentType getContentType(const TIdx col);
		DSTypes::DataType getDataType(const TIdx col);

		void print(std::ostream &output) const;
		std::string print() const;
		void printLess(std::ostream &output) const;
		std::string printLess() const;
		std::string printSize() const;
		std::string to_string() const { return printSize(); } //for ChaiScript
		void setContentTypes(const DSLib::Matrix<DSTypes::ContentType, TIdx> &cts);

		const std::vector<TableMatrix *> & matrixByIdx() const {return matrixByIdx_;};
		TableMatrix & matrixByIdx(TIdx idx);
		MatrixBase & mat(TIdx index) {return matrixByIdx(index).data();};		

		template<typename T> Matrix<T, TIdx> & mat(TIdx index=0) {
			try {
				return dynamic_cast<Matrix<T, TIdx>&>(mat(index));
            } catch (std::exception &e) {
				throw Error(ecGeneral, "Table<>.mat<T>()", e.what() + SS(" to " << typeid(Matrix<T, TIdx>).name() << " from " << typeid(mat(index)).name()));
			}
		};
		operator bool() { return isInitialized(); }

		void addRows(const Table &tab);
		void takeRows(const Table &tab);
		void resizeRows(TIdx count);

		virtual void clear();
		virtual void clearAll();

		void clone(const Table &src);
		void clone(Table &&src);

		Table dup() const;
		void breakUpDown();

		//Assign one to the other
		Table<TIdx, TId> &operator=(const Table<TIdx, TId> &other);
		Table<TIdx, TId> &operator=(Table<TIdx, TId> &&other);
		
		template<typename T>
		DSLib::Matrix<TIdx, TIdx> slice(Table<TIdx, TId> &dst, const T &selector) {
			Slicer slicer(dst, *this, 1);
			return slice(slicer, selector);
		}

		template<typename T, typename... Tn>
		DSLib::Matrix<TIdx, TIdx>  slice(Table<TIdx, TId> &dst, const T &selector, const Tn... selectors) {
			Slicer slicer(dst, *this, sizeof...(selectors)+1);			
			slicer.next(selector);
			return slice(slicer, selectors...);
		}

		DSLib::Matrix<TIdx, TIdx> slice(Table<TIdx, TId> &dst) {
			dst = (*this);
			if (cols.count() == 0)
				return Matrix<TIdx, TIdx>();
			return DSLang::operator||(DSLang::operator|(DSLib::Matrix<TIdx, TIdx>(), TIdx(0)), cols.count()-1);;
		}

		//DSLang
		//Assign constant
		template <typename T>
		Table<TIdx, TId> &operator=(const T &val);

		//Assign matrix
		template <typename T>
		Table<TIdx, TId> &operator=(const DSLib::Matrix<T> &mat);

		//Slice Table
		template<typename T, typename... Tn>
		Table<TIdx, TId> operator()(const T &selector, const Tn... selectors) {
			Table<TIdx, TId> dst;
			slice(dst, selector, selectors...);
			return dst;
		}

		//Make a full sliced copy
		Table<TIdx, TId> operator()() {
			return (*this);
		}

		//Slice Table by column
		template<typename T>
		Table<TIdx, TId> operator[](const T &selector) {
			Table<TIdx, TId> dst;
			slice(dst, (TIdx)0, rows.count_, selector);
			return dst;
		}		
	};
}
