#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

string EUR(double betrag) {
    ostringstream os;
    os << setprecision(2) << fixed << showpoint << betrag << " EUR";
    return os.str();
}

double EUR2Double(const string& euro) {
    double betrag;
    istringstream(euro) >> betrag;
    return betrag;
}

int main() {
    const string euro = EUR(48.5);
    cout << euro << endl;
    cout << EUR2Double(euro) << endl;

    cout.width(5);
    const char old_fill = cout.fill('0');
    cout << 100 << ' ' << 200 << endl;

    cout.width(5);
    cout.fill(old_fill);
    cout << 100 << ' ' << 200 << endl;

    return 0;
}
