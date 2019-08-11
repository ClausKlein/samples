#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <iostream>
#include <string>

// Entfernt folgende Leerzeichen von s.
inline std::string trim(const std::string &s)
{
    // NOTE: substr() returns a substring [pos, pos+count). If the requested
    // substring extends past the end of the string, or if count == npos, the
    // returned substring is [pos, size()).
    return s.substr(0, s.find_last_not_of(' ') + 1);
}

// Entfernt fuehrende Leerzeichen von s.
inline std::string ltrim(const std::string &s)
{
    if (s.empty()) {
        return s;
    }
    std::size_t pos = s.find_first_not_of(' ');
    if (pos == std::string::npos) {
        return "";
    } else {
        // NOTE: substr(npos) throws a std::out_of_range exception!
        return s.substr(pos);
    }
}

int main()
{
    // the test string
    std::string str(" Hello World! ");
    std::string str2("other");
    std::string blanks("     ");
    std::string empty("");
    std::string ws("\t \r\n");

#if 0
    // strings and chars to search for
    const char* ws = " \t";

    std::size_t pos = str.find_first_not_of(ws);
    if (pos != std::string::npos)
    {
        std::cout << str.substr(pos) << std::endl;
    }
#endif

    std::cout << "'" << str << "'" << std::endl;
    std::cout << "'" << trim(str) << "'" << std::endl;
    std::cout << "'" << ltrim(str) << "'" << std::endl;
    std::cout << "'" << ltrim(trim(str)) << "'" << std::endl;

    std::cout << "'" << ltrim(trim(str2)) << "'" << std::endl;

    std::cout << "'" << ltrim(trim(empty)) << "'" << std::endl; // OK

    std::cout << "'" << ltrim(trim(blanks)) << "'" << std::endl; // save
    std::cout << "'" << ltrim(blanks) << "'" << std::endl; // std::out_of_range

    std::string str1 = "     hello world!     ";
    std::cout << "'" << boost::algorithm::trim_left_copy(str1) << "'"
              << std::endl;
    std::cout << "'" << boost::algorithm::trim_right_copy(str1) << "'"
              << std::endl;

    //
    // Remove all leading and trailing spaces from the input. The input
    // sequence is modified in-place.
    //
    boost::algorithm::trim(str1); // str1 == "hello world!"
    std::cout << "'" << str1 << "'" << std::endl;
    std::cout << "'" << boost::algorithm::trim_copy(ws) << "'" << std::endl;
    std::cout << "'" << boost::algorithm::trim_copy(empty) << "'" << std::endl;
    std::cout << "'" << boost::algorithm::to_upper_copy(str) << "'"
              << std::endl;
}

/***
' Hello World! '
' Hello World!'
'Hello World! '
'Hello World!'
'other'
''
''
''
 ***/
