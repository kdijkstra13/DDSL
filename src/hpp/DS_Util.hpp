#pragma once

#include "h/DS_Util.h"

#ifndef DDSL_NO_EXT_LIB_PNG
	#include "hpp/lib/DS_Image_PNG.hpp"
#endif

#include "h/lib/DS_Table.h"

using namespace std;
using namespace DSTypes;
using namespace DSLib;

namespace DSUtil {
	template<typename T>
	inline void freeAndNull(T** p) {
		if (*p != nullptr)
			delete *p;
		*p = nullptr;
	}

	String inline fileExt(const String &file) {
		size_t dot = file.find_last_of('.');
		return (dot == string::npos)?"":file.substr(dot+1);
	}

	String inline fileBaseName(const String &file) {
		size_t dot = file.find_last_of('.');
		return (dot == string::npos) ? file : file.substr(0, dot);
	}
	
	String inline fileNameWithoutPath(const String &file) {
		if (size_t s = file.find_last_of("\\") != String::npos) {
			return file.substr(file.find_last_of("\\")+2);
		} else if (file.find_last_of("/") != String::npos) {
			return file.substr(file.find_last_of("/")+1);
		} else {
			return file;
		}			
	}

	String inline filePath(const String &file) {
		if (file.find_last_of("\\") != String::npos) {
			return file.substr(0, file.find_last_of("\\")+2);
		} else if (file.find_last_of("/") != String::npos) {
			return file.substr(0, file.find_last_of("/")+1);
		} else {
			return "";
		}			
	}

	String inline fileReplaceExt(const String &file, const String &ext) {
		return fileBaseName(file) + "." + ext;
		
	}

	String inline replaceString(const String & haystack, const String & needle, const String replace) {
		String r = haystack;
		while (r.find(needle) != string::npos)
			r.replace(r.find(needle), needle.size(), replace);
		return r;
	}

	Matrix<String> inline split(const String& str, const char delim, const char quote) {
		bool quoted=false;
		String elm="";
		Matrix<String> r;
		for (string::const_iterator c = str.begin(); c!= str.end(); c++) {
			if (*c == quote) {
				quoted = !quoted;
			} else if (*c == delim) {
				if (quoted)
					elm += *c;
				else {
					r.cols.add(elm);
					elm = "";
				}
			} else
				elm += *c;
		}
		if (elm.size() > 0)
			r.cols.add(elm);
		return r;
	}

	template <typename T>
	size_t inline readToVector(const DSTypes::String &filename, vector<T> &buf) {
		std::ifstream ifs(filename, ios::binary);
		if (!ifs) throw Error(ecNotFound, "readToVector()", SS("Cannot open file: " << filename));
		ifs.seekg(0, std::ios::end);
		size_t size = (size_t)ifs.tellg();
		ifs.seekg(0);
		buf.resize(size / sizeof(T));
		ifs.read(static_cast<char*>(static_cast<void*>(buf.data())), size);
		if (!ifs) throw Error(ecNotFound, "readToVector()", SS("Not all bytes read, only " << ifs.gcount() << " of " << size));
		return buf.size();
	}

	DSTypes::String inline readToString(const DSTypes::String &filename) {
		std::ifstream ifs(filename, ios::binary);
		if (!ifs) 
			throw Error(ecNotFound, "readAsString()", SS("Cannot open file: " << filename));		
		ifs.seekg(0, std::ios::end);
		size_t size = (size_t)ifs.tellg();
		std::string str(size, ' ');
		ifs.seekg(0);
		ifs.read(&str[0], size);
		return str;
	}

	void inline writeFromString(const DSTypes::String &filename, const DSTypes::String &content) {
		std::ofstream ofs(filename, ios::binary);
		ofs << content;
	}
	
	template<typename T>
	inline void write(ostream &s, const T &data) {		
		s.write((const char *)&data, sizeof(data));
	}
	
	template<typename T> 
	inline void read(istream &s, T &data) {
		s.read((char *)&data, sizeof(data));
	}

	template<typename TKey, typename TVal>
	inline void write(ostream &s, const std::map<TKey, TVal> &data) {
		write(s, data.size());
		for (auto it=data.begin(); it != data.end(); it++) {
			write(s, it->first);
			write(s, it->second);
		}
	}

	template<typename TKey, typename TVal>
	inline void write(istream &s, std::map<TKey, TVal> &data) {
		data.clear();
		size_t cnt;
		read(s, cnt);
		for (size_t i=0;i<cnt;i++) {
			TKey key;
			TVal val;
			read(s, key);
			read(s, val);
			data.insert(pair<TKey, TVal>(key, val));
		}
	}

	template<typename T> 
	inline void fmt(std::ostream &s, const T &data) {
		s << data;
	}

	template<> inline void write(ostream &s, const bool &data) { s << data << endl; }
	template<> inline void write(ostream &s, const String &data) { s << data << endl; }
	template<> inline void write(ostream &s, const CString &data) { char *c = (char *)data;if (c != nullptr)s << c;s << endl; }
	template<> inline void write(ostream &s, const DataType &data) { s << etos(data) << endl; }
	template<> inline void write(ostream &s, const ContentType &data) { s << etos(data) << endl; }
	template<> inline void write(ostream &s, const Order &data) { s << etos(data) << endl; }
	template<> inline void write(ostream &s, const PassThroughType &data) { s << etos(data) << endl; }
	template<> inline void write(ostream &s, const ExecType &data) { s << etos(data) << endl; }
	template<> inline void write(ostream &s, const ImageType &data) { s << etos(data) << endl; }
	template<> inline void write(ostream &s, const MatrixInt32 &data) { data.write(s); }
	template<> inline void write(ostream &s, const MatrixUInt32 &data) { data.write(s); }
	template<> inline void write(ostream &s, const MatrixString &data) { data.write(s); }
	template<> inline void write(ostream &s, const MatrixDouble &data) { data.write(s); }
	template<> inline void write(ostream &s, const MatrixFloat &data) { data.write(s); }
	template<> inline void write(ostream &s, const Table<TableIdx, TableId> &data) { data.write(s); }

	template<> inline void read(istream &s, bool &data) { s >> data; }
	template<> inline void read(istream &s, String &data) { getline(s, data); }
	template<> inline void read(istream &s, CString &data) { String str; getline(s, str); strcpy((char *)data, str.c_str()); }
	template<> inline void read(istream &s, DataType &data) { s >> stoe(data); }
	template<> inline void read(istream &s, ContentType &data) { s >> stoe(data); }
	template<> inline void read(istream &s, Order &data) { s >> stoe(data); }
	template<> inline void read(istream &s, PassThroughType &data) { s >> stoe(data); }
	template<> inline void read(istream &s, ExecType &data) { s >> stoe(data); }
	template<> inline void read(istream &s, ImageType &data) { s >> stoe(data); }
	template<> inline void read(istream &s, MatrixInt32 &data) { data.read(s); }
	template<> inline void read(istream &s, MatrixUInt32 &data) { data.read(s); }
	template<> inline void read(istream &s, MatrixString &data) { data.read(s); }
	template<> inline void read(istream &s, MatrixDouble &data) { data.read(s); }
	template<> inline void read(istream &s, MatrixFloat &data) { data.read(s); }
	template<> inline void read(istream &s, Table<TableIdx, TableId> &data) { data.read(s); }

	template<> inline void fmt(std::ostream& str, const Double & data) { double d;modf(data, &d);str << data;str << ((data - d == 0) ? ".0" : "");}
	template<> inline void fmt(std::ostream& str, const Float & data) { double d;modf(data, &d);str << data;str << ((data - d == 0) ? ".0f" : "f");}
	template<> inline void fmt(std::ostream& str, const Int32 & data) {str << data; }
	template<> inline void fmt(std::ostream& str, const UInt32 & data) {str << data << "U"; }
	template<> inline void fmt(std::ostream& str, const ContentType & data) {str << "ContentType(" << data << ")"; }
	template<> inline void fmt(std::ostream& str, const DataType & data) {str << "DataType(" << data << ")"; }
	template<> inline void fmt(std::ostream& str, const String & data) {str << "\"" << data << "\""; }
	template<> inline void fmt(std::ostream& str, const VoidPointer & data) {str << "VoidPointer(\"" << data << "\")";}
	template<> inline void fmt(std::ostream& str, const MatrixDouble & data) { stringstream ss;data.print(ss);str << "(" << replaceString(replaceString(ss.str(), "\n", ""), " ", "") << ")"; }
	template<> inline void fmt(std::ostream& str, const MatrixFloat & data) { stringstream ss;data.print(ss);str << "(" << replaceString(replaceString(ss.str(), "\n", ""), " ", "") << ")"; }
	template<> inline void fmt(std::ostream& str, const MatrixInt32 & data) { stringstream ss;data.print(ss);str << "(" << replaceString(replaceString(ss.str(), "\n", ""), " ", "") << ")"; }
	template<> inline void fmt(std::ostream& str, const MatrixUInt32 & data) { stringstream ss;data.print(ss);str << "(" << replaceString(replaceString(ss.str(), "\n", ""), " ", "") << ")"; }
	template<> inline void fmt(std::ostream& str, const MatrixString & data) { stringstream ss;data.print(ss);str << "(" << replaceString(replaceString(ss.str(), "\n", ""), " ", "") << ")"; }

	#ifndef DDSL_NO_EXT_LIB_PNG
		template<> inline void write(ostream &s, const ImagePNG<Double> &data) {data.write(s); }
		template<> inline void read(istream &s, ImagePNG<Double> &data) {data.read(s);}	
		template<> inline void fmt(std::ostream& str, const ImagePNG<Double> & data) {str << "ImagePNG<Double>(String(\"" << data.getFilename() << "\"))"; }
		template<> inline void write(ostream &s, const ImagePNG<Float> &data) {data.write(s); }
		template<> inline void read(istream &s, ImagePNG<Float> &data) {data.read(s);}	
		template<> inline void fmt(std::ostream& str, const ImagePNG<Float> & data) {str << "ImagePNG<Float>(String(\"" << data.getFilename() << "\"))"; }
	#endif
}
