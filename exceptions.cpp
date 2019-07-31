// CXXFLAGS="-Wall -std=c++98 -pedandic -Weffc++ -O -Wuninitialized -Wunused -Wold-style-cast" make exceptions -B

#include <iostream>
// #include <string>
#include <stdexcept>

using namespace std;

void myfunction(); //throw(int, bad_exception);
int foo(void *p); //throw(invalid_argument);

class illegal_division : public std::invalid_argument
{
public:
    illegal_division(const std::string &str) : std::invalid_argument(str)
    {
    }
};
int division(int a, int b); //throw(illegal_division);


int division(int a, int b) //throw(illegal_division)
{
    if (b == 0)
    {
        throw illegal_division("Durch Null teilen ist nicht erlaubt.");
    }
    return a / b;
}


void myfunction() //throw(int, bad_exception)
{
    //FIXME throw 'x'; // throws char (not in exception-specification)
}


int foo(void *p) //throw(invalid_argument)
{
    int a = 4;
    int b = a;
    if (p == NULL)
    {
        throw invalid_argument("p must not NULL!");
    }
    else
    {
        return b;
    }
}


int main()
{
    int i;
    try
    {
        myfunction();
        i = foo(&i);
        i = division(1, 0);
    }
    catch (const exception &ex)
    {
        cout << ex.what() << endl;
    }
    cout << "End" << endl;

    return i;
}

