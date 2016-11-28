// conways_game_of_life.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "windows.h"
#include <time.h>
#include <cstdlib>

using namespace std;

/* DEKLARACJA STA£YCH WYMIARÓW PLANSZY */
const int y = 30; //30 fajnie 
const int x = 80; //80 fajnie

struct cellStructure //pojedyncza komórka
{
	bool isAlive; //czy komórka jest ¿ywa w tej generacji
	int neighbours; //liczba ¿ywych s¹siadów komórki
};

void cellsSetDead(cellStructure cell[y][x]); //TWORZENIE MARTWEJ PLANSZY (SZABLONU)
void randomization(cellStructure cell[y][x]); //LOSOWANIE ¯YWYCH KOMÓREK NA PLANSZY
void cellsSetLife(cellStructure cell[y][x]); //ZAPE£NIANIE PLANSZY ¯YWYMI KOMÓRKAMI
void nextGeneration(cellStructure cell[y][x]); //PRZEJŒCIE DO KOLEJNEJ GENERAJI
char setMode(void); //USTAWIANIE TRYBU ¯YCIA (KROKOWY LUB NIESKOÑCZONY)
void showCurrentGeneration(cellStructure cell[y][x]); //WYŒWIETLANIE OBECNEJ GENERACJI


 /* DEKLARACJA STA£YCH, STANOWI¥CYCH INDEKSY WIERZCHO£KÓW CZÊŒCI PLANSZY NA KTÓREJ BÊDZIE TOCZY£O SIÊ ¯YCIE (WSZYSTKIE KOMÓRKI POZA KOMÓRKAMI NA KRAWÊDZIACH PLANSZY) */
const int first_y = 1;
const int last_y = y - 2;
const int first_x = 1;
const int last_x = x - 2;


const int DEFAULT_SLEEP_TIME = 500; // DEKLARACJA STA£EJ ODPOWIEDZIALNEJ ZA PRÊDKOŒÆ WYSWIETLANIA GENERACJI

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
		cellsSetLife(cell); //ZAPE£NIANIE PLANSZY ¯YWYMI KOMÓRKAMI
		char mode = setMode(); //USTAWIANIE TRYBU ¯YCIA (KROKOWY LUB NIESKOÑCZONY)
		/* ¯YCIE!!! */
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
		while (mode == 'c') { //tryb ci¹g³y wiêc nieskoñczona pêtla
			showCurrentGeneration(cell);
			nextGeneration(cell);
			Sleep(DEFAULT_SLEEP_TIME - speedRatio);
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorReset); //ustawianie kursora na pocz¹tek tablicy, aby j¹ nadpisaæ
		}
		if (mode == 'k') { //tryb krokowy
			char ch;
			do {
				showCurrentGeneration(cell);
				cout << "\nWciœnij dowolny znak, aby przejsc do nastepnej generacji, lub 'e' aby zakonczyc aktualne zycie (komorek!): ";
				cin >> ch;
				nextGeneration(cell);
				system("cls");
			} while (ch != 'e');
		}
		cout << "Wciœnij dowolny znak, aby sprobowac ponownie, lub 'e' aby wyjsc z programu: ";
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

void cellsSetLife(cellStructure cell[y][x]) //ZAPE£NIANIE PLANSZY ¯YWYMI KOMÓRKAMI
{
	char startMap;
	do {
		cout << "Wcisnij 's' jesli chcesz gotowa mapke, 'l' aby plansza byla zapelniana losowo: ";
		cin >> startMap;
		system("cls");
		if (startMap == 'l')
			randomization(cell); //funkcja losuj¹ca planszê
		else if (startMap == 's')
			cout << "NIEDOSTEPNE W WERSJI BETA" << endl;
		else
			cout << "nie ma takiej opcji, sprobuj ponownie." << endl;
	} while (/*startMap != 's' &&*/ startMap != 'l'); //odkomentowac pierwszy warunek po dodaniu obs³ugi szablonów
};

void nextGeneration(cellStructure cell[y][x]) //PRZEJŒCIE DO KOLEJNEJ GENERAJI
{
	//		POSZUKIWANIE ¯YJ¥CYCH KOMÓREK I...
	for (int iy = first_y; iy <= last_y; ++iy) {
		for (int jx = first_x; jx <= last_x; ++jx) {
			if (cell[iy][jx].isAlive == true) { // ...dla znalezionej ¿ywej komórki inkrementowanie wartoœci neigbours jej s¹siadów o 1
				cell[iy + 1][jx - 1].neighbours++;
				cell[iy + 1][jx].neighbours++;
				cell[iy + 1][jx + 1].neighbours++;
				cell[iy][jx - 1].neighbours++;
				cell[iy][jx + 1].neighbours++;
				cell[iy - 1][jx - 1].neighbours++;
				cell[iy - 1][jx].neighbours++;
				cell[iy - 1][jx + 1].neighbours++;
			} // martwe komórki pomijamy
		}
	}
	//		BADANIE NA PODSTAWIE ILOŒCI S¥SIADÓW CZY KOMÓRKA ZMIENI STAN ORAZ ZMIANA TYCH STANÓW
	for (int iy = first_y; iy <= last_y; ++iy) {
		for (int jx = first_x; jx <= last_x; ++jx) {
			if (cell[iy][jx].isAlive == false) { // dla martwej komórki sprawdzam czy ma dokladnie 3 sasiadow (wtedy komorka ozywa czyli zmienia sie jej stan) 
				if (cell[iy][jx].neighbours == 3)
					cell[iy][jx].isAlive = true;
			}
			else { // dla ¿ywych komórek maj¹cych 2 lub 3 s¹siadów stan sie nie zmienia (stateChange wci¹¿ ma wartoœæ false), przy innej liczbie s¹siadów komórka umiera (stateChange = true)
				if (cell[iy][jx].neighbours != 2 && cell[iy][jx].neighbours != 3)
					cell[iy][jx].isAlive = false;
			}
			cell[iy][jx].neighbours = 0; //zmiana wartoœci neigbours na pocz¹tkowe ju¿ sprawdzonych komórek, ¿eby przygotowaæ je do kolejnej generacji
		}
	}
};

char setMode(void) //USTAWIANIE TRYBU ¯YCIA (KROKOWY LUB NIESKOÑCZONY)
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

void showCurrentGeneration(cellStructure cell[y][x]) //WYŒWIETLANIE OBECNEJ GENERACJI
{
	//		przesuwanie siê po ¿ywej planszy aby wyœwietliæ obecn¹ generacjê
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