// form http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
// #c21-if-you-define-or-delete-any-default-operation-define-or-delete-them-all

// C.21: If you define or =delete any default operation, define or
// =delete them all

#include <gsl/gsl>

#include <cassert>
#include <memory> // NOTE: make_unique<>! CK
#include <string>

// NOTE: this prevents prevent slicing too! CK
class ClonableBase
{
public:
    ClonableBase() = default;
    virtual ~ClonableBase() = default;

#ifdef SHOW_ERROR
    virtual std::unique_ptr<ClonableBase> clone() const = 0;
#else
    [[nodiscard]] virtual auto clone() const -> gsl::owner<ClonableBase *> = 0;
#endif

    ClonableBase(const ClonableBase &) = delete;
    auto operator=(const ClonableBase &) -> ClonableBase & = delete;

    ClonableBase(ClonableBase &&) = delete;
    auto operator=(ClonableBase &&) -> ClonableBase & = delete;
};

class Base : public ClonableBase
{
public:
    Base() = default;
    ~Base() override = default;

    [[nodiscard]] virtual auto number() const -> int { return 0; }
};

class Derived : public Base
{
public:
    Derived() = default;
    ~Derived() override = default;

    // NO! explicit Derived(const Derived &) = default;
    // warning: copy constructor is explicitly defaulted but implicitly
    // deleted, probably because a non-static data member or a base class
    // is not copyable; definition can either be removed or explicitly
    // deleted [readability-deleted-default]

    // warning: pass by value and use std::move [modernize-pass-by-value]
    explicit Derived(std::string v) : value(std::move(v)) {}

#ifdef SHOW_ERROR
    std::unique_ptr<ClonableBase> clone() const override
    {
        return std::make_unique<Derived>();
    }
    // error: virtual function 'clone' has a different return type
    // ('unique_ptr<Derived>') than the function it overrides (which has
    // return type 'unique_ptr<ClonableBase>')
#else
    [[nodiscard]] auto clone() const -> gsl::owner<Derived *> override
    {
        return gsl::owner<Derived *>(new Derived(value));
    }
#endif

    [[nodiscard]] auto number() const -> int override { return 1; }
    auto getValue() -> std::string { return value; }
    void setValue(const std::string &v) { value = v; }

private:
    std::string value;
};

auto main() -> int
{
    Derived origin("test");

    assert(origin.number() == 1);
    // warning: do not implicitly decay an array into a pointer; consider
    // using gsl::array_view or an explicit cast instead
    // [cppcoreguidelines-pro-bounds-array-to-pointer-decay]

    gsl::owner<Derived *> copy = origin.clone();

    assert(copy->number() == 1);
    // error: no member named 'number' in
    // 'std::__1::unique_ptr<ClonableBase,
    // std::__1::default_delete<ClonableBase> >'

    assert(copy->getValue() == origin.getValue());
    // XXX assert(copy == origin);

    delete copy;
}
