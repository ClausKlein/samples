// from https://wiki.sei.cmu.edu/confluence/display/cplusplus/OOP51-CPP.+Do+not+slice+derived+objects
// Compliant Solution (Noncopyable)

// Both previous compliant solutions depend on consumers of the Employee
// and Manager types to be declared in a compliant manner with the
// expected usage of the class hierarchy. This compliant solution ensures
// that consumers are unable to accidentally slice objects by removing
// the ability to copy-initialize an object that derives from
// Noncopyable. If copy-initialization is attempted, as in the original
// definition of f(), the program is ill-formed and a diagnostic will be
// emitted. However, such a solution also restricts the Manager object
// from attempting to copy-initialize its Employee object, which subtly
// changes the semantics of the class hierarchy.

#include <iostream>
#include <string>

class Noncopyable {
    Noncopyable(const Noncopyable&) = delete;
    void operator=(const Noncopyable&) = delete;

protected:
    Noncopyable() = default;
};

class Employee : Noncopyable {
    // Remainder of the definition is unchanged.
    std::string name;

protected:
    virtual void print(std::ostream& os) const
    {
        os << "Employee: " << get_name() << std::endl;
    }

public:
    Employee(const std::string& name)
        : name(name)
    {}
    const std::string& get_name() const { return name; }
    friend std::ostream& operator<<(std::ostream& os, const Employee& e)
    {
        e.print(os);
        return os;
    }
};

class Manager : public Employee {
    const Employee&
        assistant; // Note: The definition of Employee has been modified.

    // Remainder of the definition is unchanged.
protected:
    void print(std::ostream& os) const override
    {
        os << "Manager: " << get_name() << std::endl;
        os << "Assistant: " << std::endl
           << "\t" << get_assistant() << std::endl;
    }

public:
    Manager(const std::string& name, const Employee& assistant)
        : Employee(name)
        , assistant(assistant)
    {}
    const Employee& get_assistant() const { return assistant; }
};

// If f() were declared as accepting an Employee, the program would be
// ill-formed because Employee cannot be copy-initialized.
void f(const Employee& e)
{
    std::cout << e;
}


int main()
{
    Employee coder("Joe Smith");
    Employee typist("Bill Jones");
    Manager designer("Jane Doe", typist);

    f(coder);
    f(typist);
    f(designer);
}
