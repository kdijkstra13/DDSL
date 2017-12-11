#pragma once
#include "h/lib/DS_MatrixFunc.h"
#include "hpp/lang/DS_MatrixLang.hpp"
#include "hpp/lib/DS_Image.hpp"

#ifndef DDSL_NO_EXT_LIB_PNG
#include "h/lib/DS_Image_PNG.h"
#endif

using namespace DSTypes;

namespace DSFunc {
	template<typename TInt>
	inline DSLib::MatrixBase * createFromDataType(DSTypes::DataType dt, TInt tint) {
		switch (dt) {
            case dtFloat: return new DSLib::Matrix<Float, TInt>();
            case dtDouble: return new DSLib::Matrix<Double, TInt>();
            case dtUInt32: return new DSLib::Matrix<UInt32, TInt>();
            case dtInt32:return new DSLib::Matrix<Int32, TInt>();
            case dtContentType: return new DSLib::Matrix<ContentType, TInt>();
            case dtDataType: return new DSLib::Matrix<DataType, TInt>();
            case dtString: return new DSLib::Matrix<String, TInt>();
            case dtMatrixString: return new DSLib::Matrix<MatrixString, TInt>();
            case dtMatrixInt32: return new DSLib::Matrix<DSLib::Matrix<Int32, TInt>, TInt>();
            case dtMatrixFloat: return new DSLib::Matrix<DSLib::Matrix<Float, TInt>, TInt>();
#ifndef DDSL_NO_EXT_LIB_PNG
            case dtImagePNGDouble: return new DSLib::Matrix<DSImage::ImagePNG<Double>, TInt>();
            case dtImagePNGFloat: return new DSLib::Matrix<DSImage::ImagePNG<Float>, TInt>();
#endif
			default: throw Error(ecUnsupported, "create", "Unsupported DataType. Please add your own to DS_MatrixFunc.hpp -> DSLib::MatrixBase * createFromDataType(DSTypes::DataType dt, TInt tint)");
		}
		return nullptr;
	}

	template<typename T, typename TInt>
    inline DSLib::MatrixBase * convertFromDataType(DSTypes::DataType dstType, DSLib::Matrix<T, TInt>& src) {
		switch (dstType) {
            case dtFloat: {DSLib::Matrix<Float, TInt> * dst = new DSLib::Matrix<Float, TInt>();convert(*dst, src);return dst;}
            case dtDouble: {DSLib::Matrix<Double, TInt> * dst = new DSLib::Matrix<Double, TInt>();convert(*dst, src);return dst;}
            case dtUInt32: {DSLib::Matrix<UInt32, TInt> * dst = new DSLib::Matrix<UInt32, TInt>();convert(*dst, src);return dst;}
            case dtInt32: {DSLib::Matrix<Int32, TInt> * dst = new DSLib::Matrix<Int32, TInt>();convert(*dst, src);return dst;}
            case dtContentType: {DSLib::Matrix<ContentType, TInt> * dst = new DSLib::Matrix<ContentType, TInt>();convert(*dst, src);return dst;}
            case dtDataType: {DSLib::Matrix<DataType, TInt> * dst = new DSLib::Matrix<DataType, TInt>();convert<DataType, T, TInt>(*dst, src);return dst;}
            case dtString: {DSLib::Matrix<String, TInt> * dst = new DSLib::Matrix<String, TInt>();convert(*dst, src);return dst;
		} default:
            throw Error(ecUnsupported, "convert", "Unsupported DataType. Please add your own to DS_MatrixFunc.hpp -> DSLib::MatrixBase * convertFromDataType(DSTypes::DataType dstType, DSLib::Matrix<T, TInt>& src)");
		}
	}

	template<typename ToT, typename FromT>
	inline ToT & convertValue(const FromT & from, ToT & to) {
		throw Error(ecNotImplemented, "Convert", SS("Cannot convert from " << typeName(from) << " to " << typeName(to) << ". Please add your own convertValue() specialization to DS_MatrixFunc.hpp"));
	}

    template<> inline String & convertValue(const Float &from, String &to) { std::stringstream ss;ss << from;to = ss.str();return to; }
    template<> inline String & convertValue(const Double &from, String &to) { std::stringstream ss;ss << from;to = ss.str();return to; }
    template<> inline String & convertValue(const UInt32 &from, String &to) { std::stringstream ss;ss << from;to = ss.str();return to; }
    template<> inline String & convertValue(const Int32 &from, String &to) { std::stringstream ss;ss << from;to = ss.str();return to; }
    template<> inline String & convertValue(const ContentType &from, String &to) { std::stringstream ss;ss << etos(from);to = ss.str();return to; }
    template<> inline String & convertValue(const DataType &from, String &to) { std::stringstream ss;ss << etos(from);to = ss.str();return to; }

    template<> inline Float & convertValue(const String &from, Float &to) { std::stringstream ss; ss << from; ss >> to;return to; }
    template<> inline Double & convertValue(const String &from, Double &to) { std::stringstream ss;ss << from;ss >> to;return to; }
    template<> inline UInt32 & convertValue(const String &from, UInt32 &to) { std::stringstream ss;ss << from;ss >> to;return to; }
    template<> inline Int32 & convertValue(const String &from, Int32 &to) { std::stringstream ss;ss << from;ss >> to;return to; }
    template<> inline ContentType & convertValue(const String &from, ContentType &to) { std::stringstream ss;ss << from;ss >> stoe(to);return to; }
    template<> inline DataType & convertValue(const String &from, DataType &to) { std::stringstream ss;ss << from;ss >> stoe(to);return to; }

	template<> inline Double & convertValue(const UInt32 &from, Double &to) { to = from; return to; }
	template<> inline Double & convertValue(const UInt16 &from, Double &to) { to = from; return to; }
	template<> inline UInt32 & convertValue(const Double &from, UInt32 &to) { to = (UInt32)from; return to; }

	template<> inline Float & convertValue(const UInt32 &from, Float &to) { to = (Float)from; return to; }
	template<> inline UInt32 & convertValue(const Float &from, UInt32 &to) { to = (UInt32)from; return to; }


	template <typename T, typename TInt>
    inline DSLib::Matrix<T, TInt> repeat(const DSLib::Matrix<T, TInt> &mat, const TInt n, const bool repeatAsRows) {
		if (repeatAsRows) {
            DSLib::Matrix<T, TInt> ret(mat.rows.count() * n, mat.cols.count(), mat.order());
			for (TInt i=0;i<n;i++)
				ret(i*mat.rows.count(), mat.rows.count(), TInt(0), mat.cols.count()) = mat; //DSA 
			return ret;
		} else {
            DSLib::Matrix<T, TInt> ret(mat.rows.count(), mat.cols.count() * n, mat.order());
			for (TInt i=0;i<n;i++)
				ret(TInt(0), mat.rows.count(), i*mat.cols.count(), mat.cols.count()) = mat; //DSA 
			return ret;
		}
	}

	template <typename T, typename TInt>
    inline void test(DSLib::Matrix<TInt, TInt> &dst, DSLib::Matrix<T, TInt> &lhs, DSLib::Matrix<T, TInt> &rhs, const std::function<bool(const T&, const T&)> &func) {
		dst.clear();
		if (!lhs.isInitialized() || !rhs.isInitialized())
			return;

		if	((rhs.rows.count() > 1 && lhs.rows.count() != rhs.rows.count()) ||
			 (rhs.cols.count() > 1 && lhs.cols.count() != rhs.cols.count()))
			 throw Error(ecIncompatible, "test()", "LHS rows or cols unequal to RHS rows or cols count. Or RHS rows or cols unequal to 1.");
		if (rhs.rows.count() == 1 && rhs.cols.count() == 1) {
			test(dst, lhs, rhs.val(0,0), func); //rhs is actually a scalar in disguise.
		} else if (rhs.isVector()) {
			TInt idx=0;
            typename DSLib::Matrix<T, TInt>::Index * lhsIndex;
            typename DSLib::Matrix<TInt, TInt>::Index * dstIndex;

			if (rhs.isColVector()) {
				lhsIndex = &(lhs.cols);
				dstIndex = &(dst.cols);
			} else {
				lhsIndex = &(lhs.rows);
				dstIndex = &(dst.rows);
			}
            std::vector<TInt> idxs;
			for (auto lhsIndexIt=lhsIndex->begin();lhsIndexIt!=lhsIndex->end();lhsIndexIt++) {
				if (equal(lhsIndexIt->begin(), lhsIndexIt->end(), rhs.vec().begin(), func))
					idxs.push_back(idx);
				idx++;
			}
			dstIndex->add(idxs);
		} else {
			if (lhs.order() == oRowMajor) {
				if (equal(lhs.rows->begin(), lhs.rows->end(), rhs.rows->begin(), func))
					dst.rows.add(0);
			} else {
				if (equal(lhs.cols->begin(), lhs.cols->end(), rhs.cols->begin(), func))
					dst.rows.add(0);
			}
		}
	}

	template <typename T, typename TInt>
    inline void test(DSLib::Matrix<TInt, TInt> &dst, DSLib::Matrix<T, TInt> &lhs, const T &rhs, const std::function<bool(const T&, const T&)> &func) {
		dst.clear();
		if (!lhs.isInitialized())
			return;

        typename DSLib::Matrix<T, TInt>::Index * majorIndex;
        typename DSLib::Matrix<T, TInt>::Index * minorIndex;

		if (lhs.order() == oRowMajor) {
			majorIndex = &(lhs.rows);
			minorIndex = &(lhs.cols);
		} else {
			majorIndex = &(lhs.cols);
			minorIndex = &(lhs.rows);
		}
        std::vector<TInt> majorIndices;
        std::vector<TInt> minorIndices;
        typename DSLib::CellsRegion<T, TInt>::Iterator lhsCellsIt = (*majorIndex)->begin();

		for (TInt majorCnt=0;majorCnt<majorIndex->count();majorCnt++) {
			for (TInt minorCnt=0;minorCnt<minorIndex->count();minorCnt++) {
				if (func(*lhsCellsIt, rhs)) {
					majorIndices.push_back(majorCnt);
					minorIndices.push_back(minorCnt);
				}
				lhsCellsIt++;
			}
		}
		if (lhs.isRowVector()) {
			if (lhs.order() == oRowMajor)
				dst.cols.add(minorIndices);
			else
				dst.cols.add(majorIndices);
		} else if (lhs.isColVector()) {
			if (lhs.order() == oColMajor)
				dst.cols.add(minorIndices);
			else
				dst.cols.add(majorIndices);
		} else {
			if (lhs.order() == oRowMajor) {
				dst.resize((TInt)majorIndices.size(), (TInt)2);
				TInt i =0;
				for (auto it=dst.rows.begin(); it!=dst.rows.end();it++,i++) {
					it->val(0) = majorIndices[i];
					it->val(1) = minorIndices[i];
				}
			} else {
				dst.resize((TInt)majorIndices.size(), (TInt)2);
				TInt i =0;
				for (auto it=dst.rows.begin(); it!=dst.rows.end();it++,i++) {
					it->val(0) = minorIndices[i];
					it->val(1) = majorIndices[i];
				}
			}
		}
	}

	template <typename T, typename TInt>
    inline void apply(DSLib::Matrix<T, TInt> &dst, DSLib::Matrix<T, TInt> &lhs, DSLib::Matrix<T, TInt> &rhs, const std::function<T(T&, const T&)> &func) {
		if (~lhs == 0 || ~rhs == 0)
			throw Error(ecSizeMismatch, "apply", "The lhs or rhs matrix is empty");
		//rhs is a scalar in disguise!
		if (~rhs == 1) {
			apply(dst, lhs, rhs.val(0,0), func);
			return;
		}
		//lhs and rhs are scalars
		if (~lhs == 1 && ~rhs == 1) {
			dst.resize(1,1);
			dst.val(0,0) = func(lhs.val(0,0), rhs.val(0,0));
			return;
		}
		if (!dst.isSameData(lhs))
			dst = lhs;
		if (!lhs.isInitialized() || !rhs.isInitialized())
			return;		
		if	((lhs.isColVector() && rhs.isRowVector()) || 
			 (lhs.isRowVector() && rhs.isColVector()) ||
			 (rhs.rows.count() > 1 && lhs.rows.count() != rhs.rows.count()) ||
			 (rhs.cols.count() > 1 && lhs.cols.count() != rhs.cols.count()))
			 throw Error(ecIncompatible, "apply()", "lhs/rhs Matrix/ColVector/RowVector incompatible.");
		if (rhs.isVector()) {
			if (rhs.isColVector()) {
                for (typename DSLib::Matrix<T, TInt>::Index::Iterator dstIndexIt=dst.cols.begin();dstIndexIt!=dst.cols.end();dstIndexIt++) {
                    typename DSLib::CellsContinuous<T, TInt>::Iterator dstIt = dstIndexIt->begin();
					//lambda magic and for_each					
                    std::function<void(const T&)> f2 = [&dstIt, &func] (const T &a) {*dstIt = func(*dstIt, a); dstIt++;};
					for_each(rhs.vec().begin(), rhs.vec().end(), f2);
				}
			} else {
                for (typename DSLib::Matrix<T, TInt>::Index::Iterator dstIndexIt=dst.rows.begin();dstIndexIt!=dst.rows.end();dstIndexIt++) {
                    typename DSLib::CellsContinuous<T, TInt>::Iterator dstIt = dstIndexIt->begin();
					//lambda magic and for_each					
                    std::function<void(const T&)> f2 = [&dstIt, &func] (const T &a) {*dstIt = func(*dstIt, a); dstIt++;};
					for_each(rhs.vec().begin(), rhs.vec().end(), f2);
				}
			}
		} else {
			if (dst.order() == oRowMajor) {
                typename DSLib::CellsRegion<T, TInt>::Iterator dstIt = dst.rows->begin();
                std::function<void(const T&)> f2 = [&dstIt, &func] (const T &a){*dstIt = func(*dstIt, a); dstIt++;};
				for_each(rhs.rows->begin(), rhs.rows->end(), f2);
			} else {
                typename DSLib::CellsRegion<T, TInt>::Iterator dstIt = rhs.cols->begin();
                std::function<void(const T&)> f2 = [&dstIt, &func] (const T &a){*dstIt = func(*dstIt, a); dstIt++;};
				for_each(rhs.cols->begin(), rhs.cols->end(), f2);
			}
		}
	}

	template <typename T, typename TInt>
    inline void apply(DSLib::Matrix<T, TInt> &dst, DSLib::Matrix<T, TInt> &lhs, const T &rhs, const std::function<T(T&, const T&)> &func) {
		if (~lhs == 0)
			throw Error(ecSizeMismatch, "apply", "The lhs Matrix is empty");
		if (!dst.isSameData(lhs))
			dst = lhs;
		if (!lhs.isInitialized())
			return;
		if (lhs.order() == oRowMajor) {
			for (auto dstIt = dst.rows->begin(); dstIt != dst.rows->end(); dstIt++)
				*dstIt = func(*dstIt, rhs);
		} else {
			for (auto dstIt = dst.cols->begin(); dstIt!= dst.cols->end(); dstIt++)
				*dstIt = func(*dstIt, rhs);
		}
	}

	template <typename T, typename TInt> 
    inline void range(DSLib::Matrix<T, TInt> &dst, DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
		dst = lhs;
		T last = *(lhs.vec()[lhs.vec().count()-1]);
		if (last == rhs)
			return;
        std::vector<T> vec;
		if (last < rhs) {
			for (T i=last+1;i<=rhs;i++)
				vec.push_back(i);
		} else {
			for (T i=last-1;i>=rhs;i--)
				vec.push_back(i);
		}
		if (dst.isRowVector()) {
            DSLib::Matrix<T, TInt> m(lhs.order());
			m.rows.add(vec);
			dst.cols.add(m);
		} else if (dst.isColVector()) {
            DSLib::Matrix<T, TInt> m(lhs.order());
			m.cols.add(vec);
			dst.rows.add(m);
		}
	}

	template <typename T, typename TInt> 
    inline void setIntersection(DSLib::Matrix<T, TInt> &dst, DSLib::Matrix<T, TInt> &lhs, DSLib::Matrix<T, TInt> &rhs) {
		if (!rhs.isInitialized() || !lhs.isInitialized()) {
			dst.clear();
			return;
		} else if ((lhs.isColVector() && rhs.isColVector()) || (lhs.isRowVector() && rhs.isRowVector())) {
			dst.clear();
            std::vector<T> lhsVec, rhsVec, dstVec;
			lhsVec.resize(lhs.vec().count());
			rhsVec.resize(rhs.vec().count());
			copy(lhs.vec().begin(), lhs.vec().end(), lhsVec.begin());
			copy(rhs.vec().begin(), rhs.vec().end(), rhsVec.begin());
			dstVec.resize(lhsVec.size() + rhsVec.size());
			sort(lhsVec.begin(), lhsVec.end());
			sort(rhsVec.begin(), rhsVec.end());
			auto last = set_intersection(lhsVec.begin(), lhsVec.end(), rhsVec.begin(), rhsVec.end(), dstVec.begin());
			dstVec.resize(distance(dstVec.begin(), last));
			if (lhs.isColVector())
				dst.cols.add(dstVec);	
			else
				dst.rows.add(dstVec);	
		} else
			throw Error(ecIncompatible, "setIntersection", "incompatible vectors.");
	}


	template <typename T, typename TInt>
    inline void setDifference(DSLib::Matrix<T, TInt> &dst, DSLib::Matrix<T, TInt> &lhs, DSLib::Matrix<T, TInt> &rhs) {
		dst.clear();
		if (lhs.isEmpty()) {
			dst = rhs;
			return;
		}
		if (rhs.isEmpty()) {
			dst = lhs;
			return;
		}
		if (!lhs.isVector() || !rhs.isVector())
			throw Error(ecIncompatible, "setDifference", "incompatible vectors.");
        std::vector<T> lhsVec, rhsVec, dstVec;
		lhsVec.resize(lhs.vec().count());
		rhsVec.resize(rhs.vec().count());
		copy(lhs.vec().begin(), lhs.vec().end(), lhsVec.begin());
		copy(rhs.vec().begin(), rhs.vec().end(), rhsVec.begin());
		sort(lhsVec.begin(), lhsVec.end());
		sort(rhsVec.begin(), rhsVec.end());
		if (lhsVec.size() > rhsVec.size())
			set_difference(lhsVec.begin(), lhsVec.end(), rhsVec.begin(), rhsVec.end(), std::inserter(dstVec, dstVec.begin()));
		else
			set_difference(rhsVec.begin(), rhsVec.end(), lhsVec.begin(), lhsVec.end(), std::inserter(dstVec, dstVec.begin()));
		if (lhs.isColVector())
			dst.rows.add(dstVec);
		else
			dst.cols.add(dstVec);
	}

	template <typename T, typename TInt> 
    inline void setUnion(DSLib::Matrix<T, TInt> &dst, DSLib::Matrix<T, TInt> &lhs, DSLib::Matrix<T, TInt> &rhs) {
//		if (~lhs == 1 && ~rhs == 1)
//			throw Error(ecIncompatible, "setUnion", "Ambiguous vectors. Both lhs and rhs are scalars."); //Default behaviour of || is to produce a rowvector
		if (!rhs.isInitialized()) {
			dst = lhs;;
		} else if (!lhs.isInitialized()) {
			dst = rhs;
		} else if (lhs.isRowVector() && rhs.isRowVector()) {
			dst.resize(1, lhs.vec().count() + rhs.vec().count());
            std::vector<T> lhsVec(lhs.vec().count());
            std::vector<T> rhsVec(rhs.vec().count());
			copy(lhs.vec().begin(), lhs.vec().end(), lhsVec.begin());
			copy(rhs.vec().begin(), rhs.vec().end(), rhsVec.begin());			
			sort(lhsVec.begin(), lhsVec.end());
			sort(rhsVec.begin(), rhsVec.end());
			auto last = set_union(lhsVec.begin(), lhsVec.end(), rhsVec.begin(), rhsVec.end(), dst.vec().begin());
			dst.resize(1, (TInt)distance(dst.vec().begin(), last));
		} else if (lhs.isColVector() && rhs.isColVector()) {
			dst.resize(lhs.vec().count() + rhs.vec().count(), 1);
            std::vector<T> lhsVec(lhs.vec().count());
            std::vector<T> rhsVec(rhs.vec().count());
			copy(lhs.vec().begin(), lhs.vec().end(), lhsVec.begin());
			copy(rhs.vec().begin(), rhs.vec().end(), rhsVec.begin());			
			sort(lhsVec.begin(), lhsVec.end());
			sort(rhsVec.begin(), rhsVec.end());
			auto last = set_union(lhsVec.begin(), lhsVec.end(), rhsVec.begin(), rhsVec.end(), dst.vec().begin());
			dst.resize((TInt)distance(dst.vec().begin(), last), 1);
		} else
			throw Error(ecIncompatible, "setUnion", "incompatible vectors.");
	}

	template<typename T, typename TInt>
    inline DSLib::Matrix<T, TInt> randomInt(TInt rows, TInt cols, T min, T max, const Int32 seed=-1) {
        DSLib::Matrix<T, TInt> m(rows, cols);
		std::mt19937 gen;
		if (seed >= 0) {
			gen.seed(seed);
		} else {
			std::random_device rd;
			gen.seed(rd());
		}
			
		std::uniform_int_distribution<> dis(min, max);
		for (auto it=m.rows->begin();it!=m.rows->end();it++)
			*it = T(dis(gen));
		return m;
	}

	template<typename T, typename TInt>
    inline DSLib::Matrix<T, TInt> randomReal(TInt rows, TInt cols, T min, T max, const Int32 seed=-1) {
        DSLib::Matrix<T, TInt> m(rows, cols);
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(min, max);
		for (auto it=m.rows->begin();it!=m.rows->end();it++)
			*it = T(dis(gen));
		return m;
	}

	template<typename T, typename TInt>
    inline DSLib::Matrix<T, TInt> random(TInt rows, TInt cols, const DSLib::Matrix<T> & vec, const Int32 seed=-1) {
        DSLib::Matrix<T, TInt> m(rows, cols);
		std::random_device rd;
		std::mt19937 gen;
		std::uniform_int_distribution<> dis(0, vec.vec().count() - 1);
		for (auto it = m.rows->begin();it != m.rows->end();it++)
			*it = vec.vec(TInt(dis(gen)));
		return m;
	}

	template<typename T, typename TInt>
    inline DSLib::Matrix<T, TInt> & shuffle(DSLib::Matrix<T, TInt> &mat, Double amount) {
		std::random_device rd;
		std::mt19937 gen(rd());		
		std::uniform_int_distribution<> r(0, mat.rows.count()-1);
		std::uniform_int_distribution<> c(0, mat.cols.count()-1);
		for (TInt i = 0; i < (Double)(mat.rows.count() * mat.cols.count()) * amount;i++) {
			TInt ra = (TInt)r(gen);
			TInt ca = (TInt)c(gen);
			TInt rb = (TInt)r(gen);
			TInt cb = (TInt)c(gen);
			T temp = mat.val(ra, ca);
			mat.val(ra, ca) = mat.val(rb, cb);
			mat.val(rb, cb) = temp;
		}
		return mat;
	}

	template <typename ToT, typename FromT, typename TInt>
    inline DSLib::Matrix<ToT, TInt> & convert(DSLib::Matrix<ToT, TInt> &dst, const DSLib::Matrix<FromT, TInt> &src) {
		if ((void *)&src == (void *)&dst)
			throw Error(ecIncompatible, "convert", "src and dst should be different");
        DSLib::Matrix<FromT, TInt> & src2 = const_cast<DSLib::Matrix<FromT, TInt>&>(src);
		dst.resize(src.rows.count(), src.cols.count());
		if (src.isRegion()) {
            DSLib::CellsRegion<FromT, TInt> srcCells;
            DSLib::CellsRegion<ToT, TInt> dstCells;
			if (src.rows.isAligned()) {
                srcCells = static_cast<DSLib::CellsRegion<FromT, TInt>&>(src2.rows.cells(citRegion));
                dstCells = static_cast<DSLib::CellsRegion<ToT, TInt>&>(dst.rows.cells(citRegion));
			} else if (src.cols.isAligned()) {
                srcCells = static_cast<DSLib::CellsRegion<FromT, TInt>&>(src2.cols.cells(citRegion));
                dstCells = static_cast<DSLib::CellsRegion<ToT, TInt>&>(dst.cols.cells(citRegion));
			};
			auto dstCell = dstCells.begin();
			for (auto srcCell = srcCells.begin(); srcCell != srcCells.end(); srcCell++, dstCell++)
				convertValue(*srcCell, *dstCell);
		} else {
            DSLib::CellsContinuous<FromT, TInt> srcCells;
            DSLib::CellsRegion<ToT, TInt> dstCells;
			if (src.rows.isAligned()) {
                srcCells = static_cast<DSLib::CellsContinuous<FromT, TInt>&>(src2.rows.cells(citContinuous));
                dstCells = static_cast<DSLib::CellsRegion<ToT, TInt>&>(dst.rows.cells(citRegion));
			} else if (src.cols.isAligned()) {
                srcCells = static_cast<DSLib::CellsContinuous<FromT, TInt>&>(src2.cols.cells(citContinuous));
                dstCells = static_cast<DSLib::CellsRegion<ToT, TInt>&>(dst.cols.cells(citRegion));
			};
			auto dstCell = dstCells.begin();
			for (auto srcCell = srcCells.begin(); srcCell != srcCells.end(); srcCell++, dstCell++)
				convertValue(*srcCell, *dstCell);
		}
		return dst;
	}

	template <typename T, typename TInt>
    inline DSLib::Matrix<T, TInt> & convert(DSLib::Matrix<T, TInt> &dst, const DSLib::Matrix<T, TInt> &src) {
		if ((void *)&src == (void *)&dst)
			throw Error(ecIncompatible, "convert", "src and dst should be different");
        DSLib::Matrix<T, TInt> & src2 = const_cast<DSLib::Matrix<T, TInt>&>(src);
		if (src2.order() == dst.order()) {
			dst = src2; //Ref
			return dst;
		}
		dst.resize(src.rows.count(), src.cols.count());
		if (src.isRegion()) {
            DSLib::CellsRegion<T, TInt> srcCells;
            DSLib::CellsRegion<T, TInt> dstCells;
			if (src.rows.isAligned()) {
                srcCells = static_cast<DSLib::CellsRegion<T, TInt>&>(src2.rows.cells(citRegion));
                dstCells = static_cast<DSLib::CellsRegion<T, TInt>&>(dst.rows.cells(citRegion));
			}
			else if (src.cols.isAligned()) {
                srcCells = static_cast<DSLib::CellsRegion<T, TInt>&>(src2.cols.cells(citRegion));
                dstCells = static_cast<DSLib::CellsRegion<T, TInt>&>(dst.cols.cells(citRegion));
			};
			auto dstCell = dstCells.begin();
			for (auto srcCell = srcCells.begin(); srcCell != srcCells.end(); srcCell++, dstCell++)
				*dstCell = *srcCell;
		}else {
            DSLib::CellsContinuous<T, TInt> srcCells;
            DSLib::CellsRegion<T, TInt> dstCells;
			if (src.rows.isAligned()) {
                srcCells = static_cast<DSLib::CellsContinuous<T, TInt>&>(src2.rows.cells(citContinuous));
                dstCells = static_cast<DSLib::CellsRegion<T, TInt>&>(dst.rows.cells(citRegion));
			}
			else if (src.cols.isAligned()) {
                srcCells = static_cast<DSLib::CellsContinuous<T, TInt>&>(src2.cols.cells(citContinuous));
                dstCells = static_cast<DSLib::CellsRegion<T, TInt>&>(dst.cols.cells(citRegion));
			};
			auto dstCell = dstCells.begin();
			for (auto srcCell = srcCells.begin(); srcCell != srcCells.end(); srcCell++, dstCell++)
				*dstCell = *srcCell;
		}
		return dst;
	}
	

	template <typename T, typename TInt>
    inline void transpose(DSLib::Matrix<T, TInt> &dst, DSLib::Matrix<T, TInt> &src) {
		if (!src.isInitialized())
			return;

		dst.setOrder(src.order());
		if (src.isInitialized()) {
			dst.resize(src.cols.count(), src.rows.count());
			if (src.isRegion()) {								
				if (src.rows.isAligned()) {
					std::copy(src.rows->begin(), src.rows->end(), dst.cols->begin());
				} else if (src.cols.isAligned()) {
					std::copy(src.cols->begin(), src.cols->end(), dst.rows->begin());
				};
			} else {
                DSLib::CellsContinuous<T, TInt> srcCells;
				if (src.rows.isAligned()) {
                    srcCells = static_cast<DSLib::CellsContinuous<T, TInt>&>(src.rows.cells(citContinuous)); //can only get this is isAligned() and !isRegion()
					std::copy(srcCells.begin(), srcCells.end(), dst.cols->begin());
				} else if (src.cols.isAligned()) {
                    srcCells = static_cast<DSLib::CellsContinuous<T, TInt>&>(src.cols.cells(citContinuous));
					std::copy(srcCells.begin(), srcCells.end(), dst.rows->begin());
				};
			}
		}
	}

	template <typename T, typename TInt>
    inline DSLib::Matrix<T, TInt> t(const DSLib::Matrix<T, TInt> &src) {
        DSLib::Matrix<T, TInt> ret;
        DSLib::Matrix<T, TInt> src2 = const_cast<DSLib::Matrix<T, TInt>&>(src);
		transpose(ret, src2);
		return ret;
	}

	template <typename T, typename TInt>
    inline void multiplyBase(DSLib::Matrix<T, TInt>& dst, DSLib::Matrix<T, TInt>& src1, DSLib::Matrix<T, TInt>& src2) {
		if (!src1.isInitialized() || !src2.isInitialized())
			return;

		if(src1.cols.count() != src2.rows.count())
			throw Error(ecSizeMismatch, "multiply", "src1.cols != src2.rows");

		src1.breakUpDown(src2);

		dst.clear();
		dst.resize(src1.rows.count(), src2.cols.count());

		auto src1RowIt =src1.rows.begin();

		auto dstEnd = dst.rows.end();
		for (auto dstRowIt=dst.rows.begin(); dstRowIt != dstEnd; dstRowIt++, src1RowIt++) {
			auto src2ColIt =src2.cols.begin();
			auto dstRowEnd = dstRowIt->end();
			for (auto dstCellIt=dstRowIt->begin(); dstCellIt != dstRowEnd; dstCellIt++, src2ColIt++) {
				auto src2CellIt = src2ColIt->begin();
				auto srcRowEnd = src1RowIt->end();
				T sum = (T)0;
				for (auto src1CellIt = src1RowIt->begin(); src1CellIt!=srcRowEnd; src1CellIt++, src2CellIt++) {
					sum += *src1CellIt * *src2CellIt;
				}
				*dstCellIt = (T)sum;
			}
		}
	}

	template <typename T, typename TInt>
    inline void multiplyFast(DSLib::Matrix<T, TInt>& dst, DSLib::Matrix<T, TInt>& src1, DSLib::Matrix<T, TInt>& src2) {
		if (!src1.isInitialized() || !src2.isInitialized())
			return;

		if(src1.cols.count() != src2.rows.count())
			throw Error(ecSizeMismatch, "multiplyFast", "src1.cols != src2.rows");

		TInt res_height = src1.rows.count();
		TInt res_width = src2.cols.count();
		TInt i_max = src1.cols.count();
		dst.resize(res_height, res_width);

		//auto src1RowIt =src1.rows.
		T* src1data = src1.getData();
		T* src2data = src2.getData();
		T* dstdata = dst.getData();

		TInt dststride = dst.getDataStride();
		TInt src1stride = src1.getDataStride();
		TInt src2stride = src2.getDataStride();

		if(dst.order() == oRowMajor) {
			for(TInt y_res = 0; y_res < res_height; y_res++) {
				TInt data_offset = y_res * dststride;
				TInt src1_offset = y_res * src1stride;
				for(TInt x_res = 0; x_res < res_width; x_res++) {
					TInt src2_offset = x_res * src2stride;
					// iterate over the src1 and src2 cells
					T sum = (T)0;
					for(TInt i = 0; i < i_max; i++) {
						sum += src1data[src1_offset + i] * src2data[src2_offset + i];
					}
					dstdata[data_offset + x_res] = sum;
				}
			}
		} else { // when dst.order() == oColMajor:
			for(TInt y_res = 0; y_res < res_height; y_res++) {
				TInt src1_offset = y_res * src1stride;
				for(TInt x_res = 0; x_res < res_width; x_res++) {
					TInt data_offset = x_res * dststride;
					TInt src2_offset = x_res * src2stride;
					// iterate over the src1 and src2 cells
					T sum = (T)0;
					for(TInt i = 0; i < i_max; i++) {
						sum += src1data[src1_offset + i] * src2data[src2_offset + i];
					}
					dstdata[data_offset + y_res] = sum;
				}
			}
		}
	}

	template <typename T, typename TInt>
    inline void multiply(DSLib::Matrix<T, TInt>& dst, DSLib::Matrix<T, TInt>& src1, DSLib::Matrix<T, TInt>& src2) {
		if (src1.order() == oRowMajor && src2.order() == oColMajor) {
			multiplyFast(dst, src1, src2);
		} else {
			multiplyBase(dst, src1, src2);
		}
	}

	template <typename T, typename TInt>
    inline void inverse(DSLib::Matrix<T, TInt>& dst, DSLib::Matrix<T, TInt>& src) {
		if (!src.isInitialized())
			return;

		if(src.cols.count() != 3 || src.rows.count() != 3)
			throw Error(ecNotImplemented, "inverse", "src.cols != 3 || src.rows != 3");

		dst.breakUpDown(src);

		auto row = src.rows.begin();
		auto cell = row->begin();
		T a11 = *cell++;
		T a12 = *cell++;
		T a13 = *cell;
		row++;
		cell = row->begin();
		T a21 = *cell++;
		T a22 = *cell++;
		T a23 = *cell;
		row++;
		cell = row->begin();
		T a31 = *cell++;
		T a32 = *cell++;
		T a33 = *cell;
		DataType dt = dataType(a11);
        DSLib::Matrix<T> a22a23a32a33 = 	(dt | a22	| a23 ) ^
									(dt | a32	| a33 );

        DSLib::Matrix<T> a21a23a31a33 = 	(dt | a21	| a23 ) ^
									(dt | a31	| a33 );

        DSLib::Matrix<T> a21a22a31a32 = 	(dt | a21	| a22 ) ^
									(dt | a31	| a32 );

		T detSrc = 0;
		T det_a22a23a32a33;

		determinant(det_a22a23a32a33, a22a23a32a33);
		detSrc += a11 * det_a22a23a32a33;

		T det_a21a23a31a33;
		determinant(det_a21a23a31a33, a21a23a31a33);
		detSrc -= a12 * det_a21a23a31a33;

		T det_a21a22a31a32;
		determinant(det_a21a22a31a32, a21a22a31a32);
		detSrc += a13 * det_a21a22a31a32;

		if(detSrc == (T)0)
			throw Error(ecUnexpected, "inverse", "det(src) == 0");

        DSLib::Matrix<T> a13a12a33a32 = 	(dt | a13	| a12 ) ^
									(dt | a33	| a32 );

		T det_a13a12a33a32;
		determinant(det_a13a12a33a32, a13a12a33a32);

        DSLib::Matrix<T> a12a13a22a23 = 	(dt | a12	| a13 ) ^
									(dt | a22	| a23 );

		T det_a12a13a22a23;
		determinant(det_a12a13a22a23, a12a13a22a23);

        DSLib::Matrix<T> a23a21a33a31 = 	(dt | a23	| a21 ) ^
									(dt | a33	| a31 );

		T det_a23a21a33a31;
		determinant(det_a23a21a33a31, a23a21a33a31);

        DSLib::Matrix<T> a11a13a31a33 = 	(dt | a11	| a13 ) ^
										(dt | a31	| a33 );

		T det_a11a13a31a33;
		determinant(det_a11a13a31a33, a11a13a31a33);

        DSLib::Matrix<T> a13a11a23a21 = 	(dt | a13	| a11 ) ^
										(dt | a23	| a21 );

		T det_a13a11a23a21;
		determinant(det_a13a11a23a21, a13a11a23a21);

        DSLib::Matrix<T> a12a11a32a31 = 	(dt | a12	| a11 ) ^
										(dt | a32	| a31 );

		T det_a12a11a32a31;
		determinant(det_a12a11a32a31, a12a11a32a31);

        DSLib::Matrix<T> a11a12a21a22 = 	(dt | a11	| a12 ) ^
										(dt | a21	| a22 );

		T det_a11a12a21a22;
		determinant(det_a11a12a21a22, a11a12a21a22);

		dst  = 	(dt | det_a22a23a32a33 / detSrc | det_a13a12a33a32 / detSrc | det_a12a13a22a23 / detSrc ) ^
				(dt | det_a23a21a33a31 / detSrc | det_a11a13a31a33 / detSrc | det_a13a11a23a21 / detSrc ) ^
				(dt | det_a21a22a31a32 / detSrc | det_a12a11a32a31 / detSrc | det_a11a12a21a22 / detSrc );
	}

	template <typename T, typename TInt>
    inline void determinant(T& dst, DSLib::Matrix<T, TInt>& src) {
		if (!src.isInitialized())
			return;

		if(src.cols.count() != 2 || src.rows.count() != 2)
			throw Error(ecNotImplemented, "determinant", "src.cols != 2 || src.rows != 2");

		auto row = src.rows.begin();
		auto cell = row->begin();
		T a11 = *cell++;
		T a12 = *cell;
		row++;
		cell = row->begin();
		T a21 = *cell++;
		T a22 = *cell;

		dst = a11 * a22 - a12 * a21;
	}

	template <typename T, typename TInt>
    inline void colMean(DSLib::Matrix<T, TInt> &dst, const DSLib::Matrix<T, TInt> &src) {
        DSLib::Matrix<T, TInt> & src2 = const_cast<DSLib::Matrix<T, TInt> &> (src);
		dst.resize(1, src.cols.count());
		TInt rc = src.rows.count();		
		TInt colIdx = 0;
		for (auto col = src2.cols.begin(); col != src2.cols.end(); col++, colIdx++) {
			T sum = (T)0;
			for  (auto cell = col->begin(); cell != col->end(); cell++) {
				sum += *cell;
			}
			dst.val(0, colIdx) = sum / rc;
		}
	}

	template <typename T, typename TInt>
    inline void colSD(DSLib::Matrix<T, TInt> &dst, const DSLib::Matrix<T, TInt> &src, const DSLib::Matrix<T, TInt> &mean) {
        DSLib::Matrix<T, TInt> mean2 = mean;
		if (!mean2.isInitialized())
			colMean(mean2, src);
		if (mean2.cols.count() != src.cols.count())
			throw Error(ecRangeError, "colSD()", "Wrong col count for 'mean' matrix");
        DSLib::Matrix<T, TInt> & src2 = const_cast<DSLib::Matrix<T, TInt> &> (src);
		dst.resize(1, src.cols.count());
		TInt rc = src.rows.count();
		TInt colIdx = 0;
		for (auto col = src2.cols.begin(); col != src2.cols.end(); col++, colIdx++) {
			T sum = (T)0;
			for (auto cell = col->begin(); cell != col->end(); cell++) {
				sum += (*cell - mean2.val(0, colIdx)) * (*cell - mean2.val(0, colIdx));
			}
			dst.val(0, colIdx) = sqrt(sum / rc);
		}
	}

	template <typename T, typename TInt, template <typename, typename> class TMat>
    inline void colMean(DSLib::Matrix<TMat<T, TInt>, TInt> &dst, const DSLib::Matrix<TMat<T, TInt>, TInt> &src) {
		//Note: DDSL side effects on + and /
        DSLib::Matrix<TMat<T, TInt>, TInt> & src2 = const_cast<DSLib::Matrix<TMat<T, TInt>, TInt> &> (src);
		dst.resize(1, src.cols.count());
		TInt rc = src.rows.count();		
		TInt colIdx = 0;
		for (auto col = src2.cols.begin(); col != src2.cols.end(); col++, colIdx++) {
			TMat<T, TInt> sum = *(col->begin()); //force copy of existing TMat inherited object
			sum.breakUp();
            static_cast<DSLib::Matrix<T, TInt>>(sum) = (T)0; //set internal matrix elements to zero
			for  (auto cell = col->begin(); cell != col->end(); cell++) {
				sum + *cell;
			}
			sum / (T)rc;
			dst.val(0, colIdx) = sum;
		}		
	}

	template <typename T, typename TInt, template <typename, typename> class TMat>
    inline void colSD(DSLib::Matrix<TMat<T, TInt>, TInt> &dst, const DSLib::Matrix<TMat<T, TInt>, TInt> &src, const DSLib::Matrix<TMat<T, TInt>, TInt> &mean) {
		//Note: DDSL side effects on + and /
        DSLib::Matrix<TMat<T, TInt>, TInt> mean2 = mean;
		if (!mean2.isInitialized())
			colMean(mean2, src);
		if (mean2.cols.count() != src.cols.count())
			throw Error(ecRangeError, "colSD()", "Wrong col count for 'mean' matrix");
        DSLib::Matrix<TMat<T, TInt>, TInt> & src2 = const_cast<DSLib::Matrix<TMat<T, TInt>, TInt> &> (src);
		dst.resize(1, src.cols.count());
		TInt rc = src.rows.count();		
		TInt colIdx = 0;
		for (auto col = src2.cols.begin(); col != src2.cols.end(); col++, colIdx++) {
			TMat<T, TInt> sum = *(col->begin()); //force copy of existing TMat inherited object
			sum.breakUp();
            static_cast<DSLib::Matrix<T, TInt>&>(sum) = (T)0; //set internal matrix elements to zero
			for (auto cell = col->begin(); cell != col->end(); cell++) {
				TMat<T, TInt> c = *cell;
				c.breakUp();
				c - mean2.val(0, colIdx);
				pow(c, (T)2.0);
				sum + c;
			}
			sum / (T)rc;
			pow(sum, (T)0.5);
			dst.val(0, colIdx) = sum;
		}		
	}

	template<typename T, typename TInt>
    inline void colVar(DSLib::Matrix<T, TInt>& dst, const DSLib::Matrix<T, TInt>& src, const DSLib::Matrix<T, TInt> &mean) {
        DSLib::Matrix<T, TInt> mean2 = mean;
		if (!mean2.isInitialized())
			colMean(mean2, src);
		if (mean2.cols.count() != src.cols.count())
			throw Error(ecRangeError, "colSD()", "Wrong col count for 'mean' matrix");
        DSLib::Matrix<T, TInt> & src2 = const_cast<DSLib::Matrix<T, TInt> &> (src);
		dst.resize(1, src.cols.count());
		TInt rc = src.rows.count();
		TInt colIdx = 0;
		for (auto col = src2.cols.begin(); col != src2.cols.end(); col++, colIdx++) {
			T sum = (T)0;
			for (auto cell = col->begin(); cell != col->end(); cell++) {
				sum += (*cell - mean2.val(0, colIdx)) * (*cell - mean2.val(0, colIdx));
			}
			dst.val(0, colIdx) = sum / rc;
		}
	}

	template <typename T, typename TInt, template <typename, typename> class TMat>
    inline void colVar(DSLib::Matrix<TMat<T, TInt>, TInt> &dst, const DSLib::Matrix<TMat<T, TInt>, TInt> &src, const DSLib::Matrix<TMat<T, TInt>, TInt> &mean) {
        DSLib::Matrix<TMat<T, TInt>, TInt> mean2 = mean;
		if (!mean2.isInitialized())
			colMean(mean2, src);
		if (mean2.cols.count() != src.cols.count())
			throw Error(ecRangeError, "colSD()", "Wrong col count for 'mean' matrix");
        DSLib::Matrix<TMat<T, TInt>, TInt> & src2 = const_cast<DSLib::Matrix<TMat<T, TInt>, TInt> &> (src);
		dst.resize(1, src.cols.count());
		TInt rc = src.rows.count();		
		TInt colIdx = 0;
		for (auto col = src2.cols.begin(); col != src2.cols.end(); col++, colIdx++) {
			TMat<T, TInt> sum = *(col->begin()); //force copy of existing TMat inherited object
			sum.breakUp();
            static_cast<DSLib::Matrix<T, TInt>&>(sum) = (T)0; //set internal matrix elements to zero
			for (auto cell = col->begin(); cell != col->end(); cell++) {
				TMat<T, TInt> c = *cell;
				c.breakUp();
				c - mean2.val(0, colIdx);
				pow(c, (T)2.0);
				sum + c;
			}
			sum / (T)rc;
			dst.val(0, colIdx) = sum;
		}		
	}

	template <typename T, typename TInt, template <typename, typename> class TMat>
    inline void colMin(DSLib::Matrix<TMat<T, TInt>, TInt> &dst, const DSLib::Matrix<TMat<T, TInt>, TInt> &src) {
        DSLib::Matrix<TMat<T, TInt>, TInt> & src2 = const_cast<DSLib::Matrix<TMat<T, TInt>, TInt> &> (src);
		dst.resize(1, src.cols.count());
		TInt colIdx = 0;
		for (auto col = src2.cols.begin(); col != src2.cols.end(); col++, colIdx++) {
			TMat<T, TInt> min = col->begin()->dup();
			for (auto cell = col->begin(); cell != col->end(); cell++) {
				DSLang::min(min, *cell);
			}
			dst.val(0, colIdx) = min;
		}	
	}

	template <typename T, typename TInt, template <typename, typename> class TMat>
    inline void colMax(DSLib::Matrix<TMat<T, TInt>, TInt> &dst, const DSLib::Matrix<TMat<T, TInt>, TInt> &src) {
        DSLib::Matrix<TMat<T, TInt>, TInt> & src2 = const_cast<DSLib::Matrix<TMat<T, TInt>, TInt> &> (src);
		dst.resize(1, src.cols.count());
		TInt colIdx = 0;
		for (auto col = src2.cols.begin(); col != src2.cols.end(); col++, colIdx++) {
			TMat<T, TInt> max = col->begin()->dup();
			for (auto cell = col->begin(); cell != col->end(); cell++) {
				DSLang::max(max, *cell);
			}
			dst.val(0, colIdx) = max;
		}		
	}

	template <typename T, typename TInt>
    inline void colMin(DSLib::Matrix<T, TInt> &dst, const DSLib::Matrix<T, TInt> &src) {
        DSLib::Matrix<T, TInt> & src2 = const_cast<DSLib::Matrix<T, TInt> &> (src);
		dst.resize(1, src.cols.count());
		TInt colIdx = 0;
		for (auto col = src2.cols.begin(); col != src2.cols.end(); col++, colIdx++) {
			T min = (T)*(col->begin());
			for (auto cell = col->begin(); cell != col->end(); cell++) {
				if (*cell < min)
					min = *cell;
			}
			dst.val(0, colIdx) = min;
		}
	}

	template <typename T, typename TInt>
    inline void colMax(DSLib::Matrix<T, TInt> &dst, const DSLib::Matrix<T, TInt> &src) {
        DSLib::Matrix<T, TInt> & src2 = const_cast<DSLib::Matrix<T, TInt> &> (src);
		dst.resize(1, src.cols.count());
		TInt colIdx = 0;
		for (auto col = src2.cols.begin(); col != src2.cols.end(); col++, colIdx++) {
			T mx = (T)*(col->begin());
			for (auto cell = col->begin(); cell != col->end(); cell++) {
				if (*cell > mx)
					mx = *cell;
			}
			dst.val(0, colIdx) = mx;
		}
	}
	
	template<typename T, typename TInt>
    inline DSLib::Matrix<T, TInt> & eye(DSLib::Matrix<T, TInt> &dst, const TInt n) {
		dst.resize(n, n);
		dst = (T)0;
		for (TInt i=0;i<n;i++)
			dst.val(i, i) = (T)1;
		return dst;
	}

	template<typename T, typename TInt>
    inline DSLib::Matrix<T, TInt> & diag(DSLib::Matrix<T, TInt> &dst, const DSLib::Matrix<T, TInt>& v) {
		if (!v.isVector())
			throw Error(ecParameter, "diag", "v should be a vector");		
		dst.resize(v.vec().count(), v.vec().count());
		dst = (T)0;
		for (TInt i = 0;i<v.vec().count();i++)
			dst.val(i, i) = v.vec(i);
		return dst;
	}

	template <typename T, typename TInt>
    T sum(const DSLib::Matrix<T, TInt> &src) {
        DSLib::Matrix<T, TInt> & src2 = const_cast<DSLib::Matrix<T, TInt>&>(src);
		T s=0;
		for (auto it=src2.rows->begin();it!=src2.rows->end();it++)
			s += *it;
		return s;
	}

	template <typename T, typename TInt>
    void round(DSLib::Matrix<T, TInt> &dst, TInt digits) {
		T d = (digits==0)?1:digits*10;		
		for (auto it=dst.rows->begin();it!=dst.rows->end();it++)			
			*it = std::round(*it*d)/d;
	}

	template <typename T, typename TInt>
    DSLib::Matrix<T, TInt> uniqueValues(const DSLib::Matrix<T, TInt> &mat, const T ignoreValue) {
        DSLib::Matrix<T, TInt> & mat2 = const_cast<DSLib::Matrix<T, TInt>&>(mat);
        DSLib::Matrix<T, TInt> u;
		for (auto it = mat2.rows->begin(); it != mat2.rows->end(); it++)			
			if (*it != ignoreValue && ~(u == *it) == 0)
				u | *it;
		return u;
	}

	template <typename T, typename TInt>
    DSLib::Matrix<T, TInt> uniqueValues(const DSLib::Matrix<T, TInt> &mat) {
        DSLib::Matrix<T, TInt> & mat2 = const_cast<DSLib::Matrix<T, TInt>&>(mat);
        DSLib::Matrix<T, TInt> u;
		for (auto it = mat2.rows->begin(); it != mat2.rows->end(); it++)			
			if (~(u == *it) == 0)
				u | *it;
		return u;
	}

	template <typename T, typename TInt>
    DSLib::Matrix<TInt, TInt> indexLUT(const DSLib::Matrix<T, TInt> &mat, const DSLib::Matrix<T, TInt> &lut) {
        DSLib::Matrix<T, TInt> & mat2 = const_cast<DSLib::Matrix<T, TInt>&>(mat);
        DSLib::Matrix<TInt, TInt> r(mat.rows.count(), mat.cols.count());
		auto it_dst = r.rows->begin();
		for (auto it = mat2.rows->begin(); it != mat2.rows->end(); it++, it_dst++)
			*it_dst = (TInt)(lut == *it);
		return r;
	}

	template <typename T, typename TInt>
    DSLib::Matrix<T, TInt> valueLUT(const DSLib::Matrix<TInt, TInt> &mat, const DSLib::Matrix<T, TInt> &lut) {
        DSLib::Matrix<T, TInt> & mat2 = const_cast<DSLib::Matrix<T, TInt>&>(mat);
        DSLib::Matrix<T, TInt> r(mat.rows.count(), mat.cols.count());
		auto it_dst = r.rows->begin();
		for (auto it = mat2.rows->begin(); it != mat2.rows->end(); it++, it_dst++) {
			 *it_dst = *(lut.vec()[*it]);
		}
		return r;
	}

	template <typename T, typename TInt>
    DSLib::Matrix<T, TInt> & scale(DSLib::Matrix<T, TInt> &mat, const T minSrc, const T maxSrc, const T minDst, const T maxDst) {
		//if (mat.order() == oRowMajor) {
			for (auto it = mat.rows->begin(); it != mat.rows->end(); it++) {
				if (*it < minSrc)
					*it = minSrc;
				if (*it > maxSrc)
					*it = maxSrc;
				*it = (((*it - minSrc) / (maxSrc - minSrc)) * (maxDst - minDst)) + minDst;
			}
		/*} else {
			for (auto it = mat.cols->begin(); it != mat.cols->end(); it++) {
				if (*it < minSrc)
					*it = minSrc;
				if (*it > maxSrc)
					*it = maxSrc;
				*it = (((*it - minSrc) / (maxSrc - minSrc)) * (maxDst - minDst)) + minDst;
			}
		}*/
		return mat;
	}

	template<typename T, typename TInt>
    inline std::ostream & operator<<(std::ostream & str, const DSLib::Matrix<T, TInt>& mat) {
		mat.write(str);
		return(str);
	}

	template<typename T, typename TInt>
    inline std::ostream && operator<<(std::ostream && str, const DSLib::Matrix<T, TInt>& mat) {
		mat.write(str);
		return std::move(str);
	}

	template <typename T, typename TInt>
    std::istream & operator>> (std::istream &str, DSLib::Matrix<T, TInt> &mat) {
		mat.read(str);
		return(str);
	}

	template <typename T, typename TInt>
    std::istream && operator>> (std::istream &&str, DSLib::Matrix<T, TInt> &mat) {
		mat.read(str);
		return std::move(str);
	}

	template <typename T, typename TIdx>
    T mean(const DSLib::Matrix<T, TIdx> &mat) {
        DSLib::Matrix<T, TIdx> mat2 = const_cast<DSLib::Matrix<T, TIdx>&>(mat);
		T sum = (T)0;
		for (auto it = mat2.rows->begin();it != mat2.rows->end();it++)
			sum += *it;
		return (sum / ~mat);
	}

	template <typename T, typename TIdx>
    T sd(const DSLib::Matrix<T, TIdx> &mat) {
        DSLib::Matrix<T, TIdx> mat2 = const_cast<DSLib::Matrix<T, TIdx>&>(mat);
		T mn = mean(mat);
		T sum = (T)0;
		for (auto it = mat2.rows->begin();it != mat2.rows->end();it++)
			sum += (*it - mn) * (*it - mn);
		return sqrt(sum / ~mat);
	}

	template <typename T, typename TIdx>
    T min(const DSLib::Matrix<T, TIdx> &mat) {
        DSLib::Matrix<T, TIdx> mat2 = const_cast<DSLib::Matrix<T, TIdx>&>(mat);
		T min = (T)mat.val(0, 0);
		for (auto it = mat2.rows->begin();it != mat2.rows->end();it++)
			if (*it<min)
				min = *it;
		return min;
	}

	template <typename T, typename TIdx>
    T max(const DSLib::Matrix<T, TIdx> &mat) {
        DSLib::Matrix<T, TIdx> mat2 = const_cast<DSLib::Matrix<T, TIdx>&>(mat);
		T max = (T)mat.val(0, 0);
		for (auto it = mat2.rows->begin();it != mat2.rows->end();it++)
			if (*it>max)
				max = *it;
		return max;
	}

	template <typename T, typename TIdx>
    DSLib::Matrix<T, TIdx> & scaleAndCenter(DSLib::Matrix<T, TIdx> &mat) {
		T m = mean(mat);
		T s = sd(mat);
		(mat - m) / s;
		return mat;
	}

	template <typename T, typename TIdx>
    DSLib::Matrix<T, TIdx> & scaleMinMax(DSLib::Matrix<T, TIdx> &mat) {
		scale(mat, min(mat), max(mat), (T)0, (T)1);
		return mat;
	}

	template <typename T, typename TIdx>
    DSLib::Matrix<T, TIdx> & abs(DSLib::Matrix<T, TIdx> &mat) {
		for (auto it = mat.rows->begin();it != mat.rows->end();it++)
			if (*it < (T)0)
				*it = (T)0;
		return mat;
	}
}
