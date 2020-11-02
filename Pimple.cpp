// from
// https://en.cppreference.com/w/cpp/language/pimpl
//
// Implementation

// As the object of the interface type controls the lifetime of the
// object of the implementation type, the pointer to implementation is
// usually std::unique_ptr.

// Because std::unique_ptr requires that the pointed-to type is a
// complete type in any context where the deleter is instantiated, the
// special member functions must be user-declared and defined
// out-of-line, in the implementation file, where the implementation
// class is complete.

// Because when const member function calls a function through a
// non-const member pointer, the non-const overload of the implementation
// function is called, the pointer has to be wrapped in
// std::experimental::propagate_const or equivalent.

// All private data members and all private non-virtual member functions
// are placed in the implementation class. All public, protected, and
// virtual members remain in the interface class (see GOTW #100 for the
// discussion of the alternatives).

// If any of the private members needs to access a public or protected
// member, a reference or pointer to the interface may be passed to the
// private function as a parameter. Alternatively, the back-reference may
// be maintained as part of the implementation class.

// If non-default allocators are intended to be supported for the
// allocation of the implementation object, any of the usual allocator
// awareness patterns may be utilized, including allocator template
// parameter defaulting to std::allocator and constructor argument of
// type std::pmr::memory_resource*.

// Example

// demonstrates a pImpl with const propagation, with back-reference
// passed as a parameter, without allocator awareness, and move-enabled
// without runtime checks

#ifdef __has_include // Check if __has_include is present
#    if __has_include(<experimental/propagate_const>)
#        include <experimental/propagate_const>
#        define HAS_PROPAGAGE_CONST
#    endif
#endif

#include <iostream>
#include <memory>

// interface (widget.h)
class widget
{
    class impl;

#ifdef HAS_PROPAGAGE_CONST
    std::experimental::propagate_const<std::unique_ptr<impl>> pImpl;
#else
    std::unique_ptr<impl> pImpl;
#endif

public:
    /// public API that will be forwarded to the implementation
    void draw() const;
    void draw();

    /// public API that implementation has to call
    [[nodiscard]] static auto shown() -> bool { return true; }

    explicit widget(int /*n*/);
    ~widget(); // defined in the implementation file, where impl is a complete
               // type
    widget(widget && /*unused*/); // defined in the implementation file
                                  // Note: calling draw() on moved-from object
                                  // is Undefined Behaviour (UB)!

    widget(const widget &) = delete;
    auto operator=(widget && /*unused*/)
        -> widget &; // defined in the implementation file
    auto operator=(const widget &) -> widget & = delete;
};

// implementation (widget.cpp)
class widget::impl
{
    int n; // private data

public:
    void draw(const widget &w) const
    {
        // this call to public member function requires the back-reference
        if (w.shown()) {
            std::cout << "drawing a const widget " << n << '\n';
        }
    }
    void draw(const widget &w)
    {
        if (w.shown()) {
            std::cout << "drawing a non-const widget " << n << '\n';
        }
    }
    impl(int n) : n(n) {}
};

void widget::draw() const { pImpl->draw(*this); }
void widget::draw() { pImpl->draw(*this); }
widget::widget(int n) : pImpl{std::make_unique<impl>(n)} {}
widget::widget(widget &&) = default;
widget::~widget() = default;
auto widget::operator=(widget &&) -> widget & = default;

// user (main.cpp)
auto main() -> int
{
    widget w(7);
    const widget w2(8);
    w.draw();
    w2.draw();
}
