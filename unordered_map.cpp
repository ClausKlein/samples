#include <bitset>
#include <functional> // hash()
#include <string>
#include <unordered_map>
#include <vector>
// FIXME #include <iomanipulator>    //hex
#include <iostream>

struct Key {
    std::string title;
    std::string first;
    std::string second;
};

// TODO what is the right has?
// see too
// file:///opt/local/share/doc/boost/doc/html/unordered/hash_equality.html

/***
 Tip: When writing a hash function, first look at how the equality function
 works. Objects that are equal **must** generate the same hash value. When
 objects are *not* equal they *should* generate different hash values. In this
 object equality was based just on the id so the hash function only hashes the
 id. If it was based on the object's name and author then the hash function
 should take them into account.
 ***/
struct KeyHash {
    std::size_t operator()(const Key& k) const
    {
#if 1
        return std::hash<std::string>()(k.title)
            ^ ((std::hash<std::string>()(k.first) << 1)
                  ^ (std::hash<std::string>()(k.second) << 2));
#else
        return std::hash<std::string>()(k.title + k.first + k.second);
#endif
    }
};

struct KeyEqual {
    bool operator()(const Key& lhs, const Key& rhs) const
    {
        return lhs.title == rhs.title && lhs.first == rhs.first
            && lhs.second == rhs.second;
    }
};

std::ostream& operator<<(std::ostream& stream, const Key& k)
{
    stream << k.title;
    stream << ' ';
    stream << k.first;
    stream << ' ';
    stream << k.second;
    return stream;
}


int main()
{
    // default constructor: empty map
    std::unordered_map<std::string, std::string> m1;

    // list constructor
    std::unordered_map<int, std::string> m2 = {
        { 1, "foo" },
        { 3, "bar" },
        { 2, "baz" },
    };

    // copy constructor
    std::unordered_map<int, std::string> m3 = m2;

    // move constructor
    std::unordered_map<int, std::string> m4 = std::move(m3);

    for (const auto& p : m4) {
        std::cout << p.first << " => " << p.second << '\n';
    }

    // range constructor
    std::vector<std::pair<std::bitset<8>, int>> v = { { 0x12, 1 },
        { 0x01, -1 } };
    std::unordered_map<std::bitset<8>, double> m5(v.begin(), v.end());

    //
    // constructor for a custom type
    //
    std::unordered_map<Key, std::string, KeyHash, KeyEqual> m6 = {
        { { "", "", "" }, "default" }, { { "Dr", "Peter", "Pan" }, "yet" },
        { { "Mr", "John", "Doe" }, "another" },
        { { "Mrs", "Mary", "Sue" }, "example" }
    };

    for (const auto& p : m6) {
        std::cout << p.first << " => " << p.second;
        std::cout << "; hash=0x" << std::hex << KeyHash()(p.first) << '\n';
    }
}
