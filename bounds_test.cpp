//
// see http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rsl-bounds
//
// Reason Read or write beyond an allocated range of elements typically leads to
// bad errors, wrong results, crashes, and security violations.
//
// clang++ bounds_test.cpp -o bounds_test -std=c++17

#include <algorithm>
#include <array>
#include <iterator> //use std::size(); NOTE: c++17 only
#include <string>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("bad example: using memset() to fill data")
{
    std::array<int, 10> a, b;
    memset(a.data(), 0xff,
           10); // BAD, and contains a length error (length = 10 * sizeof(int))
    auto res = memcmp(
        a.data(), b.data(),
        10); // BAD, and contains a length error (length = 10 * sizeof(int))

    if (res == 0) {
        // XXX CHECK(a == b); NOTE: this would fail! CK
        /* ... */
    }
}

TEST_CASE("good example: use std::fill to fill data")
{
    std::array<int, 10> a, b, c{}; // c is initialized to zero
    a.fill(0);
    std::fill(b.begin(), b.end(), 0); // std::fill()
    // FIXME: std::fill(b, 0);        // std::fill() + Ranges TS
    // error: no matching function for call to 'fill'

    CHECK(a == b);
}

// bounds error abound
TEST_CASE("bad example: using memcpy() or memcmp()")
{
    int i[3];
    int j[3];

    // ...
    // sizeof would properly address the bounds issues below, however I believe
    // it would benefit the developer to avoid using these functions altogether.
    // TODO: memset(i, 1, sizeof(i)); // XXX

    memset(i, 1, std::size(i)); // BAD, note: min c++17 needed!
    memcpy(j, i, 3);            // BAD
    if (memcmp(i, j, 3)) {}     // BAD seems equal ...

    // XXX NOTE: this would fail! CK
    // XXX CHECK(std::equal(std::begin(i), std::end(i), std::begin(j)));
}

// better alternatives
TEST_CASE("good example: use std::copy() and std::equal()")
{
    int i[3]{};
    int j[3]{};

    // ...

    std::fill(std::begin(i), std::end(i), 1);
    std::copy(std::begin(i), std::end(i), std::begin(j));

    CHECK(std::equal(std::begin(i), std::end(i), std::begin(j)));
}
