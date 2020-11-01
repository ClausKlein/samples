// substr.cpp

#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

static const int count = 30;
static const int access = 10000000;

auto main() -> int
{

#ifdef READ_FROM_FILE
    std::ifstream inFile("grimm.txt");
    std::stringstream strStream;
    strStream << inFile.rdbuf();
    std::string grimmsTales = strStream.str();
    size_t size = grimmsTales.size();
#else
    size_t size = 32 * 1024 * 1024;
    std::string grimmsTales;
    grimmsTales.resize(size);
#endif

    for (auto n = 0; n < 3; ++n) {
        std::cout << "Grimms' Fairy Tales size: " << size << std::endl;
        std::cout << std::endl;

        // random values
        std::random_device seed;
        std::mt19937 engine(seed());
        std::uniform_int_distribution<> uniformDist(0, size - count - 2);
        std::vector<int> randValues;
        for (auto i = 0; i < access; ++i) {
            randValues.push_back(uniformDist(engine));
        }

        std::chrono::duration<double> durString{};
        {
            auto start = std::chrono::steady_clock::now();
            for (auto i = 0; i < access; ++i) {
                grimmsTales.substr(randValues[i], count);
            }
            durString = std::chrono::steady_clock::now() - start;
            std::cout << "std::string::substr:      " << durString.count()
                      << " seconds" << std::endl;
        }

        std::chrono::duration<double> durStringView{};
        {
            std::string_view grimmsTalesView{grimmsTales.c_str(), size};
            auto start = std::chrono::steady_clock::now();
            for (auto i = 0; i < access; ++i) {
                grimmsTalesView.substr(randValues[i], count);
            }
            durStringView = std::chrono::steady_clock::now() - start;
            std::cout << "std::string_view::substr: " << durStringView.count()
                      << " seconds" << std::endl;
        }

        std::cout << std::endl;
        std::cout << "durString.count()/durStringView.count(): "
                  << durString.count() / durStringView.count() << std::endl;
        std::cout << std::endl;

        size /= 2;
        grimmsTales.resize(size);
    }
}
