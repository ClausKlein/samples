#include <algorithm> // copy_if and remove_if
#include <iostream>
#include <iterator>
#include <numeric> // iota

#if defined(__cplusplus) && __cplusplus == 201103L

#    include <vector>
using namespace std;
#    warning "using std::vector"
#    define USE_VECTOR 1

#elif __has_include(<unordered_set>) && __cplusplus >= 201402L
#    if __cplusplus == 201402L
#        warning "missing namespace std::erase_if"
#    endif

#    include <unordered_set>

template <class Key, class Predicate>
void erase_if(std::unordered_set<Key> &c, Predicate pred)
{
    for (auto i = c.begin(), last = c.end(); i != last;) {
        if (pred(*i)) {
            i = c.erase(i);
        } else {
            ++i;
        }
    }
}

#endif

inline auto is_odd(int i) -> bool { return (i % 2) == 1; }

auto main() -> int
{
    std::unordered_set<int> c = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // NOTE: Container elements may not be modified
    //  (even by non const iterators) since modification could
    //  change an element's hash and corrupt the container!
    // XXX: std::iota(c.begin(), c.end(), 0);

    //=============================
    // erase all odd numbers from c
    //=============================
    std::cout << "the odd values : ";

#if defined(USE_VECTOR)

    // NOTE: Container elements may not be modified!
    // XXX: c.erase(std::remove_if(c.begin(), c.end(), is_odd), c.end());

    // copy only odd numbers from c
    std::vector<int> v;
    std::copy_if(c.begin(), c.end(), std::back_inserter(v), is_odd);

    std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));

    for (int i : v) {
        c.erase(i); // erase all odd numbers from c
    }

#else

    std::copy_if(c.begin(), c.end(), std::ostream_iterator<int>(std::cout, " "),
                 is_odd);

    erase_if(c, is_odd); // erase all odd numbers from c
#endif

    std::cout << '\n';
    std::cout << "the set contains: ";
    std::copy(c.begin(), c.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';

    return 0;
}

/*** OUTPUT:

the odd values : 9 7 5 3 1
the set contains: 8 6 4 2 0

 ***/
