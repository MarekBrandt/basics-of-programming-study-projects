#include <iostream>
#include <string.h>
#include <cstdio>
#include <stdlib.h>
using namespace std;


//stale i struktury******************************************************
const int maxLiczbaKart = 26;
const int maxLiczbaKolorow = 7;
const int maxLiczbaGraczy = 6;
const int maxLiczbaLiterKoloru = 7;
const int maxLiczbaZielonych = 9;
const int maxLiczbaLiterZdania = 300;
const int maxLiczbaWszystkichKart = maxLiczbaKolorow * maxLiczbaKart + maxLiczbaZielonych;
const int maxIloscCyfrWartosci = 10;

const char tablicaKolorow[maxLiczbaKolorow][maxLiczbaLiterKoloru] =
{
	"green",
	"blue",
	"red",
	"violet",
	"yellow",
	"white",
	"black"
};

struct karta
{
	int wartosc;
	char kolor[maxLiczbaLiterKoloru];
	int handDeckOrPile; //if hand - 0, deck - 1, pile - 2;
	int uKtoregoGracza; //0 - na stosie, 1 - u pierwszego, 2 - u drugiego itd
	int ktoryStos; //0 - nie na stosie, 1- pierwszy stos itd
};
//deklaracje funkcji******************************************************
void utworzTalie(int liczbaKociolkow, int liczbaZielonych, int wartoscZielonych, int liczbaKartInnychKolorow, int wartosciKart[], karta talia[]);
void rozdanieKart(int liczbaGraczy, int liczbaKociolkow, int liczbaZielonych, int liczbaKartInnychKolorow, karta talia[]);
void wypiszKartyGracza(int liczbaKartWGrze, int liczbaGraczy, karta rekaGracza[][maxLiczbaGraczy], int ktoryGracz);
void wczytajWartosciKart(int* liczbaGraczy, int* liczbaKociolkow, int* liczbaZielonych, int* wartoscZielonych, int* liczbaKartInnychKolorow, int wartosciKart[]);
void wczytajStanGry(int* liczbaGraczy, int* ileKolorow, int tablicaIlosciKart[], int tablicaIlosciKartWKociolku[], int* czyBylZielonyWTalii);
void wypiszRozlozenieKart(int liczbaGraczy, int ileKolorow, int tablicaIlosciKart[], int tablicaIlosciKartWKociolku[]);
int znajdzLiczbe(char zdanie[], int poczatek);
int znajdzWszystkieLiczby(char zdanie[], int poczatek);
int policzKolory(char zdanie[], char kolor[][maxLiczbaLiterKoloru], int ileKolorow, int poczatek, int* czyBylZielonyWTalii);
int sprawdzCzyBylo(char slowo[], int ileKolorow, char kolor[][maxLiczbaLiterKoloru]);
void wczytajStanGryDoTalii(int* liczbaGraczy, int* liczbaKociolkow, int tablicaIlosciKart[], karta talia[], int* ktoraKartaWTalii);
void wczytajKarty(char liniaTekstu[], karta talia[], int* ktoraKartaWTalii, int handOrDeck);
int znajdzIndeksLiczby(char napis[], int poczatek);
int czyLiczba(char zdanie[], int iteracja);
int policzKarty(int ileKartWTalii, karta talia[], int ktoryGracz, int handOrDeckOrPile);
void wypiszIleMaKtoKart(int ileKartWTalii, karta talia[], int liczbaGraczy, int liczbaKociolkow);
int policzKoloryWTalii(karta talia[], int ileKartWTalii);
int bylTakiKolor(char kolor[], char kolory[][maxLiczbaLiterKoloru], int ileKolorow);
//************************************************************************
int main()
{
	int liczbaGraczy;
	int liczbaKociolkow = 0;
	int liczbaZielonych;
	int wartoscZielonych;
	int liczbaKartInnychKolorow;
	int wartosciKart[maxLiczbaKart];
	karta talia[maxLiczbaWszystkichKart];
	int tablicaIlosciKart[2 * maxLiczbaGraczy] = { 0 }; //w niej po ile kart ma gracz 0, 1 ,2 ... on hand i on deck
	int tablicaIlosciKartWKociolku[2 * maxLiczbaGraczy] = { 0 };//po ile kart w kociolkach 
	int czyBylZielonyWTalii = 0;
	int ileKartWTalii = 0;

	//wczytajWartosciKart(&liczbaGraczy, &liczbaKociolkow, &liczbaZielonych, &wartoscZielonych, &liczbaKartInnychKolorow, wartosciKart);
	//utworzTalie(liczbaKociolkow, liczbaZielonych, wartoscZielonych, liczbaKartInnychKolorow, wartosciKart, talia);
	//rozdanieKart(liczbaGraczy, liczbaKociolkow, liczbaZielonych, liczbaKartInnychKolorow, talia);

	//wczytajStanGry(&liczbaGraczy, &liczbaKociolkow, tablicaIlosciKart, tablicaIlosciKart, &czyBylZielonyWTalii);
	wczytajStanGryDoTalii(&liczbaGraczy, &liczbaKociolkow, tablicaIlosciKart, talia, &ileKartWTalii);
	
	wypiszIleMaKtoKart(ileKartWTalii, talia, liczbaGraczy, liczbaKociolkow);

	return 0;
}

//************************************************************************************************
void wczytajStanGryDoTalii(int* liczbaGraczy, int* liczbaKociolkow, int tablicaIlosciKart[], karta talia[], int* ktoraKartaWTalii)
{
	char wczytaneZdanie[maxLiczbaLiterZdania];

	fgets(wczytaneZdanie, maxLiczbaLiterZdania, stdin); //wczytuje pierwsze dwie linie
	fgets(wczytaneZdanie, maxLiczbaLiterZdania, stdin);//z drugiej bierze liczbe graczy

	*liczbaGraczy = znajdzLiczbe(wczytaneZdanie, 0); // wyluskuje liczbe graczy z tekstu

	for (int i = 0; i < 2 * (*liczbaGraczy); i++)//wczytuje po lini dane, liczy ilosc kart i kolorow
	{

		fgets(wczytaneZdanie, maxLiczbaLiterZdania, stdin);
		wczytajKarty(wczytaneZdanie, talia, ktoraKartaWTalii, i % 2);
	}

	*liczbaKociolkow = policzKoloryWTalii(talia, *ktoraKartaWTalii);

	//petla ktora wczyta pile
	for (int i = 0; i < *liczbaKociolkow; i++)
	{
		fgets(wczytaneZdanie, maxLiczbaLiterZdania, stdin);
		wczytajKarty(wczytaneZdanie, talia, ktoraKartaWTalii, 2);
	}
}
//**************************************************************************************************
void wczytajKarty(char liniaTekstu[], karta talia[], int* ktoraKartaWTalii, int handOrDeckOrPile)//if hand - 0, deck - 1, pile - 2
{
	int ktoryGracz;

	//ktory gracz
	ktoryGracz = znajdzLiczbe(liniaTekstu, 0);

	//tablica do trzymania przycietego zdania
	char drugieZdanie[maxLiczbaLiterZdania];
	//memset(drugieZdanie, '\0', sizeof(drugieZdanie));

	//ucina poczatek player hand ...
	int x = znajdzIndeksLiczby(liniaTekstu, 1);
	strcpy(drugieZdanie, liniaTekstu + x);

	//nadpisz ostatniego endline
	drugieZdanie[strlen(drugieZdanie) - 1] = ' ';

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
				*ktoraKartaWTalii += 1;
			}
		}
		wskNaToken = strtok(NULL, " ");

	}
}
//***********************************************************************************************
void wypiszIleMaKtoKart(int ileKartWTalii, karta talia[], int liczbaGraczy, int liczbaKociolkow)
{
	for (int i = 0; i < liczbaGraczy; i++)
	{
		cout << i + 1 << " player has " << policzKarty(ileKartWTalii, talia, i + 1, 0) << " cards on hand\n";
		cout << i + 1 << " player has " << policzKarty(ileKartWTalii, talia, i + 1, 1) << " cards in front of him\n";
	}

	for (int i = 0; i < liczbaKociolkow; i++)
	{
		cout << "there are " << policzKarty(ileKartWTalii, talia, i + 1, 2) << " cards on " << i + 1 << " pile\n";
	}
}
//**********************************************************************************************
int policzKarty(int ileKartWTalii, karta talia[], int ktoryGracz, int handOrDeckOrPile)
{
	int wynik = 0;
	for (int i = 0; i < ileKartWTalii; i++)
	{
		if (talia[i].uKtoregoGracza == ktoryGracz && talia[i].handDeckOrPile == handOrDeckOrPile) wynik++;
	}
	return wynik;
}
//***********************************************************************************************
int policzKoloryWTalii(karta talia[], int ileKartWTalii) //nie licze zielonego
{
	char kolory[maxLiczbaKolorow][maxLiczbaLiterKoloru];
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
//***********************************************************************************************
int bylTakiKolor(char kolor[], char kolory[][maxLiczbaLiterKoloru], int ileKolorow)
{
	//by nie liczylo zielonego
	if (strcmp(kolor, "green") == 0) return 2;

	for (int i = 0; i < ileKolorow; i++)
	{
		if (strcmp(kolor, kolory[i]) == 0) return 1;
	}

	return 0;
}
//************************************************************************************************
int znajdzIndeksLiczby(char napis[], int poczatek)
{
	while (czyLiczba(napis, poczatek) == 0)
	{
		poczatek++;
	}
	return poczatek;
}
//*************************************************************************************************
int czyLiczba(char zdanie[], int iteracja)
{
	if (zdanie[iteracja] >= '1' && zdanie[iteracja] <= '9') return 1; //jesli liczba zwraca 1
	if (zdanie[iteracja] == '\n') return -1;
	else return 0; //jesli nei liczba zwraca 0
}
//*************************************************************************************************
int znajdzLiczbe(char zdanie[], int poczatek)
{
	while (zdanie[poczatek] != NULL)
	{
		if (zdanie[poczatek] >= '1' && zdanie[poczatek] <= '9') return int(zdanie[poczatek] - '0');
		poczatek++;
	}
	return -1;
}

