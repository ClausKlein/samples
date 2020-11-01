//
// from
// http://bloglitb.blogspot.com/2010/07/access-to-private-members-thats-easy.html
//
// clang++ --std=c++2a -Wall -Wextra -Wpedantic access_privates.cpp
//
#include <iostream>

template <typename Tag> struct result
{
    /* export it ... */
    typedef typename Tag::type type;
    static type ptr;
};

template <typename Tag> typename result<Tag>::type result<Tag>::ptr;

template <typename Tag, typename Tag::type p> struct rob : result<Tag>
{
    /* fill it ... */
    struct filler
    {
        filler() { result<Tag>::ptr = p; }
    };
    static filler filler_obj;
};

template <typename Tag, typename Tag::type p>
typename rob<Tag, p>::filler rob<Tag, p>::filler_obj;

// So, how is it used? Let's have an example
class A
{
public:
    A() = default;

private:
    void f() { std::cout << "proof!" << std::endl; }
};

struct Af
{
    typedef void (A::*type)();
};
template struct rob<Af, &A::f>;

// Ah, that's all to expose poor A's "f" member. Now anyone can use them
// using the member pointer snytax, as does the main function below
auto main() -> int
{
    A a;
    (a.*result<Af>::ptr)();

    return 0;
}
