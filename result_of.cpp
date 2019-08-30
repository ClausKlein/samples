// Below is a program that compiles when the return type is declared using
// decltype, but that fails to compile when using result_of.

// XXX #define BUGBUG

#include <iostream>
#include <string>
#include <type_traits>
#include <utility>

struct eat
{
    template <typename T> eat(T const &) {}
};
struct not_incrementable
{};

// warning: 'auto' type specifier is a C++11 extension [-Wc++11-extensions]
struct inc
{
    template <typename T> auto operator()(T t) const -> decltype(t++)
    {
        return t++;
    }
};

template <typename A>
#ifdef BUGBUG
typename std::result_of<inc(A)>::type // XXX HARD ERROR HERE before c++14!
#else
decltype(std::declval<inc>()(std::declval<A>())) // SFINAE; works with c++11
#endif
try_inc(A a)
{
    return inc()(a);
}

not_incrementable try_inc(eat) { return not_incrementable(); }

int main()
{
    int result = try_inc(-1); // OK

    std::cout << try_inc(3.14) << std::endl; // OK
    std::cout << try_inc('0') << std::endl;  // OK

    not_incrementable y = try_inc(std::string("foo")); // OK, not_incrementable
    (void)y;
}
