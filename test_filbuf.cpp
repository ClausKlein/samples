// istream constructor
#include <fstream>  // std::filebuf
#include <iostream> // std::ios, std::istream, std::cout

// FIXME this writes one byte more!
auto main() -> int
{
    std::filebuf fb;
    if (fb.open("data.txt", std::ios::in)) // XXX text mode!
    {
        std::istream is(&fb);
        while (is) {
            std::cout << char(is.get()); // FIXME why cast?
        }
        fb.close();
    }
    return 0;
}
