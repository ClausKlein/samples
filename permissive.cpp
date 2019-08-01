//
// The typename is required by the standard. Template compilation requires a
// two step verification. During the first pass the compiler must verify the
// template syntax without actually supplying the type substitutions.
//
// Why is this necessary? Before substituing the actual KEY and VALUE types,
// the compiler cannot guarantee that the template is not specialized and that
// the specialization is not redefining the iterator keyword as something
// else.
//
//
// from
// http://stackoverflow.com/questions/642229/why-do-i-need-to-use-typedef-typename-in-g-but-not-vs
//

#include <iostream>
#include <map>

template <class T> struct remove_cv {
    typedef typename std::remove_volatile<
        typename std::remove_const<T>::type>::type type;
    // FIXME typedef std::remove_volatile<std::remove_const<T>::type>::type
    // type;
};

template <class T> struct remove_const {
    typedef T type;
};
template <class T> struct remove_const<const T> {
    typedef T type;
};

template <class T> struct remove_volatile {
    typedef T type;
};
template <class T> struct remove_volatile<volatile T> {
    typedef T type;
};

template <typename KEY, typename VALUE>
bool isInMap(const std::map<KEY, VALUE>& container, const KEY& key)
{
    typename std::map<KEY, VALUE>::const_iterator iter = container.find(
        key); // error: missing 'typename' prior to dependent type name
    return iter != container.end();
}

class X {
};

template <typename T> struct Test {
    typedef T value;
};

template <> struct Test<X> {
    static int value;
};
int Test<X>::value = 0;

template <typename T> int f(T const&)
{
    return Test<T>::value; // during first pass, Test<T>::value is interpreted
                           // as a value
}

int main()
{
    typedef std::remove_cv<const int>::type type1;
    typedef std::remove_cv<volatile int>::type type2;
    std::cout << "test1 "
              << (std::is_same<int, type1>::value ? "passed" : "failed")
              << '\n';
    std::cout << "test2 "
              << (std::is_same<int, type2>::value ? "passed" : "failed")
              << '\n';

    // FIXME f(5);           // error: missing 'typename' prior to dependent
    // type name
    // TODO int i = f(5);    // error: missing 'typename' prior to dependent
    // type name

    X x;
    return f(x); // compiles fine f: Test<T>::value is an integer
}
