#include <bitset>
#include <iostream>

using namespace std;

int main()
{
    bitset<5> a(6), b(string("10010")), x;
    cout << "a = " << a << "\nb = " << b << "\nx = " << x << endl;
    cout << "a & b = " << (a & b) << endl;
    cout << "a | b = " << (a | b) << endl;
    cout << "a ^ b = " << (a ^ b) << endl;

    cout << "working with x:" << endl;
    x[3] = true;
    x.set(2);
    cout << x << " = " << x.to_ulong() << endl;
    cout << x.count() << " Bit(s) gesetzt\n";
    x.flip();
    cout << x << " = " << x.to_ulong() << endl;
    cout << x.count() << " Bit(s) gesetzt\n";
}

/***
Claus-MBP:tmp clausklein$ ./bitset
a = 00110
b = 10010
x = 00000
a & b = 00010
a | b = 10110
a ^ b = 10100
10011 = 19
3 Bit(s) gesetzt
Claus-MBP:tmp clausklein$
 ***/
