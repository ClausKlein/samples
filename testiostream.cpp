// read a file into memory
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

using namespace std;


static long readConfigDataFromString(const char *str) {
    long sum = 0;

    if (str) {
        int key = -1;
        int value = -1;
        size_t line = 0;
        char col;
        char end;
        string comment;
        istringstream iss(str); //NOTE: implicitly use of string tmp(str);

        //===========================================
        const string &tmp = iss.str();  // get a temporary of type 'basic_string<[...], char_traits<char>
        //XXX tmp[0] = tmp[0];  //NOTE: copy on write (if refcounting is used for this std::string)!
        assert(iss.str() == tmp);                   // content
        assert(iss.str().length() == tmp.length()); // length
        assert(iss.str().c_str() == tmp.c_str());   // buffer addr
        //===========================================

        // for each line
        while (iss.good()) {
            iss >> key >> col >> value >> end;
            // TODO read over non valid contents ...
            getline(iss, comment);   // or: getline(iss, s, '\n');
            line++;
            if (iss.good() && col == ':' && end == ';') {
                cout << key << ':' << value << ';' << endl;
                sum += key;
                sum += value;
            } else {
                //TODO format error ...
                cerr << "ERROR line " << line << " near : " << comment << endl;
                //XXX break;
            }
        }
    }

    return sum;
}


int main() {
    int key = -1;
    int value = -1;
    long sum = 0;
    size_t line = 0;
    char col;
    char end;
    string comment("\n");
    ifstream ifs;

    ifs.open("data.txt");

    // for each line
    while (ifs.good()) {
        ifs >> key >> col >> value >> end;
        // TODO read over non valid contents ...
        getline(ifs, comment);   // or: getline(ifs, s, '\n');
        line++;
        if (ifs.good() && col == ':' && end == ';') {
            cout << key << ':' << value << ';' << endl;
            sum += key;
            sum += value;
        } else {
            //TODO format error ...
            cerr << "ERROR line " << line << " near : " << comment << endl;
            //XXX break;
        }
    }

    ifs.close();
    assert(sum == 143);
    cerr << "sum=" << sum << endl;
    cout << "-----------------------------" << endl;

    // ==================================================
    // and again read in as row binary at one step;
    // ==================================================
    {
        streamsize length;
        char * buffer;
        ifs.open("data.txt", ios::binary);

        // get length of file:
        ifs.seekg(0, ios::end);
        length = ifs.tellg();   // FIXME: implicit conversion loses integer precision (streamoff to streamsize)
        ifs.seekg(0, ios::beg);

        // allocate memory for the file contents:
        assert(length > 0);
        buffer = new char [length + 1];

        // read data to the allocated buffer:
        ifs.read(buffer, length);
        if (ifs.good()) {
            cout.write(buffer, length);
            cout << "-----------------------------" << endl;
            buffer[length ] = 0; // termiantes as string
            long sum = readConfigDataFromString(buffer);
            assert(sum == 143);
            cerr << "sum=" << sum << endl;
        }
        delete[] buffer;

        ifs.close();
    }

    return 0;
}

// =======
// output:
// =======
// 1:1;
// 2:3;
// 5:8;
// 13:21;
// 34:55;
// sum=143
// -----------------------------
// 0x77:088;
//
// 1:1;
// 2:3;
//  5: 8;
//  13: 21;
//  089 : 0xXX ;     #XXX non vaild line!
//
//  34: 55;
//
// error:EOF
// -----------------------------
// 1:1;
// 2:3;
// 5:8;
// 13:21;
// 34:55;
// sum=143

