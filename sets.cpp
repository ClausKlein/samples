// constructing sets

#include <complex>
#include <iostream>
#include <set>

// warning: modification of 'std' namespace can result in undefined behavior
// NOLINTNEXTLINE(cert-dcl58-cpp)
namespace std {
template <class T>
auto operator<(const std::complex<T> &lhs, const std::complex<T> &rhs) -> bool
{
    return lhs.imag() < rhs.imag() && lhs.real() < rhs.real();
}
} // namespace std

#define USE_COMPLEX
#ifndef USE_COMPLEX
typedef int MyType; // ORIG
#else
using MyType = std::complex<double>;
#endif // USE_COMPLEX

auto fncomp(const MyType lhs, const MyType rhs) -> bool { return lhs < rhs; }

struct classcomp
{
    auto operator()(const MyType &lhs, const MyType &rhs) const -> bool
    {
        return lhs < rhs;
    }
};

auto main() -> int
{
    std::set<MyType> first; // empty set of MyType

#if 0
    // Initializer list constructor
    typedef std::set<MyType, classcomp> mySetType;
    mySetType s  = { MyType(10), MyType(20), MyType(30), MyType(40), MyType(50) };
    for (mySetType::iterator it = s.begin(); it != s.end(); it++)
    {
        std::cout << *it << std::endl;
    }
    mySetType second(s.begin(), s.end());   // Iterator constructor
    mySetType third(second);                // a copy of second
#else
    MyType s[5] = {MyType(10), MyType(20), MyType(30), MyType(40), MyType(50)};
    for (MyType *it = s; it < s + 5; it++) {
        std::cout << *it << std::endl;
    }
    std::set<MyType> second(s, s + 5); // pointers used as iterators
    std::set<MyType> third(second);    // a copy of second
#endif

    std::set<MyType> fourth(third.begin(), third.end()); // iterator ctor.

    std::set<MyType, classcomp> fifth; // class as Compare

    std::set<MyType, bool (*)(MyType, MyType)> sixth(
        fncomp); // function pointer as Compare

    for (auto it : second) {
        std::cout << it << std::endl;
    }

    std::complex<double> A(2.0, 1.0);
    std::complex<double> B(1);
    if (A < B) {
        A = B;
    }
    std::cout << A << std::endl;
    std::cout << B << std::endl;

    return 0;
}
