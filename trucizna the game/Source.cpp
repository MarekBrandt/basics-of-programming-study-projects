#include <iostream>
#include <string.h>
#include <cstdio>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
using namespace std;

//stale i struktury******************************************************
const int MAX_LICZBA_KART_INNEGO_KOLORU = 50;
const int MAX_LICZBA_KOLOROW_Z_ZIELONYM = 10;
const int MAX_LICZBA_GRACZY = 20;
const int MAX_LICZBA_LITER_NAZWY_KOLORU = 10;
const int MAX_LICZBA_KART_ZIELONYCH = 10;
const int MAX_LICZBA_LITER_W_LINI = 5000;
const int MAX_LICZBA_KART_W_TALII = MAX_LICZBA_KOLOROW_Z_ZIELONYM * MAX_LICZBA_KART_INNEGO_KOLORU + MAX_LICZBA_KART_ZIELONYCH;
const int DEFAULT_VALUE_OF_CARDS = -100;
const int LICZBA_PRZETASOWAN = 100;
const int MOVE_DELAY_IN_MS = 100;

const char tablicaKolorow[MAX_LICZBA_KOLOROW_Z_ZIELONYM][MAX_LICZBA_LITER_NAZWY_KOLORU] =
{
	"green",
	"blue",
	"red",
	"violet",
	"yellow",
	"white",
	"black"
};

typedef struct karta
{
	int wartosc = DEFAULT_VALUE_OF_CARDS;
	char kolor[MAX_LICZBA_LITER_NAZWY_KOLORU] = "none";
	int handDeckOrPile = -100; //if hand - 0, deck - 1, pile - 2;
	int uKtoregoGracza = -100; //0 - na stosie, 1 - u pierwszego, 2 - u drugiego itd
	int ktoryStos = 0; //0 - nie na stosie, 1- pierwszy stos itd
}karta;

enum handDeckPile
{
	hand,
	deck,
	pile
};

enum poprawnosc
{
	niepoprawne,
	poprawne
};

enum taktyki
{
	taktykaProstePosuniecie = 1,
	taktykaWyborNajmniejszejKarty,
	taktykaPozbywanieSieNajwiekszychKart,
	taktykaWyborOptymalnejKarty
};

//deklaracje funkcji***********************************************************************************************************
void rozegrajRunde(int ktoryGracz, int* liczbaGraczy, int* liczbaKociolkow, karta talia[], int* liczbaKart, int* progEksplozji, int* aktywnyGracz, int taktyka);

void stworzenieTalii(int* liczbaGraczy, int* liczbaKociolkow, karta talia[], int* liczbaKart, int* progEksplozji);
void rozdanieKart(int liczbaGraczy, karta talia[]);
void wypiszStol(karta talia[], int ileKartWTalii, int aktywnyGracz, int liczbaGraczy, int liczbaKociolkow, int progEksplozji);
void wypiszStolDoPliku(karta talia[], int ileKartWTalii, int aktywnyGracz, int liczbaGraczy, int liczbaKociolkow, int progEksplozji);

void wczytajStanGryZPliku(int* liczbaGraczy, int* liczbaKociolkow, karta talia[], int* ktoraKartaWTalii, int* maxKartWKotle, int* aktywnyGracz, int czyPierwszeWyw);

void prostePosuniecie(karta talia[], int liczbaKartWTalii, int* aktywnyGracz, int liczbaStosow, int liczbaGraczy);
void wyborNajmniejszejKarty(karta talia[], int liczbaKartWTalii, int* aktywnyGracz, int liczbaStosow, int liczbaGraczy);
void pozbywanieSieNajwiekszychKart(karta talia[], int liczbaKartWTalii, int* aktywnyGracz, int liczbaStosow, int liczbaGraczy, int progEksplozji);
void wyborOptymalnejKarty(karta talia[], int liczbaKartWTalii, int* aktywnyGracz, int liczbaStosow, int liczbaGraczy, int progEksplozji);

void sprawdzPoprawnoscStanuGry(karta talia[], int liczbaKartWTalii, int liczbaKolorow, int liczbaGraczy, int maxWartoscKotla, int ktoryGracz);

void obslugaKoncaRundy(karta talia[], int liczbaKart, int liczbaKociolkow, int liczbaGraczy);

void wczytajWartosciKart(int* liczbaGraczy, int* liczbaKociolkow, int* liczbaZielonych, int* wartoscZielonych, int* liczbaKartInnychKolorow, int wartosciKart[], int* progEksplozji);
void utworzTalie(int liczbaKociolkow, int liczbaZielonych, int wartoscZielonych, int liczbaKartInnychKolorow, int wartosciKart[], karta talia[], int* liczbaKart);
void potasujTalie(karta talia[], int liczbaKart);
void zapiszTalieWPliku(karta talia[], int liczbaKart);

int policzKoloryWTalii(karta talia[], int ileKartWTalii);
int bylTakiKolor(char kolor[], char kolory[][MAX_LICZBA_LITER_NAZWY_KOLORU], int ileKolorow);

void wczytajKartyDoTalii(char liniaTekstu[], karta talia[], int* ktoraKartaWTalii, int handOrDeckOrPile, int ktoreWywolanie);
int znajdzLiczbe(char zdanie[], int poczatek);
void sprawdzZielone(karta talia[], int liczbaKartWTalii);
void sprawdzLiczbeKart(karta talia[], int liczbaKartWTalii, int liczbaKolorow);
int czyLiczba(char zdanie);
void sprawdzWartosciKart(karta talia[], int liczbaKartWTalii, int liczbaKolorow);
int najwiekszyElementTablicy(int t[], int rozmiar);
void zamienDwaElementy(int t[], int indeks1, int indeks2);
void porzadkujTablice(int t[], int n);
int sprawdzPoprawnoscIlosciKart(karta talia[], int liczbaKartWTalii, int liczbaGraczy);
void sprawdzPoprwnoscKociolki(karta talia[], int liczbaKartWTalii, int liczbaKociolkow, int maxWartoscKotla, int* ogolnyStan, int ktoryGracz, int liczbaGraczy);
void poprawnoscKoloruWKociolku(karta talia[], int liczbaKartWTalii, int liczbaKociolkow, int* ogolnyStan);
void wartoscKociolka(karta talia[], int liczbaKartWTalii, int liczbaKociolkow, int maxWartoscKotla, int* ogolnyStan, int ktoryGracz, int liczbaGraczy);
int czyJestStosOKolorze(karta talia[], int liczbaKartWTalii, char kolor[MAX_LICZBA_LITER_NAZWY_KOLORU]);
int znajdzWolnyStos(karta talia[], int liczbaKartWTalii, char kolor[MAX_LICZBA_LITER_NAZWY_KOLORU], int liczbaStosow);
void wypiszKarty(karta talia[], int ileKartWTalii, int handOrDeckOrPile, int ktoryGracz, int ktoryStos);
void wypiszKartydoPliku(karta talia[], int ileKartWTalii, int handOrDeckOrPile, int ktoryGracz, int ktoryStos, FILE* plik);
void ustawKarteNaKoniecTalii(karta talia[], int LiczbaKartTalii, int indeksKartyDoPrzestawienia);
void przeniesNaDeck(karta talia[], int ktoryStos, int ktoryGracz, int liczbaKartWTalii, int liczbaGraczy);
void sprawdzOdpornosci(karta talia[], int liczbaKart, int tabOdpornosci[], int liczbaKociolkow, int liczbaGraczy);
int policzPunkty(karta talia[], int liczbaKart, int ktoryGracz, int naIleOdporny, int odpornyNa[]);
void wypiszWyniki(karta talia[], int liczbaKart, int tabOdpornosci[], int liczbaKociolkow, int liczbaGraczy);
int czyKoniec(karta talia[], int liczbaKart, int ktoryGracz);
void polozNaStosODobrymKolorze(karta talia[], int liczbaKartWTalii, int liczbaStosow, int i);
int stosOMinWartosci(karta talia[], int liczbaKartWTalii, int indeksKarty, int liczbaStosow);
int policzWartoscStosu(karta talia[], int liczbaKartWTalii, int ktoryStos);
int czyStosWolny(karta talia[], int liczbaKartWTalii, int ktoryStos);
int znajdzMinMaxKarte(karta talia[], int liczbaKartWTalii, int aktywnyGracz, int liczbaStosow, int minMax);
void podsumowanieWynikow(int liczbaGraczy, int liczbaRund);
//*****************************************************************************************************************************
int main(int argc, char* argv[])
{
	int liczbaGraczy = 2;
	int aktywnyGracz = 1;
	int liczbaKociolkow = 0;
	int progEksplozji = 0;
	int ileKartWTalii = 0;
	karta talia[MAX_LICZBA_KART_W_TALII];
	int liczbaRund = 0;
	int ktoryGracz = 1; //ktorym graczem jest program

	ktoryGracz = atoi(argv[1]); // przy wejsciu podaje sie pierwsza cyfre jako numer gracza i druga jako numer taktyki(zgodnie z enumem taktyk)
	int taktyka = atoi(argv[2]);

	FILE* plik;

	if (ktoryGracz == 1)  //robi to tylko gracz pierwszy, dlatego wynik do pliku
	{
		plik = fopen("liczbaRund.txt", "w");
		cout << "Ile rund chcesz rozegrac? ";
		cin >> liczbaRund;
		if (plik != NULL)
		{
			fprintf(plik, "%d", liczbaRund);
			fclose(plik);
		}
		else
		{
			cout << "Nie zapisalo liczbaRund.txt" << endl;
		}
	}

	plik = fopen("liczbaRund.txt", "r");

	if (plik != NULL)
	{
		char liczba[10];
		fgets(liczba, 10, plik);
		liczbaRund = atoi(liczba);
		fclose(plik);
	}
	else
	{
		cout << "Nie odczytalo liczbaRund.txt" << endl;
	}

	plik = fopen("koncoweWyniki.txt", "w");

	if (plik != NULL)
	{
		fclose(plik);
	}
	else
	{
		cout << "Nie zapisalo pliku koncoweWyniki.txt" << endl;
	}

	for (int i = 0; i < liczbaRund; i++)
	{
		rozegrajRunde(ktoryGracz, &liczbaGraczy, &liczbaKociolkow, talia, &ileKartWTalii, &progEksplozji, &aktywnyGracz, taktyka);
	}

	if (ktoryGracz == liczbaGraczy) podsumowanieWynikow(liczbaGraczy, liczbaRund);


	return 0;
}
//tworzy odpowiednia talie kart
//*****************************************************************************************************************************
void stworzenieTalii(int* liczbaGraczy, int* liczbaKociolkow, karta talia[], int* liczbaKart, int* progEksplozji)
{
	int liczbaZielonych;
	int wartoscZielonych;
	int liczbaKartInnychKolorow;
	int wartosciKart[MAX_LICZBA_KART_INNEGO_KOLORU];

	wczytajWartosciKart(liczbaGraczy, liczbaKociolkow, &liczbaZielonych, &wartoscZielonych, &liczbaKartInnychKolorow, wartosciKart, progEksplozji);
	utworzTalie(*liczbaKociolkow, liczbaZielonych, wartoscZielonych, liczbaKartInnychKolorow, wartosciKart, talia, liczbaKart);
	potasujTalie(talia, *liczbaKart);
	zapiszTalieWPliku(talia, *liczbaKart);
}
//wczytuje ustawienia gry
//*****************************************************************************************************************************
void wczytajWartosciKart(int* liczbaGraczy, int* liczbaKociolkow, int* liczbaZielonych, int* wartoscZielonych, int* liczbaKartInnychKolorow, int wartosciKart[], int* progEksplozji)
{
	cout << "Podaj liczbe graczy: ";
	cin >> *liczbaGraczy;

	cout << "Podaj liczbe kociolkow: ";
	cin >> *liczbaKociolkow;

	cout << "Podaj liczbe zielonych kart: ";
	cin >> *liczbaZielonych;

	cout << "Podaj wartosc zielonych: ";
	cin >> *wartoscZielonych;

	cout << "Podaj liczbe kart innych kolorow: ";
	cin >> *liczbaKartInnychKolorow;

	cout << "Podaj wartosci kart: ";
	for (int i = 0; i < *liczbaKartInnychKolorow; i++)
	{
		int wartosc;
		cin >> wartosc;
		wartosciKart[i] = wartosc;
	}

	cout << "Podaj maksymalna wartosc w kociolku: ";
	cin >> *progEksplozji;
}
//na podstawie ustawien tworzy talie
//*****************************************************************************************************************************
void utworzTalie(int liczbaKociolkow, int liczbaZielonych, int wartoscZielonych, int liczbaKartInnychKolorow, int wartosciKart[], karta talia[], int* liczbaKart)
{
	*liczbaKart = 0;
	for (int i = 0; i < liczbaZielonych; i++)
	{

		talia[i].wartosc = wartoscZielonych;
		strncpy_s(talia[i].kolor, tablicaKolorow[0], MAX_LICZBA_LITER_NAZWY_KOLORU);
		(*liczbaKart)++;
	}
	for (int j = 0; j < liczbaKociolkow; j++)
	{
		for (int i = 0; i < liczbaKartInnychKolorow; i++)
		{
			talia[*liczbaKart].wartosc = wartosciKart[i];
			strncpy_s(talia[*liczbaKart].kolor, tablicaKolorow[j + 1], MAX_LICZBA_LITER_NAZWY_KOLORU);
			(*liczbaKart)++;
		}
	}
}
//wypisuje talie do pliku talia.txt
//*****************************************************************************************************************************
void zapiszTalieWPliku(karta talia[], int liczbaKart)
{
	FILE* plik;
	plik = fopen("talia.txt", "w");
	if (plik == NULL)
	{
		cout << "Otwieranie pliku talia.txt nie powiod³o siê!" << endl;
	}
	else
	{
		for (int i = 0; i < liczbaKart; i++)
		{
			fprintf(plik, "%d %s ", talia[i].wartosc, talia[i].kolor);
		}
		fclose(plik);
	}
}
//rozdaje graczom karty po kolei
//*****************************************************************************************************************************
void rozdanieKart(int liczbaGraczy, karta talia[])
{
	int i = 0;
	while (talia[i].wartosc != DEFAULT_VALUE_OF_CARDS)
	{
		talia[i].uKtoregoGracza = i % liczbaGraczy + 1; //przydziela po karcie graczom po kolei od 1 do x gracza
		talia[i].handDeckOrPile = hand;
		talia[i].ktoryStos = 0;
		i++;
	}
}
//odczytuje stan gry z pliku stol.txt
//*****************************************************************************************************************************
void wczytajStanGryZPliku(int* liczbaGraczy, int* liczbaKociolkow, karta talia[], int* ktoraKartaWTalii, int* maxKartWKotle, int* aktywnyGracz, int czyPierwszeWyw)
{
	char wczytaneZdanie[MAX_LICZBA_LITER_W_LINI];
	//najpierw czyszcze ilosc kart, by karty ukladaly sie od nowa
	*ktoraKartaWTalii = 0;

	FILE* plik;
	plik = fopen("stol.txt", "r");
	if (plik == NULL)
	{
		printf("Nie udalo sie otworzyc pliku stol.txt\n");
		Sleep(MOVE_DELAY_IN_MS);
		wczytajStanGryZPliku(liczbaGraczy, liczbaKociolkow, talia, ktoraKartaWTalii, maxKartWKotle, aktywnyGracz, czyPierwszeWyw);
	}
	else
	{
		fgets(wczytaneZdanie, MAX_LICZBA_LITER_W_LINI, plik); //wczytuje pierwsze dwie linie
		*aktywnyGracz = znajdzLiczbe(wczytaneZdanie, 0);

		fgets(wczytaneZdanie, MAX_LICZBA_LITER_W_LINI, plik);//z drugiej bierze liczbe graczy

		*liczbaGraczy = znajdzLiczbe(wczytaneZdanie, 0); // wyluskuje liczbe graczy z tekstu

		fgets(wczytaneZdanie, MAX_LICZBA_LITER_W_LINI, plik);//wczytuje zdanie z max liczba w kotle
		*maxKartWKotle = znajdzLiczbe(wczytaneZdanie, 0);

		for (int i = 0; i < 2 * (*liczbaGraczy); i++)//wczytuje po lini dane, liczy ilosc kart i kolorow
		{
			memset(wczytaneZdanie, '\0', MAX_LICZBA_LITER_W_LINI * sizeof(char));
			fgets(wczytaneZdanie, MAX_LICZBA_LITER_W_LINI, plik);
			wczytajKartyDoTalii(wczytaneZdanie, talia, ktoraKartaWTalii, i % 2, i + 1);
		}

		if (czyPierwszeWyw == poprawne) *liczbaKociolkow = policzKoloryWTalii(talia, *ktoraKartaWTalii);

		//petla ktora wczyta pile
		for (int i = 0; i < *liczbaKociolkow; i++)
		{
			memset(wczytaneZdanie, '\0', MAX_LICZBA_LITER_W_LINI * sizeof(char));
			fgets(wczytaneZdanie, MAX_LICZBA_LITER_W_LINI, plik);
			wczytajKartyDoTalii(wczytaneZdanie, talia, ktoraKartaWTalii, pile, i + 1);
		}
		fclose(plik);
	}
}
//w lini tekstu wyszukuje karty i dodaje je do tali
//*****************************************************************************************************************************
void wczytajKartyDoTalii(char liniaTekstu[], karta talia[], int* ktoraKartaWTalii, int handOrDeckOrPile, int ktoreWywolanie)
{
	int ktoryGracz;

	//ktory gracz
	ktoryGracz = znajdzLiczbe(liniaTekstu, 0);

	//tablica do trzymania przycietego zdania
	char drugieZdanie[MAX_LICZBA_LITER_W_LINI];
	memset(drugieZdanie, '\0', sizeof(drugieZdanie));

	//ucina poczatek player hand ..., konczy funkcje jesli nie natrafi na spacje zamiast liczby
	char* x = strchr(liniaTekstu, ':');
	if (czyLiczba(*(x + 2)) != 1) return;
	strcpy(drugieZdanie, x + 2);


	char* znak;
	znak = strrchr(drugieZdanie, 26);
	if (znak != NULL) *znak = ' ';

	znak = strrchr(drugieZdanie, '\n');
	if (znak != NULL) *znak = ' ';

	//do podzialu zdania na slowa
	char* wskNaToken;
	wskNaToken = strtok(drugieZdanie, " ");

	while (wskNaToken != NULL)
	{
		if (strcmp(wskNaToken, "\n"))
		{
			if (atoi(wskNaToken) != 0)
				talia[*ktoraKartaWTalii].wartosc = atoi(wskNaToken);
			else
			{
				talia[*ktoraKartaWTalii].handDeckOrPile = handOrDeckOrPile;
				strcpy(talia[*ktoraKartaWTalii].kolor, wskNaToken);
				talia[*ktoraKartaWTalii].uKtoregoGracza = ktoryGracz;
				talia[*ktoraKartaWTalii].ktoryStos = 0;
				if (handOrDeckOrPile == 2)
				{
					talia[*ktoraKartaWTalii].ktoryStos = ktoreWywolanie;
					talia[*ktoraKartaWTalii].uKtoregoGracza = 0;
				}
				*ktoraKartaWTalii += 1;
			}
		}
		wskNaToken = strtok(NULL, " ");
	}
}
//zwraca liczbe kolorow ktore sa w talii (nie bierze pod uwage zielonego)
//*****************************************************************************************************************************
int policzKoloryWTalii(karta talia[], int ileKartWTalii)
{
	char kolory[MAX_LICZBA_KOLOROW_Z_ZIELONYM][MAX_LICZBA_LITER_NAZWY_KOLORU];
	int ileKolorow = 0;
	for (int i = 0; i < ileKartWTalii; i++)
	{
		if (bylTakiKolor(talia[i].kolor, kolory, ileKolorow) == 0)
		{
			strcpy(kolory[ileKolorow], talia[i].kolor);
			ileKolorow++;
		}
	}
	return ileKolorow;
}
//zwraca 1 jesli kolor juz byl w talii, zero jesli to nowy kolor, i 2 jesli to zielony
//*****************************************************************************************************************************
int bylTakiKolor(char kolor[], char kolory[][MAX_LICZBA_LITER_NAZWY_KOLORU], int ileKolorow)
{
	//by nie liczylo zielonego
	if (strcmp(kolor, "green") == 0) return 2;

	for (int i = 0; i < ileKolorow; i++)
	{
		if (strcmp(kolor, kolory[i]) == 0) return 1;
	}

	return 0;
}
//zwraca 1 jesli podany znak to cyfra, i 0 jesli to nie cyfra
//*****************************************************************************************************************************
int czyLiczba(char znak)
{
	if (znak >= '1' && znak <= '9') return 1;
	else return 0;
}
//zwraca wartosc pierwszej liczby ktora znajdzie w tekscie, jesli nie znajdzie zwraca -1
//*****************************************************************************************************************************
int znajdzLiczbe(char zdanie[], int poczatek)
{
	char drugieZdanie[MAX_LICZBA_LITER_W_LINI];
	while (zdanie[poczatek] != NULL) //wyszukuje pierwsza cyfre
	{
		if (zdanie[poczatek] >= '1' && zdanie[poczatek] <= '9')
		{
			break;
		}
		poczatek++;
	}

	strcpy(drugieZdanie, zdanie + poczatek); //dzieli stringa zeby byl od cyfry i czyta liczbe
	if (atoi(drugieZdanie) != 0)
	{
		int wynik = atoi(drugieZdanie);
		return wynik;
	}

	return -1;
}
//sprawdza poprawnosc kart zielonych, wypisuje stosowne komunikaty gdy blad
//*****************************************************************************************************************************
void sprawdzZielone(karta talia[], int liczbaKartWTalii)
{
	int liczbaZielonych = 0;
	int previousValue = -10;

	for (int i = 0; i < liczbaKartWTalii; i++)
	{
		if (strcmp(talia[i].kolor, tablicaKolorow[0]) == 0)
		{
			if (liczbaZielonych == 0) previousValue = talia[i].wartosc;

			if (previousValue != talia[i].wartosc)
			{
				liczbaZielonych = -10;
				break;
			}

			previousValue = talia[i].wartosc;
			liczbaZielonych++;
		}
	}

	if (liczbaZielonych == 0) cout << "Green cards does not exist\n";
	else if (liczbaZielonych < 0) cout << "Different green cards values occurred\n";
	//else cout << "Found " << liczbaZielonych << " green cards, all with " << previousValue << " value\n";
}
//sprawdza czy kart innego koloru niz zielony, sa takie same ilosci
//*****************************************************************************************************************************
void sprawdzLiczbeKart(karta talia[], int liczbaKartWTalii, int liczbaKolorow)
{
	int liczbaKartKoloru = 0;
	int tablicaIlosciKartKoloru[MAX_LICZBA_KOLOROW_Z_ZIELONYM];
	int saRozne = 0;
	for (int j = 1; j <= liczbaKolorow; j++) // petla po kolorach
	{
		liczbaKartKoloru = 0;
		for (int i = 0; i < liczbaKartWTalii; i++) //petla po talii
		{
			if (strcmp(talia[i].kolor, tablicaKolorow[j]) == 0)
			{
				liczbaKartKoloru++;
			}
		}
		tablicaIlosciKartKoloru[j] = liczbaKartKoloru;

		if (j > 1)
		{
			if (tablicaIlosciKartKoloru[j] != tablicaIlosciKartKoloru[j - 1])
			{
				saRozne = 1;
			}
		}
	}

	//jesli wszystko w porzadku
	if (saRozne == niepoprawne)
	{
		//cout << "The number cards of all colors is equal: " << liczbaKartKoloru << endl;
	}
	else
	{
		cout << "At least two colors with a different number of cards were found:\n";
		for (int j = 1; j <= liczbaKolorow; j++) // petla po kolorach
		{
			cout << tablicaKolorow[j] << " cards are " << tablicaIlosciKartKoloru[j] << endl;
		}
	}
}
//sprawdza czy karty inne niz zielone maja takie same wartosci (wypisuje stosowne komunikaty jesli nie)
//*****************************************************************************************************************************
void sprawdzWartosciKart(karta talia[], int liczbaKartWTalii, int liczbaKolorow)
{
	int tablicaWartosci[MAX_LICZBA_KOLOROW_Z_ZIELONYM][MAX_LICZBA_KART_INNEGO_KOLORU];
	int saRozne = 0;
	int tablicaIlosciWartosciKart[MAX_LICZBA_KOLOROW_Z_ZIELONYM];// przechowuje informacje o tym ile poszegolny kolor ma wartosci

	//schemat dzialanie: 1. Zbierz wartosci wszystkich kart, podzielone na kolory
	for (int j = 1; j <= liczbaKolorow; j++) // petla po kolorach
	{
		int ileWartosci = 0;
		for (int i = 0; i < liczbaKartWTalii; i++) //petla po talii
		{
			if (strcmp(talia[i].kolor, tablicaKolorow[j]) == 0)
			{
				tablicaWartosci[j][ileWartosci] = talia[i].wartosc;
				ileWartosci++;
			}
		}
		tablicaIlosciWartosciKart[j] = ileWartosci;
	}
	//2. Posegreguj poszczegolne kolory
	for (int i = 1; i <= liczbaKolorow; i++)
	{
		porzadkujTablice(tablicaWartosci[i], tablicaIlosciWartosciKart[i]);
	}

	//3. compare poszegolne komorki
		// 3.1 znajdz najwieksza liczbe wartosci
	int indeksNajwiekszej = najwiekszyElementTablicy(tablicaIlosciWartosciKart, liczbaKolorow);

	//3.2 porownuj
	for (int i = 0; i < tablicaIlosciWartosciKart[indeksNajwiekszej]; i++)
	{
		for (int j = 1; j <= liczbaKolorow; j++)
		{
			if (j != 1)
			{
				if (tablicaWartosci[j][i] != tablicaWartosci[j - 1][i])
				{
					saRozne = 1;
				}
			}
		}
	}
	//wypisz komunikaty
	if (saRozne == 0) //sa identyczne
	{
		/*cout << "The values of cards of all colors are identical:\n";
		for (int i = 0; i < tablicaIlosciWartosciKart[indeksNajwiekszej];i++)
		{
			cout << tablicaWartosci[1][i];
			cout << " ";
		}
		cout << endl;*/
	}
	else //sa niepoprawne
	{
		cout << "The values of cards of all colors are not identical:\n";
		for (int i = 1; i <= liczbaKolorow; i++)
		{
			cout << tablicaKolorow[i] << " cards values: ";
			for (int j = 0; j < tablicaIlosciWartosciKart[i];j++)
			{
				cout << tablicaWartosci[i][j];
				cout << " ";
			}
			cout << endl;
		}
	}
}
//zwraca indeks elementy najwiekszego w tablicy
//*****************************************************************************************************************************
int najwiekszyElementTablicy(int t[], int rozmiar)
{
	int indeks = 0, i;
	for (i = 1; i < rozmiar; i++)
		if (t[i] > t[indeks])
			indeks = i;
	return indeks;
}
//zamiena indeksy dwoch elementow w tablicy
//*****************************************************************************************************************************
void zamienDwaElementy(int t[], int indeks1, int indeks2)
{
	int pomocnicza = t[indeks1];
	t[indeks1] = t[indeks2];
	t[indeks2] = pomocnicza;
}
//algorytm porzadkujacy tablice
//*****************************************************************************************************************************
void porzadkujTablice(int t[], int n)
{
	for (int i = n - 1; i > 0; i--)
		zamienDwaElementy(t, i, najwiekszyElementTablicy(t, i + 1));
}
//sprawdza poprawnosc stanu gry, wypisuje komunikaty jesli sa bledy
//*****************************************************************************************************************************
void sprawdzPoprawnoscStanuGry(karta talia[], int liczbaKartWTalii, int liczbaKolorow, int liczbaGraczy, int maxWartoscKotla, int ktoryGracz)
{
	sprawdzZielone(talia, liczbaKartWTalii);
	sprawdzLiczbeKart(talia, liczbaKartWTalii, liczbaKolorow);
	sprawdzWartosciKart(talia, liczbaKartWTalii, liczbaKolorow);

	int ogolnyStan = poprawne;
	if (sprawdzPoprawnoscIlosciKart(talia, liczbaKartWTalii, liczbaGraczy) == niepoprawne)
	{
		cout << "The number of players cards on hand is wrong\n";
		ogolnyStan = niepoprawne;
	}

	sprawdzPoprwnoscKociolki(talia, liczbaKartWTalii, liczbaKolorow, maxWartoscKotla, &ogolnyStan, ktoryGracz, liczbaGraczy);

	//if (ogolnyStan == poprawne) cout << "Current state of the game is ok" << endl;
}
//sprawdza czy ktorys gracz nie ma kart za duzo, zwraca 0 jesli jest niepoprawnie i 1 jesli poprawnie
//*****************************************************************************************************************************
int sprawdzPoprawnoscIlosciKart(karta talia[], int liczbaKartWTalii, int liczbaGraczy)
{
	int rekaPoprzedniego = 0;
	for (int i = 1; i <= liczbaGraczy; i++)
	{
		int rekaGracza = 0;
		for (int j = 0; j < liczbaKartWTalii; j++)
		{
			if (talia[j].uKtoregoGracza == i && talia[j].handDeckOrPile == hand)
			{
				rekaGracza++;
			}
		}
		if (i == 1) rekaPoprzedniego = rekaGracza;
		if (abs(rekaGracza - rekaPoprzedniego) > 1) return niepoprawne;
		rekaPoprzedniego = rekaGracza;
	}
	return poprawne;
}
//sprawdza poprawnosc kart w kociolkach
//*****************************************************************************************************************************
void sprawdzPoprwnoscKociolki(karta talia[], int liczbaKartWTalii, int liczbaKociolkow, int maxWartoscKotla, int* ogolnyStan, int ktoryGracz, int liczbaGraczy)
{
	poprawnoscKoloruWKociolku(talia, liczbaKartWTalii, liczbaKociolkow, ogolnyStan);
	wartoscKociolka(talia, liczbaKartWTalii, liczbaKociolkow, maxWartoscKotla, ogolnyStan, ktoryGracz, liczbaGraczy);
}
//sprawdza czy nie ma roznych kolorow w kociolku (z wyjatkiem zielonego), wypisuje komunikat w razie bledu
//*****************************************************************************************************************************
void poprawnoscKoloruWKociolku(karta talia[], int liczbaKartWTalii, int liczbaKociolkow, int* ogolnyStan)
{
	int znalezionopierwsza = niepoprawne;
	char poprzedniKolor[MAX_LICZBA_LITER_NAZWY_KOLORU];
	for (int i = 1; i <= liczbaKociolkow; i++)
	{
		znalezionopierwsza = niepoprawne;
		for (int j = 0; j < liczbaKartWTalii; j++)
		{
			if (talia[j].handDeckOrPile == pile && talia[j].ktoryStos == i)
			{
				if (znalezionopierwsza == poprawne)
				{
					if (strcmp(poprzedniKolor, talia[j].kolor) != 0 && strcmp(tablicaKolorow[0], talia[j].kolor) != 0) //do znalezionego koloru porownuje nastepne
					{																				//jesli jest inna niz znaleziony i zielony to jest bledne
						*ogolnyStan = niepoprawne;
						cout << "Two different colors were found on the " << i << " pile\n";
						break;
					}
				}
				else
				{
					if (strcmp(tablicaKolorow[0], talia[j].kolor) != 0) //jesli znajde na stosie karte inna niz zielona
					{
						strcpy(poprzedniKolor, talia[j].kolor);
						znalezionopierwsza = poprawne;  //zapisuje jej kolor w tab znalezionopierwsza
					}
				}
			}
		}
	}
}
//oblicza wartosc kart w kociolku
//*****************************************************************************************************************************
void wartoscKociolka(karta talia[], int liczbaKartWTalii, int liczbaKociolkow, int maxWartoscKotla, int* ogolnyStan, int ktoryGracz, int liczbaGraczy)
{
	for (int i = 1; i <= liczbaKociolkow; i++)
	{
		int sumaWKotle = 0;
		for (int j = 0; j < liczbaKartWTalii; j++)
		{
			if (talia[j].handDeckOrPile == pile && talia[j].ktoryStos == i)
			{
				sumaWKotle += talia[j].wartosc;
			}
		}
		if (sumaWKotle > maxWartoscKotla)
		{
			*ogolnyStan = niepoprawne;
			//cout << "Pile number " << i << " should explode earlier" << endl;
			przeniesNaDeck(talia, i, ktoryGracz, liczbaKartWTalii, liczbaGraczy);
		}
	}
}
//wybiera pierwszy poprawny stos i kladzie na niego karte i
//*****************************************************************************************************************************
void polozNaStosODobrymKolorze(karta talia[], int liczbaKartWTalii, int liczbaStosow, int i)
{
	int wynik = czyJestStosOKolorze(talia, liczbaKartWTalii, talia[i].kolor);
	if (wynik != 0)
	{
		talia[i].handDeckOrPile = pile;
		talia[i].ktoryStos = wynik;
		talia[i].uKtoregoGracza = 0;
	}
	else
	{
		wynik = znajdzWolnyStos(talia, liczbaKartWTalii, talia[i].kolor, liczbaStosow);
		talia[i].handDeckOrPile = pile;
		talia[i].ktoryStos = wynik;
		talia[i].uKtoregoGracza = 0;
	}
}
//zmiana aktywnego gracza na nastepnego
//*****************************************************************************************************************************
void nastepnyGracz(int liczbaGraczy, int* aktywnyGracz)
{
	if (*aktywnyGracz == liczbaGraczy)
	{
		*aktywnyGracz = 1;
	}
	else
		*aktywnyGracz += 1;
}
//taktyka opierajaca sie na polozeniu pierwszej karty na rece na pierwszy poprawny kociolek
//*****************************************************************************************************************************
void prostePosuniecie(karta talia[], int liczbaKartWTalii, int* aktywnyGracz, int liczbaStosow, int liczbaGraczy)
{
	for (int i = 0; i < liczbaKartWTalii; i++)
	{
		if (talia[i].uKtoregoGracza == *aktywnyGracz && talia[i].handDeckOrPile == hand)//karty ktore sa na rece aktywnego gracza
		{
			if (strcmp(talia[i].kolor, tablicaKolorow[0]) != 0)//jesli nie jest zielony
			{
				polozNaStosODobrymKolorze(talia, liczbaKartWTalii, liczbaStosow, i);
			}
			else//zielony wrzuc do pierwszego
			{
				talia[i].handDeckOrPile = pile;
				talia[i].ktoryStos = 1;
				talia[i].uKtoregoGracza = 0;
			}
			ustawKarteNaKoniecTalii(talia, liczbaKartWTalii, i);
			nastepnyGracz(liczbaGraczy, aktywnyGracz);

			return;
		}
	}

}
//znajdz karte najmniejsza(minMax == 0) najwieksza(minMax == 1) o najmniejszym stosie, zwroc jej indeks
//*****************************************************************************************************************************
int znajdzMinMaxKarte(karta talia[], int liczbaKartWTalii, int aktywnyGracz, int liczbaStosow, int minMax)
{
	int znalezionoPierwsza = niepoprawne;
	int wartoscKartySzukanej = 0;
	int wartoscSzukanegoStosu = 0;
	int ileTakichSamychWart = 0;
	int poszukiwanyStos = 0;
	int tablicaIndeksowKart[MAX_LICZBA_KART_INNEGO_KOLORU] = { 0 }; //ta tablica przechowa w pamieci indeksy kart, ktore mialy min wartosci
	int indeksKartyDoRuchu;

	for (int i = 0; i < liczbaKartWTalii; i++) //petla wyszukuje w rece karty o najmniejszych wart
	{
		if (talia[i].uKtoregoGracza == aktywnyGracz && talia[i].handDeckOrPile == hand)//karty ktore sa na rece aktywnego gracza
		{
			if (znalezionoPierwsza == poprawne)
			{
				if (minMax == 0)
				{
					if (wartoscKartySzukanej > talia[i].wartosc)
					{
						wartoscKartySzukanej = talia[i].wartosc;
						ileTakichSamychWart = 0; //to znaczy ze poza jedna karta, zadna wiecej nie ma takiej wart
						memset(tablicaIndeksowKart, 0, sizeof(tablicaIndeksowKart));
						tablicaIndeksowKart[0] = i;
					}
					else if (wartoscKartySzukanej == talia[i].wartosc)
					{
						ileTakichSamychWart++;
						tablicaIndeksowKart[ileTakichSamychWart] = i;
					}
				}
				else
				{
					if (wartoscKartySzukanej < talia[i].wartosc)
					{
						wartoscKartySzukanej = talia[i].wartosc;
						ileTakichSamychWart = 0; //to znaczy ze poza jedna karta, zadna wiecej nie ma takiej wart
						memset(tablicaIndeksowKart, 0, sizeof(tablicaIndeksowKart));
						tablicaIndeksowKart[0] = i;
					}
					else if (wartoscKartySzukanej == talia[i].wartosc)
					{
						ileTakichSamychWart++;
						tablicaIndeksowKart[ileTakichSamychWart] = i;
					}
				}
			}
			else
			{
				if (talia[i].wartosc)
				{
					wartoscKartySzukanej = talia[i].wartosc;
					tablicaIndeksowKart[0] = i;
					znalezionoPierwsza = poprawne;
				}
			}
		}
	}

	for (int i = 0; i <= ileTakichSamychWart; i++)
	{
		poszukiwanyStos = stosOMinWartosci(talia, liczbaKartWTalii, i, liczbaStosow);
		if (i == 0)
		{
			indeksKartyDoRuchu = tablicaIndeksowKart[i];
			wartoscSzukanegoStosu = policzWartoscStosu(talia, liczbaKartWTalii, poszukiwanyStos);
		}

		if (wartoscSzukanegoStosu > policzWartoscStosu(talia, liczbaKartWTalii, poszukiwanyStos))
		{
			wartoscSzukanegoStosu = policzWartoscStosu(talia, liczbaKartWTalii, poszukiwanyStos);
			indeksKartyDoRuchu = tablicaIndeksowKart[i];
		}
	}
	return indeksKartyDoRuchu;
}
//taktyka majaca na celu znalezienie najmniejszej karty, ktora mozna polozyc na najmniejszy kociolek
//*****************************************************************************************************************************
void wyborNajmniejszejKarty(karta talia[], int liczbaKartWTalii, int* aktywnyGracz, int liczbaStosow, int liczbaGraczy)
{
	int indeksKartyDoRuchu = znajdzMinMaxKarte(talia, liczbaKartWTalii, *aktywnyGracz, liczbaStosow, 0);

	int poszukiwanyStos = stosOMinWartosci(talia, liczbaKartWTalii, indeksKartyDoRuchu, liczbaStosow);

	talia[indeksKartyDoRuchu].handDeckOrPile = pile;
	talia[indeksKartyDoRuchu].ktoryStos = poszukiwanyStos;
	talia[indeksKartyDoRuchu].uKtoregoGracza = 0;

	ustawKarteNaKoniecTalii(talia, liczbaKartWTalii, indeksKartyDoRuchu);
	nastepnyGracz(liczbaGraczy, aktywnyGracz);
}
//taktyka majaca na celu znalezienie najmniejszej karty, ktora mozna polozyc na najmniejszy kociolek
//*****************************************************************************************************************************
void pozbywanieSieNajwiekszychKart(karta talia[], int liczbaKartWTalii, int* aktywnyGracz, int liczbaStosow, int liczbaGraczy, int progEksplozji)
{
	int indeksKartyDoRuchu = znajdzMinMaxKarte(talia, liczbaKartWTalii, *aktywnyGracz, liczbaStosow, 0);
	int poszukiwanyStos = stosOMinWartosci(talia, liczbaKartWTalii, indeksKartyDoRuchu, liczbaStosow);

	if (talia[indeksKartyDoRuchu].wartosc + policzWartoscStosu(talia, liczbaKartWTalii, poszukiwanyStos) > progEksplozji)
	{
		indeksKartyDoRuchu = znajdzMinMaxKarte(talia, liczbaKartWTalii, *aktywnyGracz, liczbaStosow, 1);
		poszukiwanyStos = stosOMinWartosci(talia, liczbaKartWTalii, indeksKartyDoRuchu, liczbaStosow);

	}

	talia[indeksKartyDoRuchu].handDeckOrPile = pile;
	talia[indeksKartyDoRuchu].ktoryStos = poszukiwanyStos;
	talia[indeksKartyDoRuchu].uKtoregoGracza = 0;

	ustawKarteNaKoniecTalii(talia, liczbaKartWTalii, indeksKartyDoRuchu);
	nastepnyGracz(liczbaGraczy, aktywnyGracz);
}
//taktyka majaca na celu znalezienie najwieszej karty, ktora nie spowoduje eksplozji, jesli nie ma takiej mozliwosci to taktyka pozbywanie sie najwiekszych kart
//*****************************************************************************************************************************
void wyborOptymalnejKarty(karta talia[], int liczbaKartWTalii, int* aktywnyGracz, int liczbaStosow, int liczbaGraczy, int progEksplozji)
{
	int indeksKartyDoRuchu = znajdzMinMaxKarte(talia, liczbaKartWTalii, *aktywnyGracz, liczbaStosow, 1);
	int poszukiwanyStos = stosOMinWartosci(talia, liczbaKartWTalii, indeksKartyDoRuchu, liczbaStosow);

	while (talia[indeksKartyDoRuchu].wartosc + policzWartoscStosu(talia, liczbaKartWTalii, poszukiwanyStos) > progEksplozji)
	{
		indeksKartyDoRuchu = znajdzMinMaxKarte(talia, liczbaKartWTalii, *aktywnyGracz, liczbaStosow, 1);
		poszukiwanyStos = stosOMinWartosci(talia, liczbaKartWTalii, indeksKartyDoRuchu, liczbaStosow);
	}

	talia[indeksKartyDoRuchu].handDeckOrPile = pile;
	talia[indeksKartyDoRuchu].ktoryStos = poszukiwanyStos;
	talia[indeksKartyDoRuchu].uKtoregoGracza = 0;

	ustawKarteNaKoniecTalii(talia, liczbaKartWTalii, indeksKartyDoRuchu);
	nastepnyGracz(liczbaGraczy, aktywnyGracz);
}
//zwraca numer stosu, na ktory moze pojsc karta, o najmniejszej wart
//*****************************************************************************************************************************
int stosOMinWartosci(karta talia[], int liczbaKartWTalii, int indeksKarty, int liczbaStosow)
{
	int ktoryStos, minWartosc, pierwszeWywolanie = niepoprawne, czyToZielony = niepoprawne;
	if (strcmp(talia[indeksKarty].kolor, tablicaKolorow[0]) != 0) //jesli karta koloru zielonego to nie szukaj stosu z tym kolorem
	{
		ktoryStos = czyJestStosOKolorze(talia, liczbaKartWTalii, talia[indeksKarty].kolor);
	}
	else
	{
		ktoryStos = 0;
		czyToZielony = poprawne;
	}

	if (ktoryStos == 0)
	{
		for (int i = 1; i <= liczbaStosow; i++)
		{
			if (czyStosWolny(talia, liczbaKartWTalii, i) == poprawne || czyToZielony == poprawne)
			{
				if (pierwszeWywolanie == poprawne)
				{
					if (minWartosc > policzWartoscStosu(talia, liczbaKartWTalii, i))
					{
						minWartosc = policzWartoscStosu(talia, liczbaKartWTalii, i);
						ktoryStos = i;
					}
				}
				else
				{
					minWartosc = policzWartoscStosu(talia, liczbaKartWTalii, i);
					pierwszeWywolanie = poprawne;
					ktoryStos = i;
				}
			}
		}
	}
	return ktoryStos;
}
//zwraca 0 jesli nie ma stosu dla danego koloru, i numer tego stosu jesli jest
//*****************************************************************************************************************************
int czyJestStosOKolorze(karta talia[], int liczbaKartWTalii, char kolor[MAX_LICZBA_LITER_NAZWY_KOLORU])
{
	for (int j = 0; j < liczbaKartWTalii; j++)
	{
		if (talia[j].handDeckOrPile == pile && strcmp(talia[j].kolor, kolor) == 0)
		{
			return talia[j].ktoryStos;
		}
	}
	return 0;
}
//zwraca indeks pierwszego wolnego stosu
//*****************************************************************************************************************************
int znajdzWolnyStos(karta talia[], int liczbaKartWTalii, char kolor[MAX_LICZBA_LITER_NAZWY_KOLORU], int liczbaStosow)
{
	for (int i = 1; i <= liczbaStosow; i++)
	{
		int zerwanoPetle = 0;
		for (int j = 0; j < liczbaKartWTalii; j++)
		{
			if (talia[j].handDeckOrPile == pile && talia[j].ktoryStos == i)
			{
				if (strcmp(talia[j].kolor, tablicaKolorow[0]) != 0 && strcmp(talia[j].kolor, kolor) != 0)
				{
					zerwanoPetle = 1;
					break;
				}
			}
		}
		if (zerwanoPetle == 0) return i;
	}
	return 0;
}
//zwraca 1 jesli (ktoryStos) stos wolny, 0 jesli nie
//*****************************************************************************************************************************
int czyStosWolny(karta talia[], int liczbaKartWTalii, int ktoryStos)
{
	for (int j = 0; j < liczbaKartWTalii; j++)
	{
		if (talia[j].handDeckOrPile == pile && talia[j].ktoryStos == ktoryStos)
		{
			if (strcmp(talia[j].kolor, tablicaKolorow[0]) != 0)
			{
				return 0;
			}
		}
	}

	return 1;
}
//zwraca wartosc stosu (ktoryStos)
//*****************************************************************************************************************************
int policzWartoscStosu(karta talia[], int liczbaKartWTalii, int ktoryStos)
{
	int wynik = 0;
	for (int j = 0; j < liczbaKartWTalii; j++)
	{
		if (talia[j].handDeckOrPile == pile && talia[j].ktoryStos == ktoryStos)
		{
			wynik += talia[j].wartosc;
		}
	}
	return wynik;
}
//wypisuje na ekran wszystko co widac na stole
//*****************************************************************************************************************************
void wypiszStol(karta talia[], int ileKartWTalii, int aktywnyGracz, int liczbaGraczy, int liczbaKociolkow, int progEksplozji)
{
	printf("active player = %d\n", aktywnyGracz);
	printf("players number = %d\n", liczbaGraczy);
	printf("explosion threshold = %d\n", progEksplozji);

	for (int i = 0; i < liczbaGraczy; i++)
	{
		printf("%d player hand cards: ", i + 1);
		wypiszKarty(talia, ileKartWTalii, hand, i + 1, 0);

		printf("%d player deck cards: ", i + 1);
		wypiszKarty(talia, ileKartWTalii, deck, i + 1, 0);

	}

	for (int i = 1; i <= liczbaKociolkow; i++)
	{
		printf("%d pile cards: ", i);
		wypiszKarty(talia, ileKartWTalii, pile, 0, i);
	}
}
//wypisuje konkretne karty na ekran
//*****************************************************************************************************************************
void wypiszKarty(karta talia[], int ileKartWTalii, int handOrDeckOrPile, int ktoryGracz, int ktoryStos)
{
	for (int i = 0; i < ileKartWTalii; i++)
	{
		if (talia[i].handDeckOrPile == handOrDeckOrPile && talia[i].uKtoregoGracza == ktoryGracz && talia[i].ktoryStos == ktoryStos)
		{
			printf("%d %s ", talia[i].wartosc, talia[i].kolor);
		}
	}
	printf("\n");
}
//zapisuje wszystko co na stole do pliku
//*****************************************************************************************************************************
void wypiszStolDoPliku(karta talia[], int ileKartWTalii, int aktywnyGracz, int liczbaGraczy, int liczbaKociolkow, int progEksplozji)
{
	FILE* plik;
	plik = fopen("stol.txt", "w");
	if (plik == NULL)
	{
		printf("Nie udalo wczytac sie pliku stol.txt\n");
	}
	else
	{
		fprintf(plik, "active player = %d\n", aktywnyGracz);
		fprintf(plik, "players number = %d\n", liczbaGraczy);
		fprintf(plik, "explosion threshold = %d\n", progEksplozji);

		for (int i = 0; i < liczbaGraczy; i++)
		{
			fprintf(plik, "%d player hand cards: ", i + 1);
			wypiszKartydoPliku(talia, ileKartWTalii, hand, i + 1, 0, plik);

			fprintf(plik, "%d player deck cards: ", i + 1);
			wypiszKartydoPliku(talia, ileKartWTalii, deck, i + 1, 0, plik);

		}

		for (int i = 1; i <= liczbaKociolkow; i++)
		{
			fprintf(plik, "%d pile cards: ", i);
			wypiszKartydoPliku(talia, ileKartWTalii, pile, 0, i, plik);
		}
		fclose(plik);
	}
}
//zapisuje konkretne karty do pliku
//*****************************************************************************************************************************
void wypiszKartydoPliku(karta talia[], int ileKartWTalii, int handOrDeckOrPile, int ktoryGracz, int ktoryStos, FILE* plik)
{
	for (int i = 0; i < ileKartWTalii; i++)
	{
		if (talia[i].handDeckOrPile == handOrDeckOrPile && talia[i].uKtoregoGracza == ktoryGracz && talia[i].ktoryStos == ktoryStos)
		{
			fprintf(plik, "%d %s ", talia[i].wartosc, talia[i].kolor);
		}
	}
	fprintf(plik, "\n");
}
//bierze karte o indeksie i przesuwa ja na sam koniec talii
//*****************************************************************************************************************************
void ustawKarteNaKoniecTalii(karta talia[], int LiczbaKartTalii, int indeksKartyDoPrzestawienia)
{
	karta chwilowaKarta;
	chwilowaKarta = talia[indeksKartyDoPrzestawienia];

	for (int i = indeksKartyDoPrzestawienia; i < LiczbaKartTalii; i++)
	{
		if (i == (LiczbaKartTalii - 1)) talia[i] = chwilowaKarta;
		else
			talia[i] = talia[i + 1];
	}
}
//bierze karty z danego stosu i przenosi je na deck gracza (ktoryGracz)
//*****************************************************************************************************************************
void przeniesNaDeck(karta talia[], int ktoryStos, int ktoryGracz, int liczbaKartWTalii, int liczbaGraczy)
{
	for (int i = 0; i < liczbaKartWTalii; i++)
	{
		if (talia[i].handDeckOrPile == pile && talia[i].ktoryStos == ktoryStos)
		{
			talia[i].handDeckOrPile = deck;
			talia[i].ktoryStos = 0;
			if (ktoryGracz == 1) talia[i].uKtoregoGracza = liczbaGraczy;
			else talia[i].uKtoregoGracza = ktoryGracz - 1;
		}
	}
}
//przeprowadza schemat skonczenia rundy, wypisanie wynikow, sprawdzenie odpornosci itd.
//*****************************************************************************************************************************
void obslugaKoncaRundy(karta talia[], int liczbaKart, int liczbaKociolkow, int liczbaGraczy)
{
	int tabOdpornosciNaKolor[MAX_LICZBA_KOLOROW_Z_ZIELONYM] = { 0 }; //przechowuje numer gracza odpornego. kolory tak jak w tab na poczatku
	sprawdzOdpornosci(talia, liczbaKart, tabOdpornosciNaKolor, liczbaKociolkow, liczbaGraczy);

	for (int i = 1; i <= liczbaKociolkow; i++)
	{
		if (tabOdpornosciNaKolor[i] != 0)
		{
			printf("Na kolor %s odporny jest gracz %d\n", tablicaKolorow[i], tabOdpornosciNaKolor[i]);
		}
	}

	wypiszWyniki(talia, liczbaKart, tabOdpornosciNaKolor, liczbaKociolkow, liczbaGraczy);
}
//wpisuje odpornosc konkretnego gracza na kolor do tablicy tabOdpornosci, zgodnie z tablica kolorow
//*****************************************************************************************************************************
void sprawdzOdpornosci(karta talia[], int liczbaKart, int tabOdpornosci[], int liczbaKociolkow, int liczbaGraczy)
{
	int znalazlemJedna = niepoprawne;
	for (int i = 1; i <= liczbaKociolkow; i++) //sprawdza kazdy kolor po kolei
	{
		int odpornyGracz = 0; //0 oznacza ze zaden gracz nie jest odporny
		int poprzedni = 0;
		int bylRemis = niepoprawne;

		for (int j = 1; j <= liczbaGraczy; j++) //petla po kazdym graczu
		{
			int aktWynik = 0;
			for (int z = 0; z < liczbaKart; z++) //liczenie kart danego koloru u gracza, szukamy tego ktory ma ich najwiecej
			{
				if (talia[z].handDeckOrPile == deck && talia[z].uKtoregoGracza == j && strcmp(talia[z].kolor, tablicaKolorow[i]) == 0) // deck, u j gracza i danego koloru
				{
					znalazlemJedna = poprawne;
					aktWynik++;
				}
			}

			if (aktWynik > poprzedni || poprzedni == 0)
			{
				poprzedni = aktWynik;
				odpornyGracz = j;
				bylRemis = niepoprawne;
			}
			else if (aktWynik == poprzedni)
			{
				bylRemis = poprawne;
			}
		}
		if (bylRemis == niepoprawne) tabOdpornosci[i] = odpornyGracz;
		else tabOdpornosci[i] = 0;
	}
	//sprawdzam czy na deckach wgl byla karta, jesli to nie bylo to tablice odpornosci ustawiam na zero
	for (int i = 0; i < liczbaKart; i++)
	{
		if (talia[i].handDeckOrPile == deck)
		{
			znalazlemJedna = poprawne;
		}
	}
	if (znalazlemJedna == niepoprawne)
	{
		for (int i = 1; i <= liczbaGraczy; i++)
		{
			tabOdpornosci[i] = 0;
		}
	}
}
//wypisuje na ekran wyniki graczy
//*****************************************************************************************************************************
void wypiszWyniki(karta talia[], int liczbaKart, int tabOdpornosci[], int liczbaKociolkow, int liczbaGraczy)
{
	FILE* plik;
	for (int i = 1; i <= liczbaGraczy; i++)
	{
		int odpornyNa[MAX_LICZBA_KOLOROW_Z_ZIELONYM];
		int licznikOdpornosci = 0;
		//sprawdz w tablicy czy gracz i jest na cos odporny
		for (int j = 1; j <= liczbaKociolkow; j++)
		{
			if (tabOdpornosci[j] == i)
			{
				odpornyNa[licznikOdpornosci] = j;
				licznikOdpornosci++;
			}
		}
		plik = fopen("koncoweWyniki.txt", "a");
		if (plik != NULL)
		{
			fprintf(plik, "Wynik gracza %d = %d\n", i, policzPunkty(talia, liczbaKart, i, licznikOdpornosci, odpornyNa));
			fclose(plik);
		}
		else
		{
			cout << "Nie odczytalo koncoweWyniki.txt" << endl;
		}
		printf("Wynik gracza %d = %d\n", i, policzPunkty(talia, liczbaKart, i, licznikOdpornosci, odpornyNa));
	}
	cout << endl;
}
//wypisuje podsumowanie wynikow
//*****************************************************************************************************************************
void podsumowanieWynikow(int liczbaGraczy, int liczbaRund)
{
	FILE* plik;
	int tabelaWynikow[MAX_LICZBA_GRACZY] = { 0 };
	int ktoryGracz = 0;
	int wynik = 0;
	int bylRemis = niepoprawne;

	plik = fopen("koncoweWyniki.txt", "r");
	if (plik != NULL)
	{
		for (int i = 0; i < liczbaGraczy * liczbaRund; i++)
		{
			fscanf(plik, "Wynik gracza %d = %d\n", &ktoryGracz, &wynik);			

			tabelaWynikow[ktoryGracz] += wynik;

		}
		fclose(plik);
	}
	else
	{
		cout << "Nie odczytalo koncoweWyniki.txt" << endl;
	}
	for (int i = 1; i <= liczbaGraczy; i++)
	{
		printf("Wynik koncowy gracza %d = %d\n", i, tabelaWynikow[i]);
		if (i == 1) { wynik = tabelaWynikow[i]; ktoryGracz = i; }
		else
		{
			if (wynik > tabelaWynikow[i]) { ktoryGracz = i; bylRemis = niepoprawne; }
			else if (wynik == tabelaWynikow[i]) bylRemis = poprawne;
		}

	}
	if (bylRemis != poprawne) {
		cout << "Zwyciezca jest gracz nr: " << ktoryGracz << endl;
	}
	else
	{
		cout << "Nastapil remis !" << endl;
	}
}
//liczy punkty na konkretnym decku w zaleznosci od odpornosci
//*****************************************************************************************************************************
int policzPunkty(karta talia[], int liczbaKart, int ktoryGracz, int naIleOdporny, int odpornyNa[])
{
	int wynik = 0;
	for (int i = 0; i < liczbaKart; i++)
	{
		if (talia[i].handDeckOrPile == deck && talia[i].uKtoregoGracza == ktoryGracz)
		{
			if (strcmp(talia[i].kolor, tablicaKolorow[0]) == 0)
			{
				wynik += 2;
			}
			else
			{
				int bylemOdporny = 0;
				for (int j = 0; j < naIleOdporny; j++) //petla sprawdzajaca odpornosci
				{
					if (strcmp(talia[i].kolor, tablicaKolorow[odpornyNa[j]]) == 0)
					{
						bylemOdporny = 1;
					}
				}
				if (bylemOdporny == 0) wynik++;
			}
		}
	}
	return wynik;
}
//sprawdza czy gracze maja jeszcze karty na rece, jesli nie maja zwraca 1, koniec rundy
//*****************************************************************************************************************************
int czyKoniec(karta talia[], int liczbaKart, int ktoryGracz) //zwraca poprawne jesli nie ma kart na rece
{
	for (int i = 0; i < liczbaKart; i++)
	{
		if (talia[i].handDeckOrPile == hand && talia[i].uKtoregoGracza == ktoryGracz) // jesli jest karta na rece, to zwraca niepoprawne
		{
			return niepoprawne;
		}
	}
	return poprawne;
}
//tasuje talie, stopien wymieszania zalezy od liczby przetasowan
//*****************************************************************************************************************************
void potasujTalie(karta talia[], int liczbaKart)
{
	srand(time(NULL));
	for (int i = 0; i < LICZBA_PRZETASOWAN; i++)
	{
		ustawKarteNaKoniecTalii(talia, liczbaKart, rand() % liczbaKart);
	}
}
//rozgrywa jedna cala runde pomiedzy graczami
//*****************************************************************************************************************************
void rozegrajRunde(int ktoryGracz, int* liczbaGraczy, int* liczbaKociolkow, karta talia[], int* liczbaKart, int* progEksplozji, int* aktywnyGracz, int taktyka)
{
	int koniec = niepoprawne;
	FILE* plik;

	//ustawienia gry
	if (ktoryGracz == 1)
	{
		stworzenieTalii(liczbaGraczy, liczbaKociolkow, talia, liczbaKart, progEksplozji); //tworzy, tasuje i zapisuje do pliku talie
		rozdanieKart(*liczbaGraczy, talia);//rozdaje karty graczom
		wypiszStolDoPliku(talia, *liczbaKart, *aktywnyGracz, *liczbaGraczy, *liczbaKociolkow, *progEksplozji);//zapisuje stan gry do pliku stol.txt
		wypiszStol(talia, *liczbaKart, *aktywnyGracz, *liczbaGraczy, *liczbaKociolkow, *progEksplozji);
		cout << endl;
		plik = fopen("czyWczytano.txt", "w");
		if (plik != NULL)
		{
			fprintf(plik, "1"); //zapisuje w pliku jesli udalo mi sie wczytac dane od uzytkownika i stworzyc nowa talie
			fclose(plik);
		}
		else
		{
			cout << "Nie odczytalo czyWczytano.txt" << endl;
		}
	}

	if (ktoryGracz != 1)
	{
		//char liczba[10];
		int czyMoznaWczytac = niepoprawne;
		int* ptr;
		ptr = &czyMoznaWczytac;


		while (czyMoznaWczytac != poprawne)
		{
			plik = fopen("czyWczytano.txt", "r");
			if (plik != NULL)
			{
				Sleep(MOVE_DELAY_IN_MS);
				if (fscanf(plik, "%d", ptr) == 1);
				else
				{

				}
				fclose(plik);
			}
			else
			{
				cout << "Nie odczytalo czyWczytano.txt" << endl;
			}
		}
	}

	int czyPierwszeWyw = poprawne;
	while (koniec == niepoprawne) //dopoki sa karty na rece graczy to gramy
	{
		wczytajStanGryZPliku(liczbaGraczy, liczbaKociolkow, talia, liczbaKart, progEksplozji, aktywnyGracz, czyPierwszeWyw);
		czyPierwszeWyw = niepoprawne;

		if (*aktywnyGracz == ktoryGracz)
		{
			wypiszStol(talia, *liczbaKart, *aktywnyGracz, *liczbaGraczy, *liczbaKociolkow, *progEksplozji); //wypisuje stan gry przed ruchem
			cout << endl;

			switch (taktyka)
			{
			case taktykaProstePosuniecie:
				prostePosuniecie(talia, *liczbaKart, aktywnyGracz, *liczbaKociolkow, *liczbaGraczy);
				break;
			case taktykaWyborNajmniejszejKarty:
				wyborNajmniejszejKarty(talia, *liczbaKart, aktywnyGracz, *liczbaKociolkow, *liczbaGraczy);
				break;
			case taktykaPozbywanieSieNajwiekszychKart:
				pozbywanieSieNajwiekszychKart(talia, *liczbaKart, aktywnyGracz, *liczbaKociolkow, *liczbaGraczy, *progEksplozji);
				break;
			case taktykaWyborOptymalnejKarty:
				wyborOptymalnejKarty(talia, *liczbaKart, aktywnyGracz, *liczbaKociolkow, *liczbaGraczy, *progEksplozji);
				break;
			default:
				break;
			}
			sprawdzPoprawnoscStanuGry(talia, *liczbaKart, *liczbaKociolkow, *liczbaGraczy, *progEksplozji, *aktywnyGracz);
			wypiszStolDoPliku(talia, *liczbaKart, *aktywnyGracz, *liczbaGraczy, *liczbaKociolkow, *progEksplozji);
			wypiszStol(talia, *liczbaKart, *aktywnyGracz, *liczbaGraczy, *liczbaKociolkow, *progEksplozji);//wypisuje stan gry po ruchu
			cout << endl;
		}
		if (czyKoniec(talia, *liczbaKart, ktoryGracz) == poprawne)
		{
			koniec = poprawne;
		}
		else
		{
			Sleep(MOVE_DELAY_IN_MS);
		}
	}
	if (ktoryGracz == *liczbaGraczy) //ostatni gracz przeprowadza obsluge konca rundy
	{
		obslugaKoncaRundy(talia, *liczbaKart, *liczbaKociolkow, *liczbaGraczy);
	}
	//na koniec czyszcze plik pozwalajacy stwierdzic czy wczytano talie
	plik = fopen("czyWczytano.txt", "w");
	if (plik != NULL)
	{
		fclose(plik);
	}
	else
	{
		cout << "Nie odczytalo czyWczytano.txt" << endl;
	}

	*aktywnyGracz = 1;
}
