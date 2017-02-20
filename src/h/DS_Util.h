#pragma once

#include "h/DS_Types.h"
#include "h/lib/DS_Matrix.h"

namespace DSUtil {

	template<typename T>
	void freeAndNull(T** p);

	DSTypes::String replaceString(const DSTypes::String &haystack, const DSTypes::String &needle, const DSTypes::String replace);

	DSLib::Matrix<DSTypes::String> split(const DSTypes::String& str, const char delim, const char quote);

	DSTypes::String readToString(const DSTypes::String &filename);

	template<typename T>
	void write(std::ostream &s, const T &data);
	
	template<typename T>
	void read(std::istream &s, T &data);

	template<typename T>
	void fmt(std::ostream &s, const T &data);
}
