//#define _SCL_SECURE_NO_WARNINGS
//#define _CRT_SECURE_NO_WARNINGS

#pragma once

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <set>
#include <exception>
#include <sstream>
#include <fstream>
#include <type_traits>
#include <functional>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <memory>
#include <iterator>
#include <random>
#include <future>
#include <functional>
#include <chrono>

#ifdef _MSC_VER
	#include <direct.h>
#else
	#include <sys/stat.h>
#endif

namespace DSTypes {
	//Disabling implicit conversions from Matrix to Scalar and from Table to Matrix is sometimes convenient for debugging.
	//Implicit conversion can clog template deduction error message because almost anything can be converted to anything.
	//#define DS_DISABLE_IMPLICIT_CONVERSIONS	

	#define PI 3.14159265358979323846
	#define E 2.7182818284590452353

#ifdef _MSC_VER
	#define OS_PATH_SEP "\\"
	#define CHDIR(dir) _chdir(dir)
	#define MKDIR(dir) _mkdir(dir)
	#define RMDIR(dir) _rmdir(dir)
#else
	#define OS_PATH_SEP "/"
	#define CHDIR(dir) chdir(dir)
	#define MKDIR(dir) mkdir(dir, EEXIST)
	#define RMDIR(dir) rmdir(dir)
#endif

	typedef float Float;
	typedef double Double;
	typedef signed char Int8;
	typedef signed short Int16;
	typedef signed int Int32;
	typedef signed long long int Int64;

	typedef unsigned char UInt8;
	typedef unsigned short UInt16;
	typedef unsigned int UInt32;
	typedef unsigned long long int UInt64;

	typedef std::string String;
	typedef const char * CString;

	//DDSL wide default dataTypes
	typedef UInt64 UInt; //max platform type (x32, x64, etc)
	typedef String TableId;
	typedef UInt32 TableIdx;
	typedef UInt32 MatrixIdx;
	typedef String Literal;

	//Special void * wrappers for storing non DSL types in Matrix<>, while retaining type strictness and there is a posibility for arithmetic using overloaded operators
	class VoidPointer {
	private:
		void *p = nullptr;
	public:
		virtual void read(std::istream &is) {};
		virtual void write(std::ostream &os) const {};

		void * get() const { return p; }
		void set(void * const p) { this->p = p; }
		
		template<typename T>
		T & get() const {return *(dynamic_cast<T*>(p));}
		template<typename T>
		void set(T &p) const { p = static_cast<void *>(&p);}

		virtual bool operator==(const VoidPointer &other) const { return p == other.p; };
		virtual bool operator!=(const VoidPointer &other) const { return p != other.p; };
		virtual bool operator>(const VoidPointer &other) const {return false; };
		virtual bool operator<(const VoidPointer &other) const { return false; };
		virtual bool operator>=(const VoidPointer &other) const { return false; };
		virtual bool operator<=(const VoidPointer &other) const { return false; };
		virtual void print(std::ostream &output) const { output << (unsigned int *)p; };


		VoidPointer() {};
		~VoidPointer() {};
		VoidPointer(void * p) { this->p = p; };

		VoidPointer(const VoidPointer &other) { clone(other); };
		VoidPointer(VoidPointer &&other) { clone(std::move(other)); };
		VoidPointer & operator=(const VoidPointer &other) { clone(other); return *this;};
		VoidPointer & operator=(VoidPointer &&other) { clone(std::move(other)); return *this;};

		void clone(const VoidPointer &other) { p = other.p; };
		void clone(VoidPointer &&other) { p = other.p;other.p = nullptr; };
	};
	std::ostream& operator<<(std::ostream &os, const VoidPointer &vp);

	//Wrapper class for caffe::Blob<Double>
	class CaffeBlobDouble : public VoidPointer {
	public:
		CaffeBlobDouble() {};
		~CaffeBlobDouble() {};
		CaffeBlobDouble(void * p) : VoidPointer(p) {};
		
		CaffeBlobDouble(const CaffeBlobDouble &other) { clone(other); };
		CaffeBlobDouble(CaffeBlobDouble &&other) { clone(std::move(other)); };
		CaffeBlobDouble(const VoidPointer &other) { clone(other); };
		CaffeBlobDouble(VoidPointer &&other) { clone(std::move(other)); };
		CaffeBlobDouble & operator=(const CaffeBlobDouble &other) { clone(other); return *this;};
		CaffeBlobDouble & operator=(CaffeBlobDouble &&other) { clone(std::move(other)); return *this;};

		void clone(const CaffeBlobDouble &other) { VoidPointer::clone(other); };
		void clone(CaffeBlobDouble &&other) { VoidPointer::clone(std::move(other)); };
	};

	enum ContentType {ctUnknown, ctFeature, ctTarget, ctResult, ctSplit, ctConfusion, ctIndex, ctParameter, ctConfidence, ctImage, ctError, ctCorrelation};
	enum DataType { dtUnknown, dtDouble, dtFloat, dtUInt8, dtUInt16, dtUInt32, dtUInt64, dtInt8, dtInt16, dtInt32, dtInt64, dtString, dtCString, dtContentType, dtDataType, dtMatrixDouble, dtMatrixInt32, dtMatrixUInt32, dtMatrixString, dtVoidPointer, dtCaffeBlobDouble, 
		#ifndef DDSL_NO_EXT_LIB_PNG
			dtImagePNGDouble, dtImagePNGFloat, 
		#endif		
		dtMatrixFloat};
	enum PassThroughType {pttAll, pttRelevant, pttNone};
	enum ExecType { etSynchronous, etAsynchronous};
	enum Order {oRowMajor, oColMajor};
	enum ErrorCode {ecGeneral, ecNotImplemented, ecInvalidEnum, ecSizeMismatch, ecRangeError, ecIncompatible, ecNotFound, ecUnexpected, ecInternal, ecParameter, ecUnsupported, ecExternalLibrary};
	enum CellsIteratorType {citRegion, citContinuous};
	enum ImageType {itM8, itMA8, itRGB8, itRGBA8, itM16, itMA16, itRGB16, itRGBA16, itDouble, itRGB8Planar, itRGB16Planar, itRGB8PlanarV, itRGB16PlanarV};

	template <typename T> bool dataTypeEqual(const DataType dt, const T &tp); //Check if variable's type is is equal to DataType
	String dataTypeToString(const DataType &dt); //From DataType to typeid().name
	DataType stringToDataType(const String &s); //From string to datatype
	ContentType stringToContentType(const String &s);

	std::size_t dataTypeHash(const DataType &dt); //From DataType to Hash

	template <typename T> String typeName(const T &type); //From variable to friendly String	
	template <typename T> DataType dataType(const T &type); //From variable to DataType
	template <typename T> DataType dataType(); //From variable to DataType via template param

	//Header code for streaming enums
	template<typename T> struct enumRef {
		T& value;
		enumRef(T& value): value(value) {};
	};

	template<typename T> struct enumConstRef {
		T const& value;
		enumConstRef(T const& value): value(value) {};
	};

	template<typename T> enumRef<T> stoe(T& e) {
		return enumRef<T>(e);
	}

	template<typename T> enumConstRef<T> etos(T const& e) {
		return enumConstRef<T>(e);
	}

	template<typename T> std::ostream& operator<<(std::ostream& str, enumConstRef<T> const& data);	
	template<typename T> std::istream& operator>>(std::istream& str, enumRef<T> & data);
	template<typename T> std::istream& operator>>(std::istream& str, enumRef<T> && data);

	//Macro for inline streaming of values to a string
	#define SS(VALUES) \
		static_cast<std::ostringstream&&>(std::ostringstream() << VALUES).str()

	//Exception handling
	class Error: public std::exception {
	public:
		Error (const ErrorCode code, std::string location, std::string message, const bool fatal=true);
		virtual const char* what() const throw();
	private:
		std::string loc_;
		std::string msg_;
		const bool ftl_;
		const ErrorCode code_;
	};

};

