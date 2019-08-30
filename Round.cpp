#include <boost/lexical_cast.hpp>

#include <array>
#include <climits>
#include <cmath>
#include <iostream>
#include <string>

// NOTE: this require more than C++17!
#if (__cplusplus > 201603L) && __cpp_lib_to_chars
#    include <charconv>
#    include <string_view>
#    include <system_error>

void test_to_chars()
{
    std::array<char, 10> str;
    auto [p, ec] = std::to_chars(str.data(), str.data() + str.size(), 3.14,
                                 std::chars_format::fixed);
    if (ec == std::errc()) {
        std::cout << std::string_view(str.data(), p - str.data());
    }
}
#else
void test_to_chars() {}
#endif

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

std::string csa_cast(const double &input)
{
    if (std::abs(input) == 0.0) {
        return std::string("0.0"); // NOTE: no sign!
    }

    double diff = input - std::round(input);
    if (std::abs(diff) == 0.0) {
        std::stringstream m_stream;
        // NOTE: we force integer values as real strings: "d.0"
        m_stream << std::setprecision(1) << std::fixed << input;
        return m_stream.str();
    }

    return std::to_string(input); // with std::setprecision(6)
}

TEST_CASE("conversion to string")
{
#define lexical_cast boost::lexical_cast<std::string>

    char buf[] = "hello";
    char *str = buf;
    const double eps = std::numeric_limits<double>::epsilon();

    test_to_chars();

    CHECK_EQ(str, lexical_cast(str));
    CHECK_EQ("A", lexical_cast('A'));
    CHECK_EQ(" ", lexical_cast(' '));
    CHECK_EQ("123", lexical_cast(123));
    CHECK_EQ("123", lexical_cast(123.));
    CHECK_EQ("1.23", lexical_cast(1.23));

    CHECK_EQ("0.0000000000000004", lexical_cast(eps * 2.0)); // failed
    CHECK_EQ("0.0000000000000002", lexical_cast(eps));       // failed
    CHECK_EQ("0.0000000000000001", lexical_cast(eps / 2.0)); // failed

    CHECK_EQ("0.000000", std::to_string(eps));
    CHECK_EQ("1.000000", std::to_string(1.));
    CHECK_EQ("3.140000", std::to_string(3.14));

    // real values are integers:
    CHECK_EQ("0.0", csa_cast(0.));
    CHECK_EQ("0.0", csa_cast(-0.));
    CHECK_EQ("0.0", csa_cast(+0.));
    CHECK_EQ("1.0", csa_cast(1.));
    CHECK_EQ("3.0", csa_cast(3.));
    CHECK_EQ("-7.0", csa_cast(-7.));
    CHECK_EQ("314.0", csa_cast(314.));

    CHECK_EQ("-0.000000", csa_cast(-eps));
    CHECK_EQ("0.000000", csa_cast(eps));
    CHECK_EQ("0.101000", csa_cast(.101));
    CHECK_EQ("1.900000", csa_cast(1.9));
    CHECK_EQ("1.400000", csa_cast(1.4));
    CHECK_EQ("-1.500000", csa_cast(-1.5));
    CHECK_EQ("1.100000", csa_cast(1.1));
    CHECK_EQ("-1.100000", csa_cast(-1.1));
    CHECK_EQ("3.100000", csa_cast(3.1));
    CHECK_EQ("3.140000", csa_cast(3.14));
    CHECK_EQ("11.100000", csa_cast(+11.1));
    CHECK_EQ("-11.100000", csa_cast(-11.1));

    CHECK_EQ("2.718282", std::to_string(2.718281828));
    CHECK_EQ("3.141593", std::to_string(3.141592654));
    CHECK_EQ("314.000000", std::to_string(314.));

    CHECK_EQ("3", lexical_cast(3.));                    // NOTE: integer value!
    CHECK_EQ("3.14", lexical_cast(3.14));               // failed
    CHECK_EQ("0.101", lexical_cast(.101));              // failed
    CHECK_EQ("1.111111111", lexical_cast(1.111111111)); // OK
    CHECK_EQ("2.718281828", lexical_cast(2.718281828)); // failed
    CHECK_EQ("3.141592654", lexical_cast(3.141592654)); // failed

    // boolean values
    CHECK_EQ("1", lexical_cast(true));
    CHECK_EQ("0", lexical_cast(false));

    CHECK_EQ("Test", lexical_cast("Test"));
    CHECK_EQ(" ", lexical_cast(" "));
    CHECK_EQ("", lexical_cast(""));
    CHECK_EQ("Test", lexical_cast(std::string("Test")));
    CHECK_EQ(" ", lexical_cast(std::string(" ")));
    CHECK_EQ("", lexical_cast(std::string("")));
}

/***
Claus-MBP:samples clausklein$ make -f Makefile Round && ./Round
clang++ -std=c++14 -Wpedantic    Round.cpp   -o Round
[doctest] doctest version is "2.3.4"
[doctest] run with "--help" for options
===============================================================================
Round.cpp:48:
TEST CASE:  conversion to string

Round.cpp:65: ERROR: CHECK_EQ( "0.0000000000000004", boost::lexical_cast<std::string>(eps * 2.0) ) is NOT correct!
  values: CHECK_EQ( 0.0000000000000004, 4.4408920985006262e-16 )

Round.cpp:66: ERROR: CHECK_EQ( "0.0000000000000002", boost::lexical_cast<std::string>(eps) ) is NOT correct!
  values: CHECK_EQ( 0.0000000000000002, 2.2204460492503131e-16 )

Round.cpp:67: ERROR: CHECK_EQ( "0.0000000000000001", boost::lexical_cast<std::string>(eps / 2.0) ) is NOT correct!
  values: CHECK_EQ( 0.0000000000000001, 1.1102230246251565e-16 )

Round.cpp:100: ERROR: CHECK_EQ( "3.14", boost::lexical_cast<std::string>(3.14) ) is NOT correct!
  values: CHECK_EQ( 3.14, 3.1400000000000001 )

Round.cpp:101: ERROR: CHECK_EQ( "0.101", boost::lexical_cast<std::string>(.101) ) is NOT correct!
  values: CHECK_EQ( 0.101, 0.10100000000000001 )

Round.cpp:103: ERROR: CHECK_EQ( "2.718281828", boost::lexical_cast<std::string>(2.718281828) ) is NOT correct!
  values: CHECK_EQ( 2.718281828, 2.7182818279999998 )

Round.cpp:104: ERROR: CHECK_EQ( "3.141592654", boost::lexical_cast<std::string>(3.141592654) ) is NOT correct!
  values: CHECK_EQ( 3.141592654, 3.1415926540000001 )

===============================================================================
[doctest] test cases:      1 |      0 passed |      1 failed |      0 skipped
[doctest] assertions:     48 |     41 passed |      7 failed |
[doctest] Status: FAILURE!
Claus-MBP:samples clausklein$
***/
