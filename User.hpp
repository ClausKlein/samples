/*** The PImpl Idiom (Pointer to IMPLementation) is a technique used for
 * separating implementation from the interface. It minimizes header
 * exposure and helps programmers to reduce build dependencies by moving
 * the private data members in a separate class and accessing them
 * through an opaque pointer.
 ***/

/* |INTERFACE| User.h file */

#pragma once
#include <memory> // unique_ptr<>
#include <string>

class User
{
public:
    // Constructor and Destructors

    ~User();
    explicit User(std::string name);

    // Asssignment Operator and Copy Constructor

    User(const User &other);
    User &operator=(User rhs);

    // Getter
    int getSalary() const;

    // Setter
    void setSalary(int);

private:
    // Internal implementation class
    class Impl;

    // Pointer to the internal implementation
    std::unique_ptr<Impl> pimpl;
};
