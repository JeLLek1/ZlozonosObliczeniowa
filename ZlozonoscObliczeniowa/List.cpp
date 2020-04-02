#include "List.h"
#include "Menu.h"
#include <iostream>
#include <conio.h>
#include <time.h>

/*
 * Konstruktor klasy List
 *
 * @param name nazwa struktury w menu
 */
List::List(std::string name) : Container(name)
{
	this->tail = nullptr;
	this->head = nullptr;
}

/*
 * Wyszukiwanie elementu po indeksie
 *
 * @param index indeks szukanego elementu
 *
 * @return szukany element
 */
ListSegment* List::search(size_t index)
{
	ListSegment* tmp;
	//szukanie albo od przodu albo od ty³u w zale¿noœci od op³acalnoœci
	if (this->size / 2 > index) {
		tmp = this->head;
		while (index > 0) {
			tmp = tmp->next;
			index--;
		}
	}
	else {
		tmp = this->tail;
		index = this->size - index;
		while (index > 1) {
			tmp = tmp->prev;
			index--;
		}
	}
	return tmp;
}

/*
 * Usuwanie podanego elemenu
 *
 * @param element element do usuniêcia
 */
void List::popElement(ListSegment* element)
{
	if (element == this->head) {
		this->popFront();
		return;
	}
	if (element == this->tail) {
		this->popBack();
		return;
	}
	element->prev->next = element->next;
	element->next->prev = element->prev;

	delete element;
	this->size--;
}

/*
 * Usuwanie podanego elemenu z przodu
 *
 * @return czy operacja zakoñczy³a siê pomyœlnie
 */
bool List::popFront()
{
	if (this->head != nullptr) {
		ListSegment* tmp = this->head;
		this->head = this->head->next;
		if (this->head != nullptr) {
			this->head->prev = nullptr;
		}
		if (this->head == nullptr || this->head->next == nullptr) {
			this->tail = this->head;
		}
		delete tmp;
		this->size--;
		return true;
	}
	return false;
}

/*
 * Usuwanie podanego elemenu z ty³u
 *
 * @return czy operacja zakoñczy³a siê pomyœlnie
 */
bool List::popBack()
{
	if (this->tail != nullptr) {
		ListSegment* tmp = this->tail;
		this->tail = this->tail->prev;
		if (this->tail != nullptr) {
			this->tail->next = nullptr;
		}
		if (this->tail == nullptr || this->tail->prev == nullptr) {
			this->head = this->tail;
		}
		delete tmp;
		this->size--;
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
bool List::pop(size_t index)
{
	if (index == 0) {
		this->popFront();
		return true;
	}

	if (index == this->size -1) {
		this->popBack();
		return true;
	}
	if (index > 0 && index < this->size-1) {
		ListSegment* tmp = this->search(index);

		tmp->prev->next = tmp->next;
		tmp->next->prev = tmp->prev;

		delete tmp;
		this->size--;
		return true;
	}
	return false;
}

/*
 * Usuwanie podanego klucza
 *
 * @param value klucz
 *
 * @return czy operacja zakoñczy³a siê pomyœlnie
 */
bool List::popValue(int value)
{
	ListSegment* tmp = this->head;
	while (tmp != nullptr) {
		if (tmp->value == value) {
			this->popElement(tmp);
			return true;
		}
		tmp = tmp->next;
	}
	return false;
}

/*
 * Wstawianie elementu z przodu
 *
 * @param value wstawiany element
 */
void List::pushFront(int value)
{
	ListSegment* tmp = new ListSegment();
	tmp->value = value;
	tmp->prev = nullptr;
	tmp->next = this->head;
	if (this->head != nullptr) {
		this->head->prev = tmp;
	}
	else {
		this->tail = tmp;
	}
	this->head = tmp;
	this->size++;
}

/*
 * Wstawianie elementu z ty³u
 *
 * @param value wstawiany element
 */
void List::pushBack(int value)
{
	ListSegment* tmp = new ListSegment();
	tmp->value = value;
	tmp->prev = this->tail;
	tmp->next = nullptr;
	if (this->tail != nullptr) {
		this->tail->next = tmp;
	}
	else {
		this->head = tmp;
	}
	this->tail = tmp;
	this->size++;
}

/*
 * Wstawianie elementu w wybranym miejsu
 *
 * @param value wstawiany element
 * @param index index miejsca do wstawienia
 * 
 * @return czy nie podano z³ego indeksu
 */
bool List::push(int value, size_t index)
{
	//je¿eli indeks 0 to nale¿y wstawiæ na pocz¹tku
	if (index == 0) {
		this->pushFront(value);
		return true;
	}
	//je¿eli indeks wiêkszy ni¿ ostatni indeks w liœcie to na koñcu
	if (index == this->size) {
		this->pushBack(value);
		return true;
	}
	if (index > 0 && index < this->size) {
		//w przeciwnym wypadku wsttawianie po indeksie
		ListSegment* tmp = new ListSegment();
		tmp->value = value;
		ListSegment* beforeTmp = this->search(index);
		tmp->prev = beforeTmp->prev;
		tmp->next = beforeTmp;
		beforeTmp->prev->next = tmp;
		beforeTmp->prev = tmp;

		this->size++;
		return true;
	}
	return false;
}

/*
 * Generowanie losowych wartoœci
 *
 * @param size wielkoœæ generowanej struktury
 */
void List::random(size_t size)
{
	this->erase();
	this->size = size;
	std::srand(time(NULL));
	for (size_t i = 0; i < size; i++) {
		this->pushBack(std::rand() % Menu::MAX_RAND);
	}
}

/*
 * Czyszczenie struktury z wartoœci
 *
 */
void List::erase()
{
	while (this->head != nullptr) {
		ListSegment* tmp = this->head;
		this->head = this->head->next;
		delete tmp;
	}
	this->size = 0;
	this->tail = nullptr;
}

/*
 * Sprawdza czy podana wartoœæ jest w strukturze
 *
 * @param value szukana wartoœæ
 *
 * @return czy znajduje siê w strukturze
 */
bool List::find(int value)
{

	ListSegment* tmp = this->head;
	while (tmp != nullptr) {
		if (tmp->value == value) {
			return true;
		}
		tmp = tmp->next;
	}
	return false;
}

/*
 * Wczytywanie danych z pliku
 *
 * @param wska¿nik na plik
 *
 * @return czy operacja przebieg³a pomyœlnie
 */
bool List::loadFile(std::fstream* file)
{
	if (file->good()) {
		std::string line;
		try {
			size_t size = 0;
			if (!file->eof()) {
				getline(*file, line);
				size = static_cast<size_t>(std::stoi(line));
				this->erase();
			}
			else {
				return false;
			}
			size_t counter = 0;
			while (!file->eof())
			{
				getline(*file, line);
				if (size > counter) {
					this->pushBack(std::stoi(line));
				}
				counter++;
			}

			if (size > counter) {
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
void List::drawMenu(int currentOption)
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
void List::handleInput(int currentOption)
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
		std::cout << "Czy na pewno chesz wykonac ta operacje? Usunie ona wszystkie dotychczasowe dane [enter]\n";
		if (_getch() == 13) {
			this->random(index);
			std::cout << "Wygenerowano " << index << " liczb.\n";
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
 * Wyœwietlanie listy od g³owy, od ogona oraz wyœwietlenie jej g³owy i ogona
 *
 */
void List::display()
{
	std::cout << "Zawartosc listy:\n";
	if(this->size>0){
		ListSegment* tmp = this->head;
		std::cout << "Od poczatku: ";
		while (tmp != nullptr) {
			std::cout << tmp->value << " ";
			tmp = tmp->next;
		}
		//test
		std::cout << std::endl;
		tmp = this->tail;
		std::cout << "Od konca: ";
		while (tmp != nullptr) {
			std::cout << tmp->value << " ";
			tmp = tmp->prev;
		}
		//test
		std::cout << std::endl;

		std::cout << "Glowa: " << this->head->value << ", Ogon: " << this->tail->value << "\n\n";
	}else {
		std::cout << "Najpierw wprowadz wartosci\n\n";
	}
}

/*
 * Wartoœæ sprawdzana przes menu podczas obs³ugi zdarzeñ klawiatury
 *
 */
int List::maxMenu()
{
	return List::MAX_MENU;
}

/*
 * Destruktor listy wywo³uje metodê czyszcz¹c¹ listy
 *
 */
List::~List()
{
	this->erase();
}
