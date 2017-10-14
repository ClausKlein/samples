//
// This example program, reports the size of a file:
//
#include <iostream>

#if __cplusplus < 201402L
    #define BOOST_FILESYSTEM_NO_DEPRECATED
    #define BOOST_FILESYSTEM_NO_LIB
    #include <boost/filesystem.hpp>

    using namespace boost::filesystem;

#else
    #include <experimental/filesystem>

    namespace std::experimental::filesystem::v1;

#endif

const char * say_what(bool b) { return b ? "true" : "false"; }

int main(int argc, char* argv[])
{
    path program(argv[0]);
    if (argc < 2)
    {
        std::cout << program.filename() << " " << file_size(program) << '\n';
        std::cout << "Usage: tut1 path\n";
        //XXX return 1;
    }
    else
    {
        program = argv[1];
    }

    path p = program;
    std::cout << p << " file_size()" << file_size(p) << '\n';

    std::cout  <<  "\ndecomposition:\n";
    std::cout  <<  "  root_name()----------: " << p.root_name() << '\n';
    std::cout  <<  "  root_directory()-----: " << p.root_directory() << '\n';
    std::cout  <<  "  root_path()----------: " << p.root_path() << '\n';
    std::cout  <<  "  relative_path()------: " << p.relative_path() << '\n';
    std::cout  <<  "  parent_path()--------: " << p.parent_path() << '\n';
    std::cout  <<  "  filename()-----------: " << p.filename() << '\n';
    std::cout  <<  "  stem()---------------: " << p.stem() << '\n';
    std::cout  <<  "  extension()----------: " << p.extension() << '\n';

    std::cout  <<  "\nquery:\n";
    std::cout  <<  "  empty()--------------: " << say_what(p.empty()) << '\n';
    std::cout  <<  "  is_absolute()--------: " << say_what(p.is_absolute()) << '\n';
    std::cout  <<  "  has_root_name()------: " << say_what(p.has_root_name()) << '\n';
    std::cout  <<  "  has_root_directory()-: " << say_what(p.has_root_directory()) << '\n';
    std::cout  <<  "  has_root_path()------: " << say_what(p.has_root_path()) << '\n';
    std::cout  <<  "  has_relative_path()--: " << say_what(p.has_relative_path()) << '\n';
    std::cout  <<  "  has_parent_path()----: " << say_what(p.has_parent_path()) << '\n';
    std::cout  <<  "  has_filename()-------: " << say_what(p.has_filename()) << '\n';
    std::cout  <<  "  has_stem()-----------: " << say_what(p.has_stem()) << '\n';
    std::cout  <<  "  has_extension()------: " << say_what(p.has_extension()) << '\n';

    return 0;
}


/***

Claus-MacBook-Pro:tmp clausklein$ make

clang++ -g -Wall -Wextra -std=c++03  -Wno-unused-parameter -Weffc++ \
-I/usr/local/include -MMD -MP -DBOOST_SYSTEM_DYN_LINK=1 \
-DBOOST_SYSTEM_NO_DEPRECATED -DBOOST_ALL_NO_LIB=1 -DBOOST_ALL_NO_AUTO_LINK=1 \
filepath.cpp  -L/usr/local/lib -lboost_filesystem -lboost_system -o filepath

Claus-MacBook-Pro:tmp clausklein$ ./filepath
"filepath" 40512
Usage: tut1 path
"./filepath" file_size()40512

decomposition:
  root_name()----------: ""
  root_directory()-----: ""
  root_path()----------: ""
  relative_path()------: "./filepath"
  parent_path()--------: "."
  filename()-----------: "filepath"
  stem()---------------: "filepath"
  extension()----------: ""

query:
  empty()--------------: false
  is_absolute()--------: false
  has_root_name()------: false
  has_root_directory()-: false
  has_root_path()------: false
  has_relative_path()--: true
  has_parent_path()----: true
  has_filename()-------: true
  has_stem()-----------: true
  has_extension()------: false
Claus-MacBook-Pro:tmp clausklein$

 ***/
