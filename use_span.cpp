// A span is a non-owning view over a contiguous sequence of objects, the
// storage of which is owned by some other object.

// XXX #include <gsl/gsl> // C++14
#include <gsl-lite/gsl-lite.hpp>

#include <array> // C++11
#include <iomanip>
#include <iostream>
#include <string_view> // C++17
#include <vector>

// XXX #include <span> // C++20 only
#define TCB_SPAN_THROW_ON_CONTRACT_VIOLATION
#include <tcb/span.hpp> // C++14

using gsl::at;
using tcb::dynamic_extent;
using tcb::span;

constexpr size_t COUNT(7);

template <class T, std::size_t N> void print(const span<T, N> &seq)
{
    if (seq.extent != writable_bytes) {
        std::cout << "static extent!\n";
    }

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
    // FIXME gsl::span<int> av = arr;
    span av{arr};   // NOTE: static extent
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
    // XXX gsl::span<size_t, COUNT> av = arr;
    span av{arr};   // NOTE: static extent
    for (size_t i = av.size(); i > 0; --i) {
        // XXX gsl::at(arr, i) = i;  // NOTE: this throws! CK
        // XXX av[av.size()] = i;  // NOTE: this throws! CK
        av[av.size() - i] = i;
    }
    print(av);
}

// BETTER: use span in the function declaration
void usage(span<int> a)
{
    if (a.size() < 2) {
        return;
    }

    print(a);
    int n = a[0]; // OK
    if (n == 0) {
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

void dynamic_demo()
{
    auto print = [](std::string_view const name, size_t ex) {
        std::cout << name << ", ";
        if (ex == dynamic_extent) {
            std::cout << "dynamic extent\n";
        } else {
            std::cout << "static extent = " << ex << '\n';
        }
    };

    {
        int a[]{1, 2, 3, 4, 5};

        span span1{a};
        print("span1", span1.extent);

        span<int, dynamic_extent> span2{a};
        print("span2", span2.extent);
    }

    {
        std::array ar{1, 2, 3, 4, 5};
        span span3{ar};
        print("span3", span3.extent);
    }

    {
        std::vector v{1, 2, 3, 4, 5};
        span span4{v};
        print("span4", span4.extent);
    }
}

void last_demo(span<const int> span)
{
    auto print = [](std::string_view const title, auto const &container) {
        std::cout << title << "[" << std::size(container) << "]{ ";
        for (auto const &elem : container) {
            std::cout << elem << ", ";
        }
        std::cout << "};\n";
    };

    print("span: ", span);

    // FIXME span<const int, 3> span_last = span.last<3>();
    auto span_last = span.last<3>();
    print("span.last<3>(): ", span_last);

    // FIXME span<const int, dynamic_extent> span_last_dynamic = span.last(2);
    auto span_last_dynamic = span.last(2);
    print("span.last(2):   ", span_last_dynamic);
}

void print(float const x, span<const std::byte> const bytes)
{
    std::cout << std::setprecision(6) << std::setw(8) << x << " = { "
              << std::hex << std::showbase;
    for (auto const b : bytes) {
        std::cout << std::to_integer<int>(b) << ' ';
    }
    std::cout << std::dec << "}\n";
}

void as_byte_demo()
{
    using namespace tcb;

    /* mutable */ float data[1]{3.141592f};

    auto const const_bytes = as_bytes(span{data});

    print(data[0], const_bytes);

    auto const writable_bytes = as_writable_bytes(span{data});

    // Change the sign bit that is the MSB (IEEE 754 Floating-Point Standard).
    writable_bytes[3] |= std::byte{0b1000'0000};

    print(data[0], writable_bytes);
}

int main(int argc, char *argv[])
{
#ifdef USE_ARGV
    constexpr size_t ARGC(1);
    if (argc != ARGC) {
        std::cerr << "Usage: " << *argv << " <port>\n";
        return 0;
    }
#endif

    try {

#ifdef USE_ARGV
        gsl::span<char *> args{argv, ARGC};
        char *portstr = gsl::at(args, 1);
        char *endptr = nullptr;
        int port = static_cast<int>(std::strtol(portstr, &endptr, 10));
        if (*endptr != '\0') {
            std::cerr << "invalid argument: " << endptr
                      << " int value expected!" << std::endl;
            return EXIT_FAILURE;
        }

        // XXX asio::io_context io_context;
        // XXX server s(io_context, port);
        // XXX io_context.run();
#else
        int port = 123;
#endif

        std::array<int, COUNT> a{1, 2, 3, 4, 5, port};
        usage(a);

        std::vector<int> v{1, 2, 3, 4, 5};
        v.push_back(port);
        v.resize(8);
        v[7] = 8; // insert new value
        span sv{v};
        if (v.size() == sv.size()) {
            sv[6] = 7;
            // XXX sv[9] = 0; // NOTE: this throws! CK
            print(sv);
            last_demo(sv);
        }

        f1();
        f2();
        as_byte_demo();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
