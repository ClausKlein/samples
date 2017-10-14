#include <vector>
#include <string>
#include <iostream>

template<typename T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& v)
{
#if 0
    s.put('{');
    char comma[3] = {'\0', ' ', '\0'};  // default separator (empty)
    for (const auto& e : v)
    {
        s << comma << e;    //NOTE: print separator before item
        comma[0] = ',';     // change separator
    }
    return s << '}';
#else
    if (!v.empty())
    {
        s << '{' << *v.begin();
        for (auto it = ++(v.begin()); it != v.end(); ++it)
        {
            s << ", " << *it;
        }
        s << '}' << std::endl;
    }
    else
    {
        s << "{}" << std::endl;
    }
    return s;
#endif
}

int main()
{
    // c++11 initializer list syntax:
    std::vector<std::string> words1 {"the", "frogurt", "is", "also", "cursed"};
    std::cout << "words1: " << words1 << std::endl;

    // words2 == words1
    std::vector<std::string> words2(words1.begin(), words1.end());
    std::cout << "words2: " << words2 << std::endl;

    // words3 == words1
    std::vector<std::string> words3(words1);
    std::cout << "words3: " << words3 << std::endl;

    // words4 is ["Mo", "Mo", "Mo", "Mo", "Mo"]
    std::vector<std::string> words4(5, "Mo");
    std::cout << "words4: " << words4 << std::endl;

    // ints is [0, 0, 0, 0, 0]
    std::vector<int> ints(5);
    std::cout << "ints: " << ints << std::endl;

    std::vector<int> empty;
    std::cout << "empty: " << empty << std::endl;
}
