// read file data using associated buffer's members
#include <iostream>     // std::cout
#include <fstream>      // std::filebuf, std::ifstream

int main()
{
    std::ifstream ifs("data.txt", std::ifstream::binary);

    // get pointer to associated buffer object
    std::filebuf* pbuf = ifs.rdbuf();

    // get file size using buffer's members
    off_t size = pbuf->pubseekoff(0, ifs.end, ifs.in);
    pbuf->pubseekpos(0, ifs.in);

    // allocate memory to contain chunks of file data
    const size_t CHUNCK_SIZE = 23;
    char* buffer = new char[CHUNCK_SIZE];

    while ((size > 0) && ifs)
    {
        // get file data
        // streamsize sgetn (char_type* s, streamsize n);
        // streamsize is a signed integral type.
        std::streamsize len = pbuf->sgetn(buffer, CHUNCK_SIZE);

        std::cerr << len << " characters read" << std::endl;

        // write content to stdout
        std::cout.write(buffer, len);
        if (!std::cout)
        {
            break;
        }

        size -= len;
    }

    // cleanup
    ifs.close();
    delete[] buffer;

    return 0;
}

