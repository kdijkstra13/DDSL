#pragma once
#include "h/lang/DS_MatrixLang.h"

#ifdef MSVC_VER
  #pragma warning(push)
  #pragma warning(disable: 4244)
#endif

#include "hpp/DS_Types.hpp"
#include "hpp/lib/DS_Matrix.hpp"
#include "hpp/lib/DS_MatrixFunc.hpp"

namespace DSLang {
	//***********************//
	//** Matrix Assignment **//
	//***********************//

	//Transpose Matrix
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator+=(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		DSLib::Matrix<T, TInt> & rrhs = const_cast<DSLib::Matrix<T, TInt> &>(rhs);
		if (&lhs != &rrhs) {
			DSFunc::transpose(lhs, rrhs);
		} else {
			auto rhs2 = rrhs;
			DSFunc::transpose(lhs, rhs2);
		}
		return lhs;
	}

	//Matrix Multiplication
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator*=(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		DSLib::Matrix<T, TInt> & rrhs = const_cast<DSLib::Matrix<T, TInt> &>(rhs);
		if (&lhs != &rrhs) {
			auto lhs2 = lhs;
			DSFunc::multiply(lhs, lhs2, rrhs);
		} else {
			auto lhs2 = lhs;
			auto rhs2 = rrhs;
			DSFunc::multiply(lhs, lhs2, rhs2);
		}
		return lhs;
	}

	//Force Matrix Order to oColMajor
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator|=(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		DSLib::Matrix<T, TInt> & rrhs = const_cast<DSLib::Matrix<T, TInt> &>(rhs);
		if (&lhs != &rrhs) {
			lhs.setOrder(DSTypes::oColMajor);
			DSFunc::convert(lhs, rrhs);
		} else {
			auto rhs2 = rrhs;
			lhs.setOrder(DSTypes::oColMajor);
			DSFunc::convert(lhs, rhs2);
		}
		return lhs;
	}

	//Force Matrix Order to oRowMajor
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator^=(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		DSLib::Matrix<T, TInt> & rrhs = const_cast<DSLib::Matrix<T, TInt> &>(rhs);
		if (&lhs != &rrhs) {
			lhs.setOrder(DSTypes::oRowMajor);
			DSFunc::convert(lhs, rrhs);
		} else {
			auto rhs2 = rrhs;
			lhs.setOrder(DSTypes::oRowMajor);
			DSFunc::convert(lhs, rhs2);
		}
		return lhs;
	}

	//Inline BreakUp Matrix
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator!(DSLib::Matrix<T, TInt> &rhs) {
		rhs.breakDown();
		rhs.breakUp(true);
		return rhs;
	}

	//Inline Duplicate Matrix
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> operator*(const DSLib::Matrix<T, TInt> &rhs) {
		return rhs.dup();
	}

	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator|=(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs) {return std::move(lhs|=rhs);};
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator^=(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs) {return std::move(lhs^=rhs);};
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator+=(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs) {return std::move(lhs+=rhs);};
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator*=(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs) {return std::move(lhs*=rhs);};
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator!(DSLib::Matrix<T, TInt> &&rhs) {return std::move(!rhs);};

	//***************************//
	//** Matrix Initialization **//
	//***************************//

	//Concat two Matrices
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator|(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		DSLib::Matrix<T, TInt> & rrhs = const_cast<DSLib::Matrix<T, TInt> &>(rhs);
		if (&lhs != &rhs) {
			lhs.cols.add(rrhs);
		} else {
			auto rhs2 = rrhs;
			lhs.cols.add(rhs2);
		}
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator^(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		DSLib::Matrix<T, TInt> & rrhs = const_cast<DSLib::Matrix<T, TInt> &>(rhs);
		if (&lhs != &rhs) {
			lhs.rows.add(rrhs);
		} else {
			auto rhs2 = rrhs;
			lhs.rows.add(rhs2);
		}
		return lhs;
	}

	//Concat a value to a matrix (works only for vector matrices)
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator|(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
		lhs.cols.add(rhs);
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator^(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
		lhs.rows.add(rhs);
		return lhs;
	}

	//Create new matrix
	template <typename T>
	DSLib::Matrix<T, DSTypes::UInt32> operator|(DSTypes::DataType lhs, const T &rhs) {
		if (!dataTypeEqual(lhs, rhs))
			throw DSTypes::Error(DSTypes::ecIncompatible, "operator|", SS("Incompatible types:" << DSTypes::dataTypeToString(lhs) << " and " << typeid(T).name()));
		DSLib::Matrix<T, DSTypes::UInt32> m(DSTypes::oRowMajor);
		m.rows.add(rhs);
		return m;
	}

	template <typename T>
	DSLib::Matrix<T, DSTypes::UInt32> operator^(DSTypes::DataType lhs, const T &rhs) {
		if (!dataTypeEqual(lhs, rhs))
			throw DSTypes::Error(DSTypes::ecIncompatible, "operator^", SS("Incompatible types:" << DSTypes::dataTypeToString(lhs) << " and " << typeid(T).name()));
		DSLib::Matrix<T, DSTypes::UInt32> m(DSTypes::oColMajor);
		m.cols.add(rhs);
		return m;
	}

	//Generate a range (works only for ordinal types)
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator||(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
		DSLib::Matrix<T, TInt> mat = lhs;
		DSFunc::range(lhs, mat, rhs);
		return lhs;
	}

	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator|(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs) {return (std::move(lhs|rhs));};
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator^(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs) {return (std::move(lhs^rhs));};
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator|(DSLib::Matrix<T, TInt> &&lhs, const T &rhs) {return std::move(lhs|rhs);};
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator^(DSLib::Matrix<T, TInt> &&lhs, const T &rhs) {return std::move(lhs^rhs);};

	//*******************//
	//** Row Selection **//
	//*******************//
	//+ = Matrix<T>
	//- = Matrix<T>.isRowVector()
	//| = Matrix<T>.isColVector()
	//. = T
	//Return indices (+ @ +) matrix index, (+ @ -) row indices of rowVector of matrix, (+ @ |) col indices of colVector of matrix, (+ == .) row|col indices of matrix, (| == .) row indices, (- == .) col indices

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator==(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		auto & llhs = const_cast<DSLib::Matrix<T, TInt>&>(lhs);
		auto & rrhs = const_cast<DSLib::Matrix<T, TInt>&>(rhs);
		DSLib::Matrix<TInt, TInt> mat(lhs.order());
		const std::function<bool(const T&, const T&)> f = [](const T &a, const T &b){return a == b;};
		DSFunc::test<T, TInt>(mat, llhs, rrhs, f);
		return mat;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator==(const DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
		auto & llhs = const_cast<DSLib::Matrix<T, TInt>&>(lhs);
		DSLib::Matrix<TInt, TInt> mat(lhs.order());
		const std::function<bool(const T&, const T&)> f = [](const T &a, const T &b){return a == b;};
		DSFunc::test<T, TInt>(mat, llhs, rhs, f);
		return mat;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator!=(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		auto & llhs = const_cast<DSLib::Matrix<T, TInt>&>(lhs);
		auto & rrhs = const_cast<DSLib::Matrix<T, TInt>&>(rhs);
		DSLib::Matrix<TInt, TInt> mat(lhs.order());
		const std::function<bool(const T&, const T&)> f = [](const T &a, const T &b){return a != b;};
		DSFunc::test<T, TInt>(mat, llhs, rrhs, f);
		return mat;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator!=(const DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
		auto & llhs = const_cast<DSLib::Matrix<T, TInt>&>(lhs);
		DSLib::Matrix<TInt, TInt> mat(lhs.order());
		const std::function<bool(const T&, const T&)> f = [](const T &a, const T &b){return a != b;};
		DSFunc::test<T, TInt>(mat, llhs, rhs, f);
		return mat;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator<=(const DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
		auto & llhs = const_cast<DSLib::Matrix<T, TInt>&>(lhs);
		DSLib::Matrix<TInt, TInt> mat(lhs.order());
		const std::function<bool(const T&, const T&)> f = [](const T &a, const T &b){return a <= b;};
		DSFunc::test<T, TInt>(mat, llhs, rhs, f);
		return mat;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator>=(const DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
		auto & llhs = const_cast<DSLib::Matrix<T, TInt>&>(lhs);
		DSLib::Matrix<TInt, TInt> mat(lhs.order());
		const std::function<bool(const T&, const T&)> f = [](const T &a, const T &b){return a >= b;};
		DSFunc::test<T, TInt>(mat, llhs, rhs, f);
		return mat;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator<=(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		auto & llhs = const_cast<DSLib::Matrix<T, TInt>&>(lhs);
		auto & rrhs = const_cast<DSLib::Matrix<T, TInt>&>(rhs);
		DSLib::Matrix<TInt, TInt> mat(lhs.order());
		const std::function<bool(const T&, const T&)> f = [](const T &a, const T &b){return a <= b;};
		DSFunc::test<T, TInt>(mat, llhs, rrhs, f);
		return mat;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator>=(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		auto & llhs = const_cast<DSLib::Matrix<T, TInt>&>(lhs);
		auto & rrhs = const_cast<DSLib::Matrix<T, TInt>&>(rhs);
		DSLib::Matrix<TInt, TInt> mat(lhs.order());
		const std::function<bool(const T&, const T&)> f = [](const T &a, const T &b){return a >= b;};
		DSFunc::test<T, TInt>(mat, llhs, rrhs, f);
		return mat;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator<(const DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
		auto & llhs = const_cast<DSLib::Matrix<T, TInt>&>(lhs);
		DSLib::Matrix<TInt, TInt> mat(lhs.order());
		const std::function<bool(const T&, const T&)> f = [](const T &a, const T &b){return a < b;};
		DSFunc::test<T, TInt>(mat, llhs, rhs, f);
		return mat;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator>(const DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
		auto & llhs = const_cast<DSLib::Matrix<T, TInt>&>(lhs);
		DSLib::Matrix<TInt, TInt> mat(lhs.order());
		const std::function<bool(const T&, const T&)> f = [](const T &a, const T &b){return a > b;};
		DSFunc::test<T, TInt>(mat, llhs, rhs, f);
		return mat;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator<(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		auto & llhs = const_cast<DSLib::Matrix<T, TInt>&>(lhs);
		auto & rrhs = const_cast<DSLib::Matrix<T, TInt>&>(rhs);
		DSLib::Matrix<TInt, TInt> mat(lhs.order());
		const std::function<bool(const T&, const T&)> f = [](const T &a, const T &b){return a < b;};
		DSFunc::test<T, TInt>(mat, llhs, rrhs, f);
		return mat;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<TInt, TInt> operator>(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		auto & llhs = const_cast<DSLib::Matrix<T, TInt>&>(lhs);
		auto & rrhs = const_cast<DSLib::Matrix<T, TInt>&>(rhs);
		DSLib::Matrix<TInt, TInt> mat(lhs.order());
		const std::function<bool(const T&, const T&)> f = [](const T &a, const T &b){return a > b;};
		DSFunc::test<T, TInt>(mat, llhs, rrhs, f);
		return mat;
	}

	//***************//
	//** Set logic **//
	//***************//

	//Set intersection between indices
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator&&(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		auto & rrhs = const_cast<DSLib::Matrix<T, TInt>&>(rhs);
		if (&lhs != &rhs) {
			auto lhs2 = lhs;
			DSFunc::setIntersection(lhs, lhs2, rrhs);
		} else {
			auto lhs2 = lhs;
			auto rhs2 = rhs;
			DSFunc::setIntersection(lhs, lhs2, rhs2);
		}
		return lhs;
	}

	//Set union between indices
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator||(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		auto & rrhs = const_cast<DSLib::Matrix<T, TInt>&>(rhs);
		if (&lhs != &rhs) {
			auto lhs2 = lhs;
			DSFunc::setUnion(lhs, lhs2, rrhs);
		} else {
			auto lhs2 = lhs;
			auto rhs2 = rhs;
			DSFunc::setUnion(lhs, lhs2, rhs2);
		}
		return lhs;
	}

	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator&&(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs) {return std::move(lhs&&rhs);};
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator||(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs) {return std::move(lhs||rhs);};

	//****************//
	//** Arithmetic **//
	//****************//
	//+ = Matrix<T>
	//- = Matrix<T>.isRowVector()
	//| = Matrix<T>.isColVector()
	//. = T
	//Return indices (+ @ +) per cell of both matrices, (+ @ -) per rowVector of matrix, (+ @ |) per colVector of matrix, (+ == .) per cel of matrix, (| @ .) @ per cel of colVector, (- @ .) @ per cel of rowVector

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator+(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
		const std::function<T(T&, const T&)> f = [](T &a, const T &b)->T{return a + b;};
		DSFunc::apply<T, TInt>(lhs, lhs, rhs, f);
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator+(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		auto & rrhs = const_cast<DSLib::Matrix<T, TInt>&>(rhs);
		const std::function<T(T&, const T&)> f = [](T &a, const T &b)->T{return a + b; };
		DSFunc::apply<T, TInt>(lhs, lhs, rrhs, f);
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator-(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
		const std::function<T(T&, const T&)> f = [](T &a, const T &b)->T{return a - b;};
		DSFunc::apply<T, TInt>(lhs, lhs, rhs, f);
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator-(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		auto & rrhs = const_cast<DSLib::Matrix<T, TInt>&>(rhs);
		const std::function<T(T&, const T&)> f = [](T &a, const T &b)->T{return a - b;};
		DSFunc::apply<T, TInt>(lhs, lhs, rrhs, f);
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator-(DSLib::Matrix<T, TInt> &rhs) {
		const std::function<T(T&, const T&)> f = [](T &a, const T &b)->T{return a * -b; };
		DSFunc::apply<T, TInt>(rhs, rhs, 1, f);
		return rhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator/(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
		const std::function<T(T&, const T&)> f = [](T &a, const T &b)->T{return a / b; };
		DSFunc::apply<T, TInt>(lhs, lhs, rhs, f);
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator/(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		auto & rrhs = const_cast<DSLib::Matrix<T, TInt>&>(rhs);
		const std::function<T(T&, const T&)> f = [](T &a, const T &b)->T{return a / b; };
		DSFunc::apply<T, TInt>(lhs, lhs, rrhs, f);
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator*(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
		const std::function<T(T&, const T&)> f = [](T &a, const T &b)->T{return a * b; };
		DSFunc::apply<T, TInt>(lhs, lhs, rhs, f);
		return lhs;	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator*(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		auto & rrhs = const_cast<DSLib::Matrix<T, TInt>&>(rhs);
		const std::function<T(T&, const T&)> f = [](T &a, const T &b)->T{return a * b; };
		DSFunc::apply<T, TInt>(lhs, lhs, rrhs, f);
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & pow(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
		const std::function<T(T&, const T&)> f = [](T &a, const T &b)->T{return (T)std::pow(a, b); };
		DSFunc::apply<T, TInt>(lhs, lhs, rhs, f);
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & pow(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		auto & rrhs = const_cast<DSLib::Matrix<T, TInt>&>(rhs);
		const std::function<T(T&, const T&)> f = [](T &a, const T &b)->T{return (T)std::pow(a, b); };
		DSFunc::apply<T, TInt>(lhs, lhs, rrhs, f);
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & min(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
		const std::function<T(T&, const T&)> f = [](T &a, const T &b)->T{return (T)std::min(a, b); };
		DSFunc::apply<T, TInt>(lhs, lhs, rhs, f);
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & min(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		auto & rrhs = const_cast<DSLib::Matrix<T, TInt>&>(rhs);
		const std::function<T(T&, const T&)> f = [](T &a, const T &b)->T{return (T)std::min(a, b); };
		DSFunc::apply<T, TInt>(lhs, lhs, rrhs, f);
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & max(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
		const std::function<T(T&, const T&)> f = [](T &a, const T &b)->T{return (T)std::max(a, b); };
		DSFunc::apply<T, TInt>(lhs, lhs, rhs, f);
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & max(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		auto & rrhs = const_cast<DSLib::Matrix<T, TInt>&>(rhs);
		const std::function<T(T&, const T&)> f = [](T &a, const T &b)->T{return (T)std::max(a, b); };
		DSFunc::apply<T, TInt>(lhs, lhs, rrhs, f);
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator%(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
		const std::function<T(T&, const T&)> f = [](T &a, const T &b)->T{return a % b; };
		DSFunc::apply<T, TInt>(lhs, lhs, rhs, f);
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator%(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
		auto & rrhs = const_cast<DSLib::Matrix<T, TInt>&>(rhs);
		const std::function<T(T&, const T&)> f = [](T &a, const T &b)->T{return a % b; };
		DSFunc::apply<T, TInt>(lhs, lhs, rrhs, f);
		return lhs;
	}

	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator+(DSLib::Matrix<T, TInt> &&lhs, const T &rhs) {return std::move(lhs+rhs);};
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator+(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs) {return std::move(lhs+rhs);};
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator-(DSLib::Matrix<T, TInt> &&rhs) {return std::move(-rhs);};
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator-(DSLib::Matrix<T, TInt> &&lhs, const T &rhs) { return std::move(lhs - rhs); };
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator-(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs) { return std::move(lhs - rhs); };
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator/(DSLib::Matrix<T, TInt> &&lhs, const T &rhs) {return std::move(lhs/rhs);};
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator/(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs) {return std::move(lhs/rhs);};
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator*(DSLib::Matrix<T, TInt> &&lhs, const T &rhs) {return std::move(lhs*rhs);};
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator*(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs){return std::move(lhs*rhs);};
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> pow(DSLib::Matrix<T, TInt> &&lhs, const T &rhs) {return std::move(pow(lhs,rhs));};
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> pow(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs){return std::move(pow(lhs,rhs));};
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator%(DSLib::Matrix<T, TInt> &&lhs, const T &rhs) {return std::move(lhs%rhs);};
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator%(DSLib::Matrix<T, TInt> &&lhs, const DSLib::Matrix<T, TInt> &rhs){return std::move(lhs%rhs);};

	//**************//
	//** Counting **//
	//**************//
	//Get the number of cells (slices give you the number of rows/cols

	//Return number of cells
	template <typename T, typename TInt>
	TInt operator~(const DSLib::Matrix<T, TInt> &rhs) {
		return rhs.cols.count() * rhs.rows.count();
	}

	//***********//
	//** Print **//
	//***********//

	//Print full Matrix
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator++(DSLib::Matrix<T, TInt> &lhs, int) {
		lhs.print(std::cout);
		std::cout << std::endl;
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> operator++(DSLib::Matrix<T, TInt> &&lhs, int) {
		lhs.print(std::cout);
		std::cout << std::endl;
		return std::move(lhs);
	}

	template <typename T, typename TInt>
	const DSLib::Matrix<T, TInt> & operator++(const DSLib::Matrix<T, TInt> &lhs, int) {
		lhs.print(std::cout);
		std::cout << std::endl;
		return lhs;
	}

	//Print Matrix meta information
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> operator--(DSLib::Matrix<T, TInt> &&lhs, int) {
		lhs.printLess(std::cout);
		std::cout << std::endl;
		return std::move(lhs);
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator--(DSLib::Matrix<T, TInt> &lhs, int) {
		lhs.printLess(std::cout);
		std::cout << std::endl;
		return lhs;
	}

	template <typename T, typename TInt>
	const DSLib::Matrix<T, TInt> & operator--(const DSLib::Matrix<T, TInt> &lhs, int) {
		lhs.printLess(std::cout);
		std::cout << std::endl;
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator++(DSLib::Matrix<T, TInt> &lhs) {
		lhs.print(std::cout);
		std::cout << std::endl;
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> operator++(DSLib::Matrix<T, TInt> &&lhs) {
		lhs.print(std::cout);
		std::cout << std::endl;
		return std::move(lhs);
	}

	template <typename T, typename TInt>
	const DSLib::Matrix<T, TInt> & operator++(const DSLib::Matrix<T, TInt> &lhs) {
		lhs.print(std::cout);
		std::cout << std::endl;
		return lhs;
	}

	//Print Matrix meta information
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> operator--(DSLib::Matrix<T, TInt> &&lhs) {
		lhs.printLess(std::cout);
		std::cout << std::endl;
		return std::move(lhs);
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator--(DSLib::Matrix<T, TInt> &lhs) {
		lhs.printLess(std::cout);
		std::cout << std::endl;
		return lhs;
	}

	template <typename T, typename TInt>
	const DSLib::Matrix<T, TInt> & operator--(const DSLib::Matrix<T, TInt> &lhs) {
		lhs.printLess(std::cout);
		std::cout << std::endl;
		return lhs;
	}

	//**********************//
	//** Matrix streaming **//
	//**********************//
	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator>>(DSLib::Matrix<T, TInt> &lhs, std::ostream &rhs) {
		lhs.write(rhs);
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> operator>>(DSLib::Matrix<T, TInt> &&lhs, std::ostream &rhs) {
		lhs.write(rhs);
		return std::move(lhs);
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator<<(DSLib::Matrix<T, TInt> &lhs, std::istream &rhs) {
		lhs.read(rhs);
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> operator<<(DSLib::Matrix<T, TInt> &&lhs, std::istream &rhs) {
		lhs.read(rhs);
		return std::move(lhs);
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator >> (DSLib::Matrix<T, TInt> &lhs, const DSTypes::String &rhs) {
		lhs.write(rhs);
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> operator >> (DSLib::Matrix<T, TInt> &&lhs, const DSTypes::String &rhs) {
		lhs.write(rhs);
		return std::move(lhs);
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> & operator<<(DSLib::Matrix<T, TInt> &lhs, const DSTypes::String &rhs) {
		lhs.read(rhs);
		return lhs;
	}

	template <typename T, typename TInt>
	DSLib::Matrix<T, TInt> operator<<(DSLib::Matrix<T, TInt> &&lhs, const DSTypes::String &rhs) {
		lhs.read(rhs);
		return std::move(lhs);
	}

	//********************//
	//** Matrix Slicing **//
	//********************//
	//Defined as member functions
};

namespace DSScript {
	//***********************//
	//** Matrix Assignment **//
	//***********************//
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator+=(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
        return DSLang::operator+=(lhs, rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator*=(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
        return DSLang::operator*=(lhs, rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator|=(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
        return DSLang::operator|=(lhs, rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator^=(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
        return DSLang::operator^=(lhs, rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator!(DSLib::Matrix<T, TInt> &rhs) {
        return DSLang::operator!(rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator*(const DSLib::Matrix<T, TInt> &rhs) {
        return DSLang::operator*(rhs);
	}

	//***************************//
	//** Matrix Initialization **//
	//***************************//
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator|(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
	    return DSLang::operator|(lhs, rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator^(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
	    return DSLang::operator^(lhs, rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator|(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
	    return DSLang::operator|(lhs, rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator^(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
	    return DSLang::operator^(lhs, rhs);
	}
	template <typename T> DSLib::Matrix<T, DSTypes::UInt32> operator|(DSTypes::DataType lhs, const T &rhs) {
        return DSLang::operator|(lhs, rhs);
	}
	template <typename T> DSLib::Matrix<T, DSTypes::UInt32> operator^(DSTypes::DataType lhs, const T &rhs) {
	    return DSLang::operator^(lhs, rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator||(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
	    return DSLang::operator^(lhs, rhs);
	}

	//*******************//
	//** Row Selection **//
	//*******************//
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator==(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
	    return DSLang::operator==(lhs,rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator==(const DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
	    return DSLang::operator==(lhs,rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator!=(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
	    return DSLang::operator==(lhs,rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator!=(const DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
	    return DSLang::operator!=(lhs,rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator<=(const DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
	    return DSLang::operator<=(lhs,rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator>=(const DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
	    return DSLang::operator>=(lhs,rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator<=(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
	    return DSLang::operator<=(lhs,rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator>=(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
	    return DSLang::operator>=(lhs,rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator<(const DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
	    return DSLang::operator<(lhs,rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator>(const DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
	    return DSLang::operator>(lhs,rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator<(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
	    return DSLang::operator<(lhs,rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<TInt, TInt> operator>(const DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
	    return DSLang::operator>(lhs,rhs);
	}

	//***************//
	//** Set logic **//
	//***************//
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator&&(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
	    return DSLang::operator&&(lhs,rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator||(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
	    return DSLang::operator||(lhs,rhs);
	}

	//****************//
	//** Arithmetic **//
	//****************//
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator+(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
	    return DSLang::operator+(lhs, rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator+(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
        return DSLang::operator+(lhs, rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator-(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
	    return DSLang::operator-(lhs, rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator-(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
	    return DSLang::operator-(lhs, rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator-(DSLib::Matrix<T, TInt> &rhs) {
	    return DSLang::operator-(rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator/(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
	    return DSLang::operator/(lhs, rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator/(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
	    return DSLang::operator/(lhs, rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator*(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
	    return DSLang::operator*(lhs, rhs);
	}
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator*(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
	    return DSLang::operator*(lhs, rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> pow(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
	    return DSLang::pow(lhs,rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> pow(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
	    return DSLang::pow(lhs,rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> min(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
	    return DSLang::min(lhs,rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> min(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
	    return DSLang::min(lhs,rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> max(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
	    return DSLang::max(lhs,rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> max(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
	    return DSLang::max(lhs,rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator%(DSLib::Matrix<T, TInt> &lhs, const T &rhs) {
	    return DSLang::operator%(lhs,rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator%(DSLib::Matrix<T, TInt> &lhs, const DSLib::Matrix<T, TInt> &rhs) {
	    return DSLang::operator%(lhs,rhs);
	}

	//**************//
	//** Counting **//
	//**************//
	template <typename T, typename TInt> TInt operator~(const DSLib::Matrix<T, TInt> &rhs) {
	    return DSLang::operator~(rhs);
	}

	//***********//
	//** Print **//
	//***********//
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator++(const DSLib::Matrix<T, TInt> &lhs, int i) {
	    return DSLang::operator++(lhs, i);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator++(const DSLib::Matrix<T, TInt> &lhs) {
	    return DSLang::operator++(lhs);
	}
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator--(const DSLib::Matrix<T, TInt> &lhs, int i) {
        return DSLang::operator--(lhs, i);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator--(const DSLib::Matrix<T, TInt> &lhs) {
	    return DSLang::operator--(lhs);
	}

	//**********************//
	//** Matrix streaming **//
	//**********************//
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator>>(DSLib::Matrix<T, TInt> &lhs, std::ostream &rhs) {
	    return DSLang::operator>>(lhs, rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator<<(DSLib::Matrix<T, TInt> &lhs, std::istream &rhs) {
	    return DSLang::operator<<(lhs, rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator>>(DSLib::Matrix<T, TInt> &lhs, const DSTypes::String &rhs) {
	    return DSLang::operator>>(lhs, rhs);
	}
	template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator<<(DSLib::Matrix<T, TInt> &lhs, const DSTypes::String &rhs) {
	    return DSLang::operator<<(lhs, rhs);
	}

	//********************//
	//** Matrix slicing **//
	//********************//
/*	template <typename T, typename TInt> DSLib::Matrix<T, TInt> &operator=(DSLib::Matrix<T, TIdx> &lhs, const DSLib::Matrix<T, TInt> &other) {
        return lhs.operator=(other);
	}
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> &operator=(DSLib::Matrix<T, TIdx> &lhs, const T &val) {
        return lhs.operator=(val);
    }
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()() {
        return lhs.operator()();
    }
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TIdx> &lhs, const TInt row) {
        return lhs.operator()(row);
    }
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TIdx> &lhs, const DSLib::Matrix<TInt, TInt> &rowIndices) {
        return lhs.operator()(rowIndices);
    }
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TIdx> &lhs, const TInt rowBegin, const TInt rowLength) {
        return lhs.operator()(rowBegin, rowLength);
    }
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TIdx> &lhs, const TInt rowBegin, const TInt rowLength, const TInt colBegin, const TInt colLength) {
        return lhs.operator()(rowBegin, rowLength, colBegin, colLength);
    }
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TIdx> &lhs, const DSLib::Matrix<TInt, TInt> &rowIndices, const DSLib::Matrix<TInt, TInt> &colIndices) {
        return lhs.operator()(rowIndices, colIndices);
    }
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator[](const DSLib::Matrix<T, TIdx> &lhs, const TInt col) {
        return lhs.operator[](col);
    }
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator[](const DSLib::Matrix<T, TIdx> &lhs, const DSLib::Matrix<TInt, TInt> &colIndices) {
        return lhs.operator[](colIndices);
    }
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TIdx> &lhs, const DSLib::Matrix<TInt, TInt> &rowIndices, const TInt col) {
        return lhs.operator()(rowIndices, col);
    }
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TIdx> &lhs, const DSLib::Matrix<TInt, TInt> &rowIndices, const TInt colBegin, const TInt colLength) {
        return lhs.operator()(rowIndices, colBegin, colLength);
    }
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TIdx> &lhs, const TInt row, const Matrix<TInt, TInt> &colIndices) {
        return lhs.operator()(row, colIndices);
    }
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TIdx> &lhs, const TInt rowBegin, const TInt rowLength, const DSLib::Matrix<TInt, TInt> &colIndices) {
        return lhs.operator()(rowBegin, rowLength, colIndices);
    }
    template <typename T, typename TInt> DSLib::Matrix<T, TInt> operator()(const DSLib::Matrix<T, TIdx> &lhs, const TInt rowBegin, const TInt rowLength, const TInt col) {
        return lhs.operator()(rowBegin, rowLength, col);
    }*/
}

#ifdef MSVC_VER
	#pragma warning(pop)
#endif
