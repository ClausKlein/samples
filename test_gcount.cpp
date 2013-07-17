// read a file into memory
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream

int main() {

    std::ifstream is("data.txt", std::ifstream::binary);
    if (is) {
        // get length of file:
        is.seekg(0, is.end);
        off_t length = is.tellg();
        is.seekg(0, is.beg);

        char * buffer = new char [length];

        std::cerr << "Reading " << length << " characters... " << std::endl;
        // read data as a block:
//FIXME warning: implicit conversion loses integer precision: 'off_t' (aka 'long long') to 'streamsize' (aka 'int') [-Wshorten-64-to-32]
        is.read(buffer, length);

        if (is) {
            std::cerr << "all characters read successfully." << std::endl;
        } else {
            std::cerr << "error: only " << is.gcount() << " could be read" << std::endl;
        }
        is.close();

        // ...buffer contains the entire file...

        delete[] buffer;

    }
    {
        char buf[20];

        std::cout << "Please, enter a word: ";
        std::cin.get(buf, sizeof(buf));
        std::cout << std::cin.gcount() << " characters read: " << buf << std::endl;

    }
    return 0;
}

