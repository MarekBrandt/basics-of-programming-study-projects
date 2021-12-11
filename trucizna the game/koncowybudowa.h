#include <iostream>
#include <string.h>
#include <cstdio>
using namespace std;

//stale i struktury******************************************************
const int maxLiczbaKart = 20;
const int maxLiczbaKolorow = 7;
const int maxLiczbaGraczy = 6;
const int maxLiczbaLiterKoloru = 7;
const int maxLiczbaZielonych = 9;
const int maxLiczbaLiterZdania = 300;
const int maxLiczbaWszystkichKart = maxLiczbaKolorow * maxLiczbaKart + maxLiczbaZielonych;
const int defaultValue = -100;

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
	int wartosc = defaultValue;
	char kolor[maxLiczbaLiterKoloru] = "none";
	int handDeckOrPile = -100; //if hand - 0, deck - 1, pile - 2;
	int uKtoregoGracza = -100; //0 - na stosie, 1 - u pierwszego, 2 - u drugiego itd
	int ktoryStos = 0; //0 - nie na stosie, 1- pierwszy stos itd
};

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

//deklaracje funkcji***********************************************************************************************************
void stworzenieTalii(int* liczbaGraczy, int* liczbaKociolkow, karta talia[], int* liczbaKart);
void utworzTalie(int liczbaKociolkow, int liczbaZielonych, int wartoscZielonych, int liczbaKartInnychKolorow, int wartosciKart[], karta talia[], int* liczbaKart);
void zapiszTalieWPliku(karta talia[], int liczbaKart);
void rozdanieKart(int liczbaGraczy, karta talia[]);
void wypiszIleMaKtoKart(int ileKartWTalii, karta talia[], int liczbaGraczy, int liczbaKociolkow);
int policzKoloryWTalii(karta talia[], int ileKartWTalii);
int bylTakiKolor(char kolor[], char kolory[][maxLiczbaLiterKoloru], int ileKolorow);
void wczytajWartosciKart(int* liczbaGraczy, int* liczbaKociolkow, int* liczbaZielonych, int* wartoscZielonych, int* liczbaKartInnychKolorow, int wartosciKart[]);
void wczytajKartyDoTalii(char liniaTekstu[], karta talia[], int* ktoraKartaWTalii, int handOrDeckOrPile, int ktoreWywolanie);
int policzKarty(int ileKartWTalii, karta talia[], int ktoryGracz, int handOrDeckOrPile);
int znajdzLiczbe(char zdanie[], int poczatek);
void sprawdzZielone(karta talia[], int liczbaKartWTalii);
void sprawdzLiczbeKart(karta talia[], int liczbaKartWTalii, int liczbaKolorow);
int czyLiczba(char zdanie);
void sprawdzWartosciKart(karta talia[], int liczbaKartWTalii, int liczbaKolorow);
int najwiekszyElementTablicy(int t[], int rozmiar);
void zamienDwaElementy(int t[], int indeks1, int indeks2);
void porzadkujTablice(int t[], int n);
void sprawdzPoprawnoscStanuGry(karta talia[], int liczbaKartWTalii, int liczbaKolorow, int liczbaGraczy, int maxWartoscKotla, int ktoryGracz);
int sprawdzPoprawnoscIlosciKart(karta talia[], int liczbaKartWTalii, int liczbaGraczy);
void wczytajStanGryDoTalii(int* liczbaGraczy, int* liczbaKociolkow, karta talia[], int* ktoraKartaWTalii, int* maxKartWKotle, int* aktywnyGracz);
void sprawdzPoprwnoscKociolki(karta talia[], int liczbaKartWTalii, int liczbaKociolkow, int maxWartoscKotla, int* ogolnyStan, int ktoryGracz, int liczbaGraczy);
void poprawnoscKoloruWKociolku(karta talia[], int liczbaKartWTalii, int liczbaKociolkow, int* ogolnyStan);
void wartoscKociolka(karta talia[], int liczbaKartWTalii, int liczbaKociolkow, int maxWartoscKotla, int* ogolnyStan, int ktoryGracz, int liczbaGraczy);
void prostePosuniecie(karta talia[], int liczbaKartWTalii, int* aktywnyGracz, int liczbaStosow, int liczbaGraczy);
int czyJestStosOKolorze(karta talia[], int liczbaKartWTalii, char kolor[maxLiczbaLiterKoloru]);
int znajdzWolnyStos(karta talia[], int liczbaKartWTalii, char kolor[maxLiczbaLiterKoloru], int liczbaStosow);
void wypiszStol(karta talia[], int ileKartWTalii, int aktywnyGracz, int liczbaGraczy, int liczbaKociolkow, int progEksplozji);
void wypiszStolDoPliku(karta talia[], int ileKartWTalii, int aktywnyGracz, int liczbaGraczy, int liczbaKociolkow, int progEksplozji);
void wypiszKarty(karta talia[], int ileKartWTalii, int handOrDeckOrPile, int ktoryGracz, int ktoryStos);
void wypiszKartydoPliku(karta talia[], int ileKartWTalii, int handOrDeckOrPile, int ktoryGracz, int ktoryStos, FILE* plik);
void ustawKarteNaKoniecTalii(karta talia[], int LiczbaKartTalii, int indeksKartyDoPrzestawienia);
void przeniesNaDeck(karta talia[], int ktoryStos, int ktoryGracz, int liczbaKartWTalii, int liczbaGraczy);
void obslugaKoncaRundy(karta talia[], int liczbaKart, int liczbaKociolkow, int liczbaGraczy);
void sprawdzOdpornosci(karta talia[], int liczbaKart, int tabOdpornosci[], int liczbaKociolkow, int liczbaGraczy);
int policzPunkty(karta talia[], int liczbaKart, int ktoryGracz, int naIleOdporny, int odpornyNa[]);
void wypiszWyniki(karta talia[], int liczbaKart, int tabOdpornosci[], int liczbaKociolkow, int liczbaGraczy);
int czyKoniec(karta talia[], int liczbaKart);
//*****************************************************************************************************************************
int main(int argc, int* argv[])
{
	int liczbaGraczy = 2;
	int aktywnyGracz = 1;
	int liczbaKociolkow = 0;
	int progEksplozji = 0;
	int ileKartWTalii = 0;
	karta talia[maxLiczbaWszystkichKart];

	stworzenieTalii(&liczbaGraczy, &liczbaKociolkow, talia, &ileKartWTalii);
	rozdanieKart(liczbaGraczy, talia);
	wypiszStolDoPliku(talia, ileKartWTalii, aktywnyGracz, liczbaGraczy, liczbaKociolkow, progEksplozji);

	//wczytajStanGryDoTalii(&liczbaGraczy, &liczbaKociolkow, talia, &ileKartWTalii, &progEksplozji, &aktywnyGracz);

	//wypiszIleMaKtoKart(ileKartWTalii, talia, liczbaGraczy, liczbaKociolkow);

	//sprawdzZielone(talia, ileKartWTalii);
	//sprawdzLiczbeKart(talia, ileKartWTalii, liczbaKociolkow);

	//sprawdzWartosciKart(talia, ileKartWTalii, liczbaKociolkow);



	//prostePosuniecie(talia, ileKartWTalii, &aktywnyGracz, liczbaKociolkow, liczbaGraczy);
	//sprawdzPoprawnoscStanuGry(talia, ileKartWTalii, liczbaKociolkow, liczbaGraczy, progEksplozji, aktywnyGracz);
	//wypiszStol(talia, ileKartWTalii, aktywnyGracz, liczbaGraczy, liczbaKociolkow, progEksplozji);

	//if (czyKoniec(talia, ileKartWTalii)==1)
	{
		//obslugaKoncaRundy(talia, ileKartWTalii, liczbaKociolkow, liczbaGraczy);
	}


	return 0;
}
//*****************************************************************************************************************************
void stworzenieTalii(int* liczbaGraczy, int* liczbaKociolkow, karta talia[], int* liczbaKart)
{
	int liczbaZielonych;
	int wartoscZielonych;
	int liczbaKartInnychKolorow;
	int wartosciKart[maxLiczbaKart];

	wczytajWartosciKart(liczbaGraczy, liczbaKociolkow, &liczbaZielonych, &wartoscZielonych, &liczbaKartInnychKolorow, wartosciKart);
	utworzTalie(*liczbaKociolkow, liczbaZielonych, wartoscZielonych, liczbaKartInnychKolorow, wartosciKart, talia, liczbaKart);
	zapiszTalieWPliku(talia, *liczbaKart);
}
//*****************************************************************************************************************************
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
//*****************************************************************************************************************************
void utworzTalie(int liczbaKociolkow, int liczbaZielonych, int wartoscZielonych, int liczbaKartInnychKolorow, int wartosciKart[], karta talia[], int* liczbaKart)
{
	*liczbaKart = 0;
	for (int i = 0; i < liczbaZielonych; i++)
	{

		talia[i].wartosc = wartoscZielonych;
		strncpy_s(talia[i].kolor, tablicaKolorow[0], maxLiczbaLiterKoloru);
		(*liczbaKart)++;
	}
	for (int j = 0; j < liczbaKociolkow; j++)
	{
		for (int i = 0; i < liczbaKartInnychKolorow; i++)
		{
			talia[*liczbaKart].wartosc = wartosciKart[i];
			strncpy_s(talia[*liczbaKart].kolor, tablicaKolorow[j + 1], maxLiczbaLiterKoloru);
			(*liczbaKart)++;
		}
	}
}
//*****************************************************************************************************************************
void zapiszTalieWPliku(karta talia[], int liczbaKart)
{
	FILE* plik;
	plik = fopen("talia.txt", "w");
	if (plik == NULL)
	{
		cout << "Otwieranie pliku nie powiod³o siê!" << endl;
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
//*****************************************************************************************************************************
void rozdanieKart(int liczbaGraczy, karta talia[])
{
	int i = 0;
	while (talia[i].wartosc != defaultValue)
	{
		talia[i].uKtoregoGracza = i % liczbaGraczy + 1; //przydziela po karcie graczom po kolei od 1 do x gracza
		talia[i].handDeckOrPile = hand;
		talia[i].ktoryStos = 0;
		i++;
	}
}
//*****************************************************************************************************************************
void wczytajStanGryDoTalii(int* liczbaGraczy, int* liczbaKociolkow, karta talia[], int* ktoraKartaWTalii, int* maxKartWKotle, int* aktywnyGracz)
{
	char wczytaneZdanie[maxLiczbaLiterZdania];

	fgets(wczytaneZdanie, maxLiczbaLiterZdania, stdin); //wczytuje pierwsze dwie linie
	*aktywnyGracz = znajdzLiczbe(wczytaneZdanie, 0);

	fgets(wczytaneZdanie, maxLiczbaLiterZdania, stdin);//z drugiej bierze liczbe graczy

	*liczbaGraczy = znajdzLiczbe(wczytaneZdanie, 0); // wyluskuje liczbe graczy z tekstu

	fgets(wczytaneZdanie, maxLiczbaLiterZdania, stdin);//wczytuje zdanie z max liczba w kotle
	*maxKartWKotle = znajdzLiczbe(wczytaneZdanie, 0);

	for (int i = 0; i < 2 * (*liczbaGraczy); i++)//wczytuje po lini dane, liczy ilosc kart i kolorow
	{
		memset(wczytaneZdanie, '\0', maxLiczbaLiterZdania * sizeof(char));
		fgets(wczytaneZdanie, maxLiczbaLiterZdania, stdin);
		wczytajKartyDoTalii(wczytaneZdanie, talia, ktoraKartaWTalii, i % 2, i + 1);
	}

	*liczbaKociolkow = policzKoloryWTalii(talia, *ktoraKartaWTalii);

	//petla ktora wczyta pile
	for (int i = 0; i < *liczbaKociolkow; i++)
	{
		memset(wczytaneZdanie, '\0', maxLiczbaLiterZdania * sizeof(char));
		fgets(wczytaneZdanie, maxLiczbaLiterZdania, stdin);
		wczytajKartyDoTalii(wczytaneZdanie, talia, ktoraKartaWTalii, pile, i + 1);
	}
}
//*****************************************************************************************************************************
void wczytajKartyDoTalii(char liniaTekstu[], karta talia[], int* ktoraKartaWTalii, int handOrDeckOrPile, int ktoreWywolanie)
{
	int ktoryGracz;

	//ktory gracz
	ktoryGracz = znajdzLiczbe(liniaTekstu, 0);

	//tablica do trzymania przycietego zdania
	char drugieZdanie[maxLiczbaLiterZdania];
	//memset(drugieZdanie, '\0', sizeof(drugieZdanie));

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
//*****************************************************************************************************************************
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
//*****************************************************************************************************************************
int policzKarty(int ileKartWTalii, karta talia[], int ktoryGracz, int handOrDeckOrPile)
{
	int wynik = 0;
	if (handOrDeckOrPile != 2)
	{
		for (int i = 0; i < ileKartWTalii; i++)
		{
			if (talia[i].uKtoregoGracza == ktoryGracz && talia[i].handDeckOrPile == handOrDeckOrPile) wynik++;
		}
	}
	else
	{
		for (int i = 0; i < ileKartWTalii; i++)
		{
			if (talia[i].ktoryStos == ktoryGracz && talia[i].handDeckOrPile == handOrDeckOrPile)
			{
				wynik++;
			}
		}
	}
	return wynik;
}
//*****************************************************************************************************************************
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
//*****************************************************************************************************************************
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
//*****************************************************************************************************************************
int czyLiczba(char zdanie)
{
	if (zdanie >= '1' && zdanie <= '9') return 1; //jesli to liczba to zwraca 1
	else return 0; //jesli nie f zwraca 0
}
//*****************************************************************************************************************************
int znajdzLiczbe(char zdanie[], int poczatek)
{
	char drugieZdanie[maxLiczbaLiterZdania];
	while (zdanie[poczatek] != NULL)
	{
		if (zdanie[poczatek] >= '1' && zdanie[poczatek] <= '9')
		{
			break;
		}
		poczatek++;
	}

	strcpy(drugieZdanie, zdanie + poczatek);
	if (atoi(drugieZdanie) != 0) return(atoi(drugieZdanie));

	return -1;
}
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
	else cout << "Found " << liczbaZielonych << " green cards, all with " << previousValue << " value\n";
}
//*****************************************************************************************************************************
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
//*****************************************************************************************************************************
void sprawdzWartosciKart(karta talia[], int liczbaKartWTalii, int liczbaKolorow)
{
	int tablicaWartosci[maxLiczbaKolorow][maxLiczbaKart];
	int saRozne = 0;
	int tablicaIlosciWartosciKart[maxLiczbaKolorow];// przechowuje informacje o tym ile poszegolny kolor ma wartosci

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
	if (saRozne == 0)
	{
		cout << "The values of cards of all colors are identical:\n";
		for (int i = 0; i < tablicaIlosciWartosciKart[indeksNajwiekszej];i++)
		{
			cout << tablicaWartosci[1][i];
			cout << " ";
		}
		cout << endl;
	}
	else
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
//*****************************************************************************************************************************
int najwiekszyElementTablicy(int t[], int rozmiar)
{
	int indeks = 0, i;                      //te trzy funkcje to algorytm sortujacy
	for (i = 1; i < rozmiar; i++)
		if (t[i] > t[indeks])
			indeks = i;
	return indeks;
}
//*****************************************************************************************************************************
void zamienDwaElementy(int t[], int indeks1, int indeks2)
{
	int pomocnicza = t[indeks1];
	t[indeks1] = t[indeks2];
	t[indeks2] = pomocnicza;
}
//*****************************************************************************************************************************
void porzadkujTablice(int t[], int n)
{
	for (int i = n - 1; i > 0; i--)
		zamienDwaElementy(t, i, najwiekszyElementTablicy(t, i + 1));
}
//*****************************************************************************************************************************
void sprawdzPoprawnoscStanuGry(karta talia[], int liczbaKartWTalii, int liczbaKolorow, int liczbaGraczy, int maxWartoscKotla, int ktoryGracz)
{
	int ogolnyStan = poprawne;
	if (sprawdzPoprawnoscIlosciKart(talia, liczbaKartWTalii, liczbaGraczy) == niepoprawne)
	{
		//cout << "The number of players cards on hand is wrong\n";
		ogolnyStan = niepoprawne;
	}

	sprawdzPoprwnoscKociolki(talia, liczbaKartWTalii, liczbaKolorow, maxWartoscKotla, &ogolnyStan, ktoryGracz, liczbaGraczy);

	//if (ogolnyStan == poprawne) cout << "Current state of the game is ok" << endl;
}
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
//*****************************************************************************************************************************
void sprawdzPoprwnoscKociolki(karta talia[], int liczbaKartWTalii, int liczbaKociolkow, int maxWartoscKotla, int* ogolnyStan, int ktoryGracz, int liczbaGraczy)
{
	poprawnoscKoloruWKociolku(talia, liczbaKartWTalii, liczbaKociolkow, ogolnyStan);
	wartoscKociolka(talia, liczbaKartWTalii, liczbaKociolkow, maxWartoscKotla, ogolnyStan, ktoryGracz, liczbaGraczy);
}
//*****************************************************************************************************************************
void poprawnoscKoloruWKociolku(karta talia[], int liczbaKartWTalii, int liczbaKociolkow, int* ogolnyStan)
{
	int znalezionopierwsza = niepoprawne;
	char poprzedniKolor[maxLiczbaLiterKoloru];
	for (int i = 1; i <= liczbaKociolkow; i++)
	{
		znalezionopierwsza = niepoprawne;
		for (int j = 0; j < liczbaKartWTalii; j++)
		{
			if (talia[j].handDeckOrPile == pile && talia[j].ktoryStos == i)
			{
				if (znalezionopierwsza == poprawne)
				{
					if (strcmp(poprzedniKolor, talia[j].kolor) != 0 && strcmp("green", talia[j].kolor) != 0)
					{
						*ogolnyStan = niepoprawne;
						cout << "Two different colors were found on the " << i << " pile\n";
						break;
					}
				}
				else
				{
					strcpy(poprzedniKolor, talia[j].kolor);
					znalezionopierwsza = poprawne;
				}
			}
		}
	}
}
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
//*****************************************************************************************************************************
void prostePosuniecie(karta talia[], int liczbaKartWTalii, int* aktywnyGracz, int liczbaStosow, int liczbaGraczy)
{
	for (int i = 0; i < liczbaKartWTalii; i++)
	{
		if (talia[i].uKtoregoGracza == *aktywnyGracz && talia[i].handDeckOrPile == hand)
		{
			if (strcmp(talia[i].kolor, tablicaKolorow[0]) != 0)//jesli nie jest zielony
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
			else//zielony wrzuc do pierwszego
			{
				talia[i].handDeckOrPile = pile;
				talia[i].ktoryStos = 1;
				talia[i].uKtoregoGracza = 0;
			}
			ustawKarteNaKoniecTalii(talia, liczbaKartWTalii, i);
			if (*aktywnyGracz == liczbaGraczy)
			{
				*aktywnyGracz = 1;
			}
			else
				*aktywnyGracz += 1;

			return;
		}
	}

}
//*****************************************************************************************************************************
int czyJestStosOKolorze(karta talia[], int liczbaKartWTalii, char kolor[maxLiczbaLiterKoloru]) //jesli nie ma, zwraca zero, jesli jest to numer stosu
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
//*****************************************************************************************************************************
int znajdzWolnyStos(karta talia[], int liczbaKartWTalii, char kolor[maxLiczbaLiterKoloru], int liczbaStosow)
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
//*****************************************************************************************************************************
void wypiszStolDoPliku(karta talia[], int ileKartWTalii, int aktywnyGracz, int liczbaGraczy, int liczbaKociolkow, int progEksplozji)
{
	FILE* plik;
	plik = fopen("stol.txt", "w");
	if (plik == NULL)
	{
		printf("Nie udalo wczytac sie pliku\n");
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
//*****************************************************************************************************************************
void obslugaKoncaRundy(karta talia[], int liczbaKart, int liczbaKociolkow, int liczbaGraczy)
{
	int tabOdpornosciNaKolor[maxLiczbaKolorow]; //przechowuje numer gracza odpornego. kolory tak jak w tab na poczatku
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
//*****************************************************************************************************************************
void sprawdzOdpornosci(karta talia[], int liczbaKart, int tabOdpornosci[], int liczbaKociolkow, int liczbaGraczy)
{
	for (int i = 1; i <= liczbaKociolkow; i++) //petla po kolorach
	{
		int odpornyGracz = 0; //0 oznacza ze zaden gracz nie jest odporny
		int poprzedni = 0;
		int bylRemis = niepoprawne;
		for (int j = 1; j <= liczbaGraczy; j++) //petla po kazdym graczu
		{
			int aktWynik = 0;
			for (int z = 0; z < liczbaKart; z++) //wybieranie kart danego koloru
			{
				if (talia[z].handDeckOrPile == deck && talia[z].uKtoregoGracza == j && strcmp(talia[z].kolor, tablicaKolorow[i]) == 0) // deck, u j gracza i danego koloru
				{
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
}
//*****************************************************************************************************************************
void wypiszWyniki(karta talia[], int liczbaKart, int tabOdpornosci[], int liczbaKociolkow, int liczbaGraczy)
{
	for (int i = 1; i <= liczbaGraczy; i++)
	{
		int odpornyNa[maxLiczbaKolorow];
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

		printf("Wynik gracza %d = %d\n", i, policzPunkty(talia, liczbaKart, i, licznikOdpornosci, odpornyNa));
	}
}
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
//*****************************************************************************************************************************
int czyKoniec(karta talia[], int liczbaKart)
{
	for (int i = 0; i < liczbaKart; i++)
	{
		if (talia[i].handDeckOrPile == hand)
		{
			return 0;
		}
	}
	return 1;
}
