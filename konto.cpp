#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <set>
using namespace std;

class Konto
{
public:
    Konto(int n, double s = 0.0) : nummer(n), stand(s) {}
    int gibNummer() const { return nummer; }
    double gibStand() const { return stand; }
    void einzahlen(double betrag) { stand += betrag; }
    friend ostream &operator<<(ostream &os, const Konto &k)
    {
        return os << k.nummer << ": " << k.stand;
    }

private:
    const int nummer;
    double stand;
};

struct VglKNr : binary_function<Konto, Konto, bool>
{
    bool operator()(const Konto &x, const Konto &y) const
    {
        return x.gibNummer() < y.gibNummer();
    }
};

int main()
{
    Konto k[3] = {Konto(10, 1.99), Konto(20, 2.98), Konto(30, 3.97)};
    typedef set<Konto, VglKNr> KontoSet;
    KontoSet s(k, k + 3);
    auto i = s.find(Konto(20));
    if (i != s.end()) {
        Konto tmp(*i);
        tmp.einzahlen(100.0);
        s.erase(i++);
        s.insert(i, tmp);
        cout << "Kontostand: " << i->gibStand() << endl;
    }
    copy(s.begin(), s.end(), ostream_iterator<Konto>(cout, "\n"));
}
