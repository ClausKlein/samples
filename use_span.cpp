// A span is a non-owning view over a contiguous sequence of objects, the
// storage of which is owned by some other object.

#define GSL_THROW_ON_CONTRACT_VIOLATION 1
#include <gsl/gsl> // C++14

#include <array> // C++11
#include <span>  // C++20
using gsl::at;
using std::span;

const size_t COUNT(7);

// Pointers should only refer to single objects!
void use(int *ptr, int value) { *ptr = value; }

// Use a gsl::span and range-for:
void f1a()
{
    int arr[COUNT];
    // FIXME gsl::span<int, COUNT> av = arr;
    span<int, COUNT> av = arr;
    int i = 0;
    for (auto &e : av)
        e = i++;
}

// Use gsl::at() for access:
void f2()
{
    int arr[COUNT];
    // FIXME gsl::span<int, COUNT> av = arr;
    span<int, COUNT> av = arr;
    for (int i = 0; i < COUNT; ++i) {
        at(arr, i) = i;
    }
}

// BETTER: use span in the function declaration
void f(span<int> a)
{
    if (a.size() < 2)
        return;

    int n = a[0]; // OK
    if (!n)
        return;

    span<int> q = a.subspan(1); // OK
    f(q);

    if (a.size() < 6)
        return;

    a[4] = 1; // OK

    a[a.size() - 1] = 2; // OK

    use(a.data(), 3); // OK
}

int main()
{
    std::array<int, COUNT> a{{1, 2, 3, 4, 5, 6}};
    f(a);
    f1a();
    f2();
}
