// from:
// http://www.linuxtopia.org/online_books/programming_books/c++_practical_programming/c++_practical_programming_254.html
//
//: C10:InnerClassIdiom.cpp
// Example of the "inner class" idiom.
//
// Here s a situation where we must (in effect) upcast to more than one type,
// but in this case we need to provide several different implementations of
// the same base type. The solution is something we ve lifted from Java, which
// takes C++ s nested class one step further. Java has a built-in feature
// called an inner class, which is like a nested class in C++, but it has
// access to the nonstatic data of its containing class by implicitly using
// the this pointer of the class object it was created within.[145]

// To implement the inner class idiom in C++, we must obtain and use a pointer
// to the containing object explicitly. Here s an example:

#include <iostream>
#include <string>

using namespace std;

class Poingable {
public:
    virtual void poing() = 0;
};

void callPoing(Poingable& p)
{
    p.poing();
}

class Bingable {
public:
    virtual void bing() = 0;
};

void callBing(Bingable& b)
{
    b.bing();
}

class Outer {
    string name;

    // Define one inner class:
    class Inner1;
    friend class Outer::Inner1;
    class Inner1 : public Poingable {
        Outer* parent;

    public:
        Inner1(Outer* p)
            : parent(p)
        {}
        void poing()
        {
            cout << "poing called for " << parent->name << endl;
            // Accesses data in the outer class object
        }
    } inner1;

    // Define a second inner class:
    class Inner2;
    friend class Outer::Inner2;
    class Inner2 : public Bingable {
        Outer* parent;

    public:
        Inner2(Outer* p)
            : parent(p)
        {}
        void bing() { cout << "bing called for " << parent->name << endl; }
    } inner2;

public:
    Outer(const string& nm)
        : name(nm)
        , inner1(this)
        , inner2(this)
    {}

    // Return reference to interfaces implemented by the inner classes:
    operator Poingable&() { return inner1; }
    operator Bingable&() { return inner2; }
};

int main()
{
    Outer x("Ping Pong");

    // Like upcasting to multiple base types!:
    callPoing(x);
    callBing(x);
}
