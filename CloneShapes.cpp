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

struct IShape
{
    virtual void draw() = 0;
    virtual auto clone() -> std::unique_ptr<IShape> = 0;
    virtual ~IShape() = default;

protected:
    IShape() = default;
    IShape(const IShape &) = default;
    auto operator=(const IShape &) -> IShape & = default;
};

struct CircleImpl : IShape
{
    void draw() override { std::cout << "Draw Circle\n"; };

protected:
    CircleImpl() = default;
    CircleImpl(const CircleImpl &) = default;
    auto operator=(const CircleImpl &) -> CircleImpl & = default;
};

struct Circle final : CircleImpl
{
    Circle() = default;
    Circle(const Circle &) = default;
    auto operator=(const Circle &) -> Circle & = default;

    auto clone() -> std::unique_ptr<IShape> override
    {
        return std::make_unique<Circle>(*this);
    }
};

struct BlueCircleImpl : CircleImpl
{
    void draw() override { std::cout << "Draw BlueCircle\n"; }

protected:
    BlueCircleImpl() = default;
    BlueCircleImpl(const BlueCircleImpl &) = default;
    auto operator=(const BlueCircleImpl &) -> BlueCircleImpl & = default;
};

struct BlueCircle final : BlueCircleImpl
{
    BlueCircle() = default;
    BlueCircle(const BlueCircle &) = default;
    auto operator=(const BlueCircle &) -> BlueCircle & = default;

    auto clone() -> std::unique_ptr<IShape> override
    {
        return std::make_unique<BlueCircle>(*this);
    }
};

auto get_circle() -> BlueCircle { return BlueCircle{}; }

using style = enum
{
    normal,
    colored
};
auto circle_factory(style colored) -> std::unique_ptr<IShape>
{
    if (colored) {
        return std::make_unique<BlueCircle>();
    } else {
        return std::make_unique<Circle>();
    }
}

auto main() -> int
{
    std::unique_ptr<IShape> s = circle_factory(normal);
    auto s2 = s->clone();
    s2->draw();

    auto c = get_circle();
    auto c2 = c;
    c2.draw();

    // NO! Circle c = get_circle(); // compiler error -> no accidential slicing
}
