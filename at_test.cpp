// A span is a non-owning view over a contiguous sequence of objects, the
// storage of which is owned by some other object.

#define GSL_THROW_ON_CONTRACT_VIOLATION 1
#include <gsl/gsl-lite.hpp>

/***
Install the project...
/opt/local/bin/cmake -P cmake_install.cmake
-- Install configuration: "Debug"
-- Installing: /usr/local/lib/cmake/gsl-lite/gsl-lite-targets.cmake
-- Installing: /usr/local/lib/cmake/gsl-lite/gsl-lite-config.cmake
-- Installing: /usr/local/lib/cmake/gsl-lite/gsl-lite-config-version.cmake
-- Up-to-date: /usr/local/include
-- Installing: /usr/local/include/gsl
-- Installing: /usr/local/include/gsl/gsl
-- Installing: /usr/local/include/gsl/gsl-lite-vc6.hpp
-- Installing: /usr/local/include/gsl/gsl-lite.h
-- Installing: /usr/local/include/gsl/gsl-lite.hpp
-- Installing: /usr/local/include/gsl.h
-- Installing: /usr/local/include/gsl.hpp
Claus-MBP:build clausklein$
***/

#include <array>
#include <iostream>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

using gsl::at;
using gsl::span;

#ifndef GSL_THROW_ON_CONTRACT_VIOLATION
#    define GSL_THROW_ON_CONTRACT_VIOLATION 1
#    warning missing GSL_THROW_ON_CONTRACT_VIOLATION defined!
#endif

TEST_CASE("at(): Terminates access to non-existing gsl::span elements")
{
    int arr[] = {1, 2, 3, 4};
    span<int> a(arr);

    CHECK(a.size() == 4);
    CHECK_THROWS_AS(at(a, 4), const std::exception &);
}
// FATAL ERROR: test case CRASHED: SIGABRT - Abort (abnormal termination) signal

TEST_CASE("at(): Terminates access to non-existing C-array elements")
{
    int a[4] = {1, 2, 3, 4};

    CHECK_THROWS_AS(at(a, 4), const std::exception &);
}
// FATAL ERROR: test case CRASHED: SIGABRT - Abort (abnormal termination) signal

TEST_CASE("at(): Terminates access to non-existing std::array elements (C++11)")
{
    std::array<int, 4> a{{1, 2, 3, 4}};

    CHECK(a.size() == 4);
    CHECK_THROWS_AS(a.at(4), const std::exception &);

    CHECK_THROWS_AS(at(a, 4), const std::exception &);
}
// FATAL ERROR: test case CRASHED: SIGABRT - Abort (abnormal termination) signal

TEST_CASE("at(): Terminates access to non-existing std::vector element (C++98)")
{
    std::vector<int> a{{1, 2, 3, 4}};

    CHECK(a.size() == 4);
    CHECK_THROWS_AS(a.at(4), const std::exception &);

    CHECK_THROWS_AS(at(a, 4), const std::exception &);
}
// FATAL ERROR: test case CRASHED: SIGABRT - Abort (abnormal termination) signal
