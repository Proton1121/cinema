#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>

using namespace std;

const float STARTOWA_SREDNIA = 2.5;
const int STARTOWA_LICZBA_OCEN = 0;
const int STARTOWE_ILE_KUPIONO = 0;
const int STARTOWA_LICZBA_KUPIONYCH_BILETOW = 0;
const int MAKS_OCENA = 5;
const float WSPOLCZYNNIK_KONESER = 0.75;
const int CZESTOTLIWOSC_KINOMANA = 3;
const int MAKS_ZNAJOMYCH = 4;
const int CZESTOTLIWOSC_SPORADYCZNYCH_LICZNIK = 9;
const int CZESTOTLIWSOC_SPORADYCZNYCH_MIANOWNIK = 10;
const int DZIEN_STARTOWY = 0;

class Film {
    string tytul;
    float srednia_ocen;
    int liczba_ocen;
    int ilekupiono;
public:
    explicit Film(string _tytul): tytul(_tytul), srednia_ocen(STARTOWA_SREDNIA), liczba_ocen(STARTOWA_LICZBA_OCEN), ilekupiono(STARTOWE_ILE_KUPIONO){};

    virtual void aktualizuj_srednia(int _ocena) {
        float nowa_srednia;
        nowa_srednia = (srednia_ocen * liczba_ocen + _ocena) / (liczba_ocen + 1);
        liczba_ocen++;
        srednia_ocen = nowa_srednia;
    };
    virtual string Tytul() const {return tytul;};
    virtual float Ocena() const {return srednia_ocen;};
    virtual int ileKupili() const {return ilekupiono;};
    virtual void kupiono(int x) {ilekupiono+=x;};

    Film() = delete;
    Film(const Film&) = delete;
    Film& operator=(const Film&) = delete;
    virtual ~Film() = default;
};

class Sala {
    int liczba_biletow_dzis;
    vector<bool> miejsca;
    Film* filmwsali;
public:
    explicit Sala(int iloscmiejsc) {
        liczba_biletow_dzis = 0;
        for (int i = 0;i < iloscmiejsc;i++) miejsca.push_back(false);
        filmwsali = nullptr;
    };

    virtual void nowyfilm(vector<Film*> filmy) {
        int i;
        i = rand() % filmy.size();
        filmwsali = filmy[i];
        for (size_t j = 0; j < miejsca.size(); j++) miejsca[j] = false;
        //jak sie wykomentuje ponizej to bedziemy liczyc ile bylo w sali w ogole
        liczba_biletow_dzis = 0;
    };
    virtual int dajLiczbabiletow() const {return liczba_biletow_dzis;};
    virtual Film* dajFilm() const {return filmwsali;};
    virtual bool zajmijmiejsca(int ile) {
        int j = 0;
        for (size_t i = 0; i < miejsca.size(); i++) { if (!miejsca[i]) j++; }
        if (j < ile) return false;
        j = 0;
        for (int i = 0; i < ile;) {
            if (!miejsca[j]) {
                miejsca[j] = true;
                i++;
            }
            j++;
        }
        liczba_biletow_dzis += ile;
        dajFilm()->kupiono(ile);
        return true;
    };

    Sala() = delete;
    Sala(const Sala&) = delete;
    Sala& operator=(const Sala&) = delete;
    virtual ~Sala() = default;
};

class Kino {
    string nazwa;
    string adres;
public:
    Kino(string _nazwa, string _adres): nazwa(_nazwa), adres(_adres) {};

    virtual void nowyrepertuar(vector<Film*> filmy) = 0;
    virtual vector<Film*> dajRepertuar() = 0;
    virtual string dajNazwe() const {return nazwa;};
    virtual string dajAdres() const {return  adres;};

    Kino() = delete;
    Kino(const Kino&) = delete;
    Kino& operator=(const Kino&) = delete;
    virtual ~Kino() = default;
};

class Kameralne: public Kino {
    Sala* sala;
public:
    Kameralne(string _nazwa,string _adres, int ilemiejsc): Kino(_nazwa, _adres){
        sala = new Sala(ilemiejsc);
    };

    virtual Sala* dajSale() const {return sala;};
    void nowyrepertuar(vector<Film*> filmy) override{sala->nowyfilm(filmy);};
    vector<Film*> dajRepertuar() override {
        vector<Film*> repertuar;
        repertuar.push_back(sala->dajFilm());
        return repertuar;
    };

    Kameralne() = delete;
    Kameralne(const Kameralne&) = delete;
    Kameralne& operator=(const Kameralne&) = delete;
    ~Kameralne() override {
        delete sala;
    };
};

class Multipleks: public Kino {
    vector<Sala*> sale;
public:
    Multipleks(string _nazwa,string _adres,const vector<int>& liczbamiejsc): Kino(_nazwa,_adres) {
        for (int i : liczbamiejsc) { sale.push_back(new Sala(i)); }
    };

    virtual vector<Sala*> dajSale() const {return sale;};
    void nowyrepertuar(vector<Film*> filmy) override {
        for (Sala* ss: sale) ss->nowyfilm(filmy);
    };
    vector<Film*> dajRepertuar() override {
        int pom = 0;
        vector<Film*> pomocniczy = {sale[0]->dajFilm()};
        for (Sala* ss:sale) {
            for(Film* ff: pomocniczy) {
                if (ss->dajFilm() == ff) {
                    pom = 1;
                    break;
                }
            }
            if (!pom) {
                pomocniczy.push_back(ss->dajFilm());
            } else {
                pom = 0;
            }
        }
        return pomocniczy;
    };

    Multipleks() = delete;
    Multipleks(const Multipleks&) = delete;
    Multipleks& operator=(const Multipleks&) = delete;
    ~Multipleks() override {
        for (auto & i : sale) delete i;
    };
};

class Mieszkaniec {
    string imie;
    string nazwisko;
    int liczba_kupionych_biletow;
public:
    Mieszkaniec(string _imie, string _nazwisko): imie(_imie), nazwisko(_nazwisko), liczba_kupionych_biletow(STARTOWA_LICZBA_KUPIONYCH_BILETOW) {};

    virtual int ocen() = 0;
    virtual void idz_do_kina(vector<Kino*> kina, vector<Film*> filmy) = 0;
    virtual string jakie_imie() const {return imie;};
    virtual string jakie_nazwisko() const {return nazwisko;};
    virtual int ile_biletow_kupil() const {return liczba_kupionych_biletow;};
    virtual void kupil(int x) {liczba_kupionych_biletow+=x;};

    Mieszkaniec() = delete;
    Mieszkaniec(const Mieszkaniec&) = delete;
    Mieszkaniec& operator=(const Mieszkaniec&) = delete;
    virtual ~Mieszkaniec() = default;
};

//koneser jest zdeterminowany
class Koneser: public Mieszkaniec {
public:
    Koneser(string _imie, string _nazwisko): Mieszkaniec(_imie, _nazwisko) {};

    int ocen() override {
        int i = floor(WSPOLCZYNNIK_KONESER*MAKS_OCENA);
        return rand()%i;
    };
    void idz_do_kina(vector<Kino*> kina, vector<Film*> filmy) override {
        float i = -1;
        int j;
        Film* wybrany;
        Kameralne* pomockam;
        Multipleks* pomocmul;
        for (Film* ff: filmy) {
            if (ff->Ocena()>i) {
                i = ff->Ocena();
                wybrany = ff;
            }
        }
        vector<Kino*> pomocniczy = kina;
        for (size_t l = 0; l<kina.size();l++) {
            j = rand()%pomocniczy.size();
            pomockam = dynamic_cast<Kameralne *>(pomocniczy[j]);
            if (pomockam) {
                if (pomockam->dajSale()->dajFilm() == wybrany) {
                    if (pomockam->dajSale()->zajmijmiejsca(1)) {
                        wybrany->aktualizuj_srednia(ocen());
                        kupil(1);
                        return;
                    }
                }
            }
            pomocmul = dynamic_cast<Multipleks*>(pomocniczy[j]);
            if (pomocmul) {
                for (Sala* ss: pomocmul->dajSale()) {
                    if (ss->dajFilm() == wybrany) {
                        if (ss->zajmijmiejsca(1)){
                            wybrany->aktualizuj_srednia(ocen());
                            kupil(1);
                            return;
                        }
                    }
                }
            }
            swap(pomocniczy[j],pomocniczy.back());
            pomocniczy.pop_back();
        }
    };

    Koneser() = delete;
    Koneser(const Koneser&) = delete;
    Koneser& operator=(const Koneser&) = delete;
    ~Koneser() override = default;
};

class Kinoman: public Mieszkaniec {
    int dni_od_ostatniego_seansu;
public:
    Kinoman(string _imie, string _nazwisko): Mieszkaniec(_imie, _nazwisko), dni_od_ostatniego_seansu(rand()%CZESTOTLIWOSC_KINOMANA) {};

    int ocen() override {
        int i;
        i = floor(MAKS_OCENA / 2) + 1;
        return (rand()%(MAKS_OCENA-i))+i;
    };
    void idz_do_kina(vector<Kino*> kina, vector<Film*> filmy) override {
        int i,j;
        Kameralne* pomockam;
        Multipleks* pomocmul;
        vector<Film*> pomocniczy;
        if (dni_od_ostatniego_seansu < CZESTOTLIWOSC_KINOMANA) {
            dni_od_ostatniego_seansu++;
            return;
        }
        int liczbaznajomych = rand()%MAKS_ZNAJOMYCH;
        i = rand()%kina.size();
        pomockam = dynamic_cast<Kameralne*>(kina[i]);
        if (pomockam) {
            if(pomockam->dajSale()->zajmijmiejsca(1+liczbaznajomych)) {
                pomockam->dajSale()->dajFilm()->aktualizuj_srednia(ocen());
                kupil(1+liczbaznajomych);
                dni_od_ostatniego_seansu = 0;
                return;
            }
            return;
        }
        pomocmul = dynamic_cast<Multipleks*>(kina[i]);
        if(pomocmul) {
            pomocniczy = pomocmul->dajRepertuar();
            j = rand()%pomocniczy.size();
            for (Sala* ss:pomocmul->dajSale()) {
                if (ss->dajFilm() == pomocniczy[j]) {
                    if(ss->zajmijmiejsca(1+liczbaznajomych)) {
                        pomocniczy[j]->aktualizuj_srednia(ocen());
                        kupil(1+liczbaznajomych);
                        dni_od_ostatniego_seansu = 0;
                        return;
                    }
                }
            }
            return;
        }
    };

    Kinoman() = delete;
    Kinoman(const Kinoman&) = delete;
    Kinoman& operator=(const Kinoman&) = delete;
    ~Kinoman() override = default;
};

class Sporadyczny: public Mieszkaniec {
public:
    Sporadyczny(string _imie, string _nazwisko): Mieszkaniec(_imie, _nazwisko) {};

    int ocen() override {
        return rand()%MAKS_OCENA;
    }
    void idz_do_kina(vector<Kino*> kina, vector<Film*> filmy) override {
        int i,j;
        Kameralne* pomockam;
        Multipleks* pomocmul;
        vector<Film*> pomocniczy;
        if (rand()%CZESTOTLIWSOC_SPORADYCZNYCH_MIANOWNIK < CZESTOTLIWOSC_SPORADYCZNYCH_LICZNIK) {
            return;
        }
        i = rand()%kina.size();
        pomockam = dynamic_cast<Kameralne*>(kina[i]);
        if (pomockam) {
            if(pomockam->dajSale()->zajmijmiejsca(1)) {
                pomockam->dajSale()->dajFilm()->aktualizuj_srednia(ocen());
                kupil(1);
                return;
            }
            return;
        }
        pomocmul = dynamic_cast<Multipleks*>(kina[i]);
        if(pomocmul) {
            pomocniczy = pomocmul->dajRepertuar();
            j = rand()%pomocniczy.size();
            for (Sala* ss:pomocmul->dajSale()) {
                if (ss->dajFilm() == pomocniczy[j]) {
                    if(ss->zajmijmiejsca(1)) {
                        pomocniczy[j]->aktualizuj_srednia(ocen());
                        kupil(1);
                        return;
                    }
                }
            }
            return;
        }
    };

    Sporadyczny() = delete;
    Sporadyczny(const Sporadyczny&) = delete;
    Sporadyczny& operator=(const Sporadyczny&) = delete;
    ~Sporadyczny() override = default;
};

class Miasto {
    string nazwa;
    int dzien;
    vector<Kino*> kina;
    vector<Mieszkaniec*> mieszkancy;
    vector<Film*> filmy;
public:
    Miasto(string _nazwa, vector<Kino*> _kina, vector<Mieszkaniec*> _mieszkancy, vector<Film*> _filmy):
    nazwa(_nazwa), dzien(DZIEN_STARTOWY), kina(_kina), mieszkancy(_mieszkancy), filmy(_filmy){};

    virtual int ktoryDzien() const {return dzien;};
    virtual vector<Kino*> dajKina() const {return kina;};
    virtual vector<Mieszkaniec*> dajMieszkancow() const {return mieszkancy;};
    virtual vector<Film*> dajFilmy() const {return filmy;};
    virtual string dajNazwe() const {return nazwa;};
    virtual void symuluj();
    virtual void symulujwieledni(int _dni);

    Miasto() = delete;
    Miasto(const Miasto&) = delete;
    Miasto& operator=(const Miasto&) = delete;
    ~Miasto() = default;
    //to bysmy zapisali gdyby bylo trzeba
    //{
    //    for (Kino* kk: kina) delete kk;
    //    for (Mieszkaniec* mm: mieszkancy) delete mm;
    //    for (Film* ff: filmy) delete ff;
    //};

    friend ostream& operator<<(ostream&, const Miasto&);
};

ostream& operator<<(ostream& os, const Miasto& miasto) {
    Kameralne* pomockam;
    Multipleks* pomocmul;
    os << "Dzień: " << miasto.ktoryDzien() <<"\n\n" << "Filmy:\n";
    for (Film* ff:miasto.dajFilmy()) {
        os << ff->Tytul() << ", ocena: " << ff->Ocena() << ", sprzedano biletow: " << ff->ileKupili() << "\n";
    }
    os <<"\n";
    os << "Kina:\n";
    for (Kino* kk:miasto.dajKina()) {
        os << kk->dajNazwe() << ", " << kk->dajAdres() <<", kupione bilety:\n";
        pomockam = dynamic_cast<Kameralne*>(kk);
        if (pomockam) {
            os << "Sala nr1: " << pomockam->dajSale()->dajFilm()->Tytul() << " - " << pomockam->dajSale()->dajLiczbabiletow() << "\n";
        }
        pomocmul = dynamic_cast<Multipleks*>(kk);
        if (pomocmul) {
            for (size_t i = 1; i<=pomocmul->dajSale().size();i++) {
                os << "Sala nr" << i << ": " << pomocmul->dajSale()[i-1]->dajFilm()->Tytul() << " - " << pomocmul->dajSale()[i-1]->dajLiczbabiletow() << "\n";
            }
        }
    }
    os <<"\n";
    os << "Mieszkancy:\n";
    for (Mieszkaniec* mm:miasto.dajMieszkancow()) {
        os << mm->jakie_imie() << " " << mm->jakie_nazwisko() << " kupil " << mm->ile_biletow_kupil() << " biletow\n";
    }
    os <<"\n";
    return os;
}

void Miasto::symuluj() {
    for (Kino* kk:kina) kk->nowyrepertuar(filmy);
    for (Mieszkaniec* mm:mieszkancy) mm->idz_do_kina(kina,filmy);
    dzien++;
    cout << *this;
}

void Miasto::symulujwieledni(int _dni) {
    for(int i = 0; i < _dni; i++) symuluj();
}

int main() {
    srand(time(nullptr));
    Kameralne Muranek("Muranek", "Bankowa 33", 10);
    Kameralne Iluzjonista("Iluzjonista", "Racławicka 93", 7);
    Multipleks Maxx("Maxx", "Krakowska 21", {6,6,6,10});
    Multipleks Saturn("Saturn", "Bakałarzy 5", {7,7,12});
    Film Draka("Wielka draka w wietnamskiej dzielnicy");
    Film Baltic("Baltic Waves");
    Film Faraonek("Faraonek");
    Film Czerwony("Na czerwonym globie");
    Film Ktos("Ktoś");
    Film Pancernik("Pancernik Arendt");
    Film Niefortunny("Niefortunny żarcik");
    Film Obcy("Trochę Obcy");
    Film Kozy("Milczenie kózek");
    Film Tompapa("Tompapa");
    Kinoman JM ("Julia", "Matczak");
    Kinoman AB ("Antoni", "Baranek");
    Kinoman WJ ("Witold", "Jamski");
    Kinoman RM ("Rafał", "Mrozek");
    Kinoman MM ("Marcin", "Mnoger");
    Kinoman JD ("Jakub", "Dmuchrowski");
    Kinoman TM ("Tomasz", "Misztal");
    Koneser MC ("Marcin", "Czerkawski");
    Koneser TZ ("Tomasz", "Zamojski");
    Koneser AW ("Anna", "Wąsowicz");
    Sporadyczny AC ("Anna", "Czarna");
    Sporadyczny JA ("Justyna", "Adamowicz");
    Sporadyczny JM2 ("Joanna", "Masarz");
    Sporadyczny JP ("Joanna", "Pysiak");
    Sporadyczny UU ("Urszula", "Ursus");
    Sporadyczny AD ("Agnieszka", "Dyla");
    Sporadyczny MM2 ("Maria", "Miła");
    Sporadyczny JJ ("Jacek", "Jacuś");
    Sporadyczny MG ("Michał", "Głuchy");
    Sporadyczny AP ("Antoni", "Psikuta");
    Miasto Olsztyn("Olsztyn",{&Muranek,&Iluzjonista,&Maxx,&Saturn},
                   {&JM,&AB,&WJ,&RM,&MM,&JD,&TM,&MC,&TZ,&AW,&AC,&JA,&JM2,&JP,&UU,&AD,&MM2,&JJ,&MG,&AP},
                   {&Draka,&Baltic,&Faraonek,&Czerwony,&Ktos,&Pancernik,&Niefortunny,&Obcy,&Kozy,&Tompapa});
    cout << Olsztyn.dajNazwe() <<":\n";
    Olsztyn.symulujwieledni(30);
}
