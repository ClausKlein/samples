#include <functional>
#include <iostream>
#include <queue>
#include <vector>

// Working with a priority_queue is similar to managing a heap in some random
// access container, with the benefit of not being able to accidentally
// invalidate the heap.

template <typename T> void fill_queue(T &q)
{
    for (size_t j = 0; j < 10; ++j) {
        q.push(rand() % 10);
    }
}

template <typename T> void print_queue(T &q)
{
    // ERROR: T does not provide a subscript operator
    // for (size_t j = 0; j < q.size(); ++j) std::cout << q[j] << " ";
    while (!q.empty()) {
        std::cout << q.top() << " ";
        q.pop();
    }
    std::cout << '\n';
}

struct xor_cmp
{
    xor_cmp(int _value = 1) : value(_value){};
    int value;
    bool operator()(int left, int right)
    {
        return (left ^ value) < (right ^ value);
    };
};

int main()
{
    // A priority queue is a container adaptor that provides constant time
    // lookup of the largest (by default) element, at the expense of
    // logarithmic insertion and extraction.
    std::priority_queue<int> q;
    // TODO: c++11 for (int n : {1, 8, 5, 6, 3, 4, 0, 9, 7, 2}) { q.push(n); }
    fill_queue(q);
    print_queue(q);

    // A user-provided Compare can be supplied to change the ordering, e.g.
    // using std::greater<T> would cause the smallest element to appear as the
    // top().
    std::priority_queue<int, std::vector<int>, std::greater<>> q2;
    // TODO: c++11 for (int n : {1, 8, 5, 6, 3, 4, 0, 9, 7, 2}) { q2.push(n); }
    fill_queue(q2);
    print_queue(q2);

    // Using lambda to compare elements.
    // XXX auto cmp = [](int left, int right) { return (left ^ 1) < (right ^
    // 1);};
    // XXX std::priority_queue<int, std::vector<int>, decltype(cmp)> q3(cmp);
    std::priority_queue<int, std::vector<int>, xor_cmp> q3(xor_cmp(1));
    // TODO: c++11 for (int n : {1, 8, 5, 6, 3, 4, 0, 9, 7, 2}) { q3.push(n); }
    fill_queue(q3);
    print_queue(q3);

    return 0;
}
