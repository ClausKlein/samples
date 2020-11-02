#define USE_BOOST
#ifndef USE_BOOST
#    include <functional>
#else
#    include <boost/functional/hash.hpp>
#endif
#include <iostream>
#include <string>

struct S
{
    S(const std::string &first, const std::string &last)
        : first_name(first), last_name(last)
    {}
    S() : first_name(""), last_name("") {}

    auto operator==(S const &other) const -> bool
    {
        return first_name == other.first_name && last_name == other.last_name;
    }

#ifdef USE_BOOST
    friend std::size_t hash_value(S const &p)
    {
#    ifndef USE_BOOST_HASH
        std::size_t seed = 0;
        boost::hash_combine(seed, p.first_name);
        boost::hash_combine(seed, p.last_name);
        return seed;
#    else // NOTE: this produce the same hash as:
        return boost::hash(p.first_name + p.last_name);
#    endif
    }
#endif

private:
    std::string first_name;
    std::string last_name;
};

#ifndef USE_BOOST
namespace std {
template <> struct hash<S>
{
    using argument_type = S;
    using value_type = std::size_t;

    auto operator()(argument_type const &s) const -> value_type
    {
        value_type const h1(std::hash<std::string>()(s.first_name));
        value_type const h2(std::hash<std::string>()(s.last_name));
        return h1 ^ (h2 << 1);
    }
};
} // namespace std
#endif

auto main() -> int
{
    S d;
    S s("Bender", "Rodriguez");

#ifndef USE_BOOST
    std::hash<S> hash_fn;
#else
    boost::hash<S> hash_fn;
#endif

    std::cout << "hash(d) = 0x" << std::hex << hash_fn(d) << "\n";
    std::cout << "hash(s) = 0x" << std::hex << hash_fn(s) << "\n";
}

/***

claus-macbook-pro:tmp clausklein$ ./hash
hash(d) = 0x28cd94bfde
hash(s) = 0xa6d8044b46f6dbc9
claus-macbook-pro:tmp clausklein$

 ***/
