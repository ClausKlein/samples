/*
 * compiles with c++98 using boost:
 * $> g++ -Wall -Wextra -std=c++98 -I/opt/local/include set2.cpp -o set2
 */
#include "concept_check_extension.hpp"

#include <boost/current_function.hpp>
#include <boost/test/minimal.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

#ifdef USE_BOOST_CONTAINER
#    include <boost/container/set.hpp>
#    include <boost/container/vector.hpp>
using namespace boost::container;
#else
#    include <set>
#    include <utility>
#    include <vector>
using namespace std;
#endif

// Helper function for printing containers.
template <typename T>
auto
operator<<(std::ostream &stream, set<T> &c) -> BOOST_CONCEPT_REQUIRES(((boost::Ostreamable<T>)), (std::ostream &))
{
    if (!c.empty()) {
        stream << '{' << *c.begin();
        for (auto it = ++(c.begin()); it != c.end(); ++it) {
            stream << ", " << *it;
        }
        stream << '}' << std::endl;
    }
    return stream;
}

template <typename T>
auto
operator<<(std::ostream &stream, vector<T> &c) -> BOOST_CONCEPT_REQUIRES(((boost::Ostreamable<T>)), (std::ostream &))
{
    if (!c.empty()) {
        stream << '{' << *c.begin();
        for (auto it = ++(c.begin()); it != c.end(); ++it) {
            stream << ", " << *it;
        }
        stream << '}' << std::endl;
    }
    return stream;
}

class Customer
{
private:
    std::string firstName;
    std::string lastName;
    size_t no;

public:
    Customer(std::string fn, std::string ln, size_t n)
        : firstName(std::move(fn)), lastName(std::move(ln)), no(n)
    {
        // XXX std::cout << BOOST_CURRENT_FUNCTION << *this << std::endl;
    }

    [[nodiscard]] inline auto firstname() const -> std::string { return firstName; }
    [[nodiscard]] inline auto lastname() const -> std::string { return lastName; }
    [[nodiscard]] inline auto number() const -> size_t { return no; }

    friend auto operator<<(std::ostream &strm, const Customer &c) -> std::ostream &
    {
        return strm << "[" << c.firstName << "," << c.lastName << "," << c.no
                    << "]";
    }
    friend auto operator!=(const Customer &c1, const Customer &c2) -> bool
    {
        // XXX std::cout << BOOST_CURRENT_FUNCTION << std::endl;
        return !(c1 == c2);
    }
    friend auto operator==(const Customer &c1, const Customer &c2) -> bool
    {
        // XXX std::cout << BOOST_CURRENT_FUNCTION << std::endl;
        return (c1.firstname() == c2.firstname()) &&
               (c1.lastname() == c2.lastname()) && (c1.number() == c2.number());
    }
    friend auto hash_value(const Customer &c) -> std::size_t
    {
        // XXX std::cout << BOOST_CURRENT_FUNCTION << std::endl;
        std::size_t seed = 0;
        boost::hash_combine(seed, c.firstname());
        boost::hash_combine(seed, c.lastname());
        boost::hash_combine(seed, c.number());
        return seed;
    }
    friend auto operator<(const Customer &c1, const Customer &c2) -> bool
    {
        // XXX std::cout << BOOST_CURRENT_FUNCTION << std::endl;
        return (c1.firstname() < c2.firstname()) ||
               ((c1.firstname() == c2.firstname()) &&
                (c1.lastname() < c2.lastname())) ||
               ((c1.firstname() == c2.firstname()) &&
                (c1.lastname() == c2.lastname()) &&
                (c1.number() < c2.number()));
    }
};

auto test_main(int /*argc*/, char * /*argv*/[]) -> int
{
    using namespace boost;

    BOOST_CONCEPT_ASSERT((EqualityComparable<Customer>));
    BOOST_CONCEPT_ASSERT((LessThanComparable<Customer>));
    BOOST_CONCEPT_ASSERT((CopyConstructibleConcept<Customer>));
    // NO! BOOST_CONCEPT_ASSERT((DefaultConstructible<Customer>));
    BOOST_CONCEPT_ASSERT((Hashable<Customer>));

    typedef std::pair<int, int> MyType;
    BOOST_CONCEPT_ASSERT((Hashable<MyType>));
    // TODO: BOOST_CONCEPT_ASSERT((Ostreamable< MyType >));

    // sorted set of unique objects of type Customer.
    using SetType = set<Customer>;
    using ResultType = vector<Customer>;
    SetType custset;

    BOOST_CONCEPT_ASSERT((Collection<SetType>));

    BOOST_CHECK(custset.empty());
    std::cout << custset << std::endl; // print empty container

    custset.insert(Customer("Nico", "Josuttis", 42));
    custset.insert(Customer("Max", "Klein", 25));
    custset.insert(Customer("Petra", "klein", 56));
    custset.insert(Customer("Julian", "Klein", 27));
    custset.insert(Customer("petra", "klein", 56));
    custset.insert(Customer("Claus", "Klein", 55));
    custset.insert(Customer("Petra", "Klein", 56));

    std::pair<SetType::iterator, bool> result =
        custset.insert(Customer("Nico", "Josuttis", 43));
    BOOST_CHECK(result.first != custset.end() &&
                result.second); // it's a valid iterator

    for (size_t i = 0; i < 3; i++) {
        Customer key("Petra", "Klein", 56);
        custset.insert(key);
    }

    std::cout << custset << std::endl;
    std::size_t len = std::distance(custset.begin(), custset.end());
    BOOST_CHECK(custset.size() == len);

    const Customer key("Petra", "Klein", 56);
    const Customer key2("Petra", "Klein", 66);
    BOOST_CHECK(key < key2);

    const Customer key3(key);
    BOOST_CHECK(key == key3);
    BOOST_CHECK(!(key < key3) && !(key3 < key));
    BOOST_CHECK(hash_value(key) == hash_value(key3));

    std::size_t count = custset.erase(key);
    if (count) {
        std::cout << count << " erased " << key << std::endl;
        BOOST_CHECK(count == 1);
    }

    auto it = custset.find(key2);
    if (it == custset.end()) {
        std::cout << "not found: " << key2 << std::endl;
        count = custset.erase(key2);
        BOOST_CHECK(count == 0);
    } else {
        count = custset.erase(key2);
        std::cout << count << " erased " << key2 << std::endl;
        BOOST_CHECK(count == 1);
    }

    std::cout << custset << std::endl;

    SetType otherset;
    otherset.insert(Customer("Petra", "Klein", 55));
    otherset.insert(Customer("Claus", "Klein", 56));
    otherset.insert(Customer("Max", "Klein", 25));
    otherset.insert(Customer("Julian", "Klein", 27));
    std::cout << otherset << std::endl;

    ResultType intersection;
    std::set_intersection(custset.begin(), custset.end(), otherset.begin(),
                          otherset.end(), std::back_inserter(intersection));

    std::cout << "set_intersection: " << intersection << std::endl;
    // set_intersection: {[Julian,Klein,27], [Max,Klein,25]}

    return 0;
}
