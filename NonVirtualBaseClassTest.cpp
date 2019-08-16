// test.cpp : Defines the entry point for the console application.
//
/***
g++-mp-4.6 -std=c++0x -Wall -Wextra -Wsign-compare -Wconversion -Wpointer-arith
-Wcomment -Wundef -Wold-style-cast -Weffc++     NonVirtualBaseClassTest.cpp -o
NonVirtualBaseClassTest NonVirtualBaseClassTest.cpp:23:7: warning: base class
'class base' has a non-virtual destructor [-Weffc++]
NonVirtualBaseClassTest.cpp:23:7: warning: 'class c_a' has virtual functions
and accessible non-virtual destructor [-Wnon-virtual-dtor]
NonVirtualBaseClassTest.cpp:35:7: warning: base class 'class base' has a
non-virtual destructor [-Weffc++] NonVirtualBaseClassTest.cpp:35:7: warning:
'class c_b' has virtual functions and accessible non-virtual destructor
[-Wnon-virtual-dtor]
***/

#include <cassert>
#include <iostream>
#include <map>

using namespace std;

class base
{
public:
    virtual ~base() = default;
    ;
    // FIXME virtual void print() const = 0;
};

class c_a : public base
{
    int a;

public:
    c_a(int _a) : a(_a){};
    const int &getvalue() const { return a; };
    virtual void print() const { cout << a << endl; };
};

class c_b : public base
{
    double b;

public:
    c_b(double _b) : b(_b){};
    const double &getvalue() const { return b; };
    virtual void print() const { cout << b << endl; };
};

class SUT
{
    int type{0};
    string id;
    union
    {
        const c_b *m_b;
        const c_a *m_a;
        const base *m_base;
    };

public:
    enum e
    {
        t_a,
        t_b
    };

    SUT() : id(""), m_base(nullptr)
    {
        // XXX u.m_base = 0;
        cout << "SUT()" << endl;
    };
    SUT(const base *_b, int t, const string _id = "")
        : type(t), id(_id), m_base(_b)
    {
        // XXX u.m_base = _b;
        cout << "SUT(base)" << endl;
    };

    const c_a &get_a() const
    {
        assert(type == t_a); // TODO assert
        return *m_a;
    }

    const c_b &get_b() const
    {
        assert(type == t_b); // TODO assert
        return *m_b;
    }

    const void *getbaseptr(const base *b) { return b; };
};

typedef map<char, SUT *> sut_map_t;

int main()
{
    c_a a(0x0815);
    const base *pa = &a;

    cout << "sizeof(base):" << sizeof(base) << endl;
    cout << "sizeof(c_a):" << sizeof(c_a) << endl;
    cout << "sizeof(c_b):" << sizeof(c_b) << endl;

    assert(&a == pa);
    a.print();
    // FIXME pa->print();

    SUT ta(&a, SUT::t_a); // create a class with c_a
    const SUT &rta = ta;  // create a reference to it
    assert(&ta == &rta);  // Obtain and compare their addresses

    assert(ta.getbaseptr(&a) == &a); // ERROR, assert failed!

    c_b b(1.314);
    const base *pb = &b;

    assert(&b == pb);
    b.print();
    // FIXME pb->print();

    SUT tb(&b, SUT::t_b); // create a class with c_b
    const SUT &rtb = tb;  // create a reference to it
    assert(&ta == &rta);  // Obtain and compare their addresses

    assert(tb.getbaseptr(&b) == &b); // ERROR, assert failed!
    map<char, SUT *> mymap;

    mymap['a'] = &ta;
    assert(mymap['a'] == &rta);
    assert(mymap['a']->get_a().getvalue() ==
           a.getvalue()); // ERROR, assert failed!

    mymap['b'] = &tb;
    assert(mymap['b'] == &rtb);
    assert(mymap['b']->get_b().getvalue() ==
           b.getvalue()); // ERROR, assert failed!

    cout << "mymap now contains " << mymap.size() << " elements." << endl;
    cout << "OK" << endl;
    return 0;
}
