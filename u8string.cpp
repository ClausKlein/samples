#include "boost/lexical_cast.hpp"

#include <boost/beast/core/static_string.hpp>

#include <string>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

using boost::beast::static_string;
using boost::beast::string_view;

TEST_CASE("testToStdString")
{
    static_string<3> ss("123");
    // TODO std::string s(ss);
    // TODO std::string s = ss;
    std::string s1(ss.substr());
    CHECK(s1 == "123");

    std::string s2(ss.c_str());
    CHECK(s2 == "123");

    // TODO CHECK(s2 == ss);

    // use std::literals::string_literals::operator""s
    {
#if __cplusplus > 201411L
        using namespace std::string_literals; // C++14
        std::string s1 = "ab\0\0cd";          // s1 contains "ab"
        std::string s2{"ab\0\0cd", 6};        // s2 contains "ab\0\0cd"
        std::string s3 = "ab\0\0cd"s;         // s3 contains "ab\0\0cd"
        CHECK(s2 == s3);
#endif
    }
}

TEST_CASE("testUstring")
{
#if __cplusplus > 201411L
    using namespace std::string_literals; // C++14

    // std::u8string        std::basic_string<char8_t>      (C++20)
    // std::u8string_view	std::basic_string_view<char8_t> (C++20)
    typedef std::basic_string<uint8_t> u8string;
    typedef std::basic_string_view<uint8_t> u8string_view;

    {
#    if 0
        u8string s1 = "ab\0\0cd";   // s1 contains "ab"
        u8string s2{ "ab\0\0cd", 6 }; // s2 contains "ab\0\0cd"
        u8string s3 = "ab\0\0cd"s;  // s3 contains "ab\0\0cd"
#    endif
    }

    // /opt/local/libexec/llvm-7.0/include/c++/v1/string:793:5: note:
    // candidate constructor not viable: no known conversion from 'const char
    // [4]' to 'const std::__1::basic_string<unsigned char,
    // std::__1::char_traits<unsigned char>, std::__1::allocator<unsigned
    // char> >' for 1st argument
    // u8string s1 = "\0123"s;
    // std::string s1("\0123"s);
    // u8string s2("\u0123"s);
    // u8string s3(u8"\u0123");

    // Initialization
    {
        // XXX char ca[] = u8"text";   // C++17: Ok. BUT not with c++10
        // see
        // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0482r6.html

        int8_t c = u8'c'; // C++17: Ok.

        uint8_t c8 = 'c'; // C++17: N/A (char8_t is not a type specifier).
        // uint8_t c8 = 'c';
        CHECK(c == c8);
    }

    // Implicit conversions
    {
#    if 0
        const char (&u8r)[] = u8"text"; // C++17: Ok.
        // This proposal: Ill-formed.

        const char *u8p = u8"text";       // C++17: Ok.
        // This proposal: Ill-formed.
#    endif
    }

    // Return type of operator ""s and operator ""sv
    {
#    if 0
        std::string s;

        s = u8"text"s;    // C++17: Ok.
        // This proposal: ill-formed.

        s = u8"text"sv;   // C++17: Ok.
        // This proposal: ill-formed.
#    endif
    }

    u8string s{0, 0x20, 0x30};
    static_string<3, uint8_t> ss(s);
    CHECK(ss.length() == 3);

    u8string_view sv = s;
    CHECK(sv == s);
    // FIXME CHECK(static_string<5, uint8_t>(sv) == s);

    // static_string.cpp:1183:11: error: no matching conversion for
    // functional-style cast
    //           from 'u8string_view' (aka 'basic_string_view<unsigned char>')
    // to 'static_string<5, uint8_t>' (aka 'static_string<5, unsigned char>')
    // CHECK(static_string<5, uint8_t>(sv) == s);
    //       ^~~~~~~~~~~~~~~~~~~~~~~~~~~~

#endif
}

template <size_t Size_> class tustring : public static_string<Size_, char>
{
public:
    tustring() = default;

    tustring(const std::string &other)
    {
        this->assign(other.begin(), other.end());
    }

    auto operator=(const std::string &other) -> tustring &
    {
        this->assign(other.begin(), other.end());
        return (*this);
    }

#if 0 // not needed! CK
    tustring &operator+=(const std::string &other)
    {
        this->append(other.begin(), other.end());
        return (*this);
    }

    tustring &operator+=(char ch)
    {
        this->push_back(ch);
        return (*this);
    }

    bool operator==(const std::string &other) { return this->compare(other); }
#endif

    auto to_string() -> std::string { return std::string(this->substr()); }
};

TEST_CASE("static_string")
{
    tustring<5> ustring5 = {"Hello"};
    CHECK(5 == ustring5.size());
    CHECK("Hello" == ustring5);

    CHECK_THROWS(ustring5 += "more");
    CHECK_THROWS(ustring5 += 'X');

    std::string toLong{"Hallo du alter Trottel!"};
    CHECK_THROWS(ustring5 = toLong);
    CHECK_NOTHROW(ustring5.assign(toLong.substr(9, 5)));
    CHECK("alter" == ustring5);

    const char str12[] = {"Only 12 char"};
    tustring<12> test12;
    CHECK(test12.empty());
    CHECK_NOTHROW(test12.insert(0, string_view(str12, 12)));
    CHECK(test12 == str12);
    CHECK(test12.capacity() == 12);

    tustring<3> test3("123");
    CHECK_THROWS(test3.reserve(20));
    CHECK(test3.capacity() == 3);
    CHECK(3 == test3.size());
    CHECK("123" == test3);

    CHECK_NOTHROW(test3.assign(3, ' '));
    CHECK(3 == test3.size());

    CHECK_THROWS(test3.resize(20, 'x'));
    CHECK_THROWS(test3.append(3, '-'));

    test3.clear();
    CHECK(test3.empty());
    CHECK_NOTHROW(test3.append(2, '_'));
    CHECK(test3 == "__");
    CHECK(test3.size() == 2);

    CHECK_THROWS(test3.swap(test12));
    CHECK(test3 == "__");
    CHECK(test3.length() == 2);

    CHECK_NOTHROW(test3 = "--");
    CHECK_NOTHROW(test3 += '+');
    CHECK(test3.length() == 3);
    CHECK(test3 != "__");
    CHECK(test3 == "--+");

    CHECK_EQ(str12, boost::lexical_cast<std::string>(test12));
    CHECK_EQ(str12, test12.to_string());

    // use std::literals::string_literals::operator""s
    {
#if __cplusplus > 201411L
        using namespace std::string_literals; // C++14
        std::string sl = "ab\0\0cd\xfe\xff"s; // sl contains "ab\0\0cd.."
        CHECK(sl.size() == 8);

        tustring<12> ss(sl);
        CHECK(ss.size() == 8);
        CHECK(ss.substr() == sl);
        CHECK(ss.substr(0, 2) == "ab");
        CHECK(ss.substr(2, 2) == "\0\0"s);
        CHECK(ss.at(7) == '\xff');
        CHECK_EQ(sl, boost::lexical_cast<std::string>(sl));
#endif
    }
}
