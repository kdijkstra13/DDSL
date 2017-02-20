#pragma once

#include "h/lib/DS_Table.h"

#include "hpp/lib/DS_TableFunc.hpp"
#include "hpp/lang/DS_TableLang.hpp"
#include "hpp/DS_Util.hpp"

using namespace std;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSUtil;

namespace DSLib {
	template <typename TIdx, typename TId>
	Table<TIdx, TId>::Slicer::Slicer(Table<TIdx, TId> &dst, Table<TIdx, TId> &src, int valueCount) {
		clear();
		remainingCount_ = valueCount;
		setSource(&src);
		setDestination(&dst);
	}

	template <typename TIdx, typename TId>
	Table<TIdx, TId>::Slicer::Slicer() {
		clear();
		remainingCount_ = 0;
		setSource(nullptr);
		setDestination(nullptr);
	}

	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::Slicer::clear() {
		remainingCount_ = 0;
		src_ = nullptr;
		dst_ = nullptr;
		useColRegion_ = false;
		useRowRegion_ = false;
		rowBegin_ = 0;
		rowLength_ = 0;
		colBegin_ = 0;
		colLength_ = 0;
		state_ = Start;
		colIndices_.clear();
		rowIndices_.clear();
		contentTypes_.clear();
		dataTypes_.clear();
	}

	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::Slicer::setValueCount(int valueCount) {
		remainingCount_ = valueCount;
	}
	
	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::Slicer::setSource(Table<TIdx, TId> *src) {
		src_ = src;
	}

	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::Slicer::setDestination(Table<TIdx, TId> *dst) {
		dst_ = dst;
	}

	template <typename TIdx, typename TId>
	bool Table<TIdx, TId>::Slicer::isInitialized() {
		return state_ == Stop;
	}

	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::Slicer::next(const TIdx &idx) {
		remainingCount_--;
		switch (state_) {
		case Start: {					
			useRowRegion_ = true;
			if (remainingCount_ == 0) {
				useColRegion_ = true;
				rowBegin_ = idx;
				rowLength_ = 1;
				colBegin_ = 0;
				colLength_ = src_->cols.count();
				state_ = Stop;
			} else {
				rowBegin_ = idx;
				state_ = RowBegin;
			}
			break;
					};
		case RowBegin: {
			useRowRegion_ = true;
			if (remainingCount_ == 0) {
				useColRegion_ = true;
				rowLength_ = idx;
				colBegin_ = 0;
				colLength_ = src_->cols.count();
				state_ = Stop;
			} else {
				rowLength_ = idx;
				state_ = RowLength;					  
			}
			break;
						};
		case RowIndex:
		case RowLength: {
			useColRegion_ = true;
			if (remainingCount_ == 0) {
				colBegin_ = idx;
				colLength_ = 1;
				state_ = Stop;
			} else {
				colBegin_ = idx;
				state_ = ColBegin;
			}
			break;
						};
		case ColBegin: {
			useColRegion_ = true;
			if (remainingCount_ == 0) {
				colLength_ = idx;						
				state_ = Stop;
			} else {
				colLength_ = idx;
				state_ = Stop;
			}
			break;										
						};
   		case ContentType: throw Error(ecUnexpected, "Slicer::next(Matrix)", SS("unexpected slicer state after ContentType with value: " << idx));
		case DataType: throw Error(ecUnexpected, "Slicer::next(Matrix)", SS("unexpected slicer state after DataType with value: " << idx));
		case ColLength:  throw Error(ecUnexpected, "Slicer::next(TIdx)", SS("unexpected slicer state after ColLength with value: " << idx));
		case ColIndex: throw Error(ecUnexpected, "Slicer::next(TIdx)", SS("unexpected slicer state after ColIndex with value: " << idx));
		case Stop: throw Error(ecUnexpected, "Slicer::next(TIdx)", SS("unexpected value after Stop with value: " << idx));
		}				
	}

	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::Slicer::next(const TId &id) {
		switch (state_) {
			case Start: next(src_->rows.idToIdx(id)); break;
			case RowBegin: next(src_->rows.idToIdx(id) - rowBegin_ + 1); break;
			case RowLength:
			case RowIndex: next(src_->cols.idToIdx(id)); break;
			case ColBegin: next(src_->cols.idToIdx(id) - colBegin_ + 1); break;
	   		case ContentType: throw Error(ecUnexpected, "Slicer::next(Matrix)", SS("unexpected slicer state after ContentType with value: " << id));
			case DataType: throw Error(ecUnexpected, "Slicer::next(Matrix)", SS("unexpected slicer state after DataType with value: " << id));
			case ColLength:  throw Error(ecUnexpected, "Slicer::next(TId)", SS("unexpected slicer state after ColLength with value: " << id));
			case ColIndex: throw Error(ecUnexpected, "Slicer::next(TId)", SS("unexpected slicer state after ColIndex with value: " << id));
			case Stop: throw Error(ecUnexpected, "Slicer::next(TId)", SS("unexpected value after Stop with value: " << id));
		}
	};

	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::Slicer::next(const DSLib::Matrix<TIdx> &mat) {
		remainingCount_--;
		switch (state_) {
		case Start: {
  			useRowRegion_ = false;
			if (remainingCount_ == 0) {
				useColRegion_ = true;
				rowIndices_ = mat;
				colBegin_ = 0;
				colLength_ = src_->cols.count();
				state_ = Stop;
			} else {
				rowIndices_ = mat;
				state_ = RowIndex;
			}
			break;
					};
		case RowBegin: {					
			if (remainingCount_ == 0) {
				useColRegion_ = false;
				rowLength_ = 1;
				colIndices_ = mat;
				state_ = Stop;
			} else {
				rowLength_ = 1;
				colIndices_ = mat;
				state_ = ColIndex;
			}
			break;
						};
		case RowLength:
		case RowIndex: {
			useColRegion_ = false;
			colIndices_ = mat;
			state_ = ColIndex;
			break;
						};
		case ContentType: throw Error(ecUnexpected, "Slicer::next(Matrix)", SS("unexpected slicer state after ContentType with value: " << mat.printLess()));
		case DataType: throw Error(ecUnexpected, "Slicer::next(Matrix)", SS("unexpected slicer state after DataType with value: " << mat.printLess()));
		case ColBegin:	throw Error(ecUnexpected, "Slicer::next(Matrix)", SS("unexpected slicer state after ColBegin with value: " << mat.printLess()));
		case ColLength: throw Error(ecUnexpected, "Slicer::next(Matrix)", SS("unexpected slicer state after colBegin with value: " << mat.printLess()));
		case ColIndex: throw Error(ecUnexpected, "Slicer::next(TIdx)", SS("unexpected slicer state after ColIndex with value: " << mat.printLess()));
		case Stop: throw Error(ecUnexpected, "Slicer::next(TIdx)", SS("unexpected value after Stop with value: " << mat.printLess()));
		}
	}

	template <typename TIdx, typename TId>	
	void Table<TIdx, TId>::Slicer::next(const DSLib::Matrix<TId> &mat) {
		DSLib::Matrix<TIdx> mat2;
		if (state_ == Start) {
			for (auto id=mat.vec().begin();id!=mat.vec().end();id++)
				mat2.rows.add(src_->rows.idToIdx(*id));
		} else if (state_ == RowBegin || state_ == RowLength || state_ == RowIndex) {
			for (auto id=mat.vec().begin();id!=mat.vec().end();id++)
				mat2.rows.add(src_->cols.idToIdx(*id));
		} else {
			throw Error(ecUnexpected, "Slicer::next(Matrix<TId>)", SS("unexpected value: " << mat.printLess()));
		}
		next(mat2);
	}

	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::Slicer::next(const DSTypes::DataType dt) {
		remainingCount_--;
		switch (state_) {
		case Start: {
			useRowRegion_ = true;
			rowBegin_ = 0;
			rowLength_ = src_->rows.count();			
			break;
					}
		case RowBegin: {
			rowLength_ = 1;
			break;
					   }
		case RowLength:
		case RowIndex:
		case DataType:
		case ContentType: break;
		case ColIndex: throw Error(ecUnexpected, "Slicer::next(DataType)", SS("unexpected value after ColIndex with value: " << etos(dt)));
		case ColBegin: throw Error(ecUnexpected, "Slicer::next(DataType)", SS("unexpected value after ColBegin with value: " << etos(dt)));
		case ColLength: throw Error(ecUnexpected, "Slicer::next(DataType)", SS("unexpected value after ColLength with value: " << etos(dt)));
		case Stop: throw Error(ecUnexpected, "Slicer::next(DataType)", SS("unexpected value after Stop with value: " << etos(dt)));
		default: throw Error(ecInvalidEnum, "Slicer::next(DataType)", SS("unexpected enum with value: " << etos(dt)));
		}
		dataTypes_.push_back(dt);
		state_ = DataType;
	}

	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::Slicer::next(const DSLib::Matrix<DSTypes::DataType> &mat) {
		remainingCount_--;
		switch (state_) {
		case Start: {
			useRowRegion_ = true;
			rowBegin_ = 0;
			rowLength_ = src_->rows.count();
			break;
		}
		case RowBegin: {
			rowLength_ = 1;
			break;
		}
		case RowLength:
		case RowIndex:
		case DataType:		
		case ContentType: break;
		case ColIndex: throw Error(ecUnexpected, "Slicer::next(DataType)", SS("unexpected value after ColIndex with value: " << mat.print()));
		case ColBegin: throw Error(ecUnexpected, "Slicer::next(DataType)", SS("unexpected value after ColBegin with value: " << mat.print()));
		case ColLength: throw Error(ecUnexpected, "Slicer::next(DataType)", SS("unexpected value after ColLength with value: " << mat.print()));
		case Stop: throw Error(ecUnexpected, "Slicer::next(DataType)", SS("unexpected value after Stop with value: " << mat.print()));
		default: throw Error(ecInvalidEnum, "Slicer::next(DataType)", SS("unexpected enum with value: " << mat.print()));
		}
		dataTypes_.insert(dataTypes_.begin(), mat.vec().begin(), mat.vec().end());
		state_ = DataType;
	}

	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::Slicer::next(const DSTypes::ContentType ct) {
		remainingCount_--;
		switch (state_) {
		case Start: {
			useRowRegion_ = true;
			rowBegin_ = 0;
			rowLength_ = src_->rows.count();			
			break;
					}
		case RowBegin: {
			rowLength_ = 1;
			break;
					   }
		case RowLength:
		case RowIndex:
		case DataType:
		case ContentType: break;
		case ColIndex: throw Error(ecUnexpected, "Slicer::next(ContentType)", SS("unexpected value after ColIndex with value: " << etos(ct)));
		case ColBegin: throw Error(ecUnexpected, "Slicer::next(ContentType)", SS("unexpected value after ColBegin with value: " << etos(ct)));
		case ColLength: throw Error(ecUnexpected, "Slicer::next(ContentType)", SS("unexpected value after ColLength with value: " << etos(ct)));
		case Stop: throw Error(ecUnexpected, "Slicer::next(ContentType)", SS("unexpected value after Stop with value: " << etos(ct)));
		default: throw Error(ecInvalidEnum, "Slicer::next(DataType)", SS("unexpected enum with value: " << etos(ct)));
		}
		contentTypes_.push_back(ct);
		state_ = ContentType;
	}

	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::Slicer::next(const DSLib::Matrix<DSTypes::ContentType> &mat) {
		remainingCount_--;
		switch (state_) {
		case Start: {
			useRowRegion_ = true;
			rowBegin_ = 0;
			rowLength_ = src_->rows.count();
			break;
		}
		case RowBegin: {
			rowLength_ = 1;
			break;
		}
		case RowLength:
		case RowIndex:
		case DataType:
		case ContentType: break;
		case ColIndex: throw Error(ecUnexpected, "Slicer::next(ContentType)", SS("unexpected value after ColIndex with value: " << mat.print()));
		case ColBegin: throw Error(ecUnexpected, "Slicer::next(ContentType)", SS("unexpected value after ColBegin with value: " << mat.print()));
		case ColLength: throw Error(ecUnexpected, "Slicer::next(ContentType)", SS("unexpected value after ColLength with value: " << mat.print()));
		case Stop: throw Error(ecUnexpected, "Slicer::next(ContentType)", SS("unexpected value after Stop with value: " << mat.print()));
		default: throw Error(ecInvalidEnum, "Slicer::next(DataType)", SS("unexpected enum with value: " << mat.print()));
		}
		contentTypes_.insert(contentTypes_.begin(), mat.vec().begin(), mat.vec().end());
		state_ = ContentType;
	}

	template<typename TIdx, typename TId>
	void DSLib::Table<TIdx, TId>::TypesToColIndices_(DSLib::Matrix<TIdx, TIdx>& colIndices, const std::vector<DataType> dataTypes, const std::vector<ContentType> contentTypes) {
		if (contentTypes.size() == 0 && dataTypes.size() == 0)
			return;
		else
			colIndices.clear();
		if (contentTypes.size() == dataTypes.size()) {			
			auto dt_it = dataTypes.begin();
			for (auto ct_it = contentTypes.begin();ct_it!=contentTypes.end();ct_it++,dt_it++) {
				auto mat = findMatrix(*ct_it, *dt_it);
				if (mat != nullptr) {
					for (auto idx = mat->colOffset_;idx < mat->colOffset_ + mat->colCount_;idx++)
						colIndices.cols.add(idx);
				}
			}
		} else if (contentTypes.size() == 0 && dataTypes.size() > 0) {
			for (auto mat = matrixByIdx_.begin();mat!=matrixByIdx_.end();mat++) {
				auto dt_it = find(dataTypes.begin(), dataTypes.end(), (*mat)->dataType_);
				if (dt_it != dataTypes.end()) {
					for (auto idx = (*mat)->colOffset_;idx < (*mat)->colOffset_ + (*mat)->colCount_;idx++)
						colIndices.cols.add(idx);
				}
			}
		} else if (dataTypes.size() == 0 && contentTypes.size() > 0) {
			for (auto mat = matrixByIdx_.begin();mat!=matrixByIdx_.end();mat++) {
				auto ct_it = find(contentTypes.begin(), contentTypes.end(), (*mat)->contentType_);
				if (ct_it != contentTypes.end()) {
					for (auto idx = (*mat)->colOffset_;idx < (*mat)->colOffset_ + (*mat)->colCount_;idx++)
						colIndices.cols.add(idx);
				}
			}
		} else if (dataTypes.size() == 1 && contentTypes.size() > 0) {
			auto dt = dataTypes[0];
			auto matvec = findMatrices(dt);
			for (auto mat = matvec.begin();mat!=matvec.end();mat++) {
				auto ct_it = find(contentTypes.begin(), contentTypes.end(), (*mat)->contentType_);
				if (ct_it != contentTypes.end()) {
					for (auto idx = (*mat)->colOffset_;idx < (*mat)->colOffset_ + (*mat)->colCount_;idx++)
						colIndices.cols.add(idx);
				}
			}
		} else if (contentTypes.size() == 1 && dataTypes.size() > 0) {
			auto ct = contentTypes[0];
			auto matvec = findMatrices(ct);
			for (auto mat = matvec.begin();mat!=matvec.end();mat++) {
				auto dt_it = find(dataTypes.begin(), dataTypes.end(), (*mat)->dataType_);
				if (dt_it != dataTypes.end()) {
					for (auto idx = (*mat)->colOffset_;idx < (*mat)->colOffset_ + (*mat)->colCount_;idx++)
						colIndices.cols.add(idx);
				}
			}
		}
	}

	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> Table<TIdx, TId>::Slicer::slice() {
		DSLib::Matrix<TIdx, TIdx> indices; //which colIndices have been sliced? No row indices are returned because this could potentially be a very big vec() given the amount of sample/records.
		dst_->clear();
		if ((contentTypes_.size() == 0 && dataTypes_.size() == 0 && colIndices_.vec().count() == 0 && colLength_ == 0) || (rowIndices_.vec().count() == 0 && rowLength_ == 0))
			return indices;
		if ((colLength_ != 0 && colBegin_ + colLength_ > src_->cols.count()) || (rowLength_ != 0  && rowBegin_ + rowLength_ > src_->rows.count()))
			throw Error(ecRangeError, "Table::Slicer::slice()", SS("Slicing out of bounds: Slice col (" << colBegin_ + colLength_ << ") exceeds col count (" <<  src_->cols.count() << ") OR " << "slice row (" << rowBegin_ + rowLength_ << ") exceeds row count (" <<  src_->rows.count() << ")"));
		if (contentTypes_.size() > 1 && dataTypes_.size() > 1 && contentTypes_.size() != dataTypes_.size())
			throw Error(ecParameter, "Table::Slicer::slice()", SS("Cannot slice multiple ContentTypes (" << contentTypes_.size() << ") and multiple DataTypes (" << dataTypes_.size() << ") when counts are not equal"));

		TIdx colOffset = 0;
		TIdx colCount = 0;
		TIdx rowCount = 0;
		TIdx idx = 0;		

		src_->TypesToColIndices_(colIndices_, dataTypes_, contentTypes_);
		useColRegion_ = (~colIndices_ > 0)?false:useColRegion_;

		//update col en row count
		dst_->rows.count_ = (rowLength_ != 0)?rowLength_:rowIndices_.vec().count();
		dst_->cols.count_ = (colLength_ != 0)?colLength_:colIndices_.vec().count();

		if (~(colIndices_ >= src_->cols.count()) > 0)
			throw Error(ecRangeError, "slice()", SS("col index out of range, should be smaller than " << src_->cols.count()));
		if (~(rowIndices_ >= src_->rows.count()) > 0)
				throw Error(ecRangeError, "slice()", SS("row index out of range, should be smaller than " << src_->rows.count()));

		//Copy row ids from source to destination
		if (src_->rows.idxToId_.size() > 0) {
			if (useRowRegion_) {
				for (auto it = src_->rows.idxToId_.begin() + rowBegin_;it != src_->rows.idxToId_.begin() + rowBegin_ + rowLength_;it++, idx++) {
					dst_->rows.idxToId_.push_back(*it);
					dst_->rows.idToIdx_.insert(pair<TId, TIdx>(*it, idx));
				}
			} else {
				TIdx idx=0;
				for (auto it = rowIndices_.vec().begin();it != rowIndices_.vec().end();it++,idx++) {
					dst_->rows.idxToId_.push_back(src_->rows.idxToId_[*it]);
					dst_->rows.idToIdx_.insert(pair<TId, TIdx>(src_->rows.idxToId_[*it], idx));
				}
			}
		}

		//Add (a part of) each Matrix to the sliced Table
		for (typename vector<TableMatrix *>::iterator it=src_->matrixByIdx_.begin();it!=src_->matrixByIdx_.end();it++) {
			TIdx colbegin, colend, collength;
			DSLib::Matrix<TIdx, TIdx> colIndicesPart;
			DSLib::Matrix<TIdx, TIdx> colFirstMat, colLastMat, rowFirstMat, rowLastMat;
			if (useColRegion_) {
				//Determine if the current Matrix is included in the slice
				TIdx begin = colBegin_;
				TIdx end = colBegin_ + colLength_;
				TIdx partBegin = (*it)->colOffset_;
				TIdx partEnd = (*it)->colOffset_ + (*it)->colCount_;
				colbegin = max(begin, partBegin);
				colend = min(end, partEnd);
				if (colend <= colbegin)
					colend = colbegin;
				collength = colend - colbegin;
				colbegin = colbegin - (*it)->colOffset_;
				colCount = collength;
			} else {
				//Make a list of col indices included in this Matrix slice
				for (auto colIdxIt=colIndices_.vec().begin();colIdxIt!=colIndices_.vec().end();colIdxIt++)
					if (*colIdxIt >= (*it)->colOffset_ && *colIdxIt < (*it)->colOffset_ + (*it)->colCount_)
						colIndicesPart.cols.add(*colIdxIt - (*it)->colOffset_);
				colCount = colIndicesPart.cols.count();
			}

			//Check if this Matrix participates in the Table slice
			if (colCount > 0) {
				//Perform the actual slicing of the Matrix
				MatrixBase * sliced = (*it)->data_->create();
				if (useColRegion_) {
					colFirstMat.rows.add(colbegin);
					colLastMat.rows.add(colbegin+collength);					
					//Determine absolute col indices for return value
					(indices | ((*it)->colOffset_ + colbegin)) || ((*it)->colOffset_ + colbegin + collength - 1);
					if (useRowRegion_) {
						rowFirstMat.rows.add(rowBegin_);
						rowLastMat.rows.add(rowBegin_+rowLength_);
						(*it)->data_->slice(*sliced, rowFirstMat, rowLastMat, colFirstMat, colLastMat);
						rowCount = rowLength_;
					} else {
						(*it)->data_->slice(*sliced, rowIndices_, colFirstMat, colLastMat, true);
						rowCount = rowIndices_.vec().count();
					}
				} else {
					DSLib::Matrix<TIdx, TIdx> newIndices = colIndicesPart;
					(!newIndices) + (*it)->colOffset_;
					indices | newIndices;
					if (useRowRegion_) {
						rowFirstMat.rows.add(rowBegin_);
						rowLastMat.rows.add(rowBegin_+rowLength_);
						(*it)->data_->slice(*sliced, colIndicesPart, rowFirstMat, rowLastMat, false);
						//(*sliced).print(cout);
						rowCount = rowLength_;
					} else {
						(*it)->data_->slice(*sliced, rowIndices_, colIndicesPart);
						rowCount = rowIndices_.vec().count();
					}
				}
				//Add the sliced Matrix to the Table
				dst_->addMatrix(*sliced, (*it)->contentType_, (*it)->dataType_, rowCount, colCount, colOffset);
				delete sliced;

				//Copy col ids from source to destination
				if (src_->cols.idxToId_.size() > 0) {
					idx = colOffset;
					if (useColRegion_) {						
						for (auto itid=src_->cols.idxToId_.begin()+colbegin+(*it)->colOffset_;itid!=src_->cols.idxToId_.begin()+colbegin+collength+(*it)->colOffset_;itid++,idx++) {
							dst_->cols.idxToId_.push_back(*itid);
							dst_->cols.idToIdx_.insert(pair<TId, TIdx>(*itid, idx));
						}
					} else {
						for (auto ci=colIndicesPart.vec().begin();ci!=colIndicesPart.vec().end();ci++,idx++) {
							dst_->cols.idxToId_.push_back(src_->cols.idxToId_[*ci+(*it)->colOffset_]);
							dst_->cols.idToIdx_.insert(pair<TId, TIdx>(src_->cols.idxToId_[*ci+(*it)->colOffset_] , idx));
						}
					}
				}
				colOffset += colCount;
			}
		}
		return indices;
	}

	//** Table **//
	//Add a list of ids to idToIdx_ and idxToId_
	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::Index::addIds(const DSLib::Matrix<TId, TIdx> &ids, TIdx insertAt) {
		if (insertAt == count_) {
			for (auto it=ids.vec().begin();it!=ids.vec().end();it++) {
				idToIdx_.insert(pair<TId, TIdx>(*it, insertAt));
				idxToId_.push_back(*it);
				insertAt++;
			}
		} else {
			for (auto it=ids.vec().begin();it!=ids.vec().end();it++) {
				idToIdx_.insert(pair<TId, TIdx>(*it, insertAt));
				idxToId_.insert(idxToId_.begin()+insertAt, *it);			
				insertAt++;
			}
		}
	}	

	//find the list of ids in idxToId_
	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::Index::findIds(DSLib::Matrix<TId, TIdx> &result, const DSLib::Matrix<TId, TIdx> &ids) {
		result.clear();
		vector<TId> res;
		if (idxToId_.size() == 0)
			return;
		for (auto id=ids.vec().begin(); id!=ids.vec().end();id++)
			if (idToIdx_.find(*id) != idToIdx_.end())
				res.push_back(*id);
		result.rows.add(res);
	}
	
	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::Index::findIds(DSLib::Matrix<TId, TIdx> &result, const std::vector<TId> &ids) {
		result.clear();
		vector<TId> res;
		if (idxToId_.size() == 0)
			return;
		for (auto id=ids.begin(); id!=ids.end();id++)
			if (idToIdx_.find(*id) != idToIdx_.end())
				res.push_back(*id);
		result.rows.add(res);
	}

	template <typename TIdx, typename TId>
	typename Table<TIdx, TId>::TableMatrix * Table<TIdx, TId>::findMatrix(DSTypes::ContentType ct, DSTypes::DataType dt) {
		auto res = matrixByType_.find(pair<DSTypes::ContentType, DSTypes::DataType>(ct, dt));
		if (res != matrixByType_.end())
			return (res->second);
		else 
			return nullptr;
	}

	template<typename TIdx, typename TId>
	vector<typename Table<TIdx, TId>::TableMatrix *> Table<TIdx, TId>::findMatrices(DSTypes::ContentType ct) {
		vector<typename Table<TIdx, TId>::TableMatrix *> v;
		for (auto mat = matrixByIdx_.begin();mat != matrixByIdx_.end();mat++)
			if ((*mat)->contentType_ == ct)
				v.push_back(*mat);
		return v;		
	}

	template<typename TIdx, typename TId>
	vector<typename Table<TIdx, TId>::TableMatrix *> Table<TIdx, TId>::findMatrices(DSTypes::DataType dt) {
		vector<typename Table<TIdx, TId>::TableMatrix *> v;
		for (auto mat = matrixByIdx_.begin();mat != matrixByIdx_.end();mat++)
			if ((*mat)->dataType_ == dt)
				v.push_back(*mat);
		return v;
	}

	template<typename TIdx, typename TId>
	map<TId, TIdx> Table<TIdx, TId>::Index::idToIdxAsMap(const TIdx start, const TIdx count) const {
		map<TId, TIdx> m;
		for (TIdx idx = start;idx < start+count;idx++)
			m.insert({idxToId_[idx], idx-start});
		return m;
	}

	template<typename TIdx, typename TId>
	vector<TId> Table<TIdx, TId>::Index::idxToIdAsVec(const TIdx start, const TIdx count) const {
		vector<TId> v;
		v.resize(count);
		copy(idxToId_.begin()+start, idxToId_.begin()+start+count, v.begin());
		return v;
	}

	template<typename TIdx, typename TId>
	DSLib::Matrix<TId, TIdx> Table<TIdx, TId>::Index::idxToId(const TIdx start, const TIdx count) const {
		if (!hasIds())
			throw Error(ecRangeError, "Table::idxToId", "This Index does not contain any Ids");
		TIdx count2 = (count == 0)?count_ - start:count;
		DSLib::Matrix<TId, TIdx> m;
		m.resize(1, count2);
		copy(idxToId_.begin()+start, idxToId_.begin()+start+count2, m.vec().begin());
		return m;
	}

	template<typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> Table<TIdx, TId>::Index::idxs() const {
		Matrix<TIdx, TIdx> m(1, count_);
		TIdx i=0;
		for (auto it=m.rows[0]->begin();it!=m.rows[0]->end();it++,i++)
			*it=i;
		return m;
	}

	//Add a new wrapped Matrix to matrixByType_
	template <typename TIdx, typename TId>
	typename Table<TIdx, TId>::TableMatrix * Table<TIdx, TId>::addMatrix(const DSLib::MatrixBase &mat, DSTypes::ContentType ct, DSTypes::DataType dt, TIdx rowCount, TIdx colCount, TIdx colOffset) {
		TableMatrix * matrixWrap = new TableMatrix(mat.copy());
		matrixWrap->colOffset_ = colOffset;
		matrixWrap->colCount_ = colCount;
		matrixWrap->rowCount_ = rowCount;
		matrixWrap->dataType_ = dt;
		matrixWrap->contentType_ = ct;
		matrixByIdx_.push_back(matrixWrap);
		matrixByType_.insert(pair<pair<ContentType, DataType>, TableMatrix *>(pair<ContentType, DataType>(ct, dt), matrixWrap));
		return matrixWrap;
	}	

	template <typename TIdx, typename TId>
	typename Table<TIdx, TId>::TableMatrix * Table<TIdx, TId>::takeMatrix(const DSLib::MatrixBase &mat, DSTypes::ContentType ct, DSTypes::DataType dt, TIdx rowCount, TIdx colCount, TIdx colOffset) {		
		TableMatrix * matrixWrap = new TableMatrix(mat.take());
		matrixWrap->colOffset_ = colOffset;
		matrixWrap->colCount_ = colCount;
		matrixWrap->rowCount_ = rowCount;
		matrixWrap->dataType_ = dt;
		matrixWrap->contentType_ = ct;
		matrixByIdx_.push_back(matrixWrap);
		matrixByType_.insert(pair<pair<ContentType, DataType>, TableMatrix *>(pair<ContentType, DataType>(ct, dt), matrixWrap));
		return matrixWrap;
	}

	template <typename TIdx, typename TId>
	typename Table<TIdx, TId>::TableMatrix * Table<TIdx, TId>::addMatrix(const DSLib::MatrixBase &&mat, DSTypes::ContentType ct, DSTypes::DataType dt, TIdx rowCount, TIdx colCount, TIdx colOffset) {
		TableMatrix * matrixWrap = new TableMatrix(mat.move());
		matrixWrap->colOffset_ = colOffset;
		matrixWrap->colCount_ = colCount;
		matrixWrap->rowCount_ = rowCount;
		matrixWrap->dataType_ = dt;
		matrixWrap->contentType_ = ct;
		matrixByIdx_.push_back(matrixWrap);
		matrixByType_.insert(pair<pair<ContentType, DataType>, TableMatrix *>(pair<ContentType, DataType>(ct, dt), matrixWrap));
		return matrixWrap;
	}

	template<typename TIdx, typename TId>
	void Table<TIdx, TId>::write(std::ostream & output) const {		
		if (!output)
			throw Error(ecNotFound, "Table::write()", "Stream not open");

		//Write type
		DSUtil::write(output, (String)"Table");		

		//Write first 3 rows for manual interpretation
		Table<TIdx, TId> tab = *this;
		if (tab.rows.count() >3)
			tab.resizeRows(3);
		String tabstr = replaceString(tab.print(), "\n", "");
		DSUtil::write(output, tabstr);

		//Write types and rowcount
		DSUtil::write(output, getIdType());
		DSUtil::write(output, getIdxType());
		DSUtil::write(output, rows.count());

		//Write first row for structure		
		tab.resizeRows(0);
		tab.write_(output);
		this->write_(output);
	}

	template<typename TIdx, typename TId>
	void Table<TIdx, TId>::write_(std::ostream & output) const {
		DSUtil::write(output, rows.count());
		if (rows.hasIds())
			rows.idxToId().write(output);
		else
			Matrix<TId, TIdx>().write(output);
		if (cols.hasIds())
			cols.idxToId().write(output);
		else
			Matrix<TId, TIdx>().write(output);
		TIdx matcount = static_cast<TIdx>(matrixByIdx_.size());
		DSUtil::write(output, matcount);
		for (auto wrap = matrixByIdx_.begin();wrap != matrixByIdx_.end();wrap++) {
			DSUtil::write(output, (*wrap)->contentType());
			DSUtil::write(output, (*wrap)->dataType());
			DSUtil::write(output, (*wrap)->colCount());
			(*wrap)->data().write(output);
		}
	}

	template<typename TIdx, typename TId>
	void Table<TIdx, TId>::write(const DSTypes::String filename) const {
		ofstream ofs(filename, fstream::binary);
		if (!ofs)
			throw Error(ecGeneral, "Table::write", "Unable to open file for writing " + filename);
		return write(ofs);
	}

	template<typename TIdx, typename TId>
	Table<TIdx, TId>& Table<TIdx, TId>::read_(std::istream & input) {
		clear();
		TIdx colcount, rowcount, matcount;
		DSUtil::read(input, rowcount);
		rows.count_ = rowcount;
		//if (rowCount == 0)
			//throw Error(ecIncompatible, "read_", "rows.count() == 0");

		Matrix<TId, TIdx> rowids, colids;
		rowids.read(input);
		colids.read(input);
		DSUtil::read(input, matcount);
		cols.count_ = 0;
		for (TIdx i = 0;i<matcount;i++) {
			ContentType ct;
			DataType dt;
			DSUtil::read(input, ct);
			DSUtil::read(input, dt);
			DSUtil::read(input, colcount);
			MatrixBase * mat = createFromDataType<TIdx>(dt);
			mat->read(input);
			addMatrix(*mat, ct, dt, rowcount, colcount, cols.count_);
			cols.count_ += colcount;
			delete mat;
		}
		if (!rowids.isEmpty())
			rows.setIds(rowids);
		if (!colids.isEmpty())
			cols.setIds(colids);
		return *this;
	}

	template<typename TIdx, typename TId>
	Table<TIdx, TId>& Table<TIdx, TId>::read(std::istream & input) {		
		const String f = "Table::read";

		if (!input)
			throw Error(ecNotFound, f, "Stream not open");

		String type;
		DSUtil::read(input, type);
		if (type != "Table")
			throw Error(ecIncompatible, f, SS("File does not contain a Table but a: " << type));

		DSUtil::read(input, type); //Throw away human readable line
		
		DataType id;
		DataType idx;
		TId idtest;
		TIdx idxtest;
		DSUtil::read(input, id);
		DSUtil::read(input, idx);		
		if (!dataTypeEqual(id, idtest))
			throw Error(ecIncompatible, f, SS("Wrong Id DataType in file. Table type is " << typeName(idtest) << " while file contains " << etos(id)));
		if (!dataTypeEqual(idx, idxtest))
			throw Error(ecIncompatible, f, SS("Wrong Idx DataType in file. Table type is " << typeName(idxtest) << " while file contains " << etos(idx)));

		TIdx rc;
		DSUtil::read(input, rc); //Throw away row count

		Table<TIdx, TId>().read_(input); //read structure (not needed here)
		return this->read_(input); //Read table
	}

	template<typename TIdx, typename TId>
	Table<TIdx, TId>& Table<TIdx, TId>::read(const DSTypes::String filename) {
		ifstream ifs(filename, fstream::binary);
		if (!ifs)
			throw Error(ecGeneral, "Table::read", "Unable to open file for reading " + filename);
		return read(ifs);
	}

	template<typename TIdx, typename TId>
	void DSLib::Table<TIdx, TId>::setContentTypes(const DSLib::Matrix<DSTypes::ContentType, TIdx> &cts) {
		matrixByType_.clear();
		TIdx i=0;
		for (auto mat=matrixByIdx_.begin();mat!=matrixByIdx_.end();mat++,i++) {
			(*mat)->contentType_ = *(cts.vec()[i]);
			matrixByType_.insert(pair<pair<ContentType, DataType>, TableMatrix *>(pair<ContentType, DataType>((*mat)->contentType_, (*mat)->dataType_), *mat));
		}
	}

	template<typename TIdx, typename TId>
	typename Table<TIdx, TId>::TableMatrix & Table<TIdx, TId>::matrixByIdx(TIdx idx) {
		if (idx >= matrixByIdx_.size()) 
			throw Error(ecRangeError, "matrixByIdx", SS("The requested index exceeds the number of matrices in this table: " << idx << " is greater than or equal to " << matrixByIdx_.size()));
		return *matrixByIdx_[idx];;		
	}

	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::updateOffsets() {
		TIdx currOffset = 0;
		for (auto m=matrixByIdx_.begin();m!=matrixByIdx_.end();m++) {
			(*m)->colOffset_ = currOffset;
			currOffset += (*m)->colCount_;
		}
	}

	template<typename TIdx, typename TId>
	bool Table<TIdx, TId>::isSameData(const Table<TIdx, TId> &other) const {
		if (this->matrixByIdx_.size() != other.matrixByIdx_.size())
			return false;
		auto m2 = other.matrixByIdx_.begin();
		for (auto m = matrixByIdx_.begin();m != matrixByIdx_.end();m++,m2++) {
			if ((*m)->dataType() != (*m2)->dataType() || (*m)->contentType() != (*m2)->contentType()) 
				return false;
			if (!(*m)->data().isSameData((*m2)->data()))
				return false;
		}
		return true;
	}

	template<typename TIdx, typename TId>
	bool Table<TIdx, TId>::isSameStructure(const Table<TIdx, TId>& other) const {
		if (other.matrixByIdx_.size() != matrixByIdx_.size() || other.rows.count_ != rows.count_ || other.cols.count_ != cols.count_ )
			return false;
		for (auto wrap=matrixByIdx_.begin(); wrap!=matrixByIdx_.end();wrap++) {
			Table<TIdx, TId> & other2 = const_cast<Table<TIdx, TId>&>(other);
			TableMatrix * oth = other2.findMatrix((*wrap)->contentType(), (*wrap)->dataType());
			if (oth == nullptr)
				return false;
			if (oth->colCount() != (*wrap)->colCount())
				return false;
		}
		return true;
	}

	template<typename TIdx, typename TId>
	DSTypes::ContentType Table<TIdx, TId>::getContentType(const TIdx col) {
		if (col >= cols.count())
			throw Error(ecRangeError, "Table::contentType", SS("col idx " << col << " >= " << cols.count()));
		for (auto matwrap = matrixByIdx_.begin(); matwrap != matrixByIdx_.end(); matwrap++) {
			if ((col >= (*matwrap)->colOffset()) && (col < (*matwrap)->colOffset() + (*matwrap)->colCount()))
				return (*matwrap)->contentType();
		}
		throw Error(ecInvalidEnum, "Table::contentType", "Invalid enum");
	}

	template<typename TIdx, typename TId>
	DSTypes::DataType Table<TIdx, TId>::getDataType(const TIdx col) {
		if (col >= cols.count())
			throw Error(ecRangeError, "Table::dataType", SS("col idx " << col << " >= " << cols.count()));
		for (auto matwrap = matrixByIdx_.begin(); matwrap != matrixByIdx_.end(); matwrap++) {
			if ((col >= (*matwrap)->colOffset()) && (col < (*matwrap)->colOffset() + (*matwrap)->colCount()))
				return (*matwrap)->dataType();
		}
		throw Error(ecInvalidEnum, "Table::dataType", "Invalid enum");
	}

	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::print(ostream &output) const {
		output << "/*Table*/" << endl;
		if (rows.hasIds()) {
			Matrix<TId, TIdx> colids;
			colids.setOrder(oRowMajor);
			colids.rows.add(rows.idxToId_);
			stringstream ss;
			colids.print(ss);
			output << replaceString(replaceString(ss.str(), "\n", ""), " ", "") << " |" << endl;
		}
		if (cols.hasIds()) {
			Matrix<TId, TIdx> rowids;
			rowids.setOrder(oRowMajor);
			rowids.rows.add(cols.idxToId_);
			stringstream ss;
			rowids.print(ss);
			output << "(" << replaceString(replaceString(ss.str(), "\n", ""), " ", "") << " ^" << endl;
		}
		TIdx idx=0;
		if (isInitialized())
			output << "(";
		for (auto it = matrixByIdx_.begin();it != matrixByIdx_.end();it++,idx++) {
			if (it != matrixByIdx_.begin())
				output << ") | ";
			output << "(ct" << etos((*it)->contentType_) << " |" << endl;
			output << (*it)->data_->print();
		};
		if (cols.hasIds())
			output << ")";
		if (isInitialized())
			output << "))";
		output << endl;
	}

	template<typename TIdx, typename TId>
	std::string Table<TIdx, TId>::print() const {
		stringstream ss;
		print(ss);
		return ss.str();
	}


	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::printLess(ostream &output) const {
		if (cols.idxToId_.size() > 0) {
			output << "colIds: ";
			size_t i = 0;
			for (auto it = cols.idxToId_.begin();it != cols.idxToId_.end();it++, i++)
				output << *it << " (" << i << "), ";
			output << endl;
		}
		if (rows.idxToId_.size() > 0) {
			output << "rowIds: ";
			for (auto it = rows.idxToId_.begin();it != rows.idxToId_.end();it++)
				output << *it << ", ";
			output << endl;
		}
		for (auto it = matrixByIdx_.begin();it != matrixByIdx_.end();it++) {
			output << "DataType: " << etos((*it)->dataType_) << " ContentType: " << etos((*it)->contentType_) << endl;
			output << "Matrix-- output: " << endl << (*it)->data_->printLess();
		};
	}

	template<typename TIdx, typename TId>
	std::string Table<TIdx, TId>::printLess() const {
		stringstream ss;
		printLess(ss);
		return ss.str();
	}


	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::clear() {
		cols.clear();
		rows.clear();
		while (matrixByIdx_.begin() != matrixByIdx_.end()) {
			delete *(matrixByIdx_.begin());
			matrixByIdx_.erase(matrixByIdx_.begin());
		}
		matrixByType_.clear();
	}

	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::clearAll() {
		cols.clear();
		rows.clear();
		while (matrixByIdx_.begin() != matrixByIdx_.end()) {
			(*(matrixByIdx_.begin()))->data().clearAll();
			delete *(matrixByIdx_.begin());
			matrixByIdx_.erase(matrixByIdx_.begin());
		}
		matrixByType_.clear();
	}

	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::clone(const Table<TIdx, TId> &src) {
		clear();
		cols = src.cols;
		cols.table_ = this;
		rows = src.rows;
		rows.table_ = this;
		cols.contra_ = &rows;
		rows.contra_ = &cols;
		for (auto mat = src.matrixByIdx_.begin(); mat != src.matrixByIdx_.end(); mat++) {
			addMatrix(((*mat)->data()), (*mat)->contentType(), (*mat)->dataType(), (*mat)->rowCount(), (*mat)->colCount(), (*mat)->colOffset());
		}
	}

	template<typename TIdx, typename TId>
	void Table<TIdx, TId>::clone(Table && src) {
		clear();
		cols = src.cols;
		cols.table_ = this;
		rows = src.rows;
		rows.table_ = this;
		cols.contra_ = &rows;
		rows.contra_ = &cols;
		for (auto mat = src.matrixByIdx_.begin(); mat != src.matrixByIdx_.end(); mat++) {
			addMatrix(std::move((*mat)->data()), (*mat)->contentType(), (*mat)->dataType(), (*mat)->rowCount(), (*mat)->colCount(), (*mat)->colOffset());
		}
	}

	template<typename TIdx, typename TId>
	Table<TIdx, TId> Table<TIdx, TId>::dup() const {
		Table<TIdx, TId> t = const_cast<Table<TIdx, TId>&>(*this);
		t.breakUpDown();
		return t;
	}

	template<typename TIdx, typename TId>
	void Table<TIdx, TId>::breakUpDown() {
		for (auto matWrap = matrixByIdx_.begin();matWrap != matrixByIdx_.end();matWrap++) {
			(*matWrap)->data().breakDown();
			(*matWrap)->data().breakUp(true);
		}
	}

	template<typename TIdx, typename TId>
	Table<TIdx, TId> & Table<TIdx, TId>::operator=(const Table<TIdx, TId> & other) {
		if (this == &other)
			return *this;

		//Direct slice assignemnt
		if (isSameStructure(other)) { 
			for (auto wrap=matrixByIdx_.begin();wrap!=matrixByIdx_.end();wrap++) {
				Table<TIdx, TId> & other2 = const_cast<Table<TIdx, TId>&>(other);
				(*wrap)->data() = other2.findMatrix((*wrap)->contentType(), (*wrap)->dataType())->data();
			}
			return *this;
		}
		clone(other);
		return *this;
	}

	template<typename TIdx, typename TId>
	Table<TIdx, TId> & Table<TIdx, TId>::operator=(Table<TIdx, TId> && other) {
		if (this == &other)
			return *this;

		//Direct slice assignemnt
		if (isSameStructure(other)) {
			for (auto wrap = matrixByIdx_.begin();wrap!=matrixByIdx_.end();wrap++) {
				(*wrap)->data() = std::move(other.findMatrix((*wrap)->contentType(), (*wrap)->dataType())->data());
			}
			return *this;
		}
		clone(std::move(other));
		return *this;
	}

	template <typename TIdx, typename TId>
	template <typename T>
	Table<TIdx, TId> & Table<TIdx, TId>::operator=(const T &val) {
		for (auto matWrap = matrixByIdx_.begin();matWrap != matrixByIdx_.end();matWrap++) {
			if ((*matWrap)->dataType() == dataType(val)) {
				DSLib::Matrix<T, TIdx> & mat = static_cast<DSLib::Matrix<T, TIdx> &>((*matWrap)->data());
				mat = val;
			} else {
			#ifdef _DEBUG
				cout << "Note: Skipped an invalid embedded matrix type" << endl;
			#endif
			}
		}
		return *this;
	}
		
	template <typename TIdx, typename TId>
	template <typename T>
	Table<TIdx, TId> & Table<TIdx, TId>::operator=(const DSLib::Matrix<T> &mat) {
		for (auto matWrap = matrixByIdx_.begin();matWrap != matrixByIdx_.end();matWrap++) {
			if ((*matWrap)->dataType() == mat.getDataType()) {
				DSLib::Matrix<T, TIdx> & mati = static_cast<DSLib::Matrix<T, TIdx> &>((*matWrap)->data());
				mati = mat;
			} else {
			#ifdef _DEBUG
				cout << "Note: Skipped an invalid embedded matrix type" << endl;
			#endif
			}
		}
		return *this;
	}

	template <typename TIdx, typename TId>
	void Table<TIdx, TId>::Index::clear() {
		count_ = 0;
		idxToId_.clear();
		idToIdx_.clear();
	}			


	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> Table<TIdx, TId>::Index::add(const DSLib::MatrixBase &mat, const TIdx rowCount, const TIdx colCount, const DSTypes::ContentType ct, const DSTypes::DataType dt, const DSLib::Matrix<TId, TIdx> &rowIds, const DSLib::Matrix<TId, TIdx> &colIds) {
		/*if (!mat.isInitialized())
			return Matrix<TIdx, TIdx>();*/
		const string f = "Index::add()";
		if (colIds.isInitialized() && !colIds.isVector())
			throw Error(ecSizeMismatch, f, SS("colIds should be a vector" << colIds.printLess()));
		if (rowIds.isInitialized() && !rowIds.isVector())
			throw Error(ecSizeMismatch, f, SS("rowIds should be a vector" << rowIds.printLess()));
		if (colIds.isInitialized() && colIds.isVector() && colIds.vec().count() != colCount)
			throw Error(ecSizeMismatch, f, SS("ColIds.count() (" << colIds.vec().count() << ") unequal to mat.cols.count() (" << colCount << ")"));
		if (rowIds.isInitialized() && rowIds.isVector() && rowIds.vec().count() != rowCount)
			throw Error(ecSizeMismatch, f, SS("RowIds.count() (" << rowIds.vec().count() << ") unequal to mat.rows.count() (" << rowCount << ")"));
		DSLib::Matrix<TIdx, TIdx> indices; //Which indices have been added?

		if (!table_->isInitialized()) {
			table_->addMatrix(mat, ct, dt, rowCount, colCount, 0);
			table_->rows.addIds(rowIds, 0);
			table_->cols.addIds(colIds, 0);
			table_->rows.count_ = rowCount;
			table_->cols.count_ = colCount;
		} else {
			if (isCol() && colIds.isInitialized() != (count() > 0) && hasIds()) {
				if (hasIds()) {
					this->idxToId_.clear();
					this->idToIdx_.clear();
				} else
					throw Error(ecIncompatible, f, "Col cannot have an id");
			}
			if (isRow() && rowIds.isInitialized() != (count() > 0) && hasIds()) {
				if (hasIds()) {
					this->idxToId_.clear();
					this->idToIdx_.clear();
				} else
					throw Error(ecIncompatible, f, "Row cannot have an id");
			}
			if (isCol() && contra_->hasIds() == false && rowIds.isInitialized())
				throw Error(ecIncompatible, f, "No row ids known, cannot weave col");
			if (isRow() && contra_->hasIds() == false && colIds.isInitialized())
				throw Error(ecIncompatible, f, "No col ids known, cannot weave row");
			if (isCol() && table_->rows.count_ != rowCount)
				throw Error(ecSizeMismatch, f, SS("Matrix row count(" << rowCount << ") not equal to table row count (" << table_->rows.count_ << ")"));
			if (isRow() && table_->cols.count_ != colCount)
				throw Error(ecSizeMismatch, f, SS("Matrix col count(" << colCount << ") not equal to table col count (" << table_->cols.count_ << ")"));
			if (isCol()) {
				DSLib::Matrix<TId, TIdx> colInter, rowInter;
				findIds(colInter, colIds);
				contra_->findIds(rowInter, rowIds);
				if (colInter.isInitialized())
					throw Error(ecIncompatible, f, SS("Col id(s) already exist " << colInter.print()));
				if (rowInter.vec().count() != rowIds.vec().count())
					throw Error(ecUnexpected, f, "Not all row ids accounted for. Cannot weave.");
			} else if (isRow()) {
				DSLib::Matrix<TId, TIdx> colInter, rowInter;
				findIds(rowInter, rowIds);
				contra_->findIds(colInter, colIds);
				if (rowInter.isInitialized())
					throw Error(ecIncompatible, f, "Row id(s) already exist");
				if (colInter.vec().count() != colIds.vec().count())
					throw Error(ecUnexpected, f, "Not all col ids accounted for. Cannot weave.");
			}
			TableMatrix * matWrap = table_->findMatrix(ct, dt);
			MatrixBase * mat2 = mat.copy();
			if (matWrap != nullptr) {
				if (isCol()) {
					if (rowIds.isInitialized())
						sortById(mat2->rowsBase(), rowIds, contra_->idToIdx_);
					addIds(colIds, matWrap->colOffset_ + matWrap->colCount_);
					indices =  (DSLib::Matrix<TIdx, TIdx>() | (matWrap->colOffset_ + matWrap->colCount_)) || colCount;
					matWrap->data_->colsBase().add(*mat2);
					matWrap->colCount_ = matWrap->colCount_ + colCount;
					count_ += colCount;
					table_->updateOffsets();
				} else if (isRow()) {
					if (colIds.isInitialized())
						sortById(mat2->colsBase(), colIds, contra_->idToIdx_);
					matWrap->data_->rowsBase().add(*mat2);
					addIds(rowIds, contra_->count_);
					indices = (DSLib::Matrix<TIdx, TIdx>() | contra_->count_) || rowCount;
					count_ += rowCount;
				}
			} else {
				if (isCol()) {
					if (rowIds.isInitialized())
						sortById(mat2->rowsBase(), rowIds, contra_->idToIdx_);
					table_->addMatrix(*mat2, ct, dt, rowCount, colCount, count_);
					addIds(colIds, count_);
					indices = (DSLib::Matrix<TIdx, TIdx>() | count_) || colCount;
					count_ += colCount;
				} else if (isRow()) {
					throw Error(ecUnexpected, f, "Cannot add a table rows with different types.");
				}
			}
			delete mat2;
		}
		return indices;
	}
	
	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> Table<TIdx, TId>::Index::take(const DSLib::MatrixBase &mat, const TIdx rowCount, const TIdx colCount, const DSTypes::ContentType ct, const DSTypes::DataType dt, const DSLib::Matrix<TId, TIdx> &rowIds, const DSLib::Matrix<TId, TIdx> &colIds) {
		if (!mat.isInitialized())
			return Matrix<TIdx, TIdx>();
		const string f = "Index::take()";
		if (colIds.isInitialized() && !colIds.isVector())
			throw Error(ecSizeMismatch, f, SS("colIds should be a vector" << colIds.printLess()));
		if (rowIds.isInitialized() && !rowIds.isVector())
			throw Error(ecSizeMismatch, f, SS("rowIds should be a vector" << rowIds.printLess()));
		if (colIds.isInitialized() && colIds.isVector() && colIds.vec().count() != colCount)
			throw Error(ecSizeMismatch, f, SS("ColIds.count() (" << colIds.vec().count() << ") unequal to mat.cols.count() (" << colCount << ")"));
		if (rowIds.isInitialized() && rowIds.isVector() && rowIds.vec().count() != rowCount)
			throw Error(ecSizeMismatch, f, SS("RowIds.count() (" << rowIds.vec().count() << ") unequal to mat.rows.count() (" << rowCount << ")"));
		DSLib::Matrix<TIdx, TIdx> indices; //Which indices have been added?

		if (!table_->isInitialized()) {
			table_->takeMatrix(mat, ct, dt, rowCount, colCount, 0);
			table_->rows.addIds(rowIds, 0);
			table_->cols.addIds(colIds, 0);
			table_->rows.count_ = rowCount;
			table_->cols.count_ = colCount;
		} else {
			if (isCol() && colIds.isInitialized() != (count() > 0) && hasIds()) {
				if (hasIds())
					throw Error(ecIncompatible, f, "Col should have an id");
				else
					throw Error(ecIncompatible, f, "Col cannot have an id");
			}
			if (isRow() && rowIds.isInitialized() != (count() > 0) && hasIds()) {
				if (hasIds())
					throw Error(ecIncompatible, f, "Row should have an id");
				else
					throw Error(ecIncompatible, f, "Row cannot have an id");
			}
			if (isCol() && contra_->hasIds() == false && rowIds.isInitialized())
				throw Error(ecIncompatible, f, "No row ids known, cannot weave col");
			if (isRow() && contra_->hasIds() == false && colIds.isInitialized())
				throw Error(ecIncompatible, f, "No col ids known, cannot weave row");
			if (isCol() && table_->rows.count_ != rowCount)
				throw Error(ecSizeMismatch, f, SS("Matrix row count(" << rowCount << ") not equal to table row count (" << table_->rows.count_ << ")"));
			if (isRow() && table_->cols.count_ != colCount)
				throw Error(ecSizeMismatch, f, SS("Matrix col count(" << colCount << ") not equal to table col count (" << table_->cols.count_ << ")"));
			if (isCol()) {
				DSLib::Matrix<TId, TIdx> colInter, rowInter;
				findIds(colInter, colIds);
				contra_->findIds(rowInter, rowIds);
				if (colInter.isInitialized())
					throw Error(ecIncompatible, f, SS("Col id(s) already exist " << colInter.print()));
				if (rowInter.vec().count() != rowIds.vec().count())
					throw Error(ecUnexpected, f, "Not all row ids accounted for. Cannot weave.");
			} else if (isRow()) {
				DSLib::Matrix<TId, TIdx> colInter, rowInter;
				findIds(rowInter, rowIds);
				contra_->findIds(colInter, colIds);
				if (rowInter.isInitialized())
					throw Error(ecIncompatible, f, "Row id(s) already exist");
				if (colInter.vec().count() != colIds.vec().count())
					throw Error(ecUnexpected, f, "Not all col ids accounted for. Cannot weave.");
			}
			TableMatrix * matWrap = table_->findMatrix(ct, dt);
			MatrixBase * mat2 = mat.copy();
			if (matWrap != nullptr) {
				if (isCol()) {
					bool sorted = false;
					if (rowIds.isInitialized())
						sorted = sortById(mat2->rowsBase(), rowIds, contra_->idToIdx_);
					addIds(colIds, matWrap->colOffset_ + matWrap->colCount_);
					indices = (DSLib::Matrix<TIdx, TIdx>() | (matWrap->colOffset_ + matWrap->colCount_)) || colCount;
					if (sorted)
						matWrap->data_->colsBase().add(*mat2);
					else
						matWrap->data_->colsBase().take(mat);
					matWrap->colCount_ = matWrap->colCount_ + colCount;
					count_ += colCount;
					table_->updateOffsets();
				} else if (isRow()) {
					bool sorted = false;
					if (colIds.isInitialized())
						sorted = sortById(mat2->colsBase(), colIds, contra_->idToIdx_);
					if (sorted)
						matWrap->data_->rowsBase().add(*mat2);
					else
						matWrap->data_->rowsBase().take(mat);
					addIds(rowIds, contra_->count_);
					indices = (DSLib::Matrix<TIdx, TIdx>() | contra_->count_) || rowCount;
					count_ += rowCount;
				}
			} else {
				if (isCol()) {
					bool sorted = false;
					if (rowIds.isInitialized())
						sorted = sortById(mat2->rowsBase(), rowIds, contra_->idToIdx_);
					if (sorted)
						table_->addMatrix(*mat2, ct, dt, rowCount, colCount, count_);
					else
						table_->takeMatrix(mat, ct, dt, rowCount, colCount, count_);
					addIds(colIds, count_);
					indices = (DSLib::Matrix<TIdx, TIdx>() | count_) || colCount;
					count_ += colCount;
				}
				else if (isRow()) {
					throw Error(ecUnexpected, f, "Cannot add a table rows with different types.");
				}
			}
			delete mat2;
		}
		return indices;
	}

	template<typename TIdx, typename TId>
	void Table<TIdx, TId>::resizeRows(TIdx count) {
		for (auto matWrap = matrixByIdx_.begin(); matWrap != matrixByIdx_.end();matWrap++) {
			(*matWrap)->data().resize2(count, (*matWrap)->colCount());
			(*matWrap)->rowCount_ = count;
		}
		rows.count_ = count;
		if (rows.idxToId_.size() > 0) {
			if (count < rows.count_) {
				for (auto id = rows.idxToId_.begin() + count; id != rows.idxToId_.end();id++)
					rows.idToIdx_.erase(rows.idToIdx_.find(*id));
			}
			rows.idxToId_.resize(count);
		}
	}

	template<typename TIdx, typename TId>
	void Table<TIdx, TId>::addRows(const Table &tab) {
		const string f = "addRows()";
		if (this->cols.count_ != 0 && this->cols.count_ != tab.cols.count_)
			throw Error(ecSizeMismatch, f, SS("cols count (" << this->cols.count_ << ") does not match the count of the cols to be added (" << tab.cols.count_ << ")"));

		if (!this->isInitialized()) {
			*this = tab;
		} else {
			if (this->cols.hasIds() != tab.cols.hasIds()) {
				if (tab.cols.hasIds())
					throw Error(ecIncompatible, f, "Row should have an id");
				else
					throw Error(ecIncompatible, f, "Row cannot have an id");			
			}

			DSLib::Matrix<TId, TIdx> colInter, rowInter, rowIds, colIds;			
			rowIds.rows.add(tab.rows.idxToId_);
			colIds.cols.add(tab.cols.idxToId_);
			rows.findIds(rowInter, rowIds);
			cols.findIds(colInter, colIds);
			if (rowInter.isInitialized())
				throw Error(ecIncompatible, f, "Row id(s) already exist");
			if (colInter.vec().count() != colIds.vec().count())
				throw Error(ecUnexpected, f, "Not all col ids accounted for. Cannot weave.");

			for (auto srcMatWrap = tab.matrixByIdx_.begin(); srcMatWrap != tab.matrixByIdx_.end(); srcMatWrap++) {				
				TableMatrix * matWrap = findMatrix((*srcMatWrap)->contentType(), (*srcMatWrap)->dataType());
				if (matWrap == nullptr)
					throw Error(ecUnexpected, f, "Cannot add a matrix with with different types.");
				MatrixBase * mat2 = (*srcMatWrap)->data().copy();
				if (tab.cols.hasIds()) {
					colIds = tab.cols.idxToId((*srcMatWrap)->colOffset_, (*srcMatWrap)->colCount_);
					map<TId, TIdx> dstColIds = cols.idToIdxAsMap(matWrap->colOffset_, matWrap->colCount_);
					sortById(mat2->colsBase(), colIds, dstColIds); //weaving
				}
				matWrap->data_->rowsBase().add(*mat2);
				matWrap->rowCount_ += tab.rows.count_;
				delete mat2;
			}
			rows.addIds(rowIds, rows.count_);
			rows.count_ += tab.rows.count_;
		}
	}

	template<typename TIdx, typename TId>
	void Table<TIdx, TId>::takeRows(const Table &tab) {
		const string f = "takeRows()";
		if (this->cols.count_ != tab.cols.count_)
			throw Error(ecSizeMismatch, f, SS("cols count (" << this->cols.count_ << ") does not match the count of the cols to be added (" << tab.cols.count_ << ")"));

		if (!this->isInitialized()) {
			*this = tab;
		}
		else {
			if (this->cols.hasIds() != tab.cols.hasIds()) {
				if (tab.cols.hasIds())
					throw Error(ecIncompatible, f, "Row should have an id");
				else
					throw Error(ecIncompatible, f, "Row cannot have an id");
			}

			DSLib::Matrix<TId, TIdx> colInter, rowInter, rowIds, colIds;
			rowIds.rows.add(tab.rows.idxToId_);
			colIds.cols.add(tab.cols.idxToId_);
			rows.findIds(rowInter, rowIds);
			cols.findIds(colInter, colIds);
			if (rowInter.isInitialized())
				throw Error(ecIncompatible, f, "Row id(s) already exist");
			if (colInter.vec().count() != colIds.vec().count())
				throw Error(ecUnexpected, f, "Not all col ids accounted for. Cannot weave.");

			for (auto srcMatWrap = tab.matrixByIdx_.begin(); srcMatWrap != tab.matrixByIdx_.end(); srcMatWrap++) {
				TableMatrix * matWrap = findMatrix((*srcMatWrap)->contentType(), (*srcMatWrap)->dataType());
				if (matWrap == nullptr)
					throw Error(ecUnexpected, f, "Cannot add a matrix with the same colIds, but different types.");
				MatrixBase * mat2 = (*srcMatWrap)->data().copy();
				bool sorted = false;
				if (tab.cols.hasIds()) {
					colIds = tab.cols.idxToId((*srcMatWrap)->colOffset_, (*srcMatWrap)->colCount_);
					map<TId, TIdx> dstColIds = cols.idToIdxAsMap(matWrap->colOffset_, matWrap->colCount_);
					sorted = sortById(mat2->colsBase(), colIds, dstColIds); //weaving
				}
				if (sorted)
					matWrap->data_->rowsBase().add(*mat2);
				else
					matWrap->data_->rowsBase().take((*srcMatWrap)->data());
				matWrap->rowCount_ += tab.rows.count_;
				delete mat2;
			}
			rows.addIds(rowIds, rows.count_);
			rows.count_ += tab.rows.count_;
		}
	}

	template<typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> Table<TIdx, TId>::Index::add(const Table &tab) {
		DSLib::Matrix<TIdx, TIdx> indices;
		if (!tab.isInitialized())
			return indices;
		if (table_->isInitialized() && isCol() && tab.rows.count() != table_->rows.count())
			throw Error(ecSizeMismatch, "add(Table)", SS("tab.rows.count != table.rows.count (" << tab.rows.count() << " != " << table_->rows.count() << ")"));
		if (table_->isInitialized() && isRow() && tab.cols.count() != table_->cols.count())
			throw Error(ecSizeMismatch, "add(Table)", SS("tab.cols.count != table.cols.count (" << tab.cols.count() << " != " << table_->cols.count() << ")"));
		if (isRow()) {
			table_->addRows(tab);
		} else if (isCol()) {
			for (auto matwrap = tab.matrixByIdx_.begin(); matwrap != tab.matrixByIdx_.end(); matwrap++) {
				DSLib::Matrix<TId, TIdx> colIds, rowIds;
				if (tab.cols.hasIds()) {
					colIds.resize(1, (*matwrap)->colCount_);
					copy(tab.cols.idxToId_.begin() + (*matwrap)->colOffset_, tab.cols.idxToId_.begin()  + (*matwrap)->colOffset_ + (*matwrap)->colCount_, colIds.vec().begin());
				}
				if (tab.rows.hasIds()) {
					rowIds.resize(1, tab.rows.count());
					copy(tab.rows.idxToId_.begin(), tab.rows.idxToId_.begin() + tab.rows.count(), rowIds.vec().begin());
				}
				indices || add(*(*matwrap)->data_, (*matwrap)->rowCount_, (*matwrap)->colCount_, (*matwrap)->contentType_, (*matwrap)->dataType_, rowIds, colIds);
			}
		}
		return indices;
	};

	template<typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> Table<TIdx, TId>::Index::take(const Table &tab) {
		DSLib::Matrix<TIdx, TIdx> indices;
		if (!tab.isInitialized())
			return indices;
		if (table_->isInitialized() && isCol() && tab.rows.count() != table_->rows.count())
			throw Error(ecSizeMismatch, "take(Table)", SS("tab.rows.count != table.rows.count (" << tab.rows.count() << " != " << table_->rows.count() << ")"));
		if (table_->isInitialized() && isRow() && tab.cols.count() != table_->cols.count())
			throw Error(ecSizeMismatch, "take(Table)", SS("tab.cols.count != table.cols.count (" << tab.cols.count() << " != " << table_->cols.count() << ")"));
		if (isRow()) {
			table_->takeRows(tab);
		} else if (isCol()) {
			for (auto matwrap = tab.matrixByIdx_.begin(); matwrap != tab.matrixByIdx_.end(); matwrap++) {
				DSLib::Matrix<TId, TIdx> colIds, rowIds;
				if (tab.cols.hasIds()) {
					colIds.resize(1, (*matwrap)->colCount_);
					copy(tab.cols.idxToId_.begin() + (*matwrap)->colOffset_, tab.cols.idxToId_.begin() + (*matwrap)->colOffset_ + (*matwrap)->colCount_, colIds.vec().begin());
				}
				if (tab.rows.hasIds()) {
					rowIds.resize(1, tab.rows.count());
					copy(tab.rows.idxToId_.begin(), tab.rows.idxToId_.begin() + tab.rows.count(), rowIds.vec().begin());
				}
				indices || take(*(*matwrap)->data_, (*matwrap)->rowCount_, (*matwrap)->colCount_, (*matwrap)->contentType_, (*matwrap)->dataType_, rowIds, colIds);
			}
		}
		return indices;
	};


	template<typename TIdx, typename TId>
	void DSLib::Table<TIdx, TId>::Index::setIds(const DSLib::Matrix<TId, TIdx>& ids) {
		if (~ids != count_ && ~ids != 0)
			throw Error(ecRangeError, "Matrix::Index::setIds", "Invalid number of elements in the id vector");
		idxToId_.clear();
		idToIdx_.clear();		
		addIds(ids, 0);
	}

	template<typename TIdx, typename TId>
	TIdx Table<TIdx, TId>::Index::idToIdx(const TId id) {
		typename std::map<TId, TIdx>::iterator idx = idToIdx_.find(id);
		if (idx != idToIdx_.end())
			return idx->second;
		throw Error(ecNotFound, "Table::Index::idToIdx()", SS(" Id \"" << id << "\" not found. Possible values are: " << ids().print()));
	}

	template<typename TIdx, typename TId>
	TId Table<TIdx, TId>::Index::idxToId(const TIdx idx) {
		try {
			return idxToId_[idx];
		} catch (...) {
			throw Error(ecNotFound, "Table::Index::idxToId()", SS(" Idx " << idx << " not found."));
		}
	}

	template<typename TIdx, typename TId>
	TIdx Table<TIdx, TId>::Index::count() const {
		 return count_;
	}
}
