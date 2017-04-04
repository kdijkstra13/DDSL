// This file is distributed under the BSD License.
// See "license.txt" for details.
// Copyright 2009-2012, Jonathan Turner (jonathan@emptycrate.com)
// Copyright 2009-2017, Jason Turner (jason@emptycrate.com)
// http://www.chaiscript.com

#include <iostream>
#include <list>
#include <regex>

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "ddsl.hpp"
#include "chaiscript/chaiscript.hpp"

#include <chaiscript/chaiscript_basic.hpp>
#include <chaiscript/language/chaiscript_parser.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

using namespace std;
using namespace DSLib;
using namespace DSTypes;
using namespace DSFunc;
using namespace DSLang;
using namespace DSModel;

//Add a type
#define CHAI_TYPE(CHAI, CLASS_TYPE, TYPE_NAME) CHAI.add(chaiscript::user_type<CLASS_TYPE>(), TYPE_NAME);

//Add a enum and enum values
#define CHAI_ENUM(CHAI, ENUM_TYPE, ENUM_NAME) CHAI.add(chaiscript::user_type<ENUM_TYPE>(), ENUM_NAME);
#define CHAI_ENUM_VALUE(CHAI, ENUM_VALUE, PREFIX) CHAI.add_global_const(chaiscript::const_var(ENUM_VALUE), SS(PREFIX << etos(ENUM_VALUE)));

//Add constructor
#define CHAI_CONSTRUCTOR(CHAI, CLASS_TYPE, TYPE_NAME, ...) CHAI.add(chaiscript::constructor<CLASS_TYPE(__VA_ARGS__)>(), TYPE_NAME);

//Add member functions
#define CHAI_MEMBER(CHAI, FUN_NAME, RETURN_TYPE, CLASS_TYPE, ...) CHAI.add(chaiscript::fun(static_cast<RETURN_TYPE(CLASS_TYPE::*)(__VA_ARGS__)>(&CLASS_TYPE::FUN_NAME)), #FUN_NAME);
#define CHAI_MEMBER_CONST(CHAI, FUN_NAME, RETURN_TYPE, CLASS_TYPE, ...) CHAI.add(chaiscript::fun(static_cast<RETURN_TYPE(CLASS_TYPE::*)(__VA_ARGS__)const>(&CLASS_TYPE::FUN_NAME)), #FUN_NAME);

//Add member operator
#define CHAI_MEMBER_OPER(CHAI, OP_NAME, RETURN_TYPE, CLASS_TYPE, ...) CHAI.add(chaiscript::fun(static_cast<RETURN_TYPE(CLASS_TYPE::*)(__VA_ARGS__)>(&CLASS_TYPE::operator OP_NAME)), #OP_NAME);
#define CHAI_MEMBER_OPER_CONST(CHAI, OP_NAME, RETURN_TYPE, CLASS_TYPE, ...) CHAI.add(chaiscript::fun(static_cast<RETURN_TYPE(CLASS_TYPE::*)(__VA_ARGS__)const>(&CLASS_TYPE::operator OP_NAME)), #OP_NAME);

//Add external operator
#define CHAI_OPER(CHAI, NS, OP_NAME, RETURN_TYPE, ...) CHAI.add(chaiscript::fun(static_cast<RETURN_TYPE(*)(__VA_ARGS__)>(NS::operator OP_NAME)), #OP_NAME);

//Add type conversion
#define CHAI_CONVERSION(CHAI, TYPE_FROM, TYPE_TO) CHAI.add(chaiscript::type_conversion<TYPE_FROM, TYPE_TO>());


//Special function to add all enums of a type
template<typename T>
void bindEnum(chaiscript::ChaiScript_Basic &chai, const String &name, const String &prefix) {
    CHAI_ENUM(chai, T, name);
	const int len = sizeof(enumStrings<T>::data) / sizeof(char*);
	for (int i=0;i<len;i++)
        CHAI_ENUM_VALUE(chai, (T)i, prefix);
}

//Add all types
void bindTypes(chaiscript::ChaiScript_Basic &chai) {
    bindEnum<DataType>(chai, "DataType", "dt");
    bindEnum<ContentType>(chai, "ContentType", "ct");
	bindEnum<PassThroughType>(chai, "PassThroughType", "ptt");
	bindEnum<ExecType>(chai, "ExecType", "et");
	bindEnum<Order>(chai, "Order", "o");
	bindEnum<ErrorCode>(chai, "ErrorCode", "ec");
	bindEnum<CellsIteratorType>(chai, "CellsIteratorType", "cit");
	bindEnum<ImageType>(chai, "ImageType", "it");

//These interfere with chai's build in types
 /* CHAI_TYPE(chai, DSTypes::UInt8, "UInt8");
    CHAI_TYPE(chai, DSTypes::UInt16, "UInt16");
    CHAI_TYPE(chai, DSTypes::UInt32, "UInt32");
    CHAI_TYPE(chai, DSTypes::UInt64, "UInt64");
    CHAI_TYPE(chai, DSTypes::Int8, "Int8");
    CHAI_TYPE(chai, DSTypes::Int16, "Int16");
    CHAI_TYPE(chai, DSTypes::Int32, "Int32");
    CHAI_TYPE(chai, DSTypes::Int64, "Int64");
    CHAI_TYPE(chai, DSTypes::Float, "Float");
    CHAI_TYPE(chai, DSTypes::Double, "Double");
    CHAI_TYPE(chai, DSTypes::String, "String");*/
}

//Add library for Matrix
template <typename T, typename TIdx>
void bindMatrixLibBasic(chaiscript::ChaiScript_Basic &chai) {

    //Typedefs
    typedef Matrix<T, TIdx> TMat;
    typedef Matrix<TIdx, TIdx> TMatIdx;
    String name = SS("Matrix" << etos(dataType<T>()));

    //Add type and constructors
    CHAI_TYPE(chai, TMat, name);
    CHAI_CONSTRUCTOR(chai, TMat, name)
    CHAI_CONSTRUCTOR(chai, TMat, name, const TMat&);

    //Add conversions
    CHAI_CONVERSION(chai, TMat, bool);
    CHAI_CONVERSION(chai, TMat, T);

    //Chai interop for converting to a string
    CHAI_MEMBER_CONST(chai, to_string, DSTypes::String, TMat);

    //Add class members
    CHAI_MEMBER_CONST(chai, print, DSTypes::String, TMat);
    CHAI_MEMBER_CONST(chai, print, void, TMat, std::ostream &);
}

//Add functions for Matrix
template <typename T, typename TIdx>
void bindMatrixFuncBasic(chaiscript::ChaiScript_Basic &chai) {

}

//Add language for Matrix
template <typename T, typename TIdx>
void bindMatrixLangBasic(chaiscript::ChaiScript_Basic &chai) {
    //Typedefs
    typedef Matrix<T, TIdx> TMat;
    typedef Matrix<TIdx, TIdx> TMatIdx;

    //Printing
    CHAI_OPER(chai, DSScript, ++, TMat, const TMat&);
    CHAI_OPER(chai, DSScript, --, TMat, const TMat&);

    //Static init
    CHAI_OPER(chai, DSScript, |, TMat, DSTypes::DataType, const T&);
    CHAI_OPER(chai, DSScript, ^, TMat, DSTypes::DataType, const T&);

    //Assignment
    CHAI_OPER(chai, DSScript, |=, TMat, TMat&, const TMat&);
    CHAI_OPER(chai, DSScript, ^=, TMat, TMat&, const TMat&);
    CHAI_OPER(chai, DSScript, +=, TMat, TMat&, const TMat&);

    //Breakup
    CHAI_OPER(chai, DSScript, !, TMat, TMat&);
    CHAI_OPER(chai, DSScript, *, TMat, const TMat&);

    //Concat
    CHAI_OPER(chai, DSScript, |, TMat, TMat&, const T&);
    CHAI_OPER(chai, DSScript, |, TMat, TMat&, const TMat&);
    CHAI_OPER(chai, DSScript, ^, TMat, TMat&, const T&);
    CHAI_OPER(chai, DSScript, ^, TMat, TMat&, const TMat&);

    //Comparing
    CHAI_OPER(chai, DSScript, ==, TMatIdx, const TMat&, const TMat&);
    CHAI_OPER(chai, DSScript, ==, TMatIdx, const TMat&, const T&);
    CHAI_OPER(chai, DSScript, !=, TMatIdx, const TMat&, const TMat&);
    CHAI_OPER(chai, DSScript, !=, TMatIdx, const TMat&, const T&);

    //Members
    CHAI_MEMBER_OPER(chai, =, TMat&, TMat, const T&);
    CHAI_MEMBER_OPER(chai, =, TMat&, TMat, const TMat&);
    CHAI_MEMBER_OPER(chai, (), TMat, TMat);
    CHAI_MEMBER_OPER(chai, (), TMat, TMat, const TIdx);
    CHAI_MEMBER_OPER(chai, (), TMat, TMat, const TMatIdx&);
    CHAI_MEMBER_OPER(chai, (), TMat, TMat, const TIdx, const TIdx);
    CHAI_MEMBER_OPER(chai, (), TMat, TMat, const TIdx, const TIdx, const TIdx, const TIdx);
    CHAI_MEMBER_OPER(chai, (), TMat, TMat, const TMatIdx &, const TMatIdx &);
    CHAI_MEMBER_OPER(chai, (), TMat, TMat, const TMatIdx &, const TIdx);
    CHAI_MEMBER_OPER(chai, (), TMat, TMat, const TMatIdx &, const TIdx, const TIdx);
    CHAI_MEMBER_OPER(chai, (), TMat, TMat, const TIdx, const TMatIdx &);
    CHAI_MEMBER_OPER(chai, (), TMat, TMat, const TIdx, const TIdx, const TMatIdx &);
    CHAI_MEMBER_OPER(chai, (), TMat, TMat, const TIdx, const TIdx, const TIdx);
    CHAI_MEMBER_OPER(chai, [], TMat, TMat, const TIdx);
    CHAI_MEMBER_OPER(chai, [], TMat, TMat, const TMatIdx &);

}

//Add numerical operators for Matrix
template <typename T, typename TIdx>
void bindMatrixLangNumeric(chaiscript::ChaiScript_Basic &chai) {
    //Typedefs
    typedef Matrix<T, TIdx> TMat;
    typedef Matrix<TIdx, TIdx> TMatIdx;

    //Add operators
    CHAI_OPER(chai, DSScript, *=, TMat, TMat&, const TMat&);
    CHAI_OPER(chai, DSScript, >,  TMatIdx, const TMat&, const TMat&);
    CHAI_OPER(chai, DSScript, >,  TMatIdx, const TMat&, const T&);
    CHAI_OPER(chai, DSScript, <,  TMatIdx, const TMat&, const TMat&);
    CHAI_OPER(chai, DSScript, <,  TMatIdx, const TMat&, const T&);
    CHAI_OPER(chai, DSScript, >=, TMatIdx, const TMat&, const TMat&);
    CHAI_OPER(chai, DSScript, >=, TMatIdx, const TMat&, const T&);
    CHAI_OPER(chai, DSScript, <=, TMatIdx, const TMat&, const TMat&);
    CHAI_OPER(chai, DSScript, <=, TMatIdx, const TMat&, const T&);
}

//Add basic operators for Matrix
template <typename T, typename TIdx>
void bindMatrixBasic(chaiscript::ChaiScript_Basic &chai) {
    bindMatrixLibBasic<T, TIdx>(chai);
    bindMatrixLangBasic<T, TIdx>(chai);
    bindMatrixFuncBasic<T, TIdx>(chai);
}

template <typename T, typename TIdx>
void bindMatrixNumeric(chaiscript::ChaiScript_Basic &chai) {
    //bindMatrixLibNumeric<T, TIdx>(chai);
    bindMatrixLangNumeric<T, TIdx>(chai);
    //bindMatrixFuncNumeric<T, TIdx>(chai);
}

void bindDDSL(chaiscript::ChaiScript_Basic &chai) {
    bindTypes(chai);

    //Typedefs
    typedef UInt32 TIdx;

    //Bind basic operators for basic types
    bindMatrixBasic<UInt8, TIdx>(chai);
    bindMatrixBasic<UInt16, TIdx>(chai);
    bindMatrixBasic<UInt32, TIdx>(chai);
    bindMatrixBasic<UInt64, TIdx>(chai);
    bindMatrixBasic<Int8, TIdx>(chai);
    bindMatrixBasic<Int16, TIdx>(chai);
    bindMatrixBasic<Int32, TIdx>(chai);
    bindMatrixBasic<Int64, TIdx>(chai);
    bindMatrixBasic<Float, TIdx>(chai);
    bindMatrixBasic<Double, TIdx>(chai);
    bindMatrixBasic<String, TIdx>(chai);
    bindMatrixBasic<DataType, TIdx>(chai);
    bindMatrixBasic<ContentType, TIdx>(chai);

    //Bind basic operators for Matrix of Matrix types
    bindMatrixBasic<Matrix<UInt32, TIdx>, TIdx>(chai);
    bindMatrixBasic<Matrix<Int32, TIdx>, TIdx>(chai);
    bindMatrixBasic<Matrix<Float, TIdx>, TIdx>(chai);
    bindMatrixBasic<Matrix<Double, TIdx>, TIdx>(chai);
    bindMatrixBasic<Matrix<String, TIdx>, TIdx>(chai);

    //Bind numeric operators
    bindMatrixNumeric<UInt8, TIdx>(chai);
    bindMatrixNumeric<UInt16, TIdx>(chai);
    bindMatrixNumeric<UInt32, TIdx>(chai);
    bindMatrixNumeric<UInt64, TIdx>(chai);
    bindMatrixNumeric<Int8, TIdx>(chai);
    bindMatrixNumeric<Int16, TIdx>(chai);
    bindMatrixNumeric<Int32, TIdx>(chai);
    bindMatrixNumeric<Int64, TIdx>(chai);
    bindMatrixNumeric<Float, TIdx>(chai);
    bindMatrixNumeric<Double, TIdx>(chai);

}

#ifdef READLINE_AVAILABLE
#include <readline/readline.h>
#include <readline/history.h>
#else

char *mystrdup (const char *s) {
  size_t len = strlen(s); // Space for length plus nul
  char *d = static_cast<char*>(malloc (len+1));
  if (d == nullptr) { return nullptr; }         // No memory
#ifdef CHAISCRIPT_MSVC
  strcpy_s(d, len+1, s);                        // Copy the characters
#else
  strncpy(d,s,len);                        // Copy the characters
#endif
  d[len] = '\0';
  return d;                            // Return the new string
}

char* readline(const char* p)
{
  std::string retval;
  std::cout << p ;
  std::getline(std::cin, retval);
  return std::cin.eof() ? nullptr : mystrdup(retval.c_str());
}

void add_history(const char* /*unused*/){}
void using_history(){}
#endif

void *cast_module_symbol(std::vector<std::string> (*t_path)()) {
  union cast_union {
    std::vector<std::string> (*in_ptr)();
    void *out_ptr;
  };

  cast_union c;
  c.in_ptr = t_path;
  return c.out_ptr;
}

std::vector<std::string> default_search_paths() {
  std::vector<std::string> paths;

#ifdef CHAISCRIPT_WINDOWS  // force no unicode
  CHAR path[4096];
  int size = GetModuleFileNameA(nullptr, path, sizeof(path)-1);

  std::string exepath(path, size);

  size_t lastslash = exepath.rfind('\\');
  size_t secondtolastslash = exepath.rfind('\\', lastslash - 1);
  if (lastslash != std::string::npos) {
    paths.push_back(exepath.substr(0, lastslash));
  }

  if (secondtolastslash != std::string::npos) {
    return {exepath.substr(0, secondtolastslash) + "\\lib\\chaiscript\\"};
  }
#else
  std::string exepath;

  std::vector<char> buf(2048);
  ssize_t size = -1;

  if ((size = readlink("/proc/self/exe", &buf.front(), buf.size())) >= 0) {
    exepath = std::string(&buf.front(), static_cast<size_t>(size));
  }
  if (exepath.empty()) {
    if ((size = readlink("/proc/curproc/file", &buf.front(), buf.size())) >= 0) {
      exepath = std::string(&buf.front(), static_cast<size_t>(size));
    }
  }
  if (exepath.empty()) {
    if ((size = readlink("/proc/self/path/a.out", &buf.front(), buf.size())) >= 0) {
      exepath = std::string(&buf.front(), static_cast<size_t>(size));
    }
  }

  if (exepath.empty()) {
    Dl_info rInfo;
    memset( &rInfo, 0, sizeof(rInfo) );
    if ( dladdr(cast_module_symbol(&default_search_paths), &rInfo) == 0 || rInfo.dli_fname == nullptr)
      return paths;
    exepath = std::string(rInfo.dli_fname);
  }

  size_t lastslash = exepath.rfind('/');

  size_t secondtolastslash = exepath.rfind('/', lastslash - 1);
  if (lastslash != std::string::npos) {
    paths.push_back(exepath.substr(0, lastslash+1));
  }
  if (secondtolastslash != std::string::npos) {
    paths.push_back(exepath.substr(0, secondtolastslash) + "/lib/chaiscript/");
  }
#endif

  return paths;
}

void help(int n) {
  if ( n >= 0 ) {
    std::cout << "DDSL evaluator powered by ChaiScript. To evaluate an expression, type it and press <enter>." << endl;
    std::cout << "Type: quit or exit to quit DDSL" << endl;
    std::cout << "      version() for version information" << endl;
    std::cout << "      help for this message" << endl;
    std::cout << "Additionally, you can inspect the runtime system using:" << endl;
    std::cout << "  dump_system() - outputs all functions registered to the system" << endl;
    std::cout << "  dump_object(x) - dumps information about the given symbol" << endl;
  } else {
    std::cout << "usage : ddsl [option]+\n";
    std::cout << "option:"                << '\n';
    std::cout << "   -h | --help"         << '\n';
    std::cout << "   -i | --interactive"  << '\n';
    std::cout << "   -c | --command cmd"  << '\n';
    std::cout << "   -v | --version"      << '\n';
    std::cout << "   -    --stdin"        << '\n';
    std::cout << "   filepath"            << '\n';
  }
}

bool throws_exception(const std::function<void ()> &f) {
  try {
    f();
  } catch (...) {
    return true;
  }
  return false;
}

chaiscript::exception::eval_error get_eval_error(const std::function<void ()> &f) {
  try {
    f();
  } catch (const chaiscript::exception::eval_error &e) {
    return e;
  }
  throw std::runtime_error("no exception throw");
}

std::string get_next_command() {
  std::string retval("quit");
  if ( ! std::cin.eof() ) {
    char *input_raw = readline("DDSL> ");
    if ( input_raw != nullptr ) {
      add_history(input_raw);

      std::string val(input_raw);
      size_t pos = val.find_first_not_of("\t \n");
      if (pos != std::string::npos) {
        val.erase(0, pos);
      }
      pos = val.find_last_not_of("\t \n");
      if (pos != std::string::npos) {
        val.erase(pos+1, std::string::npos);
      }
      retval = val;
      ::free(input_raw);
    }
  }
  if(retval == "quit" || retval == "exit" || retval == "help" || retval == "version")
    retval += "(0)";
  return retval;
}

// We have to wrap exit with our own because Clang has a hard time with
// function pointers to functions with special attributes (system exit being marked NORETURN)
void myexit(int return_val) {
  exit(return_val);
}

void interactive(chaiscript::ChaiScript_Basic& chai)
{
  using_history();
  for (;;) {
    std::string input = get_next_command();
    try {
      // evaluate input
      chaiscript::Boxed_Value val = chai.eval(input);

      //Then, we try to print the result of the evaluation to the user
      if (!val.get_type_info().bare_equal(chaiscript::user_type<void>())) {
        try {
          std::cout << chai.eval<std::function<std::string (const chaiscript::Boxed_Value &bv)> >("to_string")(val) << '\n';
        } catch (...) {} //If we can't, do nothing
      }
    } catch (const chaiscript::exception::eval_error &ee) {
      std::cout << ee.what();
      if (!ee.call_stack.empty()) {
        std::cout << "during evaluation at (" << ee.call_stack[0]->start().line << ", " << ee.call_stack[0]->start().column << ")";
      }
      std::cout << '\n';
    } catch (const std::exception &e) {
      std::cout << e.what();
      std::cout << '\n';
    }
  }
}

double now() {
  using namespace std::chrono;
  auto now = high_resolution_clock::now();
  return duration_cast<duration<double>>(now.time_since_epoch()).count();
}

std::unique_ptr<chaiscript::parser::ChaiScript_Parser_Base> create_chaiscript_parser() {
  return std::make_unique<chaiscript::parser::ChaiScript_Parser<chaiscript::eval::Noop_Tracer, chaiscript::optimizer::Optimizer_Default>>();
}

std::shared_ptr<chaiscript::Module> create_chaiscript_stdlib(){
  return chaiscript::Std_Lib::library();
}

int main(int argc, char *argv[]) {

  // Disable deprecation warning for getenv call.
#ifdef CHAISCRIPT_MSVC
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

  const char *usepath = getenv("CHAI_USE_PATH");
  const char *modulepath = getenv("CHAI_MODULE_PATH");

#ifdef CHAISCRIPT_MSVC
#pragma warning(pop)
#endif

  std::vector<std::string> usepaths;
  usepaths.push_back(""); //Add your paths here <--
  if (usepath != nullptr)
    usepaths.push_back(usepath);

  std::vector<std::string> modulepaths;
  std::vector<std::string> searchpaths = default_search_paths();
  modulepaths.insert(modulepaths.end(), searchpaths.begin(), searchpaths.end());
  modulepaths.push_back(""); //Add your paths here <--
  if (modulepath != nullptr)
    modulepaths.push_back(modulepath);

  chaiscript::ChaiScript_Basic chai(create_chaiscript_stdlib(), create_chaiscript_parser(), modulepaths, usepaths);

  chai.add(chaiscript::fun(&myexit), "exit");
  chai.add(chaiscript::fun(&myexit), "quit");
  chai.add(chaiscript::fun(&help), "help");
  chai.add(chaiscript::fun(&throws_exception), "throws_exception");
  chai.add(chaiscript::fun(&get_eval_error), "get_eval_error");
  chai.add(chaiscript::fun(&now), "now");
  //bindDDSL(chai); //bind ddsl
  bindTypes(chai);
  bindMatrixLibBasic<Int32, UInt32>(chai);
  chai.add(chaiscript::fun(static_cast<Matrix<Int32, UInt32>(Matrix<Int32, UInt32>::*)()>(&Matrix<Int32, UInt32>::operator ())), "()");
  chai.add(chaiscript::fun(static_cast<Matrix<Int32, UInt32>(Matrix<Int32, UInt32>::*)(const Int32)>(&Matrix<Int32, UInt32>::operator[])), "[]");

   //CHAI_MEMBER_OPER(chai, [], TMat, TMat, const TIdx);

  bool eval_error_ok = false;
  bool boxed_exception_ok = false;

  for (int i = 0; i < argc; i++) {
    i += (i==0&&argc>1)?1:0;
    std::string arg (i!= 0?argv[i]:"--interactive");

    enum { eInteractive, eCommand, eFile} mode = eCommand;

    if  ( arg == "-c" || arg == "--command" ) {
		if ( (i+1) >= argc ) {
			std::cout << "insufficient input following " << arg << '\n';
			return EXIT_FAILURE;
		}
		arg = argv[++i];
    } else if ( arg == "-" || arg == "--stdin" ) {
		arg = "" ;
		std::string line;
		while (std::getline(std::cin, line))
			arg += line + '\n' ;
    } else if ( arg == "-v" || arg == "--version" ) {
		arg = "print(version())" ;
    } else if ( arg == "-h" || arg == "--help" ) {
		arg = "help(-1)";
    } else if ( arg == "-e" || arg == "--evalerrorok" ) {
		eval_error_ok = true;
		continue;
    } else if ( arg == "--exception" ) {
		boxed_exception_ok = true;
		continue;
    } else if ( arg == "-i" || arg == "--interactive" ) {
		mode = eInteractive ;
    } else if ( arg.find('-') == 0 ) {
		std::cout << "unrecognised argument " << arg << '\n';
		return EXIT_FAILURE;
    } else {
		mode = eFile;
    }

    try {
      switch ( mode ) {
        case eInteractive:
          interactive(chai);
          break;
        case eCommand:
          chai.eval(arg);
          break;
        case eFile:
          chai.eval_file(arg);
      }
    } catch (const chaiscript::exception::eval_error &ee) {
      std::cout << ee.pretty_print();
      std::cout << '\n';
      if (!eval_error_ok) {
        return EXIT_FAILURE;
      }
    } catch (const chaiscript::Boxed_Value &e) {
      std::cout << "Unhandled exception thrown of type " << e.get_type_info().name() << '\n';
      if (!boxed_exception_ok) {
        return EXIT_FAILURE;
      }
    } catch (const chaiscript::exception::load_module_error &e) {
      std::cout << "Unhandled module load error\n" << e.what() << '\n';
    } catch (DSTypes::Error &e) {
      std::cout << "DDSL:" << e.what() << '\n';
      if (!boxed_exception_ok) {
        return EXIT_FAILURE;
      }
    } catch (std::exception &e) {
      std::cout << e.what() << '\n';
      if (!boxed_exception_ok) {
        return EXIT_FAILURE;
      }
    }
  }

  return EXIT_SUCCESS;
}
