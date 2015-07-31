// LINT Error 1083

#include <string>
#include <iostream>

struct Integer {
    int value;
    Integer(int value)
    {
        this->value = value;
    }
    operator int() const
    {
        return value;
    }
};

int main(int argc, char** argv)
{
    std::string help("Sorry, no arguments given!");
    Integer a(1);
    int b = a - 1;

// l1083.cpp:24:22: error: conditional expression is ambiguous; 'Integer' can be converted to 'int' and vice versa
//FIXME int c = argc % 2 ? a : b;

    std::string s = argc > 1 ? argv[1] : help;
    std::cout << s << std::endl;

    int c = argc > 1 ? argc : 0.0;

    return b || c;
}
