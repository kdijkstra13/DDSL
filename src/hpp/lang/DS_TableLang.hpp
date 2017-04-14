#pragma once

#ifdef MSVC_VER
	#pragma warning(push)
	//#pragma warning(disable: 4244)
#endif

#include "hpp/DS_Types.hpp"
#include "hpp/lib/DS_Table.hpp"
#include "hpp/lib/DS_TableFunc.hpp"
#include "hpp/lang/DS_MatrixLang.hpp"

namespace DSLang {

	//****************//
	//** Assignment **//
	//****************//
	//BreakUp matrix
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator!(DSLib::Table<TIdx, TId> &rhs) {
		rhs.breakUpDown();
		return rhs;
	}

	//Inline Duplicate Table
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> operator*(const DSLib::Table<TIdx, TId> &rhs) {
		return rhs.dup();
	}

	template <typename TIdx, typename TId> DSLib::Table<TIdx, TId> && operator!(DSLib::Table<TIdx, TId> &&rhs) {return std::move(!rhs); };

	//********************//
	//** Initialization **//
	//********************//

	//Initialize with a matrix
	template <typename T, typename TIdx, typename TId=DSTypes::TableId>
	DSLib::Table<TIdx> operator|(DSTypes::ContentType lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		DSLib::Table<TIdx> tab;
		T dummy;
		tab.cols.add(rhs, rhs.rows.count(), rhs.cols.count(), lhs, DSTypes::dataType(dummy), DSLib::Matrix<TId, TIdx>(), DSLib::Matrix<TId, TIdx>());
		return tab;
	}

	template <typename T, typename TIdx>
	DSLib::Table<TIdx> operator^(DSTypes::ContentType lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		DSLib::Table<TIdx> tab;
		T dummy;
		tab.rows.add(rhs, rhs.rows.count(), rhs.cols.count(), lhs, dataType(dummy), DSLib::Matrix<DSTypes::TableId, TIdx>(), DSLib::Matrix<DSTypes::TableId, TIdx>());
		return tab;
	}

	//*******************//
	//** Concatenation **//
	//*******************//
	//Concat two tables
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator|(DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs) {
		lhs.cols.add(rhs);
		return lhs;
	}

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator^(DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs) {
		lhs.rows.add(rhs);
		return lhs;
	}

	//Concat a table and a matrix
	template <typename TIdx, typename TId, typename T>
	DSLib::Table<TIdx, TId> & operator|(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		T dummy;
		lhs.cols.add(rhs, rhs.rows.count(), rhs.cols.count(), DSTypes::ctUnknown, dataType(dummy), DSLib::Matrix<TId, TIdx>(), DSLib::Matrix<TId, TIdx>());
		return lhs;
	}

	template <typename TIdx, typename TId, typename T>
	DSLib::Table<TIdx, TId> & operator^(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		T dummy;
		lhs.rows.add(rhs, rhs.rows.count(), rhs.cols.count(), DSTypes::ctUnknown, dataType(dummy), DSLib::Matrix<TId, TIdx>(), DSLib::Matrix<TId, TIdx>());
		return lhs;
	}
	template <typename TIdx, typename TId> DSLib::Table<TIdx, TId> operator|(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Table<TIdx, TId> &rhs){return std::move(lhs|rhs);};
	template <typename TIdx, typename TId> DSLib::Table<TIdx, TId> operator^(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Table<TIdx, TId> &rhs){return std::move(lhs^rhs);};
	template <typename TIdx, typename TId, typename T> DSLib::Table<TIdx, TId> operator|(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Matrix<T, TIdx> &rhs){return std::move(lhs|rhs);};
	template <typename TIdx, typename TId, typename T> DSLib::Table<TIdx, TId> operator^(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Matrix<T, TIdx> &rhs){return std::move(lhs^rhs);};

	//*************************//
	//** Specify Col/Row Ids **//
	//*************************//
	//Set RowIds
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator|(const DSLib::Matrix<TId, TIdx> &lhs, DSLib::Table<TIdx, TId> &rhs) {
		rhs.rows.setIds(lhs);
		return rhs;
	}

	//Set ColIds
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator^(const DSLib::Matrix<TId, TIdx> &lhs, DSLib::Table<TIdx, TId> &rhs) {
		rhs.cols.setIds(lhs);
		return rhs;
	}

	//Set ContentTypes
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator^(const DSLib::Matrix<DSTypes::ContentType, TIdx> &lhs, DSLib::Table<TIdx, TId> &rhs) {
		rhs.setContentTypes(lhs);
		return rhs;
	}

	template <typename TIdx, typename TId> DSLib::Table<TIdx, TId> operator|(const DSLib::Matrix<TId, TIdx> &lhs, DSLib::Table<TIdx, TId> &&rhs) {return std::move(lhs|rhs);};
	template <typename TIdx, typename TId> DSLib::Table<TIdx, TId> operator^(const DSLib::Matrix<TId, TIdx> &lhs, DSLib::Table<TIdx, TId> &&rhs) {return std::move(lhs^rhs);};
	template <typename TIdx, typename TId> DSLib::Table<TIdx, TId> operator^(const DSLib::Matrix<ContentType, TIdx> &lhs, DSLib::Table<TIdx, TId> &&rhs) {return std::move(lhs^rhs);};

	//*******************//
	//** Selection **//
	//*******************//
	//+ = Table<>
	//- = Table<>.isRowTuple()
	//| = Table<>.isColTuple()
	//. = T
	//Return indices (+ @ +) Table index, (+ @ -) row indices of rowTuple of Table, (+ @ .) row|col indices of Table, (| @ .) row indices, (- @ .) col indices
	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator==(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs) {
		DSLib::Matrix<TIdx, TIdx> dst;
		const std::function<void(DSLib::MatrixBase &dst, const DSLib::MatrixBase &lhs, const DSLib::MatrixBase &rhs)> f = [](DSLib::MatrixBase &dst, const DSLib::MatrixBase &lhs, const DSLib::MatrixBase &rhs) {lhs.isEqual(dst, rhs);};
		DSFunc::test(dst, lhs, rhs, f);
		return dst;
	}

	template <typename TIdx, typename TId, typename T>
	DSLib::Matrix<TIdx, TIdx> operator==(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {		
		return (const_cast<Table<TIdx, TId>&>(lhs).template mat<T>((TIdx)0) == rhs);
	}

	template <typename T, typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator==(const DSLib::Table<TIdx, TId> &lhs, const T &rhs) {		
		const DSLib::Matrix<T, TIdx> & llhs = const_cast<DSLib::Table<TIdx, TId> &>(lhs);
		return (llhs == rhs);
	}

	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator!=(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs) {
		DSLib::Matrix<TIdx, TIdx> dst;
		const std::function<void(DSLib::MatrixBase &dst, const DSLib::MatrixBase &lhs, const DSLib::MatrixBase &rhs)> f = [](DSLib::MatrixBase &dst, const DSLib::MatrixBase &lhs, const DSLib::MatrixBase &rhs) {lhs.isNotEqual(dst, rhs);};
		DSFunc::test(dst, lhs, rhs, f);
		return dst;
	}

	template <typename TIdx, typename TId, typename T>
	DSLib::Matrix<TIdx, TIdx> operator!=(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return (const_cast<Table<TIdx, TId>&>(lhs).template mat<T>((TIdx)0) != rhs);
	}

	template <typename T, typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator!=(const DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		return (const_cast<Table<TIdx, TId>&>(lhs).template mat<T>((TIdx)0) == rhs);
	}

	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator<(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs) {
		DSLib::Matrix<TIdx, TIdx> dst;
		const std::function<void(DSLib::MatrixBase &dst, const DSLib::MatrixBase &lhs, const DSLib::MatrixBase &rhs)> f = [](DSLib::MatrixBase &dst, const DSLib::MatrixBase &lhs, const DSLib::MatrixBase &rhs) {lhs.isSmaller(dst, rhs);};
		DSFunc::test(dst, lhs, rhs, f);
		return dst;
	}

	template <typename TIdx, typename TId, typename T>
	DSLib::Matrix<TIdx, TIdx> operator<(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return (const_cast<Table<TIdx, TId>&>(lhs).template mat<T>((TIdx)0) < rhs);
	}

	template <typename T, typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator<(const DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		return (const_cast<Table<TIdx, TId>&>(lhs).template mat<T>((TIdx)0) < rhs);
	}

	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator>(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs) {
		DSLib::Matrix<TIdx, TIdx> dst;
		const std::function<void(DSLib::MatrixBase &dst, const DSLib::MatrixBase &lhs, const DSLib::MatrixBase &rhs)> f = [](DSLib::MatrixBase &dst, const DSLib::MatrixBase &lhs, const DSLib::MatrixBase &rhs) {lhs.isGreater(dst, rhs);};
		DSFunc::test(dst, lhs, rhs, f);
		return dst;
	}

	template <typename TIdx, typename TId, typename T>
	DSLib::Matrix<TIdx, TIdx> operator>(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return (const_cast<Table<TIdx, TId>&>(lhs).template mat<T>((TIdx)0) > rhs);
	}
	
	/* Should work??
	template <typename T, typename TIdx, typename TId>	
	typename std::enable_if<!std::is_same<T, DSLib::Model<TIdx, TId>>::value, DSLib::Matrix<TIdx, TIdx>>::type 
	operator>(const DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		DSLib::Matrix<T, TIdx> & m = const_cast<DSLib::Table<TIdx, TId>&>(lhs).template mat<T>(0);
		return (m > rhs);
	}*/

	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator<=(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs) {
		DSLib::Matrix<TIdx, TIdx> dst;
		const std::function<void(DSLib::MatrixBase &dst, const DSLib::MatrixBase &lhs, const DSLib::MatrixBase &rhs)> f = [](DSLib::MatrixBase &dst, const DSLib::MatrixBase &lhs, const DSLib::MatrixBase &rhs) {lhs.isSmallerEqual(dst, rhs);};
		DSFunc::test(dst, lhs, rhs, f);
		return dst;
	}

	template <typename TIdx, typename TId, typename T>
	DSLib::Matrix<TIdx, TIdx> operator<=(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return (const_cast<Table<TIdx, TId>&>(lhs).template mat<T>((TIdx)0) <= rhs);		
	}

	template <typename T, typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator<=(const DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		return (const_cast<Table<TIdx, TId>&>(lhs).template mat<T>((TIdx)0) <= rhs);
	}

	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator>=(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs) {
		DSLib::Matrix<TIdx, TIdx> dst;
		const std::function<void(DSLib::MatrixBase &dst, const DSLib::MatrixBase &lhs, const DSLib::MatrixBase &rhs)> f = [](DSLib::MatrixBase &dst, const DSLib::MatrixBase &lhs, const DSLib::MatrixBase &rhs) {lhs.isGreaterEqual(dst, rhs);};
		DSFunc::test(dst, lhs, rhs, f);
		return dst;
	}

	template <typename TIdx, typename TId, typename T>
	DSLib::Matrix<TIdx, TIdx> operator>=(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {		
		return (const_cast<Table<TIdx, TId>&>(lhs).template mat<T>((TIdx)0) >= rhs);		
	}

	template <typename T, typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator>=(const DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		return (const_cast<Table<TIdx, TId>&>(lhs).template mat<T>((TIdx)0) >= rhs);
	}

	//**************//
	//** Counting **//
	//**************//
	//Get the number of cells (slices give you the number of rows/cols
	template <typename TIdx, typename TId> TIdx operator~(const DSLib::Table<TIdx, TId> &rhs) {
		return rhs.cols.count() * rhs.rows.count();
	}

	//***********//
	//** Print **//
	//***********//
	//Print full Matrix
	template <typename TIdx, typename TId> DSLib::Table<TIdx, TId> & operator++(const DSLib::Table<TIdx, TId> &lhs, int) {
		lhs.print(std::cout);
		std::cout << std::endl;
		return const_cast<DSLib::Table<TIdx, TId> &>(lhs);
	}

	//Print Matrix meta information
	template <typename TIdx, typename TId> DSLib::Table<TIdx, TId> & operator--(const DSLib::Table<TIdx, TId> &lhs, int) {
		lhs.printLess(std::cout);
		return const_cast<DSLib::Table<TIdx, TId> &>(lhs);
	}

	//Print full Matrix
	template <typename TIdx, typename TId> DSLib::Table<TIdx, TId> & operator++(const DSLib::Table<TIdx, TId> &lhs) {
		lhs.print(std::cout);
		std::cout << std::endl;
		return const_cast<DSLib::Table<TIdx, TId> &>(lhs);
	}

	//Print Matrix meta information
	template <typename TIdx, typename TId> DSLib::Table<TIdx, TId> & operator--(const DSLib::Table<TIdx, TId> &lhs) {
		lhs.printLess(std::cout);
		return const_cast<DSLib::Table<TIdx, TId> &>(lhs);
	}

	//****************//
	//** Arithmetic **//
	//****************//
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> & operator+(DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		for (auto matWrap=lhs.matrixByIdx().begin();matWrap!=lhs.matrixByIdx().end();matWrap++) {		
			if ((*matWrap)->dataType() == DSTypes::dataType(rhs)) {
				DSLib::Matrix<T, TIdx> & mat = static_cast<DSLib::Matrix<T, TIdx> &>((*matWrap)->data());
				mat + rhs;
			} else {
#ifdef _DEBUG
				cout << "Note: Skipped an invalid embedded matrix type" << endl;
#endif
			}
		}
		return lhs;
	}

	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> & operator-(DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		for (auto matWrap = lhs.matrixByIdx().begin();matWrap != lhs.matrixByIdx().end();matWrap++) {
			if ((*matWrap)->dataType() == DSTypes::dataType(rhs)) {
				DSLib::Matrix<T, TIdx> & mat = static_cast<DSLib::Matrix<T, TIdx> &>((*matWrap)->data());
				mat - rhs;
			}
			else {
#ifdef _DEBUG
				cout << "Note: Skipped an invalid embedded matrix type" << endl;
#endif
			}
		}
		return lhs;
	}

	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> & operator+(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		for (auto matWrap = lhs.matrixByIdx().begin();matWrap != lhs.matrixByIdx().end();matWrap++) {
			if ((*matWrap)->dataType() == rhs.getDataType()) {
				DSLib::Matrix<T, TIdx> & mat = static_cast<DSLib::Matrix<T, TIdx> &>((*matWrap)->data());
				mat + rhs;
			}
			else {
#ifdef _DEBUG
				cout << "Note: Skipped an invalid embedded matrix type" << endl;
#endif
			}
		}
		return lhs;
	}
	
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> & operator-(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		for (auto matWrap = lhs.matrixByIdx().begin();matWrap != lhs.matrixByIdx().end();matWrap++) {
			if ((*matWrap)->dataType() == rhs.getDataType()) {
				DSLib::Matrix<T, TIdx> & mat = static_cast<DSLib::Matrix<T, TIdx> &>((*matWrap)->data());
				mat - rhs;
			}
			else {
#ifdef _DEBUG
				cout << "Note: Skipped an invalid embedded matrix type" << endl;
#endif
			}
		}
		return lhs;
	}

	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> & operator*(DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		for (auto matWrap = lhs.matrixByIdx().begin();matWrap != lhs.matrixByIdx().end();matWrap++) {
			if ((*matWrap)->dataType() == DSTypes::dataType(rhs)) {
				DSLib::Matrix<T, TIdx> & mat = static_cast<DSLib::Matrix<T, TIdx> &>((*matWrap)->data());
				mat * rhs;
			}
			else {
#ifdef _DEBUG
				cout << "Note: Skipped an invalid embedded matrix type" << endl;
#endif
			}
		}
		return lhs;
	}
	
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> & operator*(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		for (auto matWrap = lhs.matrixByIdx().begin();matWrap != lhs.matrixByIdx().end();matWrap++) {
			if ((*matWrap)->dataType() == rhs.getDataType()) {
				DSLib::Matrix<T, TIdx> & mat = static_cast<DSLib::Matrix<T, TIdx> &>((*matWrap)->data());
				mat * rhs;
			}
			else {
#ifdef _DEBUG
				cout << "Note: Skipped an invalid embedded matrix type" << endl;
#endif
			}
		}
		return lhs;
	}

	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> & operator/(DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		for (auto matWrap = lhs.matrixByIdx().begin();matWrap != lhs.matrixByIdx().end();matWrap++) {
			if ((*matWrap)->dataType() == DSTypes::dataType(rhs)) {
				DSLib::Matrix<T, TIdx> & mat = static_cast<DSLib::Matrix<T, TIdx> &>((*matWrap)->data());
				mat / rhs;
			}
			else {
#ifdef _DEBUG
				cout << "Note: Skipped an invalid embedded matrix type" << endl;
#endif
			}
		}
		return lhs;
	}
	
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> & operator/(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		for (auto matWrap = lhs.matrixByIdx().begin();matWrap != lhs.matrixByIdx().end();matWrap++) {
			if ((*matWrap)->dataType() == rhs.getDataType()) {
				DSLib::Matrix<T, TIdx> & mat = static_cast<DSLib::Matrix<T, TIdx> &>((*matWrap)->data());
				mat / rhs;
			}
			else {
#ifdef _DEBUG
				cout << "Note: Skipped an invalid embedded matrix type" << endl;
#endif
			}
		}
		return lhs;
	}
	
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> & pow(DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		for (auto matWrap = lhs.matrixByIdx().begin();matWrap != lhs.matrixByIdx().end();matWrap++) {
			if ((*matWrap)->dataType() == DSTypes::dataType(rhs)) {
				DSLib::Matrix<T, TIdx> & mat = static_cast<DSLib::Matrix<T, TIdx> &>((*matWrap)->data());
				pow(mat, rhs);
			}
			else {
#ifdef _DEBUG
				cout << "Note: Skipped an invalid embedded matrix type" << endl;
#endif
			}
		}
		return lhs;
	}
	
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> & pow(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		for (auto matWrap = lhs.matrixByIdx().begin();matWrap != lhs.matrixByIdx().end();matWrap++) {
			if ((*matWrap)->dataType() == rhs.getDataType()) {
				DSLib::Matrix<T, TIdx> & mat = static_cast<DSLib::Matrix<T, TIdx> &>((*matWrap)->data());
				pow(mat, rhs);
			}
			else {
#ifdef _DEBUG
				cout << "Note: Skipped an invalid embedded matrix type" << endl;
#endif
			}
		}
		return lhs;
	}

	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> & operator%(DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		for (auto matWrap = lhs.matrixByIdx().begin();matWrap != lhs.matrixByIdx().end();matWrap++) {
			if ((*matWrap)->dataType() == DSTypes::dataType(rhs)) {
				DSLib::Matrix<T, TIdx> & mat = static_cast<DSLib::Matrix<T, TIdx> &>((*matWrap)->data());
				mat % rhs;
			}
			else {
#ifdef _DEBUG
				cout << "Note: Skipped an invalid embedded matrix type" << endl;
#endif
			}
		}
		return lhs;
	}

	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> & operator%(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		for (auto matWrap = lhs.matrixByIdx().begin();matWrap != lhs.matrixByIdx().end();matWrap++) {
			if ((*matWrap)->dataType() == rhs.getDataType()) {
				DSLib::Matrix<T, TIdx> & mat = static_cast<DSLib::Matrix<T, TIdx> &>((*matWrap)->data());
				mat % rhs;
			}
			else {
#ifdef _DEBUG
				cout << "Note: Skipped an invalid embedded matrix type" << endl;
#endif
			}
		}
		return lhs;
	}

	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator+(DSLib::Table<TIdx, TId> &&lhs, const T &rhs) {return std::move(lhs+rhs);};
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator+(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Matrix<T, TIdx> &rhs) {return std::move(lhs+rhs);};
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator-(DSLib::Table<TIdx, TId> &&lhs, const T &rhs) { return std::move(lhs - rhs); };
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator-(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Matrix<T, TIdx> &rhs) { return std::move(lhs-rhs);};
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator*(DSLib::Table<TIdx, TId> &&lhs, const T &rhs) {return std::move(lhs*rhs);};
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator*(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Matrix<T, TIdx> &rhs) {return std::move(lhs*rhs);};
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator/(DSLib::Table<TIdx, TId> &&lhs, const T &rhs) {return std::move(lhs/rhs);};
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator/(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Matrix<T, TIdx> &rhs) {return std::move(lhs/rhs);};
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator%(DSLib::Table<TIdx, TId> &&lhs, const T &rhs) {return std::move(lhs%rhs);};
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> operator%(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Matrix<T, TIdx> &rhs) {return std::move(lhs%rhs);};
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> pow(DSLib::Table<TIdx, TId> &&lhs, const T &rhs) {return std::move(pow(lhs,rhs));};
	template <typename TIdx, typename TId, typename T>  DSLib::Table<TIdx, TId> pow(DSLib::Table<TIdx, TId> &&lhs, const DSLib::Matrix<T, TIdx> &rhs) {return std::move(pow(lhs,rhs));};

	//***************//
	//** Streaming **//
	//***************//
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator >> (DSLib::Table<TIdx, TId> &lhs, std::ostream &rhs) {
		lhs.write(rhs);
		return lhs;
	}

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator >> (DSLib::Table<TIdx, TId> &&lhs, std::ostream &rhs) {
		lhs.write(rhs);
		return std::move(lhs);
	}

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator<<(DSLib::Table<TIdx, TId> &lhs, std::istream &rhs) {
		lhs.read(rhs);
		return lhs;
	}

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator<<(DSLib::Table<TIdx, TId> &&lhs, std::istream &rhs) {
		lhs.read(rhs);
		return std::move(lhs);
	}

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator >> (DSLib::Table<TIdx, TId> &lhs, const DSTypes::String &rhs) {
		lhs.write(rhs);
		return lhs;
	}

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator >> (DSLib::Table<TIdx, TId> &&lhs, const DSTypes::String &rhs) {
		lhs.write(rhs);
		return std::move(lhs);
	}

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator<<(DSLib::Table<TIdx, TId> &lhs, const DSTypes::String &rhs) {
		lhs.read(rhs);
		return lhs;
	}

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> && operator<<(DSLib::Table<TIdx, TId> &&lhs, const DSTypes::String &rhs) {
		lhs.read(rhs);
		return std::move(lhs);
	}


	//*************//
	//** Slicing **//
	//*************//
	//Defined as member functions

};


namespace DSScript {
	//****************//
	//** Assignment **//
	//****************//
	//BreakUp matrix
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator!(DSLib::Table<TIdx, TId> &rhs) {
		return DSLang::operator!(rhs);
	}

	//Inline Duplicate Table
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> operator*(const DSLib::Table<TIdx, TId> &rhs) {
		return DSLang::operator*(rhs);
	}

	//********************//
	//** Initialization **//
	//********************//

	//Initialize with a matrix
	template <typename T, typename TIdx, typename TId = DSTypes::TableId>
	DSLib::Table<TIdx> operator|(DSTypes::ContentType lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return DSLang::operator|(lhs, rhs);
	}

	template <typename T, typename TIdx>
	DSLib::Table<TIdx> operator^(DSTypes::ContentType lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return DSLang::operator^(lhs, rhs);
	}

	//*******************//
	//** Concatenation **//
	//*******************//
	//Concat two tables
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator|(DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs) {
		return DSLang::operator|(lhs, rhs);
	}

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator^(DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs) {
		return DSLang::operator^(lhs, rhs);
	}

	//Concat a table and a matrix
	template <typename TIdx, typename TId, typename T>
	DSLib::Table<TIdx, TId> & operator|(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return DSLang::operator|(lhs, rhs);
	}

	template <typename TIdx, typename TId, typename T>
	DSLib::Table<TIdx, TId> & operator^(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return DSLang::operator^(lhs, rhs);
	}

	//*************************//
	//** Specify Col/Row Ids **//
	//*************************//
	//Set RowIds
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator|(const DSLib::Matrix<TId, TIdx> &lhs, DSLib::Table<TIdx, TId> &rhs) {
		return DSLang::operator|(lhs, rhs);
	}

	//Set ColIds
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator^(const DSLib::Matrix<TId, TIdx> &lhs, DSLib::Table<TIdx, TId> &rhs) {
		return DSLang::operator^(lhs, rhs);
	}

	//Set ContentTypes
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator^(const DSLib::Matrix<DSTypes::ContentType, TIdx> &lhs, DSLib::Table<TIdx, TId> &rhs) {
		return DSLang::operator^(lhs, rhs);
	}

	//*******************//
	//** Selection **//
	//*******************//
	//+ = Table<>
	//- = Table<>.isRowTuple()
	//| = Table<>.isColTuple()
	//. = T
	//Return indices (+ @ +) Table index, (+ @ -) row indices of rowTuple of Table, (+ @ .) row|col indices of Table, (| @ .) row indices, (- @ .) col indices
	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator==(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs) {
		return DSLang::operator==(lhs, rhs);
	}

	template <typename TIdx, typename TId, typename T>
	DSLib::Matrix<TIdx, TIdx> operator==(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return DSLang::operator==(lhs, rhs);
	}

	template <typename T, typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator==(const DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		return DSLang::operator==(lhs, rhs);
	}

	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator!=(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs) {
		return DSLang::operator!=(lhs, rhs);
	}

	template <typename TIdx, typename TId, typename T>
	DSLib::Matrix<TIdx, TIdx> operator!=(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return DSLang::operator!=(lhs, rhs);
	}

	template <typename T, typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator!=(const DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		return DSLang::operator!=(lhs, rhs);
	}

	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator<(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs) {
		return DSLang::operator<(lhs, rhs);
	}

	template <typename TIdx, typename TId, typename T>
	DSLib::Matrix<TIdx, TIdx> operator<(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return DSLang::operator<(lhs, rhs);
	}

	template <typename T, typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator<(const DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		return DSLang::operator<(lhs, rhs);
	}

	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator>(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs) {
		return DSLang::operator>(lhs, rhs);
	}

	template <typename TIdx, typename TId, typename T>
	DSLib::Matrix<TIdx, TIdx> operator>(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return DSLang::operator>(lhs, rhs);
	}

	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator<=(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs) {
		return DSLang::operator<=(lhs, rhs);
	}

	template <typename TIdx, typename TId, typename T>
	DSLib::Matrix<TIdx, TIdx> operator<=(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return DSLang::operator<=(lhs, rhs);
	}

	template <typename T, typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator<=(const DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		return DSLang::operator<=(lhs, rhs);
	}

	template <typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator>=(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Table<TIdx, TId> &rhs) {
		return DSLang::operator|(lhs, rhs);
	}

	template <typename TIdx, typename TId, typename T>
	DSLib::Matrix<TIdx, TIdx> operator>=(const DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return DSLang::operator>=(lhs, rhs);
	}

	template <typename T, typename TIdx, typename TId>
	DSLib::Matrix<TIdx, TIdx> operator>=(const DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		return DSLang::operator>=(lhs, rhs);
	}

	//**************//
	//** Counting **//
	//**************//
	//Get the number of cells (slices give you the number of rows/cols
	template <typename TIdx, typename TId> TIdx operator~(const DSLib::Table<TIdx, TId> &rhs) {
		return DSLang::operator~(rhs);
	}

	//***********//
	//** Print **//
	//***********//
	//Print full Matrix
	template <typename TIdx, typename TId> DSLib::Table<TIdx, TId> operator++(const DSLib::Table<TIdx, TId> &lhs) {
		return DSLang::operator++(lhs);
	}

	//Print Matrix meta information
	template <typename TIdx, typename TId> DSLib::Table<TIdx, TId> operator--(const DSLib::Table<TIdx, TId> &lhs) {
		return DSLang::operator--(lhs);
	}

	//****************//
	//** Arithmetic **//
	//****************//
	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & operator+(DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		return DSLang::operator+(lhs, rhs);
	}

	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & operator-(DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		return DSLang::operator-(lhs, rhs);
	}

	template <typename TIdx, typename TId, typename T> 
	DSLib::Table<TIdx, TId> & operator+(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return DSLang::operator+(lhs, rhs);
	}

	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & operator-(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return DSLang::operator-(lhs, rhs);
	}

	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & operator*(DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		return DSLang::operator*(lhs, rhs);
	}

	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & operator*(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return DSLang::operator*(lhs, rhs);
	}

	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & operator/(DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		return DSLang::operator/(lhs, rhs);
	}

	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & operator/(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return DSLang::operator/(lhs, rhs);
	}

	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & pow(DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		return DSLang::pow(lhs, rhs);
	}

	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & pow(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return DSLang::pow(lhs, rhs);
	}

	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & operator%(DSLib::Table<TIdx, TId> &lhs, const T &rhs) {
		return DSLang::operator%(lhs, rhs);
	}

	template <typename TIdx, typename TId, typename T>  
	DSLib::Table<TIdx, TId> & operator%(DSLib::Table<TIdx, TId> &lhs, const DSLib::Matrix<T, TIdx> &rhs) {
		return DSLang::operator%(lhs, rhs);
	}

	//***************//
	//** Streaming **//
	//***************//
	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator>>(DSLib::Table<TIdx, TId> &lhs, std::ostream &rhs) {
		return DSLang::operator>>(lhs, rhs);
	}

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator<<(DSLib::Table<TIdx, TId> &lhs, std::istream &rhs) {
		return DSLang::operator<<(lhs, rhs);
	}

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator>>(DSLib::Table<TIdx, TId> &lhs, const DSTypes::String &rhs) {
		return DSLang::operator>>(lhs, rhs);
	}

	template <typename TIdx, typename TId>
	DSLib::Table<TIdx, TId> & operator<<(DSLib::Table<TIdx, TId> &lhs, const DSTypes::String &rhs) {
		return DSLang::operator<<(lhs, rhs);
	}
}

#ifdef MSVS_VER
	#pragma warning(pop)
#endif
