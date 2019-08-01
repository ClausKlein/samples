#undef USE_BOOST
#ifndef USE_BOOST
    #include <functional>
#else
    #include <boost/functional/hash.hpp>
#endif
#include <string>
#include <iostream>

struct S
{
    std::string first_name;
    std::string last_name;

    S() : first_name(""), last_name("") {}

    bool operator==(S const& other) const
    {
        return first_name == other.first_name && last_name == other.last_name;
    }

#ifdef USE_BOOST
    friend std::size_t hash_value(S const& p)
    {
#ifndef USE_BOOST_HASH
        std::size_t seed = 0;
        boost::hash_combine(seed, p.first_name);
        boost::hash_combine(seed, p.last_name);
        return seed;
#else   //NOTE: this produce the same hash as:
        return boost::hash(p.first_name + p.last_name);
#endif

    }
#endif
};

#ifndef USE_BOOST
namespace std
{
template<>
struct hash<S>
{
    typedef S argument_type;
    typedef std::size_t value_type;

    value_type operator()(argument_type const& s) const
    {
        value_type const h1(std::hash<std::string>()(s.first_name));
        value_type const h2(std::hash<std::string>()(s.last_name));
        return h1 ^ (h2 << 1);
    }
};
}
#endif


int main()
{
    S d;
    S s;
    s.first_name = "Bender";
    s.last_name =  "Rodriguez";

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
