// from
// https://wiki.sei.cmu.edu/confluence/display/cplusplus/OOP51-CPP.+Do+not+slice+derived+objects
// Noncompliant Code Example

// In this noncompliant code example, an object of the derived Manager
// type is passed by value to a function accepting a base Employee type.
// Consequently, the Manager objects are sliced, resulting in information
// loss and unexpected behavior when the print() function is called.

#include <iostream>
#include <string>
#include <utility>

class Employee
{
    std::string name;

protected:
    virtual void print(std::ostream &os) const
    {
        os << "Employee: " << get_name() << std::endl;
    }

public:
    Employee(std::string name) : name(std::move(name)) {}
    [[nodiscard]] const std::string &get_name() const { return name; }
    friend std::ostream &operator<<(std::ostream &os, const Employee &e)
    {
        e.print(os);
        return os;
    }
};

class Manager : public Employee
{
    Employee assistant;

protected:
    void print(std::ostream &os) const override
    {
        os << "Manager: " << get_name() << std::endl;
        os << "Assistant: " << std::endl
           << "\t" << get_assistant() << std::endl;
    }

public:
    Manager(const std::string &name, Employee assistant)
        : Employee(name), assistant(std::move(assistant))
    {}
    [[nodiscard]] const Employee &get_assistant() const { return assistant; }
};

#ifndef FIXIT
void f(Employee e) { std::cout << e; }
#else
void f(const Employee &e) { std::cout << e; }
#endif

int main()
{
    Employee coder("Joe Smith");
    Employee typist("Bill Jones");
    Manager designer("Jane Doe", typist);

    f(coder);
    f(typist);
    f(designer);
}
