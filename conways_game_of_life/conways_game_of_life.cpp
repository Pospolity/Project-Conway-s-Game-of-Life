// conways_game_of_life.cpp : Program napisany przez Jakuba Dziechciewicza jako pierwszy projekt na Podstawy Programowania Komputerów (Informatyka PolSl sem.1).
//

#include "stdafx.h"
#include <iostream>
#include "windows.h"
#include <time.h>
#include <cstdlib>
#include <string>
#include <fstream>

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
int podajInt(int min, int max);
string intToString(int number); //FUNKCJA ZAMIENIAJ¥CYA LICZBY TYPU INT NA TE SAME LICZBY JAKO OBIEKT STRING. AKTUALNIE TLKO DLA LICZB OD 0 DO 99 (wiêcej nie potrzeba)
void load(cellStructure cell[y][x], string template_name, int structure_number);
void templates(cellStructure cell[y][x]);
void cellsSetLife(cellStructure cell[y][x]); //ZAPE£NIANIE PLANSZY ¯YWYMI KOMÓRKAMI
void nextGeneration(cellStructure cell[y][x]); //PRZEJŒCIE DO KOLEJNEJ GENERAJI
char setMode(void); //USTAWIANIE TRYBU ¯YCIA (KROKOWY LUB NIESKOÑCZONY)
void showCurrentGeneration(cellStructure cell[y][x]); //WYŒWIETLANIE OBECNEJ GENERACJI
int setSpeedRatio(void);


 /* DEKLARACJA STA£YCH, STANOWI¥CYCH INDEKSY WIERZCHO£KÓW CZÊŒCI PLANSZY NA KTÓREJ BÊDZIE TOCZY£O SIÊ ¯YCIE (WSZYSTKIE KOMÓRKI POZA KOMÓRKAMI NA KRAWÊDZIACH PLANSZY) */
const int first_y = 1;
const int last_y = y - 2;// przystosowanie do dzialania jako indeks tabeli st¹d -2
const int first_x = 1;
const int last_x = x - 2;


const int DEFAULT_SLEEP_TIME = 500; // DEKLARACJA STA£EJ ODPOWIEDZIALNEJ ZA PRÊDKOŒÆ WYSWIETLANIA GENERACJI
const int DEFAULT_ERROR_SLEEP_TIME = 10000; //DEKLARACJA STA£EJ ODPOWIEDZIALNEJ ZA PAUZE PO WYSWIETLENIU B£ÊDU

int main()
{
	srand(time(NULL));
	COORD cursorReset;
	cursorReset.X = 0;
	cursorReset.Y = 0;
	cellStructure cell[y][x];
	
	long long generationCount = 0;
	char start;
	do {
		cellsSetDead(cell); //TWORZENIE MARTWEJ PLANSZY (SZABLONU)
		cellsSetLife(cell); //ZAPE£NIANIE PLANSZY ¯YWYMI KOMÓRKAMI
		char mode = setMode(); //USTAWIANIE TRYBU ¯YCIA (KROKOWY LUB NIESKOÑCZONY)
		/* ¯YCIE!!! */
		int speedRatio = 1;
		if (mode == 'c')
			speedRatio = setSpeedRatio();
		while (mode == 'c') { //tryb ci¹g³y wiêc nieskoñczona pêtla
			showCurrentGeneration(cell);
			cout << "Generacja: " << generationCount << endl;
			nextGeneration(cell);
			generationCount++;
			Sleep(DEFAULT_SLEEP_TIME - speedRatio);
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorReset); //ustawianie kursora na pocz¹tek tablicy, aby j¹ nadpisaæ
		}
		if (mode == 'k') { //tryb krokowy
			char ch;
			do {
				showCurrentGeneration(cell);
				cout << "Generacja: " << generationCount << endl;
				cout << "Wciœnij dowolny znak, aby przejsc do nastepnej generacji, lub 'e' aby zakonczyc aktualne zycie (komorek!): ";
				cin >> ch;
				nextGeneration(cell);
				generationCount++;
				system("cls");
			} while (ch != 'e');
		}
		cout << "Wciœnij dowolny znak, aby sprobowac ponownie, lub 'e' aby wyjsc z programu: ";
		cin >> start;
		generationCount = 0;
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

int podajInt(int min, int max) {
	int number;
	cin >> number;
	while (number < min || number > max) {
		//system("cls");
		cout << "Podales nieprawidlowa cyfre. Sprobuj ponownie" << endl;
		cin >> number;
	};
	return number;
};

string intToString(int number) { // FUNKCJA ZAMIENIAJ¥CYA LICZBY TYPU INT NA TE SAME LICZBY JAKO OBIEKT STRING. AKTUALNIE TLKO DLA LICZB OD 0 DO 99 (wiêcej nie potrzeba)
	string temporary_string;
	if (number < 10) {
		temporary_string = " ";
		temporary_string[0] = char(number + '0');
	};
	if (number >= 10 && number < 100) {
		temporary_string = "  ";
		temporary_string[0] = char((number / 10) + '0');
		temporary_string[1] = char(number + '0');
	};
	
	return temporary_string;
};

void load(cellStructure cell[y][x], string template_name, int structure_number) {
	int lineCount = 0; //licznik bie¿¹cych linii pliku txt
	ifstream fromFile;
	fromFile.open("szablony.txt");
	if (!fromFile.good()) {
		cout << "Nie udalo sie otworzyc pliku 'szablony.txt' lub jest on uszkodzony. Sprawdz czy znajduje sie on w sciezce z plikiem .exe tego programu. Plansza zostanie zapelniona losowo za kilka sekund. Postepuj wedlug dalszych instrukcji." << endl;
		Sleep(DEFAULT_ERROR_SLEEP_TIME);
		randomization(cell);
	};
	string temp_string;
	do { // poszukiwanie szablonu
		if (fromFile.eof()) {
			cout << "Nie znaleziono szablonu. Sprawdz czy nie zmodyfikowales przypadkiem pliku 'szablony.txt'. Plansza zostanie zapelniona losowo za kilka sekund. Postepuj wedlug dalszych instrukcji." << endl;
			Sleep(DEFAULT_ERROR_SLEEP_TIME);
			randomization(cell);
			system("cls");
			return;
		}
		getline(fromFile, temp_string);
		lineCount++;
	} while (temp_string != "#" + template_name);
	
	string structure_number_as_string;
	structure_number_as_string = intToString(structure_number);
	do { // poszukiwanie struktury
		getline(fromFile, temp_string);
		lineCount++;
		if (temp_string[0] == '#' || fromFile.eof()) {
			cout << "Nie znaleziono struktury o danym numerze. Sprawdz czy nie zmodyfikowales przypadkiem pliku 'szablony.txt'. Plansza zostanie zapelniona losowo za kilka sekund. Postepuj wedlug dalszych instrukcji." << endl;
			Sleep(DEFAULT_ERROR_SLEEP_TIME);
			randomization(cell);
			system("cls");
			return;
		}
	} while (temp_string != "$" + structure_number_as_string);
	
	int yFromFile, xFromFile, tempNumber, digitCount;
	bool iscorrect;
	getline(fromFile, temp_string);
	while (temp_string[0] != '$' && temp_string[0] != '#' && !fromFile.eof()){ // ustawianie ¿ywych komórek na planszy, wed³ug recepty zapisanej w pliku
		tempNumber = 0;
		digitCount = 0;
		iscorrect = true; //nie zmienia sie dla poprawnego przebiegu programu
		if(temp_string.length() == 0) //wykluczenie pustych wierszy
			iscorrect = false;
		for (int i = 0; i < temp_string.length(); i++) {
			if (isdigit(temp_string[i])) { // zamiana liczby w string na int i dodanie jej do tempNumber
				if (digitCount == 0)
					tempNumber = tempNumber + int(temp_string[i] - '0'); 
				else
					tempNumber = tempNumber * 10 + int(temp_string[i] - '0');
				digitCount++;
			}
			else if (temp_string[i] == ':') {
				if (tempNumber >= first_y && tempNumber <= last_y)
					yFromFile = tempNumber;
				else {
					cout << "Bledne dane w linii " << lineCount << " pliku .txt. Podane wymiary przekraczaj¹ wymiary planszy.Program bêdzie kontynuowany po kilku sekundach, jednak to pole zostanie pominiete.";
					Sleep(DEFAULT_ERROR_SLEEP_TIME);
					break; //przejscie do nastepnej linii
					iscorrect = false;
				}
				tempNumber = 0;
				digitCount = 0;
			}
			else if (temp_string[i] == '.') {
				if (tempNumber >= first_x && tempNumber <= last_x)
					xFromFile = tempNumber;
				else {
					cout << "Bledne dane w linii " << lineCount << " pliku .txt. Podane wymiary przekraczaj¹ wymiary planszy.Program bêdzie kontynuowany po kilku sekundach, jednak to pole zostanie pominiete.";
					Sleep(DEFAULT_ERROR_SLEEP_TIME);
					break; //przejscie do nastepnej linii
					iscorrect = false;
				}
				tempNumber = 0;
				digitCount = 0;
			}
			else{
				cout << "Bledne dane w linii " << lineCount << " pliku .txt. Program bêdzie kontynuowany po kilku sekundach, jednak plansza moze byc zapelniona niepoprawnie." << endl;
				Sleep(DEFAULT_ERROR_SLEEP_TIME);
			}
		}
		if(iscorrect == true)
			cell[yFromFile][xFromFile].isAlive = true;
		yFromFile = 0;
		xFromFile = 0;
		getline(fromFile, temp_string);
		lineCount++;
	}
	
	system("cls");
};

void templates(cellStructure cell[y][x]) {
	cout << "Oto lista dostepnych szablonow, wybierz jeden wpisujac przypisana do niego cyfre i zatwierdz enterem: " << endl << endl;
	cout << "1. STRUKTURY STATYCZNE" << endl << "2. OSCYLATORY" << endl << "3. STATKI" << endl << "4. DZIALA" << endl << endl;
	int opt = podajInt(1, 4); //wybor szablonu struktur (szablon to np. oscylator, statek itd. - nie jest to oficjalne nazewnictwo)
	system("cls");
	cout << "Wybierz interesujaca strukture i zatwierdz enterem: " << endl << endl;
	int opt2;
	switch (opt) { //wyswietanie mozliwych opcji do wyboru i wybranie poszczegolnych struktur
		case 1: 
			cout << "1. klocek" << endl << "2. lodz" << endl << "3. bochenek" << endl << "4. krysztal" << endl << "5. koniczynka" << endl << "6. staw" << endl;
			opt2 = podajInt(1, 6);
			load(cell, "STRUKTURY_STATYCZNE", opt2);
			break;
		case 2:
			cout << "1. blinker" << endl << "2. zabka" << endl << "3. fontanna" << endl << "4. krokodyl" << endl;
			opt2 = podajInt(1, 4);
			load(cell, "OSCYLATORY", opt2);
			break;
		case 3:
			cout << "1. glider" << endl << "2. dakota LWSS" << endl << "3. dakota MWSS" << endl << "4. dakota HWSS" << endl;
			opt2 = podajInt(1, 4);
			load(cell, "STATKI", opt2);
			break;
		case 4:
			cout << "1.  gosper glider gun" << endl;
			opt2 = podajInt(1, 1);
			load(cell, "DZIALA", opt2);
			break;
	}
	

	
};

void cellsSetLife(cellStructure cell[y][x]) //ZAPE£NIANIE PLANSZY ¯YWYMI KOMÓRKAMI
{
	char startMap;
	do {
		cout << "Wcisnij 's' jesli chcesz zobaczyc ktoras z ciekawych gotowych struktur lub 'l' aby plansza byla zapelniana losowo: ";
		cin >> startMap;
		system("cls");
		if (startMap == 'l')
			randomization(cell); //funkcja losuj¹ca planszê
		else if (startMap == 's') 
			templates(cell); //funkcja wype³niaj¹ca plansze szablonem
		else
			cout << "nie ma takiej opcji, sprobuj ponownie." << endl;
	} while (startMap != 's' && startMap != 'l'); //odkomentowac pierwszy warunek po dodaniu obs³ugi szablonów
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

int setSpeedRatio(void) {
	int speedRatio;
	do {
		cout << "Podaj predkosc od 0 (wolno) do 500 (bardzo szybko) z jaka beda sie zmieniac generacje: ";
		cin >> speedRatio;
		system("cls");
		if (speedRatio < 0 || speedRatio > 500)
			cout << "Prosze sprobowac ponownie, tym razem czytajac ze zrozumieniem." << endl;
	} while (speedRatio < 0 || speedRatio > 500);
	return speedRatio;
};
