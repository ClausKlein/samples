//
// This example demonstrates the difference between clock() time and real time
//
#include <atomic>
#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

// the function f() does some time-consuming work
void f()
{
    static std::atomic<size_t> d(0);

    for (size_t n = 0; n < 10000; ++n) {
        for (size_t m = 0; m < 10000; ++m) {
            d = d + (d * n * m);
        }
    }
}

auto main() -> int
{
    using namespace std::chrono;

    std::clock_t c_start = std::clock();
    auto t_start = high_resolution_clock::now();
    std::thread t1(f);
    std::thread t2(f); // f() is called on two threads
    t1.join();
    t2.join();
    std::clock_t c_end = std::clock();
    auto t_end = high_resolution_clock::now();

    std::cout << "CPU time used: "
              << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n";
    std::cout << "Wall clock time passed: "
              << duration_cast<milliseconds>(t_end - t_start).count()
              << " ms\n";
}

/***
Claus-MBP:samples clausklein$ time ./testclock
CPU time used: 11656.6 ms
Wall clock time passed: 6139 ms

real	0m6.158s
user	0m11.597s
sys	0m0.065s
Claus-MBP:samples clausklein$
 ***/
