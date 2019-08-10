#include <chrono>
#include <iostream>
#include <string>

#ifdef DEBUG
#  define TRACE
#endif

namespace testing {
using namespace std::chrono;
using namespace std::string_literals;

auto c = 299'792'458; // m/s2
auto q2 = 0b0000'1111'0000'0000;
auto ss_number = 123'456'7890;

std::string stringWithNull = "abc\0\0def"s;

auto hello = "Hello!"s; // a std::string
auto world = "world";   // a C-style string
auto interval = 100ms;  // using <chrono>

const double some_value(3.14);

enum
{
  good,
  better,
  amazing
};

struct Cable
{
  int& operator[](size_t); // OK
  int x;
  // ...
};

static void something(){};
static void something_else(){};

double foo(int x)
{
  if (0 < x) {
    // ...
  }

  switch (x) {
    case 0:
      // ...
      break;
    case amazing:
      // ...
      break;
    default:
      // ...
      break;
  }

  if (0 < x)
    ++x;

  if (x < 0)
    something();
  else
    something_else();

  return some_value;
}
}

int main()
{
  testing::foo(testing::amazing);

  std::cout << "This is code with .clang-format -style=Mozilla (from cmake project)!\n";
}
