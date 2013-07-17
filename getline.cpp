#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    char buffer[23];
    int line = 0;
    while (cin.getline(buffer, sizeof(buffer), '\n') || (cin.gcount() > 0)) {
        int count = cin.gcount();
        if (cin.eof()) {
            cout << "Last Line  ";    //NOTE: cin.fail() == false
        } else if (cin.fail()) {
            cout << "Long Line  ";
            cin.clear(cin.rdstate() & ~ios::failbit);
        } else {
            --count;                //XXX ignore endl
            cout << "Line " << setw(6) << ++line;
        }
        cout << " (" << setw(2) << count << " char): " << buffer << endl;
    }

    return 0;
}

