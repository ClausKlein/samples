// from
// https://cpppatterns.com/patterns/pimpl.html
//
// INTENT

// Remove compilation dependencies on internal class implementations and
// improve compile times.

// DESCRIPTION

// When a header file changes, any files that #include that file will
// need to be recompiled. In the case of a class header, this is true
// even if those changes only apply to private members of the class. The
// PIMPL idiom hides private members from any users of the header file,
// allowing these internal details to change without requiring
// recompilation of the client code.

// foo.h - header file
#include <memory>

class foo
{
public:
    foo();
    ~foo();
    foo(foo &&);
    foo &operator=(foo &&);

private:
    class impl;
    std::unique_ptr<impl> pimpl;
};

// foo.cpp - implementation file
class foo::impl
{
public:
    void do_internal_work() { internal_data = 5; }

private:
    int internal_data = 0;
};

foo::foo() : pimpl{std::make_unique<impl>()} { pimpl->do_internal_work(); }
foo::~foo() = default;
foo::foo(foo &&) = default;
foo &foo::operator=(foo &&) = default;
