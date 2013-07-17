/* copy.cpp

   Copy the file named argv[1] to a new file named in argv[2].
*/

#include <iostream>
#include <fstream>

using namespace std;

int
main(int argc, char *argv[]) {

    if (strcmp(argv[1], "--help") == 0) {
        cerr << argv[0] << " [old-file [new-file]]" << endl;
        exit(EXIT_FAILURE);
    }

    const istream &in(cin);
    ostream &out(cout);

    // Open input and output files
    if (argc > 1) {
        cerr << argv[1] << endl;
        ifstream _in(argv[1], ios::in | ios::binary);
        //FIXME const in = _in;
        if (argc > 2) {
            cerr << argv[2] << endl;
            ofstream _out(argv[2], ios::out | ios::binary | ios::trunc);
            _out << _in.rdbuf();  // or: in >> out.rdbuf();
            //FIXME private operator= out=_out;
        } else {
            cout << _in.rdbuf();
        }
    } else {
        //FIXME Bus error cout << cin.rdbuf();  // or: cin >> cout.rdbuf();
    }

    // copy contents
    //XXX out << in.rdbuf();  // or: in >> out.rdbuf();

    if (out && in) {
        exit(EXIT_SUCCESS);
    } else {
        exit(EXIT_FAILURE);
    }
}
