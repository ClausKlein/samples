#include <iostream>
#include <string>

// Declaring a string of fixed size

// In c++17, there will be std::string_view which offers the same
// (immutable) interface as std::string.

// In the meantime, you can wrap a char array and add whatever services
// to it you choose, eg:
template <std::size_t N> struct immutable_string
{
    using ref = const char (&)[N + 1];
    constexpr immutable_string(ref s) : s(s) {}

    [[nodiscard]] constexpr auto begin() const { return (const char *)s; }
    [[nodiscard]] constexpr auto end() const { return begin() + size(); }
    [[nodiscard]] constexpr std::size_t size() const { return N; }
    [[nodiscard]] constexpr ref c_str() const { return s; }

private:
    ref s;

    friend std::ostream &operator<<(std::ostream &os, immutable_string s)
    {
        return os.write(s.c_str(), s.size());
    }
};

template <std::size_t NL, std::size_t NR>
std::string operator+(immutable_string<NL> l, immutable_string<NR> r)
{
    std::string result;
    result.reserve(l.size() + r.size());
    result.assign(l.begin(), l.end());
    result.insert(result.end(), r.begin(), r.end());
    return result;
}

template <std::size_t N> auto make_immutable_string(const char (&s)[N])
{
    return immutable_string<N - 1>(s);
}

int main()
{
    auto x = make_immutable_string<13>("hello, world");
    std::cout << x << std::endl;

    auto a = make_immutable_string("foo");
    auto b = make_immutable_string("bar");
    auto c = a + b;
    std::cout << c << std::endl;
}
