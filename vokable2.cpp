#include <iostream>
#include <map>
#include <string>
using namespace std;

auto main() -> int
{
    using VokMulMap = multimap<string, string>;
    using V = VokMulMap::value_type;
    const V f[] = {V("two", "Zwei"), V("three", "Drei"), V("four", "Vier"),
                   V("two", "Paar")};

    VokMulMap vokabeln(f, f + 4);
    const auto ins = vokabeln.insert(V("one", "Eins"));
    vokabeln.insert(ins, V("one", "man"));

    cout << "Folgende Vokabeln sind enthalten:\n";
    auto iter = vokabeln.begin();
    for (; iter != vokabeln.end(); ++iter) {
        cout << "  (" << iter->first << ", " << iter->second << ")\n";
    }

    cout << "Bedeutungen für one: ";
    const auto von = vokabeln.lower_bound("one"),
               bis = vokabeln.upper_bound("one");
    for (iter = von; iter != bis; ++iter) {
        cout << iter->second << ' ';
    }

    cout << "\nBedeutungen für two: ";
    const pair<VokMulMap::iterator, VokMulMap::iterator> p =
        vokabeln.equal_range("two");
    for (iter = p.first; iter != p.second; ++iter) {
        cout << iter->second << ' ';
    }

    VokMulMap::size_type n = vokabeln.count("one");
    cout << '\n' << n << "-mal ist 'one' enthalten.\n";

    n = vokabeln.erase("one");
    cout << n << "-mal wurde 'one' gelöscht.\n";

    return 0;
}
