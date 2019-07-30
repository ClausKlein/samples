// see
// https://en.cppreference.com/w/cpp/language/ub
// https://en.wikipedia.org/wiki/Fermat%27s_Last_Theorem
//
#include <iostream>

bool fermat()
{
    const size_t MAX = 1531;
    size_t a = 1, b = 1, c = 1;

    //
    // Endless loop with no side effects (XXX) is Undefined Behaviour (UB)
    //
    while (true) {
        if (((a * a * a) == ((b * b * b) + (c * c * c)))) {
            //TODO std::cout << a << ',' << b << ',' << c << std::endl; //XXX
            return true;
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
            //TODO break; // XXX
        }
    }
    return false;
}

int main()
{
    if (fermat())
        std::cout << "Fermat's Last Theorem has been disproved.\n";
    else
        std::cout << "Fermat's Last Theorem has not been disproved.\n";
}
// Claus-MBP:samples clausklein$
// g++ -O2 endlessloop.cpp -o endlessloop
// time timeout 100 ./endlessloop
// Fermat's Last Theorem has been disproved.
//
// real	0m0.029s
// user	0m0.004s
// sys	0m0.007s
// Claus-MBP:samples clausklein$
