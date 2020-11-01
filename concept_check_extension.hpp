#ifndef BOOST_LIBS_CONCEPT_CHECK_EXTENSION_HPP
#define BOOST_LIBS_CONCEPT_CHECK_EXTENSION_HPP

#include <boost/concept/requires.hpp>
#include <boost/concept_check.hpp>
#include <boost/detail/iterator.hpp>
#include <boost/functional/hash.hpp>

#include <sstream>

namespace boost {

template <class TT> struct Hashable
{
public:
    BOOST_CONCEPT_USAGE(Hashable)
    {
        std::size_t hash = hash_value(a); // require hash_value function
        ignore_unused_variable_warning(hash);
    }

private:
    TT a;
};

template <class TT> struct Ostreamable
{
public:
    BOOST_CONCEPT_USAGE(Ostreamable)
    {
        std::stringstream out; // dynamic buffer
        out << a;              // require ostream operator
        ignore_unused_variable_warning(out);
    }

private:
    TT a;
};

} // namespace boost

// DEMO only
namespace fake {
using namespace boost;

template <typename RanIter>
auto // return value
    sort(RanIter, RanIter) -> BOOST_CONCEPT_REQUIRES(
        ((Mutable_RandomAccessIterator<RanIter>))(
            (LessThanComparable<
                typename Mutable_RandomAccessIterator<RanIter>::value_type>))

            ,
        (void))
{}
} // namespace fake

#endif // BOOST_LIBS_CONCEPT_CHECK_EXTENSION_HPP
