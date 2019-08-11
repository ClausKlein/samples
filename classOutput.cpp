#include <iostream>

// Use a friend function only if the functionality cannot be achieved via
// the public interfaces!
class Date
{
    int mo, da, yr;

public:
    Date(int m, int d, int y) : mo(m), da(d), yr(y) {}
    friend std::ostream &operator<<(std::ostream &os, const Date &dt);
};

std::ostream &operator<<(std::ostream &os, const Date &dt)
{
    os << dt.mo << '/' << dt.da << '/' << dt.yr;
    return os;
}

// Assuming that we're talking about overloading operator << for all
// classes derived from std::ostream to handle the Matrix class (and not
// overloading << for Matrix class), it makes more sense to declare the
// overload function outside the Math namespace in the header.
namespace Math {

class Matrix
{
public:
    Matrix() = default;
    // [...]
    void print(std::ostream &stream) const { stream << "class Matrix {...} "; }
};

// XXX std::ostream& operator<<(std::ostream& stream, const Math::Matrix&
// matrix);
} // namespace Math

// In C++11 you can use the following template to print any object which has a
// T::print(std::ostream&)const; member.
template <class T>
auto operator<<(std::ostream &os, const T &t) -> decltype(t.print(os), os)
{
    t.print(os);
    return os;
}

// XXX In your implementation
// std::ostream& Math::operator<<(std::ostream& stream, const Math::Matrix&
// matrix)
// {
//     matrix.print(stream); // assuming you define print for matrix
//     return stream;
// }

int main()
{
    Math::Matrix matrix;
    std::cout << matrix << std::endl;

    Date dt(5, 6, 92);
    std::cout << dt << std::endl;
}
