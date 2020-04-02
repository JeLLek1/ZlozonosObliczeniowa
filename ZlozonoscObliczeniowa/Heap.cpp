#include "Heap.h"
#include "Menu.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <time.h>

Heap::Heap(std::string name) : Container(name)
{
	this->heap = new int[Heap::HEAP_MAX_SIZE];
}

//Zwraca poziom drzewa-1 dla index-1
size_t Heap::heapLevel(size_t index)
{
	size_t targetlevel=0;
	while (index >>= 1) ++targetlevel;
	return targetlevel;
}

size_t Heap::powerTwo(size_t index)
{
	size_t result = 1;
	for (size_t i = 0; i < index; i++) {
		result *= 2;
	}
	return result;
}

/*
 * Rysowanie elementów drzewa inorder
 * Kropka na liœciach wyœwietlana jest aby po przesuniêciu okna nie by³o czerwonych pasków
 *
 * Funckja jest rekurencyjna na podstawie inorder, jednak dla ³adniejszego uk³adu u¿ywa spaceArray, która trzyma d³ugoœci najd³u¿szych liczb z poziomu oraz levelDrown dla sprawdzenia który poziom ma mieæ przed³u¿enie znakiem |
 *
 * @param spaceArray tablica z d³ugoœci¹ spacji na poziom
 * @param levelDrown czy dany poziom zosta³ rozpoczêty, czy nie (dla rysowania znaku |)
 * @param index obecnie sprawdzany wêze³
 * @param level obecnie sprawdzany poziom
 */
void Heap::drawHeap(int* spaceArray, size_t index, int* levelDrawn, int level=0)
{
	if (index < this->size) {
		this->drawHeap(spaceArray, this->getChildRight(index), levelDrawn, level+1);

		levelDrawn[level]++;
		std::string line = "";
		for (size_t j = 0; j < level; j++) {
			if (levelDrawn[j] % 2 == 1 && j != 0) {
				line += "|";
			}
			else {
				line += " ";
			}
			for (int k = 0; k < spaceArray[j]; k++) {
				line += " ";
			}
		}
		if (level != 0) {
			if (index % 2 == 0) {
				line += static_cast<char>(218);
			}
			else {
				line += static_cast<char>(192);
			}
		}
		line += std::to_string(this->heap[index]);
		if ((index <this->size/2)) {
			for (int i = 0; i < spaceArray[level] - Menu::numberLength(heap[index]); i++) {
				line += "-";
			}
			if (this->getChildLeft(index) == this->size-1) {
				line += static_cast<char>(191);
			}
			else {
				line += static_cast<char>(180);
			}
		}

		line += "\n";
		std::cout << line;
		this->drawHeap(spaceArray, this->getChildLeft(index), levelDrawn, level + 1);
	}
}

/*
 * Zwraca rodzica na podstawie indeksu
 */
size_t Heap::getParent(size_t children)
{
	return (children - 1) / 2;
}

/*
 * Zwraca lewe dziecko na podstawie indeksu
 */
size_t Heap::getChildLeft(size_t parent)
{
	return parent*2+1;
}

/*
 * Zwraca prawe dziecko na podstawie indeksu
 */
size_t Heap::getChildRight(size_t parent)
{
	return parent * 2 + 2;
}

/*
 * Naprawa w dó³ dopuki nie dojdzie do korzenia od ostatniego nie liœcia
 */
void Heap::floydConvert()
{
	for (size_t i = this->size / 2; i > 0; i--) {
		this->repairDown(i - 1);
	}
}

/*
 * Naprawa w dó³ od wybranego indeksu
 */
void Heap::repairDown(size_t parent)
{
	size_t largestChild = this->getChildLeft(parent);
	while (largestChild < this->size) {
		if (largestChild + 1 < this->size && this->heap[largestChild] < this->heap[largestChild + 1]) {
			largestChild++;
		}
		if (this->heap[parent] < this->heap[largestChild]) {
			int tmp = this->heap[parent];
			this->heap[parent] = this->heap[largestChild];
			this->heap[largestChild] = tmp;
			parent = largestChild;
			largestChild = this->getChildLeft(parent);
		}
		else {
			largestChild = this->size;
		}
	}
}
/*
 * Naprawa w górê od wybranego indeksu
 */
void Heap::repairUp(size_t index)
{
	while (index != 0 && this->heap[Heap::getParent(index)] < this->heap[index]) {
		int tmp = this->heap[Heap::getParent(index)];
		this->heap[Heap::getParent(index)] = this->heap[index];
		this->heap[index] = tmp;
		index = Heap::getParent(index);
	}
}

/*
 * Wstawianie nowego elementu wraz z napraw¹ w górê
 */
bool Heap::push(int value)
{
	if (this->size < this->HEAP_MAX_SIZE - 1) {
		this->heap[this->size] = value;
		this->repairUp(this->size);
		this->size++;
		return true;
	}
	return false;
}
/*
 * Generowanie losowe struktury o podanej wielkoœci wraz z napraw¹ po wstawieniu wszystkiego do tablicy
 */
bool Heap::random(size_t size)
{
	if (size <= Heap::HEAP_MAX_SIZE) {
		this->size = size;
		std::srand(time(NULL));
		for (size_t i= 0; i < size; i++) {
			this->heap[i] = std::rand() % Menu::MAX_RAND;
		}
		this->floydConvert();
		return true;
	}
	else {
		return false;
	}
}
/*
 * Szukanie elementu o podanym kluczu
 *
 * zwraca informacje czy znaleziono
 */
bool Heap::find(int value)
{
	size_t i = 0;
	while (i < this->size) {
		if (this->heap[i] == value) {
			return true;
		}
		i++;
	}
	return false;
}

/*
 * Usuwanie korzenia. Po usuniêciu wystarczy naprawa w dó³
 */
bool Heap::popRoot()
{
	if (this->size > 0) {
		this->size--;
		this->heap[0] = this->heap[this->size];
		this->repairDown(0);
		return true;
	}
	return false;
}

/*
 * Usuwanie klucza. Po usuniêciu nale¿y sprawdziæ czy naprawiæ zarówno w dó³ jak i w górê
 */
bool Heap::popValue(int value)
{
	size_t i = 0;
	while (i < this->size) {
		if (this->heap[i] == value) {
			this->size--;
			this->heap[i] = this->heap[this->size];
			this->repairDown(i);
			this->repairUp(i);
			return true;
		}
		i++;
	}
	return false;
}

/*
 * Wczytywanie z pliku
 */
bool Heap::loadFile(std::fstream* file)
{
	if (file->good()) {
		std::string line;
		try {
			if (!file->eof()) {
				getline(*file, line);
				this->size = static_cast<size_t>(std::stoi(line));
			}
			else {
				return false;
			}
			if (this->size > Heap::HEAP_MAX_SIZE) {
				this->size = 0;
				return false;
			}
			size_t counter = 0;
			while (!file->eof())
			{
				getline(*file, line);
				if (size > counter) {
					this->heap[counter]=std::stoi(line);
				}
				counter++;
			}

			if (size > counter) {
				this->erase();
				return false;
			}
			else {
				this->floydConvert();
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
//Czysczenie struktury (tu wystarczy podaæ 0 jako wielkoœæ)
void Heap::erase()
{
	this->size = 0;
}

/**
 * Rysowanie menu dla struktury
 *
 * @param currentOption Obecnie wyró¿niona opcja w menu
 */
void Heap::drawMenu(int currentOption)
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
/**
 * Wykonuje akcje odpowiednia do wybranej pozycji menu. Wyœwietla równie¿ komunikaty w razie niepoprawnych danych
 *
 * @return currentOption Indeks wybranej opcji w menu
 */
void Heap::handleInput(int currentOption)
{
	int index = -1;
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
		std::cout << "Podaj wartosc do usuniecia: ";
		value = Menu::cinInt();
		if (this->popValue(value)) {
			std::cout << "Pierwsze znalezione " << value << " zostalo usuniete\n";
			this->display();
		}
		else {
			std::cout << "Blad. Nie znaleziono podanej wartosci.\n\n";
		}
		break;
	case 2:
		std::cout << "Podaj wartosc do wstawienia: ";
		value = Menu::cinInt();
		if (this->push(value)) {
			std::cout << "Operacja przebiegla pomyslnie\n";
			this->display();
		}
		else {
			std::cout << "Blad. Maksymalna mozliwa ilosc wezlow to "<<Heap::HEAP_MAX_SIZE<<"\n\n";
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
		std::cout << "Czy na pewno chesz wykonac ta operacje? Usunie ona wszystkie dotychczasowe dane [enter]\n";
		if (_getch() == 13) {
			if (this->random(index)) {
				std::cout << "Wygenerowano " << index << " liczb.\n";
				this->display();
			}
			else {
				std::cout<< "Blad. Maksymalna mozliwa ilosc wezlow to " << Heap::HEAP_MAX_SIZE << "\n\n";
			}
		}
		else {
			std::cout << "Przerwano wykonywanie akcji.\n\n";
		}
		break;
	case 5:
		this->display();
		break;
	}
}
/**
 * Wyœwietlanie zawartoœci struktury w odpowiedniej formie
 */
void Heap::display()
{
	if (this->size > 0) {
		std::cout << "Tablica kopca: " << std::endl;
		for (size_t i = 0; i < this->size; i++) {
			std::cout << this->heap[i] << " ";
		}
		std::cout << std::endl;
		std::cout << "Drzewo kopca: " << std::endl;

		//heapSize jest wielkoœci¹ drzewa - 1 potrzebn¹ do okreœlenia wielkoœci tablicy na odleg³oœci miêdzy poziomami
		size_t heapLevel = Heap::heapLevel(this->size);
		int* spaceArray = new int[heapLevel];
		int* levelDrawn = new int[heapLevel + 1];


		memset(levelDrawn, 0, (1+heapLevel) * sizeof(int));

		//pêtla wstawiaj¹ca do tablicy odstêpy miêdzy poszczególnymi poziomami (na podstawie wielkoœci liczb)
		for (size_t i = 0; i < heapLevel; i++) {
			size_t j = Heap::powerTwo(i) - 1;
			int maxSize = Menu::numberLength(this->heap[j]);
			for (++j; j < (Heap::powerTwo(i + 1) - 1) && j < this->size; j++) {
				int length = Menu::numberLength(this->heap[j]);
				if (maxSize < length) {
					maxSize = length;
				}
			}

			spaceArray[i] = maxSize;
		}
		if(heapLevel>0)
			spaceArray[0]--;

		this->drawHeap(spaceArray, 0, levelDrawn);
		delete[] spaceArray;
		delete[] levelDrawn;
		std::cout << "\n";
	}
	else {
		std::cout << "Najpierw wprowadz wartosci\n\n";
	}
}

/**
 * Iloœæ elementów menu dla struktury wykorzystywana w klasie Menu do obs³ugi menu za pomoc¹ klawiszy
 *
 * @return Indeks ostatniego elementu w menu
 */
int Heap::maxMenu()
{
	return Heap::MAX_MENU;
}
/**
 * Destruktor klasy Heap usuwaj¹cy wszystkie elementy w strukturze. Usuwa ca³¹ tablicê kopca
 */
Heap::~Heap()
{
	delete[] this->heap;
}
