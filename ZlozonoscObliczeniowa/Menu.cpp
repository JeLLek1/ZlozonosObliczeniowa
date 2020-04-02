#include "Menu.h"
#include <iostream>
#include <fstream>
#include <conio.h>
//Informacja u góry ekranu
const std::string Menu::INFORMATION = "  =====================================\n       Program testowania struktur\n       (a & d) - wybor struktury\n     (w & s) - wybor pozycji menu\n  (Enter) - akceptacja, (esc) - wyjscie\n  =====================================\n\n";
//Iloœæ struktur
const size_t Menu::STRUCTURES_COUND = 6;
//Konstruktor klasy Menu
Menu::Menu()
{
	this->option = 0;
	this->input = 0;
	this->structure = 0;
	this->conatainers = new Container * [Menu::STRUCTURES_COUND] {new Array("Tablica   "), new List("Lista     "), new Heap("Kopiec    "), new RBTree("Drzewo RB "), new Avl("Drzewo AVL"), new TimeMeasure("Testy     ") };

}
//Element pomocniczy zwracaj¹cy d³ugoœæ jak¹ zajmie liczba w konsoli
int Menu::numberLength(int number)
{
	int length = 0;
	if (number < 0) {
		number *= -1;
		length++;
	}
	do {
		number /= 10;
		length++;
	} while (number > 0);
	return length;
}
//cin dla liczby int z sprawdzaniem poprawnoœci
int Menu::cinInt()
{
	int value;
	std::cin >> value;
	while (std::cin.good() == false)
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Podaj wartosc calkowita: ";
		std::cin >> value;
	}

	return value;
}

//cin dla liczby size_t z sprawdzaniem poprawnoœci
size_t Menu::cinSize_t()
{
	size_t value;
	std::cin >> value;
	while (std::cin.good() == false)
	{
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Podaj poprawna wartosc indeksu: ";
		std::cin >> value;
	}

	return value;
}

//pêtla g³ówna programu
void Menu::loop()
{
	while (input != 27) {
		this->handleInput();
		system("CLS");
		this->drawMenu();
		this->input = _getch();
	}
}

//rysowanie menu
void Menu::drawMenu()
{
	std::cout << Menu::INFORMATION;
	std::cout << "              <[ "<<this->conatainers[this->structure]->name() << " ]>\n\n";
	std::cout << "      Wielkosc struktury: " << this->conatainers[this->structure]->returnSize() << "\n\n";
	this->conatainers[this->structure]->drawMenu(this->option);
}

//obs³uga zdarzeñ klawiatury
void Menu::handleInput()
{
	switch (this->input)
	{
	case 97:
		if (this->structure == 0) {
			this->structure = Menu::STRUCTURES_COUND-1;
		}
		else {
			this->structure--;
		}
		this->option = 0;
		break;
	case 100:
		if (this->structure == Menu::STRUCTURES_COUND-1) {
			this->structure = 0;
		}
		else {
			this->structure++;
		}
		this->option = 0;
		break;
	case 119:
		if (this->option == 0) {
			this->option = this->conatainers[this->structure]->maxMenu();
		}
		else {
			this->option--;
		}
		break;
	case 115:
		if (this->option == this->conatainers[this->structure]->maxMenu()) {
			this->option = 0;
		}
		else {
			this->option++;
		}
		break;
	case 13:
		system("CLS");

		this->conatainers[this->structure]->handleInput( this->option);

		system("PAUSE");
		break;
	}
}

//destruktor klasy Menu
Menu::~Menu()
{
	for (size_t i = 0; i < Menu::STRUCTURES_COUND; i++) {
		delete this->conatainers[i];
	}
	delete[] this->conatainers;
}