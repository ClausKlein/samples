// build:
// g++ -std=c++11 -Wall -Wextra -o timevalue timevalue.cpp
//
#define __STDC_LIMIT_MACROS
#include <stdint.h>
#include <sys/types.h>  //BYTE_ORDER GHS PPC: #define __BIG_ENDIAN__
#include <assert.h>

#include <iostream>
#include <iomanip>

using namespace std;

union time_value {
    int64_t sec;

#if defined(__LITTLE_ENDIAN__) || defined(__i386) || defined(_WIN32)
#warning LittleEndian
    struct {
        uint32_t low;
        uint32_t high;
    } ul;
    struct {
        uint16_t word1;
        uint16_t word2;
        uint16_t word3;
        uint16_t word4;
    } uw;
    struct {
        uint8_t byte1;
        uint8_t byte2;
        uint8_t byte3;
        uint8_t byte4;
        uint8_t byte5;
        uint8_t byte6;
        uint8_t byte7;
        uint8_t byte8;
    } ub;
#else
#error BigEndian
    struct {
        uint32_t high;
        uint32_t low;
    } ul;
#endif

} test;

int main() {
    const int64_t tests[16] = {
        INT64_MIN, -4294967296,
        INT32_MIN, INT16_MIN, -1LL, 0LL, 1LL,  INT16_MAX, 439041101LL, INT32_MAX,
        0x80000000, 0xffffffff, 0x100000000, 0x1ffffffff, 0x1a2b3c4d5e6f7788, INT64_MAX
    };

    int64_t sec = 0LL;
    uint32_t high = 0;
    uint32_t low = 0;

    for (int i = 0; i < 16; i++) {
        test.sec = tests[i];
        cout << hex << showbase << setw(20) << test.sec << dec
             << " high:" << setw(12) << test.ul.high
             << " low:" << setw(12) << test.ul.low
             << " time_t:" << setw(20) << test.sec << endl;

        high = static_cast<uint32_t>(test.sec >> 32);
        low = static_cast<uint32_t>(test.sec);

        sec = (static_cast<int64_t>(high) << 32) | low;
        //FIXME sec = (static_cast<int64_t>(high) << 32LL) | static_cast<uint32_t>(low);

        //XXX if (sec != test.sec)
        {
            cout << hex << setw(20) << sec << ' ' << setw(17) << high << ' ' << setw(16) << low << endl;
        }

        if (test.sec == 1LL) {
            assert(test.ub.byte1 == 1);
            assert(test.uw.word1 == 1);
            assert(test.ul.low == 1L);
        }

        assert(sec == test.sec);

        assert(high == test.ul.high);
        assert(low == test.ul.low);
    }

    return 0;
}

/***

$ g++ -std=c++0x -Wall -Wextra -o timevalue /cygdrive/o/DATEN/backup/timevalue.cpp && ./timevalue.exe
/cygdrive/o/DATEN/backup/timevalue.cpp:15:2: Warnung: #warning LittleEndian
-9223372036854775808 high: -2147483648 loww:           0 time_t:0x8000000000000000
         -4294967296 high:          -1 loww:           0 time_t:0xffffffff00000000
         -2147483648 high:          -1 loww: -2147483648 time_t:0xffffffff80000000
                  -1 high:          -1 loww:          -1 time_t:0xffffffffffffffff
                   0 high:           0 loww:           0 time_t:                 0
                   1 high:           0 loww:           1 time_t:               0x1
          2147483647 high:           0 loww:  2147483647 time_t:        0x7fffffff
          2147483648 high:           0 loww: -2147483648 time_t:        0x80000000
          4294967295 high:           0 loww:          -1 time_t:        0xffffffff
          4294967296 high:           1 loww:           0 time_t:       0x100000000
          8589934591 high:           1 loww:          -1 time_t:       0x1ffffffff
 9223372036854775807 high:  2147483647 loww:          -1 time_t:0x7fffffffffffffff

 ***/
