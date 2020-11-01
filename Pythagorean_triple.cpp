// see
// https://en.cppreference.com/w/cpp/language/ub
// https://en.wikipedia.org/wiki/Pythagorean_triple
//
#include <iostream>

auto Pythagorean_triple() -> bool
{
    const size_t MAX = 1000;
    size_t a = 1, b = 1, c = 1;
    // Endless loop with no side effects is UB
    while (true) {
        if (((a * a) == ((b * b) + (c * c)))) {
            std::cout << a << ',' << b << ',' << c << std::endl;
            // FIXME if (c >= 975) return true;
        }
        a++;
        if (a > MAX) {
            a = 1;
            b++;
        }
        if (b > MAX) {
            b = 1;
            c++;
        }
        if (c > MAX) {
            c = 1;
            break; // XXX
        }
    }
    return false;
}

auto main() -> int
{
    if (Pythagorean_triple())
        std::cout << "done.\n";
    else
        std::cout << "break.\n";
}
