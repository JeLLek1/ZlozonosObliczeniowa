#include "TimeMeasure.h"
#include "Menu.h"
#include <iostream>
#include <conio.h>

//Plik wyników
const std::string TimeMeasure::RESULTFILE = "result.txt";

//restart timera
void TimeMeasure::restart()
{
	this->start = std::chrono::high_resolution_clock::now();

}

//dlugosc od ostatniego restartu
long long TimeMeasure::returnTime()
{
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(end - this->start).count();
}

//testy
bool TimeMeasure::test(int size)
{
	std::string fileOutput = "";
	fileOutput += "ilosc testow: " + std::to_string(size) + "\n";
	fileOutput += "Test na tablicy - dodawanie:\n-pocz¹tek: ";
	this->restart();
	for (int i = 0; i < size; i++) {
		this->array->pushFront(i);
	}
	fileOutput += std::to_string(this->returnTime()) += "us\n-koniec: ";
	if (!this->array->loadFile(this->file)) {
		return false;
	}

	this->restart();
	for (int i = 0; i < size; i++) {
		this->array->pushBack(i);
	}
	fileOutput += std::to_string(this->returnTime()) += "us\n-œrodek: ";
	if (!this->array->loadFile(this->file)) {
		return false;
	}

	this->restart();
	for (int i = 0; i < size; i++) {
		this->array->push(i/2, static_cast<size_t>(i/2));
	}
	fileOutput += std::to_string(this->returnTime()) += "us\n:Test na tablicy - usuwanie\n-poczatek:";
	if (!this->array->loadFile(this->file)) {
		return false;
	}

	this->restart();
	for (int i = 0; i < size; i++) {
		this->array->popFront();
	}
	fileOutput += std::to_string(this->returnTime()) += "us\n-koniec: ";
	if (!this->array->loadFile(this->file)) {
		return false;
	}

	this->restart();
	for (int i = 0; i < size; i++) {
		this->array->popBack();
	}
	fileOutput += std::to_string(this->returnTime()) += "us\n-œrodek: ";
	if (!this->array->loadFile(this->file)) {
		return false;
	}

	this->restart();
	for (int i = 0; i < size; i++) {
		this->array->pop(i / 2);
	}
	fileOutput += std::to_string(this->returnTime())+"us\nTest na tablicy - wyszukiwanie: ";
	if(!this->array->loadFile(this->file)) {
		return false;
	}

	this->restart();
	for (int i = 0; i < size; i++) {
		this->array->find(i);
	}
	fileOutput += std::to_string(this->returnTime()) + "us\nTest na liœcie - dodawanie: \n -na poczatek: ";

	this->restart();
	for (int i = 0; i < size; i++) {
		this->list->pushFront(i);
	}
	fileOutput += std::to_string(this->returnTime()) += "us\n-koniec: ";
	if (!this->list->loadFile(this->file)) {
		return false;
	}

	this->restart();
	for (int i = 0; i < size; i++) {
		this->list->pushBack(i);
	}
	fileOutput += std::to_string(this->returnTime()) += "us\n-œrodek: ";
	if (!this->list->loadFile(this->file)) {
		return false;
	}

	this->restart();
	for (int i = 0; i < size; i++) {
		this->list->push(i / 2, static_cast<size_t>(i / 2));
	}
	fileOutput += std::to_string(this->returnTime()) += "us\n:Test na liœcie - usuwanie\n-poczatek:";
	if (!this->list->loadFile(this->file)) {
		return false;
	}

	this->restart();
	for (int i = 0; i < size; i++) {
		this->list->popFront();
	}
	fileOutput += std::to_string(this->returnTime()) += "us\n-koniec: ";
	if (!this->list->loadFile(this->file)) {
		return false;
	}

	this->restart();
	for (int i = 0; i < size; i++) {
		this->list->popBack();
	}
	fileOutput += std::to_string(this->returnTime()) += "us\n-œrodek: ";
	if (!this->list->loadFile(this->file)) {
		return false;
	}

	this->restart();
	for (int i = 0; i < size; i++) {
		this->list->pop(i / 2);
	}
	fileOutput += std::to_string(this->returnTime()) + "us\nTest na liœcie - wyszukiwanie: ";
	if (!this->list->loadFile(this->file)) {
		return false;
	}

	this->restart();
	for (int i = 0; i < size; i++) {
		this->list->find(i);
	}
	fileOutput += std::to_string(this->returnTime()) + "us\nTest na kopcu - dodawanie: ";
	if (!this->array->loadFile(this->file)) {
		return false;
	}

	this->restart();
	for (int i = 0; i < size; i++) {
		this->heap->push(i);
	}
	fileOutput += std::to_string(this->returnTime()) + "us\nTest na kopcu - usuwanie: \n -test dostêpu do tablicy dla usuwania: ";
	if (!this->heap->loadFile(this->file)) {
		return false;
	}

	this->restart();
	int test;
 	for (int i = 0; i < size; i++) {
		test = this->array->array[i];
	}
	fileOutput += std::to_string(this->returnTime()) + "us\n -Usuwanie: ";

	this->restart();
	for (int i = 0; i < size; i++) {
		this->heap->popValue(this->array->array[i]);
	}
	fileOutput += std::to_string(this->returnTime()) + "us\nTest na kopcu -Wyszukiwanie: ";
	if (!this->heap->loadFile(this->file)) {
		return false;
	}

	this->restart();
	for (int i = 0; i < size; i++) {
		this->heap->find(i);
	}
	fileOutput += std::to_string(this->returnTime()) + "us\nDrzewo czerwono czarne - dodawanie: ";
	if (!this->heap->loadFile(this->file)) {
		return false;
	}

	this->restart();
	for (int i = 0; i < size; i++) {
		this->rbTree->push(i);
	}
	fileOutput += std::to_string(this->returnTime()) + "us\nDrzewo czerwono czarne Usuwanie (korekta z kopca nale¿y wzi¹æ): ";
	if (!this->rbTree->loadFile(this->file)) {
		return false;
	}

	this->restart();
	for (int i = 0; i < size; i++) {
		this->rbTree->popValue(this->array->array[i]);
	}
	fileOutput += std::to_string(this->returnTime()) + "us\nDrzewo czerwono czarne - wyszukiwanie: ";
	if (!this->rbTree->loadFile(this->file)) {
		return false;
	}

	this->restart();
	for (int i = 0; i < size; i++) {
		this->rbTree->find(i);
	}
	fileOutput += std::to_string(this->returnTime()) + "us\nDrzewo avl - dodawanie: ";

	this->restart();
	for (int i = 0; i < size; i++) {
		this->avl->push(i);
	}
	fileOutput += std::to_string(this->returnTime()) + "us\nDrzewo avl - usuwanie (nale¿y wzi¹æ poprawkê z kopca): ";

	if (!this->avl->loadFile(this->file)) {
		return false;
	}
	this->restart();
	for (int i = 0; i < size; i++) {
		this->avl->popValue(this->array->array[i]);
	}
	fileOutput += std::to_string(this->returnTime()) + "us\nDrzewo avl - wyszukiwanie: ";

	if (!this->avl->loadFile(this->file)) {
		return false;
	}
	this->restart();
	for (int i = 0; i < size; i++) {
		this->avl->find(i);
	}
	fileOutput += std::to_string(this->returnTime()) + "us";

	std::fstream fileTemp;
	fileTemp.open(TimeMeasure::RESULTFILE, std::ios::out | std::ios::trunc);
	if (fileTemp.good()) {
		fileTemp << fileOutput;
		fileTemp.close();
		return true;
	}

	return false;
}

//konstruktor
TimeMeasure::TimeMeasure(std::string name) : Container(name)
{
	this->file = nullptr;
	this->array = new Array("");
	this->list = new List("");
	this->heap = new Heap("");
	this->rbTree = new RBTree("");
	this->avl = new Avl("");
}

//wczytanie pliku do kontenerów
bool TimeMeasure::loadFile()
{
	if (this->array->loadFile(this->file)&&
		this->list->loadFile(this->file)&&
		this->heap->loadFile(this->file)&&
		this->rbTree->loadFile(this->file)&&
		this->avl->loadFile(this->file)) {
		this->size = this->array->returnSize();
		return true;
	}
	this->erase();
	return false;
}

//zamkniêcie ca³kowicie pliku
void TimeMeasure::erase() {
	if (file != nullptr) {
		if (file->is_open()) {
			this->file->close();
		}
		delete this->file;
		this->file = nullptr;
	}
}

//indeks ostatniego elementu menu
int TimeMeasure::maxMenu()
{
	return TimeMeasure::MAX_MENU;
}

/**
 * Rysowanie menu dla struktury
 *
 * @param currentOption Obecnie wyró¿niona opcja w menu
 */
void TimeMeasure::drawMenu(int currentOption)
{
	if (currentOption == 0) {
		std::cout << "    ->";
	}
	else {
		std::cout << "      ";
	}
	std::cout << "Wczytaj plik\n";
	if (currentOption == 1) {
		std::cout << "    ->";
	}
	else {
		std::cout << "      ";
	}
	std::cout << "Testuj\n";
}
/**
 * Wykonuje akcje odpowiednia do wybranej pozycji menu. Wyœwietla równie¿ komunikaty w razie niepoprawnych danych
 *
 * @return currentOption Indeks wybranej opcji w menu
 */
void TimeMeasure::handleInput(int currentOption)
{
	int value = 0;
	switch (currentOption)
	{
	case 0:
	{
		std::string fileName;
		std::cout << "Podaj nazwe pliku: ";
		std::getline(std::cin, fileName);
		this->file = new std::fstream("" + fileName, std::ios::in);
		if (this->loadFile()) {
			std::cout << "Operacja przebiegla pomyslnie\n";
			this->display();
		}
		else {
			std::cout << "Blad podczas otwierania pliku. Plik nie zostal znaleziony, lub zawiera nieprawidlowe dane jak tekst, lub mniejsza ilosc danych w stosunku do podanej dlugosci.\n\n";
		}

	}
	break;
	case 1:
		if (this->file == nullptr) {
			std::cout << "Najpierw wczytaj plik\n\n";
		}
		else {
			std::cout << "Podaj ilosc testow: ";
			value = Menu::cinInt();
			if (value < this->size) {
				if (this->test(value)) {
					std::cout << "Wyniki testu zapisane zostaly do " << TimeMeasure::RESULTFILE << "\n\n";
				}
				else {
					std::cout << "Wyst¹pi³ problem podczas zapisu lub testowania \n\n";
				}
			}
			else {
				std::cout << "Za ma³a struktura dla tylu testów\n\n";
			}
		}

		break;
	}
}

/**
 * Wyœwietlanie zawartoœci struktury w odpowiedniej formie
 */
void TimeMeasure::display()
{
}

TimeMeasure::~TimeMeasure()
{
	this->erase();
	delete this->array;
	delete this->list;
	delete this->heap;
	delete this->rbTree;
	delete this->avl;
}
