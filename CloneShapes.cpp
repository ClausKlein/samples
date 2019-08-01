//
// from https://github.com/isocpp/CppCoreGuidelines/issues/1151
//

// Q: It would be nice to have a concrete counterexample to C.67 where a
// derived class wants to copy for a good reason.

// A: I certainly want to be able to copy a Circle even if it is derived
// from a IShape and considering that a circle is probably just 3 doubles
// (center & radius) I prefer to not use clone for that (which requires a
// dynamic memory allocation an returns either a raw pointer or a
// smart_pointer to the base class) if it isn't necessary.

#include <iostream>
#include <memory>


struct IShape {
    virtual void draw()                     = 0;
    virtual std::unique_ptr<IShape> clone() = 0;
    virtual ~IShape()                       = default;

protected:
    IShape()              = default;
    IShape(const IShape&) = default;
    IShape& operator=(const IShape&) = default;
};


struct CircleImpl : IShape {
    void draw() override { std::cout << "Draw Circle\n"; };

protected:
    CircleImpl()                  = default;
    CircleImpl(const CircleImpl&) = default;
    CircleImpl& operator=(const CircleImpl&) = default;
};


struct Circle final : CircleImpl {
    Circle()              = default;
    Circle(const Circle&) = default;
    Circle& operator=(const Circle&) = default;

    std::unique_ptr<IShape> clone() override
    {
        return std::make_unique<Circle>(*this);
    }
};


struct BlueCircleImpl : CircleImpl {
    void draw() override { std::cout << "Draw BlueCircle\n"; }

protected:
    BlueCircleImpl()                      = default;
    BlueCircleImpl(const BlueCircleImpl&) = default;
    BlueCircleImpl& operator=(const BlueCircleImpl&) = default;
};


struct BlueCircle final : BlueCircleImpl {
    BlueCircle()                  = default;
    BlueCircle(const BlueCircle&) = default;
    BlueCircle& operator=(const BlueCircle&) = default;

    std::unique_ptr<IShape> clone() override
    {
        return std::make_unique<BlueCircle>(*this);
    }
};

BlueCircle get_circle()
{
    return BlueCircle {};
}

typedef enum { normal, colored } style;
std::unique_ptr<IShape> circle_factory(style colored)
{
    if (colored) {
        return std::make_unique<BlueCircle>();
    } else {
        return std::make_unique<Circle>();
    }
}

int main()
{
    std::unique_ptr<IShape> s = circle_factory(normal);
    auto s2                   = s->clone();
    s2->draw();

    auto c  = get_circle();
    auto c2 = c;
    c2.draw();

    // NO! Circle c = get_circle(); // compiler error -> no accidential slicing
}

