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

 /* DEKLARACJA STA£YCH, STANOWI¥CYCH INDEKSY WIERZCHO£KÓW CZÊŒCI PLANSZY NA KTÓREJ BÊDZIE TOCZY£O SIÊ ¯YCIE (WSZYSTKIE KOMÓRKI POZA KOMÓRKAMI NA KRAWÊDZIACH PLANSZY) */
const int first_y = 1;
const int last_y = y - 2;
const int first_x = 1;
const int last_x = x - 2;

int main()
{
	srand(time(NULL));
	COORD cursorReset;
	cursorReset.X = 0;
	cursorReset.Y = 0;
	cellStructure cell[y][x];
	cellsSetDead(cell); //TWORZENIE MARTWEJ PLANSZY (SZABLONU)
	/* O¯YWIANIE PLANSZY */
	char startMap;
	cout << "wciœnij s jesli chcesz gotowa mapke, lub dowolny inny przycisk aby plansza byla zapelniana losowo: ";
	cin >> startMap;
	system("cls");
	if (startMap == 's') {} //wkleiæ funkcje losuj¹c¹ mapê
	 //LOSOWANKO
		 for (int iy = first_y; iy <= last_y; ++iy) {
			 for (int jx = first_x; jx <= last_x; ++jx) {
				 if (rand() % 10 > 6)
					 cell[iy][jx].isAlive = true;
			 }
		 }
	
	/* ¯YCIE!!! */
	char ch = 'a';
	//while (ch != 'e'){ //przejœcie do nastêpnej generacji lub zamkniecie programu klikaj¹c 'e'
	while (ch == 'a') {
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
		//		ponowne przesuwanie siê po ¿ywej planszy
		for (int iy = first_y; iy <= last_y; ++iy) {
			for (int jx = first_x; jx <= last_x; ++jx) {
				if (cell[iy][jx].isAlive == true) { // dla znalezionej ¿ywej komórki inkrementowanie wartoœci neigbours jej s¹siadów o 1
					cell[iy + 1][jx - 1].neighbours++;
					cell[iy + 1][jx].neighbours++;
					cell[iy + 1][jx + 1].neighbours++;
					cell[iy][jx - 1].neighbours++;
					cell[iy][jx + 1].neighbours++;
					cell[iy - 1][jx - 1].neighbours++;
					cell[iy - 1][jx].neighbours++;
					cell[iy - 1][jx + 1].neighbours++;
				}
			}
		}
		/*
		//		wyœwietlanie tablicy s¹siedztw
		for (int iy = first_y; iy <= last_y; ++iy) {
		for (int jx = first_x; jx <= last_x; ++jx) {
		cout << cell[iy][jx].neighbours;
		}
		cout << endl;
		}
		*/
		//		kolejne	przesuwanie siê po ¿ywej planszy
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

		Sleep(15);
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorReset); //ustawianie kursora na pocz¹tek tablicy, aby j¹ nadpisaæ
																				//cin >> ch;
	}
	return 0;
}

void cellsSetDead(cellStructure cell[y][x]) { //TWORZENIE MARTWEJ PLANSZY (SZABLONU)
	for (int iy = 0; iy < y; iy++) {
		for (int jx = 0; jx < x; jx++) {
			cell[iy][jx].isAlive = false;
			cell[iy][jx].neighbours = 0;
		}
	}
};