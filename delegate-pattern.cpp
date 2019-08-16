// from
// http://www.bogotobogo.com/DesignPatterns/delegation.php
//
// Conceptually, a delegate is nothing more than a type-safe method reference.
// A delegate allows us to delegate the act of calling a method to somebody
// else. A delegate also can be thought of as an object that contains an
// ordered list of methods with the same signature and return type.
//
// Using delegation can make a design more flexible as shown in the example
// below. The RemoteHost does not have to refer to RemoteHostA or RemoteHostB
// in any way because the switching of delegation is abstracted from
// RemoteHost.
//
// The RemoteHost class can delegate to RemoteHostA or RemoteHostB.
//
// "Delegation is a good design choice only when it simplifies more than it
// complicates. It isn't easy to give rules that tell you exactly when to use
// delegation, because how effective it will be depends on the context and on
// how much experience you have with it. Delegation works best when it's used
// in highly stylized ways-that is, in standard patterns." - from Design
// Pattern - Elements of Resuable Object-Oriented Software by Eirch Gamma et
// al.
//
// Here is an example:

#include <iostream>
#include <memory>

using namespace std;

// abstract interface
class Host_IF
{
public:
    virtual ~Host_IF() = default;
    ;
    virtual void f() = 0;

protected:
    Host_IF *my_base;
};

class RemoteHostA : public Host_IF
{
public:
    void f() override { cout << "A::f()" << endl; }
};

class RemoteHostB : public Host_IF
{
public:
    void f() override { cout << "B::f()" << endl; }
};

// This class has a delegate switch to either RemoteHostA or RemoteHostB
class RemoteHost : public Host_IF
{
public:
    explicit RemoteHost() : Host_IF() { m_host = make_shared<RemoteHostA>(); }

    void f() override { m_host->f(); }

    void connectA() { m_host = make_shared<RemoteHostA>(); }
    void connectB() { m_host = make_shared<RemoteHostB>(); }

private:
    shared_ptr<Host_IF> m_host;
};

int main()
{
    auto *remote = new RemoteHost();

    remote->f(); // A::f()

    remote->connectB();
    remote->f(); // B::f()

    remote->connectA();
    remote->f(); // A::f()

    delete remote;
    return 0;
}
