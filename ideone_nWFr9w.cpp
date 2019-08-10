#include <algorithm>
#include <boost/functional/hash.hpp>
#include <boost/unordered_map.hpp>
#include <cassert>
#include <complex>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

// Mathematically speaking, there is no ordering defined for complex numbers,
// which is why there is no operator< defined for complex. You can try
// inventing your own ordering function (such as ordering them
// lexicographically) but that requires writing your own comparator function:
template <class T>
bool operator<(const std::complex<T>& l, const std::complex<T>& r)
{
  return (l.real() == r.real() ? l.imag() < r.imag() : l.real() < r.real());
}

template <class T>
std::size_t hash(const std::complex<T>& p)
{
  std::size_t seed = 0;
  boost::hash_combine(seed, p.real());
  boost::hash_combine(seed, p.imag());
  return seed;
}

// However, I'm not quite sure what you're trying to achieve because there is
// no sense in saying that one complex number is "bigger" than another.
int main()
{
  std::istringstream iss("(1,-2)\n"
                         "(+1, +1)\n"
                         "(-1, -1)\n"
                         "(2,2)\n"
                         "(1,1.2)\n"
                         "(-2, 0)\n"
                         "(0, 1)\n"
                         "(0, 2)\n"
                         "(2, 0)\n"
                         "(0, 0)\n"
                         "(1, 0)\n"
                         "(0, -2)\n");
  typedef std::complex<double> Complex;
  std::vector<Complex> nums;
  boost::unordered_map<Complex, double> container;

  Complex c;
  while (iss >> c) {
    nums.push_back(c);
  }

#if 1
  std::sort(std::begin(nums), std::end(nums), [](Complex x, Complex y) {
    return (x.real() == y.real() ? x.imag() < y.imag() : x.real() < y.real());
  });
#else
  std::sort(nums.begin(), nums.end(), complex_comparator<double>);
#endif

  c = Complex(-9, -9);
  for (auto& v : nums) {
    using namespace std;

    container[v] = abs(v);
    cout << v << "\tmag=" << setw(8) << left << container[v]
         << "\thash=" << hex << setw(16) << right << ::hash(v) << '\n';

    assert(c < v);
    c = v;
  }

  return 0;
}
