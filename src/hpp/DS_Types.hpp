#pragma once

#include "h/DS_Types.h"
#include "hpp/lib/DS_Matrix.hpp"

#ifndef DDSL_NO_EXT_LIB_PNG
  #include "h/lib/DS_Image_PNG.h"
#endif

using namespace std;

namespace DSTypes {
	namespace {
		const char * dataTypeName_[] = {
			"Unknown",
			typeid(Double).name(),
			typeid(Float).name(),
			typeid(UInt8).name(),
			typeid(UInt16).name(),
			typeid(UInt32).name(),
			typeid(UInt64).name(),
			typeid(Int8).name(),
			typeid(Int16).name(),
			typeid(Int32).name(),
			typeid(Int64).name(),
			typeid(String).name(),
			typeid(CString).name(),
			typeid(ContentType).name(),
			typeid(DataType).name(),
			typeid(MatrixFloat).name(),
			typeid(MatrixDouble).name(),
			typeid(MatrixInt32).name(),
			typeid(MatrixUInt32).name(),
			typeid(MatrixString).name(),
			typeid(VoidPointer).name(),
			typeid(CaffeBlobDouble).name(),
			#ifndef DDSL_NO_EXT_LIB_PNG
				typeid(ImagePNG<Double>).name(),
				typeid(ImagePNG<Float>).name(),
			#endif
		};
		size_t dataTypeHash_[] = {
			0,
			typeid(Double).hash_code(),
			typeid(Float).hash_code(),
			typeid(UInt8).hash_code(),
			typeid(UInt16).hash_code(),
			typeid(UInt32).hash_code(),
			typeid(UInt64).hash_code(),
			typeid(Int8).hash_code(),
			typeid(Int16).hash_code(),
			typeid(Int32).hash_code(),
			typeid(Int64).hash_code(),
			typeid(String).hash_code(),
			typeid(CString).hash_code(),
			typeid(ContentType).hash_code(),
			typeid(DataType).hash_code(),
			typeid(MatrixFloat).hash_code(),
			typeid(MatrixDouble).hash_code(),
			typeid(MatrixInt32).hash_code(),
			typeid(MatrixUInt32).hash_code(),
			typeid(MatrixString).hash_code(),
			typeid(VoidPointer).hash_code(),
			typeid(CaffeBlobDouble).hash_code(),
			#ifndef DDSL_NO_EXT_LIB_PNG
				typeid(ImagePNG<Double>).hash_code(),
				typeid(ImagePNG<Float>).hash_code()
			#endif
		};
	}

	inline Error::Error (const ErrorCode code, std::string location, std::string message, const bool fatal) : loc_(location), msg_(message), ftl_(fatal), code_(code) {}

	//Error handling
    inline const char* Error::what() const noexcept {
		std::stringstream ss;
		ss << "[" << loc_ << "," << etos(code_) << (ftl_?"":",F") << "] " << msg_;
		int size = (int)ss.str().size();
		char * str = (char*)malloc(size + 1);
		strcpy(str, ss.str().c_str());
		return str;
	};

	//Check if a variable is of the same DataType
	template <typename T> inline bool dataTypeEqual(const DataType dt, const T &tp) {
		return (dataTypeHash_[dt] == typeid(T).hash_code());
	}

	//Get the name of a DataType
	inline String dataTypeToString(const DataType &dt) {
		return dataTypeName_[dt];
	}

	//Get the hash of a DataType
	inline size_t dataTypeHash(const DataType &dt) {
		return dataTypeHash_[dt];
	}

	namespace {
        template<typename T> struct enumStrings {static char const* data[];};
        template<> char const* enumStrings<ContentType>::data[] = { "Unknown", "Feature", "Target", "Result", "Split", "Confusion", "Index", "Parameter", "Confidence", "Image", "Error", "Correlation"};
        template<> char const* enumStrings<DataType>::data[] = { "Unknown", "Double", "Float", "UInt8", "UInt16", "UInt32", "UInt64", "Int8", "Int16", "Int32", "Int64", "String", "CString", "ContentType", "DataType", "MatrixFloat", "MatrixDouble", "MatrixInt32", "MatrixUInt32", "MatrixString", "VoidPointer", "CaffeBlobDouble", "ImagePNGDouble", "ImagePNGFloat"};
        template<> char const* enumStrings<PassThroughType>::data[] = {"All", "Relevant", "None"};
        template<> char const* enumStrings<ExecType>::data[] = { "Synchronous", "Asynchronous"};
        template<> char const* enumStrings<Order>::data[] = {"RowMajor", "ColMajor"};
        template<> char const* enumStrings<ErrorCode>::data[] = {"General", "NotImplemented", "InvalidEnum", "SizeMismatch", "RangeError", "Incompatible", "NotFound", "Unexpected", "Internal", "Parameter", "Unsupported", "ExternalLibrary"};
        template<> char const* enumStrings<CellsIteratorType>::data[] = {"Region", "Continuous"};
        template<> char const* enumStrings<ImageType>::data[] = {"M8", "MA8", "RGB8", "RGBA8", "M16", "MA16", "RGB16", "RGBA16", "itDouble", "RGB8Planar", "RGB16Planar", "RGB8PlanarV", "RGB16PlanarV"};
	}
	//Stream a VoidPointer
	inline std::ostream& operator<<(std::ostream &os, const VoidPointer &vp) { return os << "(void*)" << (UInt)vp.get(); };

	//Find the DataType of a variable
	template <typename T> inline DataType dataType(const T &type) {
		const int len = sizeof(enumStrings<DataType>::data) / sizeof(char*);
		for (int i=0;i<len;i++)
			if (dataTypeEqual(DataType(i), type))
				return DataType(i);
		return dtUnknown;
	}

	//Find the DataType of a template param
	template <typename T> inline DataType dataType() {
		T t;
		return dataType(t);
	}

	//Find the user friendly name of the DataType of a variable
	template <typename T> inline String typeName(const T &type) {
		size_t * f = find(std::begin(dataTypeHash_), std::end(dataTypeHash_), typeid(T).hash_code());
		if (f != std::end(dataTypeHash_)) {
			return enumStrings<DataType>::data[distance(std::begin(dataTypeHash_), f)];
		}
		return typeid(T).name();
	}

	//Find the user friendly name of the DataType of a variable
	template <typename T> inline String typeName() {
	    T t;
	    return typeName(t);
	}

	inline ContentType stringToContentType(const String &s) {
		auto begin = std::begin(enumStrings<ContentType>::data);
		auto end = std::end(enumStrings<ContentType>::data);
		auto find = std::find(begin, end, s);
		if (find != end)
			return static_cast<ContentType>(distance(begin, find));
		return ctUnknown;
	}

	inline DataType stringToDataType(const String &s) {
		auto begin = std::begin(enumStrings<DataType>::data);
		auto end = std::end(enumStrings<DataType>::data);
		auto find = std::find(begin, end, s);
		if (find != end)
			return static_cast<DataType>(distance(begin, find));
		return dtUnknown;
	}

	//Stream an Enum
	template<typename T> inline std::ostream& operator<<(std::ostream& str, enumConstRef<T> const& data) {
	   return str << enumStrings<T>::data[data.value];
	}

	template<typename T>
	inline std::istream& operator>>(std::istream& str, enumRef<T> & data) {
		std::string value;
		getline(str, value);
		auto begin = std::begin(enumStrings<T>::data);
		auto end = std::end(enumStrings<T>::data);
		auto find = std::find(begin, std::end(enumStrings<T>::data), value);
		if (find != end)
			data.value = static_cast<T>(distance(begin, find));
		else
			data.value = static_cast<T>(0);
		return str;
	};

	template<typename T>
	inline std::istream& operator >> (std::istream& str, enumRef<T> && data) {
		return operator>>(str, data);
	};
};
