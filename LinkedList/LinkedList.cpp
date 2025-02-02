#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <string>

using namespace std;

template <typename T>
struct Wezel {
    T dane;
    Wezel* poprzedni;
    Wezel* nastepny;

    Wezel(T dane) : dane(dane), poprzedni(nullptr), nastepny(nullptr) {}
};

struct some_object {
    int number;
    char letter;

    some_object(int n, char l) : number(n), letter(l) {}

    string toString() const {
        return "{" + to_string(number) + ", " + letter + "}";
    }
};


template <typename T>
class Lista {
private:
    int rozmiar;
    Wezel<T>* czolo;
    Wezel<T>* ogon;

public:
    Lista() : rozmiar(0), czolo(nullptr), ogon(nullptr) {}

    // (a) 
    void dodajNaKoniec(T dane) {
        Wezel<T>* nowy_wezel = new Wezel<T>(dane);
        if (ogon == nullptr) {
            czolo = ogon = nowy_wezel;
        }
        else {
            ogon->nastepny = nowy_wezel;
            nowy_wezel->poprzedni = ogon;
            ogon = nowy_wezel;
        }
        rozmiar++;
    }

    // (b) 
    void dodajNaPoczatek(T dane) {
        Wezel<T>* nowy_wezel = new Wezel<T>(dane);
        if (czolo == nullptr) {
            czolo = ogon = nowy_wezel;
        }
        else {
            nowy_wezel->nastepny = czolo;
            czolo->poprzedni = nowy_wezel;
            czolo = nowy_wezel;
        }
        rozmiar++;
    }

    // (c) Usunięcie ostatniego elementu
    void usunOstatni() {
        if (ogon == nullptr) return;
        if (czolo == ogon) {
            delete ogon;
            czolo = ogon = nullptr;
        }
        else {
            Wezel<T>* temp = ogon;
            ogon = ogon->poprzedni;
            ogon->nastepny = nullptr;
            delete temp;
        }
        rozmiar--;
    }

    // (d) Usunięcie pierwszego elementu
    void usunPierwszy() {
        if (czolo == nullptr) return;
        if (czolo == ogon) {
            delete czolo;
            czolo = ogon = nullptr;
        }
        else {
            Wezel<T>* temp = czolo;
            czolo = czolo->nastepny;
            czolo->poprzedni = nullptr;
            delete temp;
        }
        rozmiar--;
    }

    // (e) Zwrócenie danych -itego elementu listy
    T zwrocElement(int indeks) {
        if (indeks < 0 || indeks >= rozmiar) {
            throw out_of_range("Indeks poza zakresem.");
        }
        Wezel<T>* obecny = czolo;
        for (int i = 0; i < indeks; i++) {
            obecny = obecny->nastepny;
        }
        return obecny->dane;
    }

    // (f) Ustawienie (podmiana) danych i-tego elementu listy
    void ustawElement(int indeks, T nowe_dane) {
        if (indeks < 0 || indeks >= rozmiar) {
            throw out_of_range("Indeks poza zakresem.");
        }
        Wezel<T>* obecny = czolo;
        for (int i = 0; i < indeks; i++) {
            obecny = obecny->nastepny;
        }
        obecny->dane = nowe_dane;
    }

    // (g) Wyszukanie elementu
    Wezel<T>* wyszukajElement(T dane) {
        Wezel<T>* obecny = czolo;
        while (obecny != nullptr) {
            if (obecny->dane == dane) {
                return obecny;
            }
            obecny = obecny->nastepny;
        }
        return nullptr;
    }

    // (h) Wyszukanie i usunięcie elementu
    bool wyszukajIUsun(T dane) {
        Wezel<T>* obecny = czolo;
        while (obecny != nullptr) {
            if (obecny->dane->number == dane->number && obecny->dane->letter == dane->letter) {
                if (obecny == czolo) {
                    czolo = czolo->nastepny;
                    if (czolo) czolo->poprzedni = nullptr;
                }
                else if (obecny == ogon) {
                    ogon = ogon->poprzedni;
                    if (ogon) ogon->nastepny = nullptr;
                }
                else {
                    obecny->poprzedni->nastepny = obecny->nastepny;
                    obecny->nastepny->poprzedni = obecny->poprzedni;
                }
                delete obecny->dane;
                delete obecny;
                rozmiar--;
                return true;
            }
            obecny = obecny->nastepny;
        }
        return false;
    }

    // (i) Dodanie elementu z wymuszeniem porządku
    void dodajZPorzadkiem(T dane, bool (*cmp)(const T, const T)) {
        Wezel<T>* nowy_wezel = new Wezel<T>(dane);
        if (czolo == nullptr || cmp(dane, czolo->dane)) {
            dodajNaPoczatek(dane);
        }
        else if (cmp(ogon->dane, dane)) {
            dodajNaKoniec(dane);
        }
        else {
            Wezel<T>* obecny = czolo;
            while (obecny->nastepny != nullptr && cmp(obecny->nastepny->dane, dane)) {
                obecny = obecny->nastepny;
            }
            nowy_wezel->nastepny = obecny->nastepny;
            nowy_wezel->poprzedni = obecny;
            if (obecny->nastepny != nullptr) {
                obecny->nastepny->poprzedni = nowy_wezel;
            }
            obecny->nastepny = nowy_wezel;
        }
        rozmiar++;
    }

    // (j) Czyszczenie listy
    void czyscListe() {
        while (czolo != nullptr) {
            Wezel<T>* temp = czolo;
            czolo = czolo->nastepny;
            delete temp->dane;
            delete temp;
        }
        ogon = nullptr;
        rozmiar = 0;
    }

    // (k) Zwrócenie napisowej reprezentacji listy
    string toString() const {
        string result = "Lista (rozmiar: " + to_string(rozmiar) + "): ";
        Wezel<T>* obecny = czolo;
        while (obecny != nullptr) {
            result += obecny->dane->toString() + " ";
            obecny = obecny->nastepny;
        }
        return result;
    }

    ~Lista() {
        czyscListe();
    }
};

bool some_objects_cmp(const some_object* so1, const some_object* so2) {
    if (so1->number != so2->number) {
        return so1->number < so2->number;
    }
    else {
        return so1->letter < so2->letter;
    }
}

some_object* generate_random_object() {
    int number = rand() % 10001;
    char letter = 'a' + rand() % 26;
    return new some_object(number, letter);
}


int main() {
    srand(time(0));


    const int MAX_ORDER = 6;
    Lista<some_object*>* ll = new Lista<some_object*>();

    for (int o = 1; o <= MAX_ORDER; o++) {
        const int n = pow(10, o);

        clock_t t1 = clock();
        for (int i = 0; i < n; i++) {
            some_object* so = generate_random_object();
            ll->dodajNaKoniec(so);
        }
        clock_t t2 = clock();
        double czas_dodawania = (t2 - t1) * 1000.0 / CLOCKS_PER_SEC;
        cout << "Dodano " << n << " elementow. Czas dodawania: " << czas_dodawania << " ms.\n";

        const int m = pow(10, 4);
        t1 = clock();
        for (int i = 0; i < m; i++) {
            some_object* so = generate_random_object();
            ll->wyszukajIUsun(so);
            delete so;
        }
        t2 = clock();
        double czas_wyszukiwania = (t2 - t1) * 1000.0 / CLOCKS_PER_SEC;
        cout << "Wyszukano i usunieto " << m << " elementow. Czas wyszukiwania i usuwania: " << czas_wyszukiwania << " ms.\n";

        ll->czyscListe();
    }

    delete ll;

    /*

    Lista<some_object*>* testowalista = new Lista<some_object*>;

    testowalista->dodajNaKoniec(new some_object(10, 'y'));
    testowalista->dodajNaKoniec(new some_object(15, 'x'));

    cout << "po dodaniu na koniec: " << testowalista->toString() << endl;

    testowalista->dodajNaPoczatek(new some_object(20, 'b'));

    cout << "po dodaniu na poczatek: " << testowalista->toString() << endl;

    testowalista->usunPierwszy();
    cout << "po usunieciu pierwszego : " << testowalista->toString() << endl;

    testowalista->usunOstatni();
    cout << "po usunieciu ostatniego : " << testowalista->toString( ) << endl;


    // Zwracanie i podmiana elementu na indeksie
    testowalista->dodajNaKoniec(new some_object(15, 'b'));
    testowalista->dodajNaKoniec(new some_object(25, 'c'));
    some_object* element = testowalista->zwrocElement(1);
    cout << "stan listy: " << testowalista->toString() << endl;
    cout << "Element na indeksie 1: " << element->toString() << endl;// element o indeksie 1

    testowalista->ustawElement(1, new some_object(30, 'd'));
    cout << "Po podmianie elementu na indeksie 1: " << testowalista->toString() << endl;


    // Wyszukanie i usunięcie elementu
    cout << "wyszukiwanie obiektu {30,d} i usuniecie. " << endl;
    some_object* do_usuniecia = new some_object(30, 'd');
    bool usunieto = testowalista->wyszukajIUsun(do_usuniecia);
    cout << (usunieto ? "Usunieto element." : "Nie znaleziono elementu do usuniecia.") << endl;
    cout << "Po usunieciu elementu: " << testowalista->toString() << endl;


    testowalista->czyscListe();
    cout << "po wyczyszczeniu listy : " << testowalista->toString() << endl;
    */
    return 0;
}
