// A span is a non-owning view over a contiguous sequence of objects, the
// storage of which is owned by some other object.

#include <gsl/gsl> // C++14

#include <array> // C++11
#include <iostream>
#include <span> // C++20

using gsl::at;
using std::span;

constexpr size_t COUNT(7);

template <class T, std::size_t N> void print(const span<T, N> &seq)
{
    for (const auto &elem : seq) {
        std::cout << elem << ' ';
    }
    std::cout << '\n';
}

// Pointers should only refer to single objects!
void use(int *ptr, int value) { *ptr = value; }

// Use a gsl::span and range-for:
void f1()
{
    int arr[COUNT];
    // TODO: span<int, COUNT> av = arr; // C++20 only
    // FIXME gsl::span<int> av = arr;
    span av{arr};
    int i = 0;
    for (auto &e : av) {
        e = i++;
    }
    print(av);
}

// Use gsl::at() for access:
void f2()
{
    size_t arr[COUNT];
    // XXX span<size_t, COUNT> av = arr; // C++20 only
    gsl::span<size_t, COUNT> av = arr;
    for (size_t i = av.size(); i > 0; --i) {
        // FIXME: gsl::at(arr, i) = i;  // NOTE: terminates! CK
        av[av.size() - i] = i;
    }
    // XXX print(av);
}

// BETTER: use span in the function declaration
void usage(span<int> a)
{
    if (a.size() < 2) {
        return;
    }

    print(a);
    int n = a[0]; // OK
    if (!n) {
        return;
    }

    span<int> q = a.subspan(1); // OK
    usage(q);

    if (a.size() < 6) {
        return;
    }

    a[4] = 1;            // OK
    a[a.size() - 1] = 2; // OK
    use(a.data(), 3);    // OK
    print(a);
}

int main(int argc, char *argv[])
{
    constexpr size_t ARGC(2);
    if (argc != ARGC) {
        std::cerr << "Usage: " << *argv << " <port>\n";
        return 0;
    }

    try {
        gsl::span<char *> args = {argv, ARGC};
        auto portstr = gsl::at(args, 1);
        char *endptr;
        int port = std::strtol(portstr, &endptr, 10);
        if (*endptr != '\0') {
            std::cerr << "invalid argument: " << endptr
                      << " int value expected!" << std::endl;
            return EXIT_FAILURE;
        }

        // XXX asio::io_context io_context;
        // XXX server s(io_context, port);
        // XXX io_context.run();

        std::array<int, COUNT> a{{1, 2, 3, 4, 5, port}};
        usage(a);
        print(span(a));

        f1();
        f2();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
