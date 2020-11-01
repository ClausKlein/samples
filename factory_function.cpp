// Discussion: Use a factory function if you need "virtual behavior" during
// initialization

#include <memory>

using namespace std;

class B
{
protected:
    class Token
    {};

public:
    explicit B(Token) {}
    virtual ~B() = default;
    virtual void f() = 0;

    template <class T>
    static auto create() -> shared_ptr<T> // interface for creating objects
    {
        auto p = make_shared<T>(typename T::Token{});
        p->post_initialize();
        return p;
    }

protected:
    virtual void post_initialize() // called right after construction
    {
        /* ... */
        f(); // GOOD: virtual dispatch is safe
             /* ... */
    }
};

class D : public B
{ // some derived class
protected:
    class Token
    {};

public:
    explicit D(Token) : B{typename B::Token{}} {}
    void f() override{/* ...  */};

protected:
    // NOTE: only friend class member 'create' can have a qualified
    // name B:: and access to protected D::Token
    template <class T> friend auto B::create() -> shared_ptr<T>;
};

auto main() -> int
{
    shared_ptr<D> p = D::create<D>(); // creating a D object
}
