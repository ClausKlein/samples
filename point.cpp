
// Copyright 2005 Daniel James.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <complex>
#include <iostream>

#include <utility> // std::rel_ops;

#include <boost/functional/hash.hpp>
#include <boost/test/minimal.hpp>
#include <boost/unordered_map.hpp>

// This example illustrates how to use boost::hash_combine to generate a hash
// value from the different members of a class. For full details see the hash
// tutorial.

template <typename Map> void print_map(Map& m)
{
    std::cout << '{';
    // FIXME: for (Map::iterator p = m.begin(); p != m.end(); ++p)
    for (auto& p : m) {
        std::cout << p.first << ':' << p.second << ' ';
    }
    std::cout << "}\n";
}

#if 1
typedef std::complex<int> Point;

// NOTE: Because complex numbers are naturally thought of as existing on a
// two-dimensional plane, there is no natural linear ordering on the set of
// complex numbers.
std::size_t hash(Point const& p)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, p.real());
    boost::hash_combine(seed, p.imag());
    return seed;
}

bool operator<(const Point& lhs, const Point& rhs)
{
    return lhs.real() < rhs.real(); // NB. intentionally ignores imag()
}

#else

class Point {
    int x;
    int y;

public:
    Point()
        : x(0)
        , y(0)
    {}
    Point(int x, int y)
        : x(x)
        , y(y)
    {}

    bool operator==(Point const& other) const
    {
        return x == other.x && y == other.y;
    }

    // NOTE: strict weak ordering reqired
    // see http://en.cppreference.com/w/cpp/concept/LessThanComparable
    // and http://en.cppreference.com/w/cpp/utility/rel_ops/operator_cmp
    bool operator<(Point const& other) const
    {
        // FIXME: compares the magnitude of the points:
        // XXX return ((x * x + y * y) < (other.x * other.x + other.y *
        // other.y));
        return (x < other.x) || ((x == other.x) && (y < other.y));
    }

    friend std::size_t hash_value(Point const& p)
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, p.x);
        boost::hash_combine(seed, p.y);
        return seed;
    }

    friend std::ostream& operator<<(std::ostream& stream, const Point& p)
    {
        stream << '{';
        stream << p.x << ',' << p.y;
        stream << '}';
        return stream;
    }
};

#endif

int test_main(int /*argc*/, char* /*argv*/[])
{
    using namespace std::rel_ops;

    boost::hash<Point> point_hasher;
    boost::unordered_map<Point, int> container;

    Point p0;
    Point p1(0, 0);
    Point p2(1, -2);
    Point p3(4, 1);
    Point p5(2, 1);
    Point p4 = p1;

    std::cout << "p0: " << p0 << std::endl;
    container[p0] = std::abs(p0);

    BOOST_CHECK(p0 == p1);
    BOOST_CHECK(!(p0 < p1));
    // TODO: BOOST_CHECK(!(p0 > p1));
    BOOST_CHECK(!(p1 < p0));
    BOOST_CHECK(point_hasher(p0) == point_hasher(p1));

    std::cout << "p1: " << p1 << std::endl;
    container[p1] = std::abs(p1);
    BOOST_CHECK(point_hasher(p1) == point_hasher(p4));
    BOOST_CHECK(!(point_hasher(p1) < point_hasher(p4)));
    BOOST_CHECK(!(point_hasher(p4) < point_hasher(p1)));
    container[p4] = std::abs(p4);

    // These tests could legally fail, but if they did it'd be a pretty bad
    // hash function.
    std::cout << "p2: " << p2 << std::endl;
    container[p2] = std::abs(p2);
    BOOST_CHECK(point_hasher(p1) != point_hasher(p2));
    BOOST_CHECK(p1 < p2);

    std::cout << "p3: " << p3 << std::endl;
    container[p3] = std::abs(p3);
    BOOST_CHECK(point_hasher(p1) != point_hasher(p3));
    BOOST_CHECK(p2 < p3);

    // NOTE: if comp(a,b)==true and comp(b,c)==true then comp(a,c)==true
    BOOST_CHECK(p1 < p3);

    std::cout << "p5: " << p5 << std::endl;
    container[p5] = std::abs(p5);
    BOOST_CHECK(!(p2 == p5));
    BOOST_CHECK(p2 < p5);
    // TODO: BOOST_CHECK(!(p2 > p5));
    BOOST_CHECK(!(p5 < p2));
    BOOST_CHECK(point_hasher(p2) != point_hasher(p5));

    BOOST_CHECK(p5 < p3);
    BOOST_CHECK(p5 != p3);

    print_map(container);

    return 0;
}
