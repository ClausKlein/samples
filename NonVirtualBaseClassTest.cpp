// test.cpp : Defines the entry point for the console application.

#include <cassert>
#include <exception>
#include <iostream>
#include <map>

using namespace std;

class base
{
public:
    virtual ~base() = default;
    // FIXME virtual void print() const = 0;
};

class c_a : public base
{
    int a;

public:
    c_a(int _a) : a(_a){};
    [[nodiscard]] auto getvalue() const -> const int & { return a; };
    virtual void print() const { cout << a << endl; };
};

class c_b : public base
{
    double b;

public:
    c_b(double _b) : b(_b){};
    [[nodiscard]] auto getvalue() const -> const double & { return b; };
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

    SUT() : id(""), m_base(nullptr) { cout << "SUT()" << endl; };
    SUT(const base *_b, int t, const string _id = "")
        : type(t), id(_id), m_base(_b)
    {
        cout << "SUT(base)" << endl;
    };

    [[nodiscard]] auto get_a() const -> const c_a &
    {
        assert(type == t_a); // TODO assert
        if (type == t_a) {
            return *m_a;
}
        throw(std::runtime_error("invallid type a"));
    }

    [[nodiscard]] auto get_b() const -> const c_b &
    {
        assert(type == t_b); // TODO assert
        if (type == t_b) {
            return *m_b;
}
        throw(std::runtime_error("invallid type b"));
    }

    auto getbaseptr(const base *b) -> const void * { return b; };
};

using sut_map_t = map<char, SUT *>;

auto main() -> int
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
