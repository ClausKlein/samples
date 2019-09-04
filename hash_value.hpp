#include <boost/functional/hash/extensions.hpp>

# 44 "/opt/local/include/boost/functional/hash/extensions.hpp"
namespace foo {

template <class A, class B> std::size_t hash_value(std::pair<A, B> const &v)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, v.first);
    boost::hash_combine(seed, v.second);
    return seed;
}

template <class T> std::size_t hash_value(std::complex<T> const &v)
{
    boost::hash<T> hasher;
    std::size_t seed = hasher(v.imag());
    seed ^= hasher(v.real()) + (seed << 6) + (seed >> 2);
    return seed;
}

# 256 "/opt/local/include/boost/functional/hash/extensions.hpp"
template <class T> struct hash : std::unary_function<T, std::size_t>
{

    std::size_t operator()(T const &val) const
    {
        return boost::hash_value(val);
    }
};

# 316 "/opt/local/include/boost/functional/hash/extensions.hpp"
} // namespace foo
