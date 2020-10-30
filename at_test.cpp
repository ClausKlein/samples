// A span is a non-owning view over a contiguous sequence of objects, the
// storage of which is owned by some other object.

#include <gsl-lite/gsl-lite.hpp>

/***
Install the project...
-- Install configuration: "Debug"
-- Up-to-date: /usr/local/lib/cmake/gsl-lite/gsl-lite-targets.cmake
-- Up-to-date: /usr/local/lib/cmake/gsl-lite/gsl-lite-config.cmake
-- Up-to-date: /usr/local/lib/cmake/gsl-lite/gsl-lite-config-version.cmake
-- Up-to-date: /usr/local/include/gsl/gsl-lite.hpp
-- Up-to-date: /usr/local/include/gsl-lite/gsl-lite.hpp
***/

#include <array>
#include <iostream>
#include <vector>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
/***
Install the project...
-- Install configuration: ""
-- Installing: /usr/local/include/doctest/doctest.h
-- Installing: /usr/local/lib/cmake/doctest/doctestConfig.cmake
-- Installing: /usr/local/lib/cmake/doctest/doctestConfigVersion.cmake
-- Installing: /usr/local/lib/cmake/doctest/doctest.cmake
-- Installing: /usr/local/lib/cmake/doctest/doctestAddTests.cmake
-- Installing: /usr/local/lib/cmake/doctest/doctestTargets.cmake
***/

//XXX namespace gsl = ::gsl_lite; // convenience alias

using gsl::at;
using gsl::span;

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
