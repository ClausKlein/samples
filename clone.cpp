// from http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
// #c130-for-making-deep-copies-of-polymorphic-classes-prefer-a-virtual-clone-function-instead-of-copy-constructionassignment

// C.130: For making deep copies of polymorphic classes prefer a virtual
// clone function instead of copy construction/assignment

// Reason Copying a polymorphic class is discouraged due to the slicing
// problem, see C.67. If you really need copy semantics, copy deeply:
// Provide a virtual clone function that will copy the actual
// most-derived type and return an owning pointer to the new object, and
// then in derived classes return the derived type (use a covariant
// return type). Example

#include <cassert>
#include <gsl/pointers>


class B {
public:
    B()          = default;
    virtual ~B() = 0;

    virtual gsl::owner<B*> clone() const = 0;

    B(const B&) = delete;
    B& operator=(const B&) = delete;
};

class D : public B {
public:
    D() = default;
    virtual ~D() {}

    gsl::owner<D*> clone() const override
    {
        return gsl::owner<D*>(new D()); // TODO this is not a clone! CK
    }
};

// Generally, it is recommended to use smart pointers to represent
// ownership (see R.20). However, because of language rules, the
// covariant return type cannot be a smart pointer: D::clone can’t return
// a unique_ptr<D> while B::clone returns unique_ptr<B>. Therefore, you
// either need to consistently return unique_ptr<B> in all overrides, or
// use owner<> utility from the Guidelines Support Library.

// see
// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#SS-views
// and https://github.com/Microsoft/GSL

int main()
{
#if 0
    D d1;
    auto d2 = d1.clone();
    // TODO assert (*d2 == d1);
#endif
}
