// from
//
// Dependency Injected (DI)
//
// "Dependency injection is a software design pattern that allows the removal
// of hard-coded dependencies and makes it possible to change them, whether at
// run-time or compile-time." -wiki.
//
// What is dependency injection (DI)?
//
// It allows us to standardize and centralize the way objects are constructed
// in our application by supplying (injecting) an external dependency into a
// software component rather than creating an dependency within.
//
// Here is a brief description from https://docs.angularjs.org/guide/di. Here
// is some samples of using it in AngularJS, AngularJS - Dependency Injection.
//
// There are three ways a component (object or function) can get a hold of its
// dependencies:
// * The component can create the dependency, typically using the new operator.
// * The component can look up the dependency, by referring to a global
// variable.
// * The component can have the dependency passed to it where it is needed.
//
// The first two options of creating or looking up dependencies are not
// optimal because they hard code the dependency to the component. This makes
// it difficult, if not impossible, to modify the dependencies. This is
// especially problematic in tests, where it is often desirable to provide
// mock dependencies for test isolation.
//
// The third option is the most viable, since it removes the responsibility of
// locating the dependency from the component. The dependency is simply handed
// to the component.

#include <iostream>
#include <string>

using namespace std;

class Shape_IF
{
public:
    virtual ~Shape_IF() = default;
    virtual void draw() = 0;
};

class Circle : public Shape_IF
{
public:
    void draw() override { cout << "circle\n"; }
};

class Triangle : public Shape_IF
{
public:
    void draw() override { cout << "triangle\n"; }
};

/* 1. This class does not have hard-coded shapes
   such as Triangle and Circle
   2. So, it's decoupled and has no dependency
   3. The specific information is injected by other class
   4. This code can be remained untouched
      when we switch the shape to draw
*/
class Drawing
{
public:
    void drawShape(Shape_IF *pShape) { pShape->draw(); }

private:
    // TODO: unused! Shape_IF *pShape;
};

/* 1. This class pulled the hard-coded shape info out of
      the Drawing class (compare with the previous example)
   2. This class is an interface that can be modified
      depending on what to draw
   3. This class is doing the dependency injection
*/
class IDrawing
{
public:
    IDrawing() { d = new Drawing; }
    ~IDrawing() { delete d; }

    void draw(string s)
    {
        if (s == "triangle") {
            d->drawShape(new Triangle); // TODO: memory leak! CK
        } else if (s == "circle") {
            d->drawShape(new Circle); // TODO: memory leak! CK
        } else {
            cerr << " Need shape" << endl;
        }
    }

private:
    Drawing *d;
};

// As we can see from the code, the Drawing class does not have hard-coded
// shapes anymore. Now, it's decoupled and has no dependency. The specific
// shape information is injected by other interface class (IDrawing) and it
// does not instantiate anything within the class. Therefore, this code can be
// remained untouched even though we switch the shape to draw.

// The interface class IDrawing pulled the hard-coded shape info out of the
// Drawing class. This class as an interface can be modified depending on what
// to draw, and it is doing the dependency injection.

// Pros and Cons of Dependency Injection
/***
 * Pros
    Loosely coupled code
    Increase testibility
    Can use Inversion of Control(IoC) container
 * Cons
    Increase code complexity
    Hard to understand code, at least initially
***/

int main()
{
    IDrawing drawer;
    drawer.draw("circle");
    drawer.draw("triangle");

    return 0;
}
