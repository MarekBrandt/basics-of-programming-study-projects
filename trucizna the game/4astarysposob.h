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
	int handDeckOrPile = 0; //if hand - 0, deck - 1, pile - 2;
	int uKtoregoGracza; //0 - na stosie, 1 - u pierwszego, 2 - u drugiego itd
	int ktoryStos = 0; //0 - nie na stosie, 1- pierwszy stos itd
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

	//wczytajWartosciKart(&liczbaGraczy, &liczbaKociolkow, &liczbaZielonych, &wartoscZielonych, &liczbaKartInnychKolorow, wartosciKart);
	//utworzTalie(liczbaKociolkow, liczbaZielonych, wartoscZielonych, liczbaKartInnychKolorow, wartosciKart, talia);
	//rozdanieKart(liczbaGraczy, liczbaKociolkow, liczbaZielonych, liczbaKartInnychKolorow, talia);

	//wczytajStanGry(&liczbaGraczy, &liczbaKociolkow, tablicaIlosciKart, tablicaIlosciKart, &czyBylZielonyWTalii);
	wczytajStanGryDoTalii(&liczbaGraczy, &liczbaKociolkow, tablicaIlosciKart, tablicaIlosciKart, &czyBylZielonyWTalii);
	//wypiszRozlozenieKart(liczbaGraczy, liczbaKociolkow, tablicaIlosciKart, tablicaIlosciKart);
	sprawdzZieloneKarty();

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
void wczytajStanGry(int* liczbaGraczy, int* ileKolorow, int tablicaIlosciKart[], int tablicaIlosciKartWKociolku[], int* czyBylZielonyWTalii)
{
	char wczytaneZdanie[maxLiczbaLiterZdania], playersNumberInfo[maxLiczbaLiterZdania];

	fgets(wczytaneZdanie, maxLiczbaLiterZdania, stdin); //wczytuje pierwsze dwie linie
	fgets(playersNumberInfo, maxLiczbaLiterZdania, stdin);//z drugiej bierze liczbe graczy

	*liczbaGraczy = znajdzLiczbe(playersNumberInfo, 0); // wyluskuje liczbe graczy z tekstu
	char kolory[maxLiczbaKolorow][maxLiczbaLiterKoloru];


	for (int i = 0; i < 2 * (*liczbaGraczy); i++)//wczytuje po lini dane, liczy ilosc kart i kolorow
	{

		fgets(wczytaneZdanie, maxLiczbaLiterZdania, stdin);
		tablicaIlosciKart[i] = znajdzWszystkieLiczby(wczytaneZdanie, 2);
		*ileKolorow = policzKolory(wczytaneZdanie, kolory, *ileKolorow, 20, czyBylZielonyWTalii); //nie licze zielonego	
	}


	for (int i = 0; i < *ileKolorow; i++)
	{
		fgets(wczytaneZdanie, maxLiczbaLiterZdania, stdin);
		tablicaIlosciKartWKociolku[i] = znajdzWszystkieLiczby(wczytaneZdanie, 2);
	}

}
//***********************************************************************************************
void wczytajStanGryDoTalii(int* liczbaGraczy, int* ileKolorow, int tablicaIlosciKart[], int tablicaIlosciKartWKociolku[], int* czyBylZielonyWTalii)
{
	char wczytaneZdanie[maxLiczbaLiterZdania], playersNumberInfo[maxLiczbaLiterZdania];

	fgets(wczytaneZdanie, maxLiczbaLiterZdania, stdin); //wczytuje pierwsze dwie linie
	fgets(playersNumberInfo, maxLiczbaLiterZdania, stdin);//z drugiej bierze liczbe graczy

	*liczbaGraczy = znajdzLiczbe(playersNumberInfo, 0); // wyluskuje liczbe graczy z tekstu
	char kolory[maxLiczbaKolorow][maxLiczbaLiterKoloru];


	for (int i = 0; i < 2 * (*liczbaGraczy); i++)//wczytuje po lini dane, liczy ilosc kart i kolorow
	{
		fgets(wczytaneZdanie, maxLiczbaLiterZdania, stdin);
		tablicaIlosciKart[i] = znajdzWszystkieLiczby(wczytaneZdanie, 2, i);
		*ileKolorow = policzKolory(wczytaneZdanie, kolory, *ileKolorow, 20, czyBylZielonyWTalii); //nie licze zielonego	
	}


	for (int i = 0; i < *ileKolorow; i++)
	{
		fgets(wczytaneZdanie, maxLiczbaLiterZdania, stdin);
		tablicaIlosciKartWKociolku[i] = znajdzWszystkieLiczby(wczytaneZdanie, 2);
	}

}
//************************************************************************************************
void wypiszRozlozenieKart(int liczbaGraczy, int ileKolorow, int tablicaIlosciKart[], int tablicaIlosciKartWKociolku[])
{
	int j = 1;
	for (int i = 0; i < 2 * liczbaGraczy; i += 2)
	{

		cout << j << " player has " << tablicaIlosciKart[i] << " cards on hand" << endl;
		cout << j << " player has " << tablicaIlosciKart[i + 1] << " cards in front of him" << endl;
		j++;
	}

	for (int i = 0; i < ileKolorow; i++)
	{
		cout << "there are " << tablicaIlosciKartWKociolku[i] << " cards on " << i + 1 << " pile" << endl;
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
int znajdzWszystkieLiczby(char zdanie[], int poczatek, int ktoryGracz)
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
//*****************************************************************************************************
//zwraca ilosc nowych kolorow ktore znalazla i dopisuje je do tablicy kolor
int policzKolory(char zdanie[], char kolor[][maxLiczbaLiterKoloru], int ileKolorow, int poczatek, int* czyBylZielonyWTalii)//liczy kolory, ale odrzuca zielony
{																															//zmienia czyBylZielony na 1 jesli byl
	char slowo[maxLiczbaLiterKoloru];
	while (zdanie[poczatek] != NULL)
	{
		int wykonaloSie = 0;
		int i = 0;
		while (zdanie[poczatek] >= 'a' && zdanie[poczatek] <= 'z')
		{
			slowo[i] = zdanie[poczatek];
			poczatek++;
			i++;
			wykonaloSie = 1;
		}
		slowo[i] = NULL;


		if (wykonaloSie == 1)
		{
			if (sprawdzCzyBylo(slowo, ileKolorow, kolor) == -1)
			{
				strncpy_s(kolor[ileKolorow], slowo, maxLiczbaLiterKoloru);
				ileKolorow++;
			}
			else if (sprawdzCzyBylo(slowo, ileKolorow, kolor) == 0)
			{
				*czyBylZielonyWTalii = 1;
			}
		}
		else
		{
			poczatek++;
		}
	}
	return ileKolorow;
}
//******************************************************************************************************
int sprawdzCzyBylo(char slowo[], int ileKolorow, char kolor[][maxLiczbaLiterKoloru])//jesli zielony to mowi ze juz byl
{
	if (strcmp(slowo, tablicaKolorow[0]) != 0)
	{
		for (int i = 0; i < ileKolorow; i++) //jesli koloru nie bylo zwraca -1, jesli byl to jego indeks
		{
			if (strcmp(slowo, kolor[i]) == 0) //jesli sa rowne to sie wykona 
			{
				return i;
			}
		}
	}
	else
	{
		return 0;
	}
	return -1;
}
//***********************************************************************************************************
void sprawdzZieloneKarty()
{

}
