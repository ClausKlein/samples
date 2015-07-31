/* The following code example is taken from the book
 * "The C++ Standard Library - A Tutorial and Reference, 2nd Edition"
 * by Nicolai M. Josuttis, Addison-Wesley, 2012
 *
 * (C) Copyright Nicolai M. Josuttis 2012.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 *
 * compiles with c++98 using boost:
 * $> g++ -g -Wall -Wextra -std=c++98 -Weffc++ -I/opt/local/include set2.cpp -o set2
 * $> g++ -g -Wall -Wextra -std=c++98 -Weffc++ -I/opt/local/include set2.cpp -o set2-E.cpp -E
 *
 */
#include <string>
#include <iostream>
#include <cassert>
#include <boost/current_function.hpp>
#include <boost/unordered_set.hpp>

#include "concept_check_extension.hpp"

#define USE_SET
#ifdef USE_SET
#include <boost/container/set.hpp>
using namespace boost::container;
#else
// workarount to compile only
#include <set>
using namespace std;
#endif

#if 1
// Helper function for printing containers.
template<typename T>
BOOST_CONCEPT_REQUIRES(((boost::Ostreamable<T>)), (std::ostream&))
operator<<(std::ostream& stream, set<T>& c)
{
    if (!c.empty()) {
        stream << '{' << *c.begin();
        for (typename set<T>::iterator it = ++(c.begin()); it != c.end(); ++it) {
            stream << ", " << *it;
        }
        stream << '}' << std::endl;
    }
    return stream;
}

template<typename T>
BOOST_CONCEPT_REQUIRES(((boost::Ostreamable<T>)), (std::ostream&))
operator<<(std::ostream& stream, boost::unordered_set<T>& c)
{
    if (!c.empty()) {
        stream << '{' << *c.begin();
        for (typename boost::unordered_set<T>::iterator it = ++(c.begin()); it != c.end(); ++it) {
            stream << ", " << *it;
        }
        stream << '}' << std::endl;
    }
    return stream;
}
#else
// TODO find a genereral template solution for all kind of containers! CK
template<typename T>
std::ostream& operator<<(std::ostream& stream, Collection<T>& c)
{
    if (!c.empty()) {
        stream << '{' << *c.begin();
        for (typename Collection<T>::iterator it = ++(c.begin()); it != c.end(); ++it) {
            stream << ", " << *it;
        }
        stream << '}' << std::endl;
    }
    return stream;
}
#endif


class Customer
{
private:
    std::string fname;
    std::string lname;
    long   no;

public:
    Customer(const std::string& fn, const std::string& ln, long n)
        : fname(fn), lname(ln), no(n)
    {
        std::cout << BOOST_CURRENT_FUNCTION << *this << std::endl;
    }

    std::string firstname() const
    {
        return fname;
    }
    std::string lastname() const
    {
        return lname;
    }
    long number() const
    {
        return no;
    }

    friend std::ostream& operator << (std::ostream& strm, const Customer& c)
    {
        return strm << "[" << c.fname << "," << c.lname << "," << c.no << "]";
    }
    friend bool operator!=(const Customer& c1, const Customer& c2)
    {
        std::cout << BOOST_CURRENT_FUNCTION << std::endl;
        return !(c1 == c2);
    }
    friend bool operator==(const Customer& c1, const Customer& c2)
    {
        std::cout << BOOST_CURRENT_FUNCTION << std::endl;
        return (c1.firstname() == c2.firstname()) &&
               (c1.lastname() == c2.lastname()) &&
               (c1.number() == c2.number());
    }
    friend std::size_t hash_value(const Customer& c)
    {
        std::cout << BOOST_CURRENT_FUNCTION << std::endl;
        std::size_t seed = 0;
        boost::hash_combine(seed, c.firstname());
        boost::hash_combine(seed, c.lastname());
        boost::hash_combine(seed, c.number());
        return seed;
    }
    friend bool operator<(const Customer& c1, const Customer& c2)
    {
        std::cout << BOOST_CURRENT_FUNCTION << std::endl;
        return (c1.firstname() < c2.firstname()) ||
               ((c1.firstname() == c2.firstname()) &&
                (c1.lastname() < c2.lastname())) ||
               ((c1.firstname() == c2.firstname()) &&
                (c1.lastname() == c2.lastname()) &&
                (c1.number() < c2.number()));
    }
};


int main()
{
    using namespace boost;

    BOOST_CONCEPT_ASSERT((EqualityComparable<Customer>));
    BOOST_CONCEPT_ASSERT((LessThanComparable<Customer>));
    BOOST_CONCEPT_ASSERT((CopyConstructibleConcept<Customer>));
    //NO! BOOST_CONCEPT_ASSERT((DefaultConstructible<Customer>));
    BOOST_CONCEPT_ASSERT((Hashable< Customer >));

    typedef std::pair<int, int> MyType;
    BOOST_CONCEPT_ASSERT((Hashable< MyType >));
    //TODO BOOST_CONCEPT_ASSERT((Ostreamable< MyType >));

    unordered_set<MyType> testOnly;
    testOnly.insert(MyType(0, 1));
    MyType myKey(1, 2);
    testOnly.insert(myKey);
    assert(testOnly.end() != testOnly.find(myKey));
    //TODO std::cout << testOnly << std::endl;

#ifdef USE_SET
    // sorted set of unique objects of type Key.
    typedef set<Customer> SetType;
    SetType custset;
#else
    typedef unordered_set<Customer> SetType;
    SetType custset;
    custset.reserve(16);
#endif

    BOOST_CONCEPT_ASSERT((Collection< SetType >));

    assert(custset.empty());
    std::cout << custset << std::endl;  //print empty container

    custset.insert(Customer("Nico", "Josuttis", 42));
    custset.insert(Customer("Max", "Klein", 25));
    custset.insert(Customer("Julian", "Klein", 27));
    custset.insert(Customer("Claus", "Klein", 55));
    custset.insert(Customer("Petra", "Klein", 56));
    custset.insert(Customer("Petra", "klein", 56));
    custset.insert(Customer("petra", "klein", 56));

    std::pair<SetType::iterator, bool> result = custset.insert(Customer("Nico", "Josuttis", 43));
    assert(result.first != custset.end() && result.second); // it's a valid iterator

    for (long i = 0; i < 10; i++) {
        Customer key("Petra", "Klein", i + 56);
        custset.insert(key);
    }

    std::cout << custset << std::endl;

#ifdef USE_SET
    bool sorted = true;
#else
    std::cout << "with reserve(" << custset.bucket_count() << "): load_factor=" << custset.load_factor() << std::endl;
    custset.rehash(39);
    std::cout << "after rehash(" << custset.bucket_count() << "): load_factor=" << custset.load_factor() << std::endl;
    bool sorted = false; //TODO std::is_sorted(custset.begin(), custset.end());
#endif

    std::size_t len = std::distance(custset.begin(), custset.end());
    std::cout << len << " Customer inserted, sorted = " << sorted << std::endl;
    assert(custset.size() == len);

    std::cout << custset << std::endl;

    Customer key("Petra", "Klein", 56);
    Customer key2("Petra", "Klein", 67);
    assert(key < key2);

    Customer key3(key);
    assert(key == key3);
    assert(!(key < key3) && !(key3 < key));
    assert(hash_value(key) == hash_value(key3));

    std::size_t count = custset.erase(key);
    if (count) {
        std::cout << count << " erased " << key << std::endl;
    }
    SetType::iterator it = custset.find(key2);
    if (it == custset.end()) {
        std::cout << "not found: " << key2 << std::endl;
    } else {
        assert(count == 1);
        std::cout << count << " erased " << key << std::endl;
    }

    count = custset.erase(key2);
    assert(count == 0);
    std::cout << "not found: " << key2 << std::endl;

    std::cout << custset << std::endl;
}
