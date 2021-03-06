#include <cstdint>
#include <functional>
#include <iostream>
#include <set>

namespace MySpace {
struct MyStruct
{
    uint32_t x;
    int32_t y;
};

auto operator<(const MyStruct &lhs, const MyStruct &rhs) -> bool
{
    return (lhs.x < rhs.x) || ((lhs.x == rhs.x) && (lhs.y < rhs.y));
}
} // namespace MySpace

#define USE_MYCOMP
#ifdef USE_MYCOMP
struct MyComp
{
    auto operator()(const MySpace::MyStruct &lhs,
                    const MySpace::MyStruct &rhs) const -> bool
    {
        // XXX return (lhs.x < rhs.x) || ((lhs.x == rhs.x) && (lhs.y < rhs.y));
        return (lhs < rhs);
    }
};
#endif

auto main() -> int
{

#ifdef USE_MYCOMP
    std::set<MySpace::MyStruct, MyComp> s;
#else
    std::set<MySpace::MyStruct> s;
#endif
    MySpace::MyStruct ms1 = {5, -5};
    MySpace::MyStruct ms2 = {10, 20};
    MySpace::MyStruct ms3 = {10, 20};
    MySpace::MyStruct ms4 = {10, 30};

#ifdef USE_MYCOMP
    std::pair<std::set<MySpace::MyStruct, MyComp>::iterator, bool> result;
#else
    std::pair<std::set<MySpace::MyStruct>::iterator, bool> result;
#endif

    s.insert(ms1);
    result = s.insert(ms2);
    std::cout << "inserted ms2: " << std::boolalpha << result.second
              << std::endl;
    result = s.insert(ms3);
    std::cout << "inserted ms3: " << std::boolalpha << result.second
              << std::endl;
    result = s.insert(ms4);
    std::cout << "inserted ms4: " << std::boolalpha << result.second
              << std::endl;

#ifdef USE_MYCOMP
    for (auto it : s)
#else
    for (std::set<MySpace::MyStruct>::iterator it = s.begin(); it != s.end();
         it++)
#endif
    {
        std::cout << "it->x: " << it.x << "it->y: " << it.y << std::endl;
    }

#ifdef USE_MYCOMP
    auto loc = s.find(ms2);
#else
    std::set<MySpace::MyStruct>::iterator loc = s.find(ms2);
#endif
    if (loc != s.end()) {
        std::cout << "loc->x: " << loc->x << "loc->y: " << loc->y << std::endl;
    }

    return 0;
}
