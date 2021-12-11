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
	int wartosc = -100;
	char kolor[maxLiczbaLiterKoloru] = "none";
	int handDeckOrPile = -100; //if hand - 0, deck - 1, pile - 2;
	int uKtoregoGracza = -100; //0 - na stosie, 1 - u pierwszego, 2 - u drugiego itd
	int ktoryStos = 0; //0 - nie na stosie, 1- pierwszy stos itd
};
//deklaracje funkcji******************************************************
void utworzTalie(int liczbaKociolkow, int liczbaZielonych, int wartoscZielonych, int liczbaKartInnychKolorow, int wartosciKart[], karta talia[]);
void rozdanieKart(int liczbaGraczy, int liczbaKociolkow, int liczbaZielonych, int liczbaKartInnychKolorow, karta talia[]);
void wypiszKartyGracza(int liczbaKartWGrze, int liczbaGraczy, karta rekaGracza[][maxLiczbaGraczy], int ktoryGracz);
void wypiszIleMaKtoKart(int ileKartWTalii, karta talia[], int liczbaGraczy, int liczbaKociolkow);
int policzKoloryWTalii(karta talia[], int ileKartWTalii);
int bylTakiKolor(char kolor[], char kolory[][maxLiczbaLiterKoloru], int ileKolorow);
void wczytajWartosciKart(int* liczbaGraczy, int* liczbaKociolkow, int* liczbaZielonych, int* wartoscZielonych, int* liczbaKartInnychKolorow, int wartosciKart[]);
void wczytajKartyDoTalii(char liniaTekstu[], karta talia[], int* ktoraKartaWTalii, int handOrDeckOrPile);
void wczytajStanGryDoTalii(int* liczbaGraczy, int* liczbaKociolkow, karta talia[], int* ktoraKartaWTalii);
int policzKarty(int ileKartWTalii, karta talia[], int ktoryGracz, int handOrDeckOrPile);
int czyLiczba(char zdanie[], int iteracja);
int znajdzIndeksLiczby(char napis[], int poczatek);
int znajdzLiczbe(char zdanie[], int poczatek);
void sprawdzZielone(karta talia[], int liczbaKartWTalii);
void sprawdzLiczbeKart(karta talia[], int liczbaKartWTalii, int liczbaKolorow);
//************************************************************************
int main()
{
	int liczbaGraczy;
	int liczbaKociolkow = 0;
	//int liczbaZielonych;
	//int wartoscZielonych;
	//int liczbaKartInnychKolorow;
	//int wartosciKart[maxLiczbaKart];
	int ileKartWTalii = 0;
	karta talia[maxLiczbaWszystkichKart];

	//wczytajWartosciKart(&liczbaGraczy, &liczbaKociolkow, &liczbaZielonych, &wartoscZielonych, &liczbaKartInnychKolorow, wartosciKart);
	//utworzTalie(liczbaKociolkow, liczbaZielonych, wartoscZielonych, liczbaKartInnychKolorow, wartosciKart, talia);
	//rozdanieKart(liczbaGraczy, liczbaKociolkow, liczbaZielonych, liczbaKartInnychKolorow, talia);

	wczytajStanGryDoTalii(&liczbaGraczy, &liczbaKociolkow, talia, &ileKartWTalii);

	//wypiszIleMaKtoKart(ileKartWTalii, talia, liczbaGraczy, liczbaKociolkow);

	//sprawdzZielone(talia, ileKartWTalii);
	sprawdzLiczbeKart(talia, ileKartWTalii, liczbaKociolkow);

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
//**************************************************************************************************
void wczytajKartyDoTalii(char liniaTekstu[], karta talia[], int* ktoraKartaWTalii, int handOrDeckOrPile)//if hand - 0, deck - 1, pile - 2
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
				*ktoraKartaWTalii += 1;
			}
		}
		wskNaToken = strtok(NULL, " ");
	}
}
//************************************************************************************************
void wczytajStanGryDoTalii(int* liczbaGraczy, int* liczbaKociolkow, karta talia[], int* ktoraKartaWTalii)
{
	char wczytaneZdanie[maxLiczbaLiterZdania];

	fgets(wczytaneZdanie, maxLiczbaLiterZdania, stdin); //wczytuje pierwsze dwie linie
	fgets(wczytaneZdanie, maxLiczbaLiterZdania, stdin);//z drugiej bierze liczbe graczy

	*liczbaGraczy = znajdzLiczbe(wczytaneZdanie, 0); // wyluskuje liczbe graczy z tekstu

	for (int i = 0; i < 2 * (*liczbaGraczy); i++)//wczytuje po lini dane, liczy ilosc kart i kolorow
	{
		fgets(wczytaneZdanie, maxLiczbaLiterZdania, stdin);
		wczytajKartyDoTalii(wczytaneZdanie, talia, ktoraKartaWTalii, i % 2);
	}

	*liczbaKociolkow = policzKoloryWTalii(talia, *ktoraKartaWTalii);

	//petla ktora wczyta pile
	for (int i = 0; i < *liczbaKociolkow; i++)
	{
		fgets(wczytaneZdanie, maxLiczbaLiterZdania, stdin);
		wczytajKartyDoTalii(wczytaneZdanie, talia, ktoraKartaWTalii, 2);
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
//*************************************************************************************************
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
	else cout << "Found " << liczbaZielonych << " green cards, all with " << previousValue << " value\n";
}
//**********************************************************************************************************
void sprawdzLiczbeKart(karta talia[], int liczbaKartWTalii, int liczbaKolorow)
{
	int liczbaKartKoloru = 0;
	int tablicaIlosciKartKoloru[maxLiczbaKolorow];
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
	if (saRozne == 0)
	{
		cout << "The number cards of all colors is equal: " << liczbaKartKoloru << endl;
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
