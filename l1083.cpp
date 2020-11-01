// LINT Error 1083

#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace Test {
struct Integer
{
    int value;
    Integer(int _value) : value(_value) {}
    operator int() const { return value; }
};

struct String // TODO : public std::string
{
    std::string value;
    String(std::string _value) : value(std::move(_value)) {}
    operator const char *() const { return value.c_str(); }
};
} // namespace Test

auto main(int argc, char **argv) -> int
{
    using namespace std::string_literals;
    using namespace std;

    std::string help = "\"\0\"\tSorry, no arguments given!"s;
    std::cout << "length = " << help.length() << " size = " << help.size()
              << std::endl;

    std::vector<uint8_t> v{'u', 'v', 0, 1,    2,
                           3,   4,   5, 0xfe, 0xff}; // initializer-list ctor is
                                                     // preferred to (int, char)
    std::string s3{0x61, 'a', 0, 1,   2,
                   3,    4,   5, 0x7f}; // initializer-list ctor is preferred to
                                        // (int, char)
    std::string s1 = "ab\0\0cd";        // s1 contains "ab"
    std::string s2{"ab\0\0cd", 6};      // s2 contains "ab\0\0cd"
    std::cout << "s1: " << s1.size() << " \"" << s1 << "\"\n";
    std::cout << "s2: " << s2.size() << " \"" << s2 << "\"\n";
    std::cout << "s3: " << s3.size() << " \"" << s3 << "\"\n";
    std::cout << "#" << v.size() << ":" << std::string(v.begin(), v.end())
              << "\n";

    Test::Integer a(1);
    int b = a - 1;

    // l1083.cpp:32:22: error: conditional expression is ambiguous; 'Integer'
    // can be converted to 'int' and vice versa
    // FIXME int c = argc % 2 ? a : b;

    Test::String S = argc > 1 ? argv[1] : help;
    std::cout << S << std::endl;
    std::string s = argc > 1 ? argv[1] : help;
    std::cout << s << std::endl;

    int err = argc > 1 ? argc : 0.0;

    cout << typeid(uint8_t).name() << endl;
    cout << typeid(uint16_t).name() << endl;
    cout << typeid(uint32_t).name() << endl;
    cout << typeid(uint64_t).name() << endl;
    cout << typeid(float).name() << endl;
    cout << typeid(double).name() << endl;
    cout << typeid(help).name() << endl;
    cout << typeid(a).name() << endl;
    cout << typeid(S).name() << endl;

    return b || err;
}
