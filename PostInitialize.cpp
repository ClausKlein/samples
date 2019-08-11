// from https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
// #discussion-use-a-factory-function-if-you-need-virtual-behavior-during-initialization

// Discussion: Use a factory function if you need "virtual behavior"
// during initialization

#include <iostream>
#include <memory>
#include <string>

#define USE_OVERLOADS

class B
{
protected:
    B() = default;
    virtual void post_initialize() // called right after construction
    {
        f(); // GOOD: virtual dispatch is safe
    }

public:
    // GOOD: polymorphic class suppresses copying
    B(const B &) = delete;
    B &operator=(const B &) = delete;
    B(const B &&) = delete;
    B &operator=(const B &&) = delete;

    virtual ~B() = default;
    virtual void f() = 0;

    template <class T>
    static std::shared_ptr<T> create() // interface for creating objects
    {
        auto p = std::make_shared<T>();
        p->post_initialize();
        return p;
    }
};

class D : public B
{ // some derived class
public:
    ~D() override { std::cout << "~D::D() called" << std::endl; }

    void f() override { std::cout << value << std::endl; }

#ifdef USE_OVERLOADS
    explicit D(std::string v) : value(std::move(v)) {}
    void set_value(const std::string &v) { value = v; }

    // function templates can be specialized like all function templates.
    static std::shared_ptr<D> create(std::string v)
    {
        auto p = std::make_shared<D>(v);
        p->post_initialize();
        return p;
    }

protected:
    D() = default; // NOTE: protected too! CK
#else
    template <class T> friend std::shared_ptr<T> B::create();
#endif

private:
    std::string value;
};

int main()
{
#ifdef USE_OVERLOADS
    {
        const std::string s("Hello World");
        std::shared_ptr<D> p = D::create(s); // create an init D object
        p->set_value("Good by");
        p->f();
    }
#else
    // error: 'D::D()' is protected within this context
    std::shared_ptr<D> d = D::create<D>(); // create a default D object
#endif
}
