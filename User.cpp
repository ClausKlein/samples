// from
// https://www.geeksforgeeks.org/pimpl-idiom-in-c-with-examples/
//

/* |IMPLEMENTATION| User.cpp file */

#include "User.hpp"

#include <iostream>

struct User::Impl
{
    explicit Impl(std::string name) : name(std::move(name)){};

    ~Impl();

    void welcomeMessage() { std::cout << "Welcome, " << name << std::endl; }

    std::string name;
    int salary = -1;
};

// Constructor connected with our Impl structure
User::User(std::string name) : pimpl(std::make_unique<Impl>(name))
{
    pimpl->welcomeMessage();
}

// Default Destructor
User::~User() = default;

// Assignment operator and Copy constructor

User::User(const User &other) : pimpl(std::make_unique<Impl>(*other.pimpl)) {}

User &User::operator=(User rhs)
{
    std::swap(pimpl, rhs.pimpl);
    return *this;
}

// Getter and setter
int User::getSalary() const { return pimpl->salary; }

void User::setSalary(int salary)
{
    pimpl->salary = salary;
    std::cout << "Salary set to " << salary << std::endl;
}
