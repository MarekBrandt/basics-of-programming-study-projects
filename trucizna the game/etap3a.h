#include <iostream>
#include <string.h>
#include <cstdio>
using namespace std;

//stale i struktury******************************************************
const int maxLiczbaKart = 26;
const int maxLiczbaKolorow = 7;
const int maxLiczbaGraczy = 6;
const int maxLiczbaLiterKoloru = 7;
const int maxLiczbaZielonych = 9;
const int maxLiczbaLiterZdania = 300;
const int maxLiczbaWszystkichKart = maxLiczbaKolorow * maxLiczbaKart + maxLiczbaZielonych;

const char tablicaKolorow[maxLiczbaLiterKoloru][maxLiczbaKolorow] =
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
};
//deklaracje funkcji******************************************************
void utworzTalie(int liczbaKociolkow, int liczbaZielonych, int wartoscZielonych, int liczbaKartInnychKolorow, int wartosciKart[], karta talia[]);
void rozdanieKart(int liczbaGraczy, int liczbaKociolkow, int liczbaZielonych, int liczbaKartInnychKolorow, karta talia[]);
void wypiszKartyGracza(int liczbaKartWGrze, int liczbaGraczy, karta rekaGracza[][maxLiczbaGraczy], int ktoryGracz);
void wczytajWartosciKart(int* liczbaGraczy, int* liczbaKociolkow, int* liczbaZielonych, int* wartoscZielonych, int* liczbaKartInnychKolorow, int wartosciKart[]);
void wczytajStanGry();
int znajdzLiczbe(char zdanie[], int poczatek);
int znajdzWszystkieLiczby(char zdanie[], int poczatek);
//************************************************************************
int main()
{
	/*int liczbaGraczy;
	int liczbaKociolkow;
	int liczbaZielonych;
	int wartoscZielonych;
	int liczbaKartInnychKolorow;
	int wartosciKart[maxLiczbaKart];
	karta talia[maxLiczbaWszystkichKart];*/
	//wczytajWartosciKart(&liczbaGraczy, &liczbaKociolkow, &liczbaZielonych, &wartoscZielonych, &liczbaKartInnychKolorow, wartosciKart);
	//utworzTalie(liczbaKociolkow, liczbaZielonych, wartoscZielonych, liczbaKartInnychKolorow, wartosciKart, talia);
	//rozdanieKart(liczbaGraczy, liczbaKociolkow, liczbaZielonych, liczbaKartInnychKolorow, talia);
	wczytajStanGry();

	return 0;
}
//************************************************************************
void wczytajWartosciKart(int* liczbaGraczy, int* liczbaKociolkow, int* liczbaZielonych, int* wartoscZielonych, int* liczbaKartInnychKolorow, int wartosciKart[])
{
	cin >> *liczbaGraczy >> *liczbaKociolkow >> *liczbaZielonych >> *wartoscZielonych
		>> *liczbaKartInnychKolorow;
	for (int i = 0; i < *liczbaKartInnychKolorow; i++)
	{
		int wartosc;
		cin >> wartosc;
		wartosciKart[i] = wartosc;
	}
}
//**************************************************************************
void utworzTalie(int liczbaKociolkow, int liczbaZielonych, int wartoscZielonych, int liczbaKartInnychKolorow, int wartosciKart[], karta talia[])
{
	for (int i = 0; i < liczbaZielonych; i++)
	{

		talia[i].wartosc = wartoscZielonych;
		strncpy_s(talia[i].kolor, tablicaKolorow[0], maxLiczbaLiterKoloru);
		//cout << talia[i].wartosc << " " << talia[i].kolor << " ";
	}
	for (int j = 0; j < liczbaKociolkow; j++)
	{
		for (int i = liczbaZielonych + liczbaKartInnychKolorow * j; i < liczbaKartInnychKolorow + liczbaZielonych + liczbaKartInnychKolorow * j; i++)
		{
			talia[i].wartosc = wartosciKart[(i - liczbaZielonych) % liczbaKartInnychKolorow];
			strncpy_s(talia[i].kolor, tablicaKolorow[j + 1], maxLiczbaLiterKoloru);
			//cout << talia[i].wartosc << " " << talia[i].kolor << " ";
		}
	}
}
//*****************************************************************************
void rozdanieKart(int liczbaGraczy, int liczbaKociolkow, int liczbaZielonych, int liczbaKartInnychKolorow, karta talia[])
{
	int liczbaKartWGrze = liczbaKociolkow * liczbaKartInnychKolorow + liczbaZielonych;
	int ktoraKartaZTali = 0;

	karta rekaGracza[maxLiczbaKart][maxLiczbaGraczy];
	for (int i = 0; i < liczbaKartWGrze / liczbaGraczy + 1; i++)
	{
		for (int j = 0; j < liczbaGraczy; j++)
		{
			rekaGracza[i][j].wartosc = talia[ktoraKartaZTali].wartosc;
			strncpy_s(rekaGracza[i][j].kolor, talia[ktoraKartaZTali].kolor, maxLiczbaLiterKoloru);
			if (ktoraKartaZTali >= liczbaKartWGrze - 1)
			{
				i = liczbaKartWGrze; //konczy obie petle
				j = liczbaGraczy;
			}
			ktoraKartaZTali++;
		}
	}

	cout << "active player = 1" << endl;
	cout << "players number = " << liczbaGraczy << endl;

	for (int i = 0; i < liczbaGraczy; i++)
	{
		cout << i + 1 << " player hand cards: "; wypiszKartyGracza(liczbaKartWGrze, liczbaGraczy, rekaGracza, i);
		cout << endl;
		cout << i + 1 << " player deck cards:" << endl;
	}
	for (int i = 0; i < liczbaKociolkow; i++)
	{
		cout << i + 1 << " pile cards:" << endl;
	}
}
//*********************************************************************************
void wypiszKartyGracza(int liczbaKartWGrze, int liczbaGraczy, karta rekaGracza[][maxLiczbaGraczy], int ktoryGracz)
{
	int ktoraKartaZTali = 0;
	for (int i = 0; i < liczbaKartWGrze / liczbaGraczy + 1; i++)
	{
		for (int j = 0; j < liczbaGraczy; j++)
		{
			if (j == ktoryGracz) {
				cout << rekaGracza[i][j].wartosc << " " << rekaGracza[i][j].kolor << " ";
			}
			if (ktoraKartaZTali == liczbaKartWGrze - 1)
			{
				i = liczbaKartWGrze;
				j = liczbaGraczy;
			}
			ktoraKartaZTali++;
		}
	}
}
//***********************************************************************************************
void wczytajStanGry()
{
	char activePlayer[maxLiczbaLiterZdania], playersNumber[maxLiczbaLiterZdania];
	fgets(activePlayer, maxLiczbaLiterZdania, stdin);
	fgets(playersNumber, maxLiczbaLiterZdania, stdin);
	int tablicaIlosciKart[2 * maxLiczbaGraczy] = { 0 };
	int liczbaGraczy = znajdzLiczbe(playersNumber, 0);

	for (int i = 0; i < 2 * liczbaGraczy; i++)
	{

		fgets(activePlayer, maxLiczbaLiterZdania, stdin);
		tablicaIlosciKart[i] = znajdzWszystkieLiczby(activePlayer, 2);
	}
	int j = 1;
	for (int i = 0; i < 2 * liczbaGraczy; i += 2)
	{

		cout << j << " player has " << tablicaIlosciKart[i] << " cards on hand" << endl;
		cout << j << " player has " << tablicaIlosciKart[i + 1] << " cards in front of him" << endl;
		j++;
	}
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
//*****************************************************************************************************
int znajdzWszystkieLiczby(char zdanie[], int poczatek)
{
	int wynik = 0;
	while (zdanie[poczatek] != NULL)
	{
		if (zdanie[poczatek] >= '1' && zdanie[poczatek] <= '9')
		{
			wynik++;
			poczatek++;
		}
		poczatek++;
	}
	return wynik;
}