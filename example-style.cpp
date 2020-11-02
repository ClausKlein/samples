#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

#ifdef DEBUG
#    define TRACE
#endif

namespace testing {
using namespace std::chrono;
using namespace std::string_literals;

auto c = 299'792'458; // m/s2
auto q2 = 0b0000'1111'0000'0000;
auto ss_number = 123'456'7890;

std::string stringWithNull = "abc\0\0def"s;

auto hello = "Hello!"s; // a std::string
auto world = "world";   // a C-style string
auto interval = 100ms;  // using <chrono>

const double some_value(3.14);

enum
{
    good,
    better,
    amazing
};

struct Foo
{
    Foo() = default;
    Foo(int a, int b, int c) : bam(a), blah(b), baz(c) {}
    auto operator[](size_t) -> int &; // OK
    // ...
    int bam{0};  // NOLINT
    int blah{0}; // NOLINT
    int baz{0};  // NOLINT
};

static void something() {}
static void something_else() {}

auto foo(int x) -> double
{
    if (0 < x) {
        // ...
    }

    switch (x) {
    case 0:
        // ...
        break;
    case amazing:
        // ...
        break;
    default:
        // ...
        break;
    }

    if (0 < x)
        ++x;

    if (x < 0)
        something();
    else
        something_else();

    return some_value;
}

} // namespace testing

#if __cpp_lib_string_view
auto doSomething(const std::string_view &sv) -> int
{
    // Terminators never need 'something' done to them because ...
    if (sv.empty())
        return 0;

    // We conservatively avoid transforming instructions with multiple uses
    // because goats like cheese.
    if (sv == "bad")
        return 0;

    // This is really just here for example.
    if (!sv.find("abd"))
        return 0;

    // ... some long code ....
    return 1;
}
#endif

auto main() -> int
{
    int data[] = {0, 1, 1, 2, 3, 5, 8, 13, 21};

    testing::Foo a{1, 2, 3};
    std::vector<testing::Foo> foo{{1, 2, 0}, {2, 3, 0}};

    std::sort(foo.begin(), foo.end(),
              [&](testing::Foo a, testing::Foo b) -> bool {
                  if (a.blah < b.blah)
                      return true;
                  if (a.baz < b.baz)
                      return true;
                  return a.bam < b.bam;
              });

    testing::foo(testing::amazing);

    std::cout << "This is code with .clang-format -style=Mozilla (from cmake "
                 "project)!\n";
}
