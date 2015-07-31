// LINT Error 1083

#include <string>
#include <iostream>

struct Integer {
    int value;
    Integer(int _value): value(_value) {}
    operator int() const
    {
        return value;
    }
};

struct String //TODO : public std::string
{
    std::string value;
    String(std::string _value): value(_value) {}
    operator const char *() const
    {
        return value.c_str();
    }
};

int main(int argc, char** argv)
{
    std::string help("Sorry, no arguments given!");
    Integer a(1);
    int b = a - 1;

// l1083.cpp:32:22: error: conditional expression is ambiguous; 'Integer' can be converted to 'int' and vice versa
    //FIXME int c = argc % 2 ? a : b;

    String S = argc > 1 ? argv[1] : help;
    std::cout << S << std::endl;
    std::string s = argc > 1 ? argv[1] : help;
    std::cout << s << std::endl;

    int err = argc > 1 ? argc : 0.0;

    return b || err;
}
