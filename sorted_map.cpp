// from:
// http://en.cppreference.com/w/cpp/container/map/map
//
#include <iomanip>
#include <iostream>
#include <map>
#include <string>

template <typename Map> void print_map(Map& m)
{
    std::cout << '{';
    for (auto& p : m) {
        std::cout << p.first << ':' << p.second << ' ';
    }
    std::cout << "}\n";
}

struct Point {
    double x, y;
};
std::ostream& operator<<(std::ostream& stream, const Point& p)
{
    stream << '{';
    stream << p.x << ',' << p.y;
    stream << '}';
    return stream;
}

struct PointCmp {
    bool operator()(const Point& lhs, const Point& rhs) const
    {
        return lhs.x < rhs.x; // NB. intentionally ignores y
    }
};

int main()
{
    // Custom key class option 1:
    // Use a comparison struct
    std::map<Point, double, PointCmp> mag = { { { 5, -12 }, 13 },
        { { 3, 4 }, 5 }, { { -8, -15 }, 17 } };
    std::cout << "The magnitude lookup table is:\n";
    print_map(mag);

    // Custom key class option 2:
    // Use a comparison lambda
    // This lambda sorts points according to their magnitudes.
    // NOTE: these magnitudes are taken from the local map mag!
    auto cmpLambda = [&mag](const Point& lhs, const Point& rhs) {
        return mag[lhs] > mag[rhs];
    };
    // You could also use a lambda that is not dependent on local variables,
    // like this: auto cmpLambda = [](const Point &lhs, const Point &rhs) {
    // return lhs.y < rhs.y; };
    std::map<Point, double, decltype(cmpLambda)> magy(cmpLambda);

    // Various ways of inserting elements:
    magy.insert(std::pair<Point, double>({ 5, -12 }, 13));
    magy.insert({ { 3, 4 }, 5 });
    magy.insert({ Point { -8.0, -15.0 }, 17 });
    magy.insert({ Point { -0.8, -15.0 }, 0 });

    std::cout << "Sorted with lockup from mag table:\n";
    for (auto p : magy)
        std::cout << "The magnitude of key " << p.first << " is " << p.second
                  << '\n';

    return 0;
}

/***
klein_cl@WD002402 ~/workspace/cpp
g++ -std=c++11    sorted_map.cpp   -o sorted_map
$ ./sorted_map
The magnitude lookup table is:
{{-8,-15}:17 {3,4}:5 {5,-12}:13 }
Sorted with lockup from mag table:
The magnitude of key {-8,-15} is 17
The magnitude of key {5,-12} is 13
The magnitude of key {3,4} is 5
The magnitude of key {-0.8,-15} is 0
 ***/
