// see
// http://www.bogotobogo.com/cplusplus/friendclass.php
//
// Friend Classes
//
// Just like functions are made friends of classes, we can also make one class
// to be a friend of another class. Then, the friend class will have access to
// all the private members of the other class.

#include <iostream>
using namespace std;

class Square;

class Rectangle
{
  int width, height;

public:
  Rectangle(int w = 1, int h = 1)
    : width(w)
    , height(h)
  {
  }
  void display() { cout << "Rectangle: " << width * height << endl; };
  void morph(Square&);
};

class Square
{
  int side;

public:
  Square(int s = 1)
    : side(s)
  {
  }
  void display() { cout << "Square: " << side * side << endl; };
  friend class Rectangle;
};

void Rectangle::morph(Square& s)
{
  width = s.side;
  height = s.side;
}

int main()
{
  Rectangle rec(5, 10);
  Square sq(5);
  cout << "Before:" << endl;
  rec.display();
  sq.display();

  rec.morph(sq);

  cout << "\nAfter:" << endl;
  rec.display();
  sq.display();

  return 0;
}

// We declared Rectangle as a friend of Square so that Rectangle member
// functions could have access to the private member, Square::side
//
// In our example, Rectangle is considered as a friend class by Square but
// Rectangle does not consider Square to be a friend, so Rectangle can access
// the private members of Square but not the other way around.
