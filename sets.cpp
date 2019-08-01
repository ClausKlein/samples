// constructing sets

#if (defined(__GNUC__) && (__GNUC__ < 4))
#    include <stdarg.h>
#    include <wchar.h>
int swprintf(wchar_t* ws, size_t n, const wchar_t* format, ...);
int vswprintf(wchar_t* ws, size_t n, const wchar_t* format, va_list arg);
#endif

#include <complex>
#include <iostream>
#include <set>

namespace std
{
template <class T>
bool operator<(const std::complex<T>& lhs, const std::complex<T>& rhs)
{
    return lhs.imag() < rhs.imag() && lhs.real() < rhs.real();
}
} // namespace std

#define USE_COMPLEX
#ifndef USE_COMPLEX
typedef int MyType; // ORIG
#else
typedef std::complex<double> MyType;
#endif // USE_COMPLEX

bool fncomp(const MyType lhs, const MyType rhs)
{
    return lhs < rhs;
}

struct classcomp {
    bool operator()(const MyType& lhs, const MyType& rhs) const
    {
        return lhs < rhs;
    }
};

int main()
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
    MyType s[5] = { MyType(10), MyType(20), MyType(30), MyType(40),
        MyType(50) };
    for (MyType* it = s; it < s + 5; it++) {
        std::cout << *it << std::endl;
    }
    std::set<MyType> second(s, s + 5); // pointers used as iterators
    std::set<MyType> third(second);    // a copy of second
#endif


    std::set<MyType> fourth(third.begin(), third.end()); // iterator ctor.

    std::set<MyType, classcomp> fifth; // class as Compare

    std::set<MyType, bool (*)(MyType, MyType)> sixth(
        fncomp); // function pointer as Compare

    for (std::set<MyType>::iterator it = second.begin(); it != second.end();
         it++) {
        std::cout << *it << std::endl;
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

/***
$ gcc --version
gcc (GCC) 3.4.4

$ make sets -B "CXXFLAGS=-Wall -Wextra -pedantic -DUSE_COMPLEX"
g++ -Wall -Wextra -pedantic -DUSE_COMPLEX    sets.cpp   -o sets

/usr/lib/gcc/i686-pc-cygwin/3.4.4/include/c++/bits/stl_function.h: In member
function `bool std::less<_Tp>::operator()(const _Tp&, const _Tp&) const [with
_Tp = MyType]':
/usr/lib/gcc/i686-pc-cygwin/3.4.4/include/c++/bits/stl_tree.h:871: instantiated
from `std::pair<typename std::_Rb_tree<_Key, _Val, _KeyOfValue, _Compare,
_Alloc>::iterator, bool> std::_Rb_tree<_Key, _Val, _KeyOfValue, _Compare,
_Alloc>::insert_unique(const _Val&) [with _Key = MyType, _Val = MyType,
_KeyOfValue = std::_Identity<MyType>, _Compare = std::less<MyType>, _Alloc =
std::allocator<MyType>]'
/usr/lib/gcc/i686-pc-cygwin/3.4.4/include/c++/bits/stl_tree.h:996: instantiated
from `void std::_Rb_tree<_Key, _Val, _KeyOfValue, _Compare,
_Alloc>::insert_unique(_II, _II) [with _InputIterator = int*, _Key = MyType,
_Val = MyType, _KeyOfValue = std::_Identity<MyType>, _Compare =
std::less<MyType>, _Alloc = std::allocator<MyType>]'
/usr/lib/gcc/i686-pc-cygwin/3.4.4/include/c++/bits/stl_set.h:174: instantiated
from `std::set<_Key, _Compare, _Alloc>::set(_InputIterator, _InputIterator)
[with _InputIterator = int*, _Key = MyType, _Compare = std::less<MyType>,
_Alloc = std::allocator<MyType>]' sets.cpp:36:   instantiated from here
/usr/lib/gcc/i686-pc-cygwin/3.4.4/include/c++/bits/stl_function.h:227: error:
no match for 'operator<' in '__x < __y' make: *** [sets] Error 1

KLEIN_CL@wd002072 /cygdrive/c/temp
$

claus-kleins-macbook-pro:cpp clausklein$ g++-mp-4.6 -std=c++0x -Wall -Wextra
-Wsign-compare -Wconversion -Wpointer-arith -Wcomment -Wundef -Wold-style-cast
-Weffc++     sets.cpp   -o sets
***/
