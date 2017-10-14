// Use g++ -std=c++11 or clang++ -std=c++11 to compile.

#include <vector> // the general-purpose vector container
#include <iostream>
#include <algorithm> // remove and remove_if

bool is_odd(int i)
{
    return (i % 2) != 0;
}

void print(const std::vector<int> &vec)
{

#if CPP_11_ENABLED
    for (const auto& i : vec)
    {
        std::cout << i << ' ';
    }
#else
    std::copy(vec.begin(), vec.end(),
              std::ostream_iterator<int>(std::cout, " "));
#endif

    std::cout << std::endl;
}

int main()
{
    // initialises a vector that holds the numbers from 0-9.
    std::vector<int> v = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    print(v);

    // removes all elements with the value 5
    v.erase(std::remove(v.begin(), v.end(), 5), v.end());
    print(v);

    // removes all odd numbers
    v.erase(std::remove_if(v.begin(), v.end(), is_odd), v.end());
    print(v);

    return 0;
}

/***
Output:
0 1 2 3 4 5 6 7 8 9
0 1 2 3 4 6 7 8 9
0 2 4 6 8
***/
