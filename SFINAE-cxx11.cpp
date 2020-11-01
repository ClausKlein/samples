//
// see https://de.wikipedia.org/wiki/Substitution_failure_is_not_an_error
// and http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3462.html
//

#include <iostream>
#include <type_traits>

// warning: default template arguments for a function template are a C++11
// extension
template <typename T,
          typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
void f(T Value)
{
    std::cout << "Int: " << Value << '\n';
}

template <typename T, typename std::enable_if<std::is_floating_point<T>::value,
                                              int>::type = 0>
void f(T Value)
{
    std::cout << "Float: " << Value << '\n';
}

template <typename T,
          typename std::enable_if<std::is_pointer<T>::value, int>::type = 0>
void f(T Value)
{
    std::cout << "Pointer: " << Value << '\n';
}

auto main() -> int
{
    int n = 42;

    f(n);   // Int: 42
    f(2.7); // Float: 2.7
    f(&n);  // Pointer: 002DFA14
}
