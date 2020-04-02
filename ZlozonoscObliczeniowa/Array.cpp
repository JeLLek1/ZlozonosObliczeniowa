#include "Array.h"
#include "Menu.h"
#include <new>
#include <iostream>
#include <time.h>
#include <conio.h>
/*
 * Konstruktor klasy Array
 *
 * @param name Nazwa struktury wykorzystywana podczas wyœwietlania w menu
 */
Array::Array(std::string name) : Container(name)
{
	this->array = new int [0];
}
/*
 * Usuwanie podanego elemenu z przodu
 *
 * @return czy operacja zakoñczy³a siê pomyœlnie
 */
bool Array::popFront()
{
	if (this->size > 0) {
		this->size--;
		int* tmp = new int[this->size];
		for (size_t i = 0; i < this->size; i++) {
			tmp[i] = this->array[i + 1];
		}
		delete[] this->array;
		this->array = tmp;
		return true;
	}
	return false;
}
/*
 * Usuwanie podanego elemenu z ty³u
 *
 * @return czy operacja zakoñczy³a siê pomyœlnie
 */
bool Array::popBack()
{
	if (this->size > 0) {
		this->size--;
		int* tmp = new int[this->size];
		for (size_t i = 0; i < this->size; i++) {
			tmp[i] = this->array[i];
		}
		delete[] this->array;
		this->array = tmp;
		return true;
	}
	return false;
}
/*
 * Usuwanie elementu w podanym indeksie
 *
 * @param index index elementu
 *
 * @return czy operacja zakoñczy³a siê pomyœlnie
 */
bool Array::pop(size_t index)
{
	if (this->size > 0 && index < this->size) {
		int* tmp = new int[this->size - 1];
		for (size_t i = 0; i < index; i++) {
			tmp[i] = this->array[i];
		}
		for (size_t i = index + 1; i < this->size; i++) {
			tmp[i - 1] = this->array[i];
		}
		this->size--;
		delete[] this->array;
		this->array = tmp;
		return true;
	}
	return false;
}
/*
 * Wstawianie elementu z przodu
 *
 * @param value wstawiany element
 */
void Array::pushFront(int value)
{
		int* tmp = new int[this->size + 1];
		tmp[0] = value;
		for (size_t i = 0; i < this->size; i++) {
			tmp[i + 1] = this->array[i];
		}
		this->size++;
		delete[] this->array;
		this->array = tmp;
}
/*
 * Wstawianie elementu z ty³u
 *
 * @param value wstawiany element
 */
void Array::pushBack(int value)
{
		int* tmp = new int[this->size + 1];
		for (size_t i = 0; i < this->size; i++) {
			tmp[i] = this->array[i];
		}
		tmp[this->size] = value;
		this->size++;
		delete[] this->array;
		this->array = tmp;
}
/*
 * Wstawianie elementu w wybranym miejsu
 *
 * @param value wstawiany element
 * @param index index miejsca do wstawienia
 *
 * @return czy nie podano z³ego indeksu
 */
bool Array::push(int value, size_t index)
{
	if (index <= this->size) {
		int* tmp = new int[this->size + 1];
		for (size_t i = 0; i < index; i++) {
			tmp[i] = this->array[i];
		}
		tmp[index] = value;
		for (size_t i = index; i < this->size; i++) {
			tmp[i + 1] = this->array[i];
		}
		this->size++;
		delete[] this->array;
		this->array = tmp;
		return true;
	}
	return false;
}
/*
 * Sprawdza czy podana wartoœæ jest w strukturze
 *
 * @param value szukana wartoœæ
 *
 * @return czy znajduje siê w strukturze
 */
bool Array::find(int value)
{
	size_t i = 0;
	while (i < this->size) {
		if (this->array[i] == value) {
			return true;
		}
		i++;
	}
	return false;
}
/*
 * Generowanie losowych wartoœci
 *
 * @param size wielkoœæ generowanej struktury
 */
void Array::random(size_t size)
{
	delete[] this->array;
	this->array = new int[size];
	this->size = size;
	std::srand(static_cast<unsigned int>(time(NULL)));
	for (size_t i = 0; i < size; i++) {
		this->array[i] = std::rand()%Menu::MAX_RAND;
	}
}
/*
 * Czyszczenie struktury z wartoœci
 *
 */
void Array::erase()
{

	delete[] this->array;
	this->array = new int[0];
	this->size = 0;
}
/*
 * Wczytywanie danych z pliku
 *
 * @param wska¿nik na plik
 *
 * @return czy operacja przebieg³a pomyœlnie
 */
bool Array::loadFile(std::fstream* file)
{
	if (file->good()) {
		std::string line;
		try {
			if (!file->eof()) {
				getline(*file, line);
				this->size = static_cast<size_t>(std::stoi(line));
				delete[] this->array;
				this->array = new int[this->size];
			}
			else {
				return false;
			}
			size_t counter = 0;
			while (!file->eof())
			{
				getline(*file, line);
				if (this->size > counter) {
					this->array[counter] = std::stoi(line);
				}
				counter++;
			}

			if (this->size > counter) {
				this->erase();
				return false;
			}
			else {
				file->clear();
				file->seekg(0, std::ios::beg);
				return true;
			}
		}
		catch (std::exception) {
			this->erase();
			return false;
		}
		file->close();
	}
	return false;
}

/*
 * Rysowanie menu struktury
 *
 * @param currentOption wyró¿niona opcja w menu
 */
void Array::drawMenu(int currentOption)
{
	if (currentOption == 0) {
		std::cout << "    ->";
	}
	else {
		std::cout << "      ";
	}
	std::cout << "Zbuduj z pliku\n";
	if (currentOption == 1) {
		std::cout << "    ->";
	}
	else {
		std::cout << "      ";
	}
	std::cout << "Usun\n";
	if (currentOption == 2) {
		std::cout << "    ->";
	}
	else {
		std::cout << "      ";
	}
	std::cout << "Dodaj\n";
	if (currentOption == 3) {
		std::cout << "    ->";
	}
	else {
		std::cout << "      ";
	}
	std::cout << "Znajdz\n";
	if (currentOption == 4) {
		std::cout << "    ->";
	}
	else {
		std::cout << "      ";
	}
	std::cout << "Utworz losowo\n";
	if (currentOption == 5) {
		std::cout << "    ->";
	}
	else {
		std::cout << "      ";
	}
	std::cout << "Wyswietl\n";
}
/*
 * W zale¿noœci od wybranej opcji wykonanie odpowiedniej akcji
 *
 * @param currentOption wyró¿niona opcja w menu
 */
void Array::handleInput(int currentOption)
{
	size_t index = -1;
	int value = 0;
	switch (currentOption)
	{
	case 0: 
		{
			std::string fileName;
			std::cout << "Podaj nazwe pliku: ";
			std::getline(std::cin, fileName);
			std::cout << "Czy na pewno chesz wykonac ta operacje? Usunie ona wszystkie dotychczasowe dane [enter]\n";
			if (_getch() == 13) {
				std::fstream* file = new std::fstream(fileName, std::ios::in);
				if (this->loadFile(file)) {
					std::cout << "Operacja przebiegla pomyslnie\n";
					this->display();
				}
				else {
					std::cout << "Blad podczas otwierania pliku. Plik nie zostal znaleziony, lub zawiera nieprawidlowe dane jak tekst, lub mniejsza ilosc danych w stosunku do podanej dlugosci.\n\n";
				}

				delete file;
			}
			else {
				std::cout << "Akce przerwano\n\n";
			}
		}
		break;
	case 1:
		if (this->size == 0) {
			std::cout << "Brak zawartosci tablicy\n";
		}
		else {
			std::cout << "Podaj indeks do usuniecia: ";
			index = Menu::cinSize_t();
			if (this->pop(index)) {
				std::cout << "Operacja przebiegla pomyslnie\n";
				this->display();
			}
			else {
				std::cout << "Blad. Sprawdz czy podales poprawny indeks\n\n";
			}
		}
		break;
	case 2:
		std::cout << "Podaj indeks na ktory chcesz wstawic wartosc z przedzialu [0 - " << this->size << "]: ";
		index = Menu::cinSize_t();
		std::cout << "Podaj wartosc do wstawienia: ";
		value = Menu::cinInt();
		if (this->push(value, index)) {
			std::cout << "Operacja przebiegla pomyslnie\n";
			this->display();
		}
		else {
			std::cout << "Blad. Sprawdz czy podales poprawny indeks\n\n";
		}
		break;
	case 3:
		std::cout << "Podaj wartosc do wyszukania: ";
		value = Menu::cinInt();
		if (this->find(value)) {
			this->display();
			std::cout << "Podana wartosc znajduje sie w struktura\n\n";
		}
		else {
			this->display();
			std::cout << "Podanej wartosci nie ma w strukturze\n\n";
		}
		break;
	case 4:
		std::cout << "Podaj ilosc wartosci do wygenerowania: ";
		index = Menu::cinSize_t();
		std::cout << "Czy na pewno chesz wykonac ta operacje? Usunie ona wszystkie dotychczasowe dane [enter]";
		if (_getch() == 13) {
			this->random(index);
			std::cout << "\nWygenerowano "<<index<<" liczb.\n";
			this->display();
		}
		else {
			std::cout << "\nPrzerwano wykonywanie akcji.\n\n";
		}
		break;
	case 5:
		this->display();
		break;
	}
}
/*
 * Wyœwietlanie struktury
 *
 */
void Array::display()
{
	if (this->size > 0) {
		std::cout << "Zawartosc tablicy:\n";
		for (size_t i = 0; i < this->size; i++) {
			std::cout << this->array[i] << " ";
		}
		std::cout << "\n\n";
	}
	else {
		std::cout << "Najpierw wprowadz wartosci\n\n";
	}
}
/*
 * Wartoœæ sprawdzana przes menu podczas obs³ugi zdarzeñ klawiatury
 *
 */
int Array::maxMenu()
{
	return Array::MAX_MENU;
}
/*
 * Destruktor tablicy czyszcz¹cy pamiêæ z tablicy
 *
 */
Array::~Array()
{
	delete[] this->array;
}
