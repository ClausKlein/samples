//
// clang++ -g -Wall -I/usr/local/include -std=c++14 -L/usr/local/lib copy-test.cpp  /usr/local/lib/libboost_unit_test_framework.a -o copy-test
//

#include <vector>
#include <string>
#include <iostream>
#include <type_traits>

#include <boost/lexical_cast.hpp>
#include <boost/test/unit_test.hpp>

using namespace boost;

void test_abstract();
void test_copy();

unit_test::test_suite *init_unit_test_suite(int, char *[])
{
    unit_test::test_suite *suite =
        BOOST_TEST_SUITE("lexical_cast unit test");
    suite->add(BOOST_TEST_CASE(&test_abstract));
    suite->add(BOOST_TEST_CASE(&test_copy));

    return suite;
}

class A
{
public:
    virtual ~A() {};
    virtual void out(std::ostream &) const = 0;
};

class B: public A
{
public:
    B() : m_str("default") {};
    B(const std::string & str) : m_str(str), m_data(str.begin(), str.end()) {};
    virtual ~B()
    {
        check();
        std::cout << m_str << " deleted" << std::endl;
    };
    virtual void out(std::ostream &O) const
    {
        O << m_str;
    }
    void check()
    {
        std::string data(m_data.begin(), m_data.end());
        std::cout << data << " checked:" << std::endl;
        BOOST_CHECK(m_str == data);
    }
private:
    std::string m_str;
    std::vector<char> m_data;
};

std::ostream &operator<<(std::ostream &O, const A &a)
{
    a.out(O);
    return O;
}

void test_abstract()
{
    const A &a = B("const A&");
    BOOST_CHECK(boost::lexical_cast<std::string>(a) == "const A&");
}

void test_copy()
{
    std::vector<B> myList;
    {
        B *a = new B("A");
        {
            myList.push_back(*a);
            B b("X");
            b.check();
            myList.push_back(b);
            b = *a;
            b.check();

            BOOST_CHECK(boost::lexical_cast<std::string>(*a) == "A");
            std::cout << std::endl;
            delete a;

            BOOST_CHECK(boost::lexical_cast<std::string>(b) == "A");

            const B c(b);
            myList.push_back(c);
            BOOST_CHECK(boost::lexical_cast<std::string>(c) == "A");
        }

        B d;
        myList.push_back(d);
        BOOST_CHECK(boost::lexical_cast<std::string>(d) == "default");
    }
    std::cout << std::endl;

    std::cout << std::boolalpha
              << "B is trivally copy-assignable? "
              << std::is_trivially_copy_assignable<B>::value << '\n'
              << "B is copy-assignable? "
              << std::is_copy_assignable<B>::value << '\n'
              << "B is assignable from B? "
              << std::is_assignable<B, B>::value << '\n'
              << "B is trivially assignable from const B? "
              << std::is_trivially_assignable<B, const B>::value << '\n'
              << std::endl;
}

