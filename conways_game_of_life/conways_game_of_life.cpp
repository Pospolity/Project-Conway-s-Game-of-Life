// conways_game_of_life.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "windows.h"
#include <time.h>
#include <cstdlib>

using namespace std;

/* DEKLARACJA STA�YCH WYMIAR�W PLANSZY */
const int y = 30; //30 fajnie 
const int x = 80; //80 fajnie

struct cellStructure //pojedyncza kom�rka
{
	bool isAlive; //czy kom�rka jest �ywa w tej generacji
	int neighbours; //liczba �ywych s�siad�w kom�rki
};

void cellsSetDead(cellStructure cell[y][x]); //TWORZENIE MARTWEJ PLANSZY (SZABLONU)
void randomization(cellStructure cell[y][x]); //LOSOWANIE �YWYCH KOM�REK NA PLANSZY
void cellsSetLife(cellStructure cell[y][x]); //ZAPE�NIANIE PLANSZY �YWYMI KOM�RKAMI
void nextGeneration(cellStructure cell[y][x]); //PRZEJ�CIE DO KOLEJNEJ GENERAJI
char setMode(void); //USTAWIANIE TRYBU �YCIA (KROKOWY LUB NIESKO�CZONY)
void showCurrentGeneration(cellStructure cell[y][x]); //WY�WIETLANIE OBECNEJ GENERACJI


 /* DEKLARACJA STA�YCH, STANOWI�CYCH INDEKSY WIERZCHO�K�W CZʌCI PLANSZY NA KT�REJ B�DZIE TOCZY�O SI� �YCIE (WSZYSTKIE KOM�RKI POZA KOM�RKAMI NA KRAW�DZIACH PLANSZY) */
const int first_y = 1;
const int last_y = y - 2;
const int first_x = 1;
const int last_x = x - 2;


const int DEFAULT_SLEEP_TIME = 500; // DEKLARACJA STA�EJ ODPOWIEDZIALNEJ ZA PR�DKO�� WYSWIETLANIA GENERACJI

int main()
{
	srand(time(NULL));
	COORD cursorReset;
	cursorReset.X = 0;
	cursorReset.Y = 0;
	cellStructure cell[y][x];
	
	char start;
	do {
		cellsSetDead(cell); //TWORZENIE MARTWEJ PLANSZY (SZABLONU)
		cellsSetLife(cell); //ZAPE�NIANIE PLANSZY �YWYMI KOM�RKAMI
		char mode = setMode(); //USTAWIANIE TRYBU �YCIA (KROKOWY LUB NIESKO�CZONY)
		/* �YCIE!!! */
		int speedRatio = 1;
		if (mode == 'c') {
			do {
				cout << "Podaj predkosc od 0 (wolno) do 500 (bardzo szybko) z jaka beda sie zmieniac generacje: ";
				cin >> speedRatio;
				system("cls");
				if (speedRatio < 0 || speedRatio > 500)
					cout << "Prosze sprobowac ponownie, tym razem czytajac ze zrozumieniem." << endl;
			} while (speedRatio < 0 || speedRatio > 500);
		}
		while (mode == 'c') { //tryb ci�g�y wi�c niesko�czona p�tla
			showCurrentGeneration(cell);
			nextGeneration(cell);
			Sleep(DEFAULT_SLEEP_TIME - speedRatio);
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorReset); //ustawianie kursora na pocz�tek tablicy, aby j� nadpisa�
		}
		if (mode == 'k') { //tryb krokowy
			char ch;
			do {
				showCurrentGeneration(cell);
				cout << "\nWci�nij dowolny znak, aby przejsc do nastepnej generacji, lub 'e' aby zakonczyc aktualne zycie (komorek!): ";
				cin >> ch;
				nextGeneration(cell);
				system("cls");
			} while (ch != 'e');
		}
		cout << "Wci�nij dowolny znak, aby sprobowac ponownie, lub 'e' aby wyjsc z programu: ";
		cin >> start;
	} while (start != 'e');
	
	return 0;
}

void cellsSetDead(cellStructure cell[y][x]) //TWORZENIE MARTWEJ PLANSZY (SZABLONU)
{ 
	for (int iy = 0; iy < y; iy++) {
		for (int jx = 0; jx < x; jx++) {
			cell[iy][jx].isAlive = false;
			cell[iy][jx].neighbours = 0;
		}
	}
};

void randomization(cellStructure cell[y][x]) 
{
	for (int iy = first_y; iy <= last_y; ++iy) {
		for (int jx = first_x; jx <= last_x; ++jx) {
			if (rand() % 10 > 6)
				cell[iy][jx].isAlive = true;
		}
	}
};

void cellsSetLife(cellStructure cell[y][x]) //ZAPE�NIANIE PLANSZY �YWYMI KOM�RKAMI
{
	char startMap;
	do {
		cout << "Wcisnij 's' jesli chcesz gotowa mapke, 'l' aby plansza byla zapelniana losowo: ";
		cin >> startMap;
		system("cls");
		if (startMap == 'l')
			randomization(cell); //funkcja losuj�ca plansz�
		else if (startMap == 's')
			cout << "NIEDOSTEPNE W WERSJI BETA" << endl;
		else
			cout << "nie ma takiej opcji, sprobuj ponownie." << endl;
	} while (/*startMap != 's' &&*/ startMap != 'l'); //odkomentowac pierwszy warunek po dodaniu obs�ugi szablon�w
};

void nextGeneration(cellStructure cell[y][x]) //PRZEJ�CIE DO KOLEJNEJ GENERAJI
{
	//		POSZUKIWANIE �YJ�CYCH KOM�REK I...
	for (int iy = first_y; iy <= last_y; ++iy) {
		for (int jx = first_x; jx <= last_x; ++jx) {
			if (cell[iy][jx].isAlive == true) { // ...dla znalezionej �ywej kom�rki inkrementowanie warto�ci neigbours jej s�siad�w o 1
				cell[iy + 1][jx - 1].neighbours++;
				cell[iy + 1][jx].neighbours++;
				cell[iy + 1][jx + 1].neighbours++;
				cell[iy][jx - 1].neighbours++;
				cell[iy][jx + 1].neighbours++;
				cell[iy - 1][jx - 1].neighbours++;
				cell[iy - 1][jx].neighbours++;
				cell[iy - 1][jx + 1].neighbours++;
			} // martwe kom�rki pomijamy
		}
	}
	//		BADANIE NA PODSTAWIE ILO�CI S�SIAD�W CZY KOM�RKA ZMIENI STAN ORAZ ZMIANA TYCH STAN�W
	for (int iy = first_y; iy <= last_y; ++iy) {
		for (int jx = first_x; jx <= last_x; ++jx) {
			if (cell[iy][jx].isAlive == false) { // dla martwej kom�rki sprawdzam czy ma dokladnie 3 sasiadow (wtedy komorka ozywa czyli zmienia sie jej stan) 
				if (cell[iy][jx].neighbours == 3)
					cell[iy][jx].isAlive = true;
			}
			else { // dla �ywych kom�rek maj�cych 2 lub 3 s�siad�w stan sie nie zmienia (stateChange wci�� ma warto�� false), przy innej liczbie s�siad�w kom�rka umiera (stateChange = true)
				if (cell[iy][jx].neighbours != 2 && cell[iy][jx].neighbours != 3)
					cell[iy][jx].isAlive = false;
			}
			cell[iy][jx].neighbours = 0; //zmiana warto�ci neigbours na pocz�tkowe ju� sprawdzonych kom�rek, �eby przygotowa� je do kolejnej generacji
		}
	}
};

char setMode(void) //USTAWIANIE TRYBU �YCIA (KROKOWY LUB NIESKO�CZONY)
{
	char mode;
	do{
		cout << "Wcisnij 'k' aby rozpoczac zycie w trybie krokowym lub 'c' aby uruchomic w trybie ciaglym (nieskonczonym): ";
		cin >> mode;
		system("cls");
		if (mode != 'k' && mode != 'c')
			cout << "nie ma takiej opcji, sprobuj ponownie." << endl;
	} while (mode != 'k' && mode != 'c');
	return mode;
};

void showCurrentGeneration(cellStructure cell[y][x]) //WY�WIETLANIE OBECNEJ GENERACJI
{
	//		przesuwanie si� po �ywej planszy aby wy�wietli� obecn� generacj�
	for (int iy = first_y; iy <= last_y; ++iy) {
		for (int jx = first_x; jx <= last_x; ++jx) {
			if (cell[iy][jx].isAlive == false)
				cout << char(' '); //0xB0
			else
				cout << char(0xB2);
		}
		cout << endl;
	}
};