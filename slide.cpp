#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <span>
#include <string>

// The example uses std::span to implement some algorithms on contiguous ranges.
using std::span;

template <class T, std::size_t N>
[[nodiscard]] constexpr auto slide(span<T, N> s, std::size_t offset,
                                   std::size_t width)
{
    return s.subspan(offset, offset + width <= s.size() ? width : 0U);
}

// TODO: void print(const auto& seq)
template <class T, std::size_t N> void print(const span<T, N> &seq)
{
    for (const auto &elem : seq) {
        std::cout << elem << ' ';
    }
    std::cout << '\n';
}

void test_slide()
{
    // TODO: cppcoreguidelines-avoid-c-arrays
    constexpr int a[]{0, 1, 2, 3, 4, 5, 6, 7, 8}; // NOLINT
    constexpr int b[]{8, 7, 6};                   // NOLINT
    constexpr std::size_t width{6};

    for (std::size_t offset{};; ++offset) {
        auto s = slide(span{a}, offset, width);
        if (s.empty()) {
            break;
        }
        print(s);
    }
    std::cout << '\n';
}

int main()
{
    test_slide();

    // construction uses aggregate initialization
    std::array<int, 3> a1{
        {1, 2, 3}}; // double-braces required in C++11 prior to
                    // the CWG 1270 revision (not needed in C++11
                    // after the revision and in C++14 and beyond)

    std::array<int, 3> a2 = {1, 2, 3}; // double braces never required after =
    const std::array<std::string, 3> a3 = {std::string("<"), "-", ">"};

    // container operations are supported
    std::sort(a1.begin(), a1.end());
    print(span(a1));
    std::reverse_copy(a2.begin(), a2.end(),
                      std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';

    // ranged for loop is supported
    for (const auto &s : a3) {
        std::cout << s << ' ';
    }
    std::cout << '\n';

    // deduction guide for array creation (since C++17)
    constexpr std::array a4{3.1, 1.2, 4.3}; // -> std::array<double, 3>
    print(span{a4});                        // XXX print(span<double,3>(a4));
}
