#include <boost/lexical_cast.hpp>

#include <array>
#include <climits>
#include <cmath>
#include <string>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// NOTE: this require more than C++17!
#if (__cplusplus >= 201703L) && __cpp_lib_to_chars
#    include <charconv>
#    include <iostream>
#    include <string_view>
#    include <system_error>

// to_chars_result to_chars(char* first, char* last, double value, chars_format
// fmt, int precision = 6);
TEST_CASE("test_to_chars")
{
    std::array<char, 23> buff{{0}};
    auto [p, ec] = std::to_chars(buff.data(), buff.data() + buff.size(), 3.14,
                                 std::chars_format::fixed, 9);
    if (ec == std::errc()) {
        std::cout << std::string_view(buff.data(), p - buff.data());
    }
    CHECK_EQ("3.140000000", std::string_view(buff.data(), buff.size()));
}
#endif

template <class T> std::string lexical_cast(T input)
{
    return boost::lexical_cast<std::string>(input);
}

template <> std::string lexical_cast(const double input)
{
    return boost::lexical_cast<std::string>(input);
}

std::string csa_cast(const double input)
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

    std::array<char, 6> buf{{"hello"}};
    char *str = buf.data();
    const double eps = std::numeric_limits<double>::epsilon();

    // chars
    CHECK_EQ(buf.data(), lexical_cast(str));
    CHECK_EQ("A", lexical_cast('A'));
    CHECK_EQ(" ", lexical_cast(' '));

    // integer
    CHECK_EQ("123", lexical_cast(123));
    CHECK_EQ("123", lexical_cast(123.));

    // simple real
    CHECK_EQ("1.23", lexical_cast(1.23));

    // extream real
    CHECK_EQ("4.4408920985006262e-16", lexical_cast(eps * 2.0));
    CHECK_EQ("2.2204460492503131e-16", lexical_cast(eps));
    CHECK_EQ("1.1102230246251565e-16", lexical_cast(eps / 2.0));

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
    CHECK_EQ("-4440890.0", csa_cast(-4.44089e+6));

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
    CHECK_EQ("0.000044", csa_cast(4.44089e-5));

    CHECK_EQ("2.718282", std::to_string(2.718281828));
    CHECK_EQ("3.141593", std::to_string(3.141592654));
    CHECK_EQ("314.000000", std::to_string(314.));
    CHECK_EQ("0.000044", std::to_string(4.44089e-5));
    CHECK_EQ("-4440890.000000", std::to_string(-4.44089e+6));

    CHECK_EQ("3", lexical_cast(3.)); // NOTE: integer value!

    // selected real values
    CHECK_EQ("3.1400000000000001", lexical_cast(3.14));
    CHECK_EQ("0.10100000000000001", lexical_cast(.101));
    CHECK_EQ("1.111111111",
             lexical_cast(1.111111111)); // NOTE: only same value as string
    CHECK_EQ("2.7182818279999998", lexical_cast(2.718281828));
    CHECK_EQ("3.1415926540000001", lexical_cast(3.141592654));

    // boolean values
    CHECK_EQ("1", lexical_cast(true));
    CHECK_EQ("0", lexical_cast(false));

    // strings
    CHECK_EQ("Test", lexical_cast("Test"));
    CHECK_EQ(" ", lexical_cast(" "));
    CHECK_EQ("", lexical_cast(""));
    CHECK_EQ("Test", lexical_cast(std::string("Test")));
    CHECK_EQ(" ", lexical_cast(std::string(" ")));
    CHECK_EQ("", lexical_cast(std::string("")));
}
