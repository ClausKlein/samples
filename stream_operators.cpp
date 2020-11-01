#include <boost/assert.hpp>

#include <iostream>
#include <sstream>
#include <utility> // std::rel_ops
// As of C++20, std::rel_ops are deprecated in favor of operator<=>.
// see https://en.cppreference.com/w/cpp/language/default_comparisons

class Paragraph
{
public:
    explicit Paragraph(std::string init) : m_para(std::move(init)) {}
    // NOTE: not needed! CK void operator=(Paragraph const &rhs) { m_para =
    // rhs.m_para; }

    [[nodiscard]] std::string const &to_string() const { return m_para; }

    bool operator==(Paragraph const &rhs) const { return m_para == rhs.m_para; }
    bool operator<(Paragraph const &rhs) const { return m_para < rhs.m_para; }

private:
    std::string m_para;
};

// For generic basic_istream and basic_ostream T << Paragraph
// OUTPUT << Paragraph
template <typename charT, typename traits>
std::basic_ostream<charT, traits> &
operator<<(std::basic_ostream<charT, traits> &outputStream, const Paragraph &p)
{
    // do the insertion of p
    return outputStream << p.to_string();
}

// INPUT >> Paragraph
template <typename charT, typename traits>
std::basic_istream<charT, traits> &
operator>>(std::basic_istream<charT, traits> &inputStream, Paragraph &p)
{
    // do the extract of p
    std::string s;
    inputStream >> s;
    p = Paragraph(s);
    return inputStream;
}

#if 0
// The following code will work only for char-based streams.
// OUTPUT << A
std::ostream &operator<<(std::ostream &outputStream, const Paragraph &p)
{
    // do the insertion of p
    return outputStream << p.to_string();
}

// INPUT >> A
std::istream &operator>>(std::istream &inputStream, Paragraph &p)
{
    // TODO do the extract of p
    return inputStream;
}

// As it is only viable if you only work with char-based streams, you
// should avoid it on platforms where wchar_t code is common (i.e. on
// Windows).
#endif

int main()
{
    using namespace std::rel_ops;

    Paragraph p("Plop");
    Paragraph q(p);

    BOOST_ASSERT(p == q);

    std::istringstream istrs("Ploq");
    istrs >> q;

    std::cout << p << "<=>" << q << std::endl;

    std::cout << std::boolalpha;
    std::cout << "not equal?     : " << (p != q) << '\n';
    std::cout << "greater?       : " << (p > q) << '\n';
    std::cout << "less equal?    : " << (p <= q) << '\n';
    std::cout << "greater equal? : " << (p >= q) << '\n';
}
