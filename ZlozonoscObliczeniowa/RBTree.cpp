#include "RBTree.h"
#include "Menu.h"
#include <iostream>
#include <conio.h>
#include <fstream>
#include <Windows.h>
#include <time.h>
/*
 * Rekurencyjne usuwanie z pamiêci od zadanego wirzcho³ka.
 *
 * @param node Wierzcholdek od ktorego ma sie zaczac usuwanie.
 *
 * @warning metoda nie dba o ustawianie this->NIL dla rodzica elementu od którego zaczêto usuwanie, oraz o zmianê d³ugoœci struktury. Nale¿y o to zadbaæ po usuniêciu.
 */
void RBTree::ereaseFrom(Node* node)
{
	if (node == this->NIL) return;

	this->ereaseFrom(node->left);
	this->ereaseFrom(node->right);
	delete node;
}

/*
 * Wyznaczanie wielkoœci drzewa
 *
 * @return wielkosc drzewa
 */
size_t RBTree::treeLevel(Node* fromNode, size_t level = 0)
{
	if (fromNode == this->NIL) return level;

	size_t left = treeLevel(fromNode->left, level + 1);
	size_t right = treeLevel(fromNode->right, level + 1);
	if (left < right) {
		return right;
	}
	return left;
}

/*
 * Wyznaczanie d³ugoœci najd³u¿szej liczby na poziom
 *
 * @param spaceArray tablica z d³ugoœci¹ spacji na poziom
 * @param node obecnie sprawdzany wêze³
 * @param level obecnie sprawdzany poziom
 */
void RBTree::fillSpaceArray(int* spaceArray, Node* node, size_t level = 0)
{
	if (node->left == this->NIL && node->right == this->NIL) return;
	int space = Menu::numberLength(node->value);
	if (space > spaceArray[level]) spaceArray[level] = space;

	if (node->left != this->NIL) this->fillSpaceArray(spaceArray, node->left, level + 1);
	if (node->right != this->NIL) this->fillSpaceArray(spaceArray, node->right, level + 1);
}

/*
 * Rysowanie elementów drzewa inorder
 * Kropka na liœciach wyœwietlana jest aby po przesuniêciu okna nie by³o czerwonych pasków
 *
 *
 * Funckja jest rekurencyjna na podstawie inorder, jednak dla ³adniejszego uk³adu u¿ywa spaceArray, która trzyma d³ugoœci najd³u¿szych liczb z poziomu oraz levelDrown dla sprawdzenia który poziom ma mieæ przed³u¿enie znakiem |
 *
 * @param spaceArray tablica z d³ugoœci¹ spacji na poziom
 * @param levelDrown czy dany poziom zosta³ rozpoczêty, czy nie (dla rysowania znaku |)
 * @param node obecnie sprawdzany wêze³
 * @param level obecnie sprawdzany poziom
 * @param right czy obecny wêze³ jest prawy czy lewy
 */
void RBTree::drawTree(int* spaceArray, bool* levelDrown, Node* node, int level = 0, bool right = true)
{
	if (node == this->NIL) return;
	//warunek dla lewej czêœci drzewa znaku |
	if (!right) {
		if (level > 0) {
			levelDrown[level - 1] = true;
		}
	}
	//czêœæ inorder
	this->drawTree(spaceArray, levelDrown, node->right, level + 1, true);
	//warudenk dla prawej czêœci drzewa znaku |
	if (right) {
		if (level > 0) {
			levelDrown[level - 1] = true;
		}
	}
	//znaki specjalne i ostêpy
	std::string line = "";
	for (int i = 0; i < level; i++) {
		if (i>0 && levelDrown[i-1]) {
			line += "|";
		}
		else {
			line += " ";
		}
		for (int j = 0; j < spaceArray[i]; j++) {
			line += " ";
		}
	}
	if (level>0) {
		if (right) {
			line += static_cast<char>(218);
		}
		else {
			line += static_cast<char>(192);
		}
	}
	std::cout << line;
	//wyœwietlenie klucza wraz z kolorem
	if (node->color == Color::RED) {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 79);
		std::cout << node->value;
		SetConsoleTextAttribute(hConsole, 15);
	}
	else {
		std::cout << node->value;
	}
	if (node->left != this->NIL || node->right != this->NIL) {
		for (int i = 0; i < spaceArray[level] - Menu::numberLength(node->value); i++) {
			std::cout << "-";
		}
		if (node->left != this->NIL && node->right != this->NIL) {
			std::cout << static_cast<char>(180);
		}
		else if (node->left != this->NIL) {
			std::cout << static_cast<char>(191);
		}
		else {
			std::cout << static_cast<char>(217);
		}
	}
	else {
		std::cout << ".";
	}
	std::cout << "\n";
	//warunek 
	//warunek dla lewej czêœci drzewa znaku |
	if (!right) {
		if (level > 0) {
			levelDrown[level - 1] = false;
		}
	}
	//inorder
	this->drawTree(spaceArray, levelDrown, node->left, level + 1, false);
	//warunek dla prawej czêœci drzewa znaku |
	if (right) {
		if (level > 0) {
			levelDrown[level - 1] = false;
		}
	}
}

/**
 * Rotacja w lewo dla podanego wêz³a
 *
 * @param node wêze³ dla którego ma zostaæ wykonana rotacja
*/
void RBTree::rotateLeft(Node* node)
{ 
	if (node->parent == this->NIL) {
		this->root = node->right;
	}
	else {
		if (node == node->parent->left) {
			node->parent->left = node->right;
		}
		else {
			node->parent->right = node->right;
		}
	}
	node->right->parent = node->parent;

	node->parent = node->right;
	node->right = node->parent->left;
	if (node->right != this->NIL) {
		node->right->parent = node;
	}
	node->parent->left = node;
}
/**
 * Rotacja w prawo dla podanego wêz³a
 *
 * @param node wêze³ dla którego ma zostaæ wykonana rotacja
*/
void RBTree::rotateRight(Node* node)
{
	if (node->parent == this->NIL) {
		this->root = node->left;
	}
	else {
		if (node == node->parent->left) {
			node->parent->left = node->left;
		}
		else {
			node->parent->right = node->left;
		}
	}
	node->left->parent = node->parent;

	node->parent = node->left;
	node->left = node->parent->right;
	if (node->right != this->NIL) {
		node->left->parent = node;
	}
	node->parent->right = node;
}

/*
 * Wyszukuje nastêpnika wybranego wêz³a
 *
 * @param node wêze³, dla którego nastêpnik ma zostaæ znaleziony
 *
 * @return wêze³, który jest nastêpnikiem
 */
Node* RBTree::findSuccesor(Node* node)
{
	if (node != this->NIL) {
		if (node->right != this->NIL) {
			Node* tmp;
			tmp = node->right;
			while (tmp->left != this->NIL) {
				tmp = tmp->left;
			}
			return tmp;
		}
		while (node->parent != this->NIL && node->parent->left != node) {
			node = node->parent;
		}
		return node->parent;
	}
	return this->NIL;
}

/*
 * Konstruktor klasy RBTree
 *
 * @param name Nazwa struktury wykorzystywana podczas wyœwietlania w menu
 */
RBTree::RBTree(std::string name) : Container(name)
{
	this->NIL = new Node(0, this->NIL, nullptr);
	this->root = this->NIL;
}

/**
 * Wstawianie do struktury wybranej wartoœci oraz naprawa
 *
 * @param value wartoœæ do wstawienia
 */
void RBTree::push(int value)
{
	//wstawianie elementu do drzewa bst
	Node* temp;
	//je¿eli brak ko¿enia nowy element jest korzeniem
	if (this->root == this->NIL) {
		this->root = temp = new Node(value, this->NIL, this->NIL);
	}
	else {
		Node* tmp = this->root;
		Node* parent = this->NIL;
		//wyszukanie miejsca dla nowego elementu
		while (tmp != this->NIL) {
			parent = tmp;
			if (value < parent->value) {
				tmp = tmp->left;
			}
			else {
				tmp = tmp->right;
			}
		}

		if (value < parent->value) {
			parent->left = temp = new Node(value, parent, this->NIL);
		}
		else {
			parent->right = temp = new Node(value, parent, this->NIL);
		}
	}
	//Naprawa drzewa czerwono-czarnego
	while (temp != this->root && temp->parent->color == Color::RED && temp->color != Color::BLACK) {
		
		Node* parent = temp->parent;
		Node* gParent = parent->parent;
		if (parent == gParent->left) {
			Node* uncle = gParent->right;
			//Je¿eli wójek czerwony to wystarczy zamiana kolorów. NIL jest czarny
			if (uncle!=this->NIL && uncle->color == Color::RED) {
				parent->color = Color::BLACK;
				uncle->color = Color::BLACK;
				gParent->color = Color::RED;
				temp = gParent;
			}
			else {
				if (temp == parent->right) {
					this->rotateLeft(parent);
				}
				//Kolejna czêœæ zamiany left righ jest taka sama jak left left wiêc s¹ po³¹czone
				this->rotateRight(gParent);
				Color ctmp = gParent->color;
				gParent->color = gParent->parent->color;
				gParent->parent->color = ctmp;
				temp = gParent->parent;
			}
		}
		else {
			//lustrzana sytuacja tej z góry
			Node* uncle = gParent->left;
			if (uncle != this->NIL && uncle->color == Color::RED) {
				parent->color = Color::BLACK;
				uncle->color = Color::BLACK;
				gParent->color = Color::RED;
				temp = gParent;
			}
			else {
				if (temp == parent->left) {
					this->rotateRight(parent);
				}
				this->rotateLeft(gParent);
				Color ctmp = gParent->color;
				gParent->color = gParent->parent->color;
				gParent->parent->color = ctmp;
				temp = gParent->parent;
			}
		}
	}
	this->root->color = Color::BLACK;

	this->size++;


}

/**
 * Usuwanie wszystkich elementów drzewa
 */
void RBTree::erase()
{
	this->ereaseFrom(this->root);
	this->root = this->NIL;
	this->size = 0;
}

/**
 * Generuje losowe klucze i wstawia je do struktury
 * 
 * @param size wielkoœæ struktury
 */
void RBTree::random(size_t size)
{
	this->erase();
	this->size = size;
	std::srand(static_cast<unsigned int>(time(NULL)));
	for (size_t i = 0; i < size; i++) {
		this->push(std::rand() % Menu::MAX_RAND);
	}
}

/**
 * Wyszukuje wartoœæ w strukturze
 * 
 * @param value wartoœæ do wyszukania
 *
 * @return wartoœæ logiczn¹ czy znaleziono
 */
bool RBTree::find(int value)
{
	Node* tmp = this->root;
	Node* parent = this->NIL;
	while (tmp != this->NIL) {
		parent = tmp;
		if (value < parent->value) {
			tmp = tmp->left;
		}
		else if(value>parent->value) {
			tmp = tmp->right;
		}
		else {
			return true;
		}
	}
	return false;
}

/**
 * Usuwanie elementu o podanym kluczu
 *
 * @param value klucz do usuniêcia
 *
 * @return czy operacja przebieg³a pomyœlnie
 */
bool RBTree::popValue(int value)
{
	//Wyszukanie elementu do usuniêcia
	Node* toRemove = this->root;
	while (toRemove != this->NIL && toRemove->value != value) {
		if (value < toRemove->value) {
			toRemove = toRemove->left;
		}
		else {
			toRemove = toRemove->right;
		}
	}
	//je¿eli znaleziony
	if (toRemove != this->NIL) {
		//usuwanie jak w zwyk³ym BST na podstawie nastêpnika
		Node* succ;
		Node* succChild;
		if (toRemove->left == this->NIL || toRemove->right == this->NIL) {
			succ = toRemove;
		}
		else {
			succ = this->findSuccesor(toRemove);
		}
		
		if (succ->left != this->NIL) {
			succChild = succ->left;
		}
		else {
			succChild = succ->right;
		}

		succChild->parent = succ->parent;

		if (succ->parent == this->NIL) {
			this->root = succChild;
		}
		else {
			if (succ == succ->parent->left) {
				succ->parent->left = succChild;
			}
			else {
				succ->parent->right = succChild;
			}
		}

		if (succ != toRemove) {
			toRemove->value = succ->value;
		}

		//Naprawa drzewa czerwono-czarnego
		if (succ->color == Color::BLACK) {
			//dopóki nie przejdziemy przez wszystkie elementy
			while (succChild != this->root && succChild->color == Color::BLACK) {
				if (succChild == succChild->parent->left) {
					//na podstawie warunku okreœlono który wêze³ jest bratem sprawdzanego wêz³a
					Node* sibling = succChild->parent->right;
					//przypadek je¿eli wêze³ brata-sibling jest czerwony. Rotacja w prawo
					if (sibling->color == Color::RED) {
						sibling->color = Color::BLACK;
						succChild->parent->color = Color::RED;
						this->rotateLeft(succChild->parent);
						//przejœcie do kolejnego elementu w górê
						sibling = succChild->parent->right;
					}
					//je¿eli wêze³ sibling jest czarny a jest dzieci równie¿ to wystarczy zmieniæ kolor rodzica
					if (sibling->left->color == Color::BLACK && sibling->right->color == Color::BLACK) {
						sibling->color = Color::RED;
						//przejœcie do kolejnego elementu w górê
						succChild = succChild->parent;
					}
					else {
						//ptzypadek gdy brat sprawdzanego wêz³a jest czarny (sibling) a dzieci siblin s¹ lewe czerwone a prawe czarne
						if (sibling->right->color == Color::BLACK) {
							sibling->left->color = Color::BLACK;
							sibling->color = Color::RED;
							this->rotateRight(sibling);
							sibling = succChild->parent->right;
						}
						//po zmianie sytuacji u góry otrzymujemy sytuacje poni¿ej
						//sibling jest czarny a jego syn czerwony
						sibling->color = succChild->parent->color;
						succChild->parent->color = Color::BLACK;
						sibling->right->color = Color::BLACK;
						this->rotateLeft(succChild->parent);
						succChild = this->root;
					}
				}
				else {
					//Lustrzane odbicie z warunku wy¿ej
					Node* sibling = succChild->parent->left;
					if (sibling->color == Color::RED) {
						sibling->color = Color::BLACK;
						succChild->parent->color = Color::RED;
						this->rotateRight(succChild->parent);
						sibling = succChild->parent->left;
					}
					if (sibling->left->color == Color::BLACK && sibling->right->color == Color::BLACK) {
						sibling->color = Color::RED;
						succChild = succChild->parent;
					}
					else {
						if (sibling->left->color == Color::BLACK) {
							sibling->right->color = Color::BLACK;
							sibling->color = Color::RED;
							this->rotateLeft(sibling);
							sibling = succChild->parent->left;
						}
						sibling->color = succChild->parent->color;
						succChild->parent->color = Color::BLACK;
						sibling->left->color = Color::BLACK;
						this->rotateRight(succChild->parent);
						succChild = this->root;
					}
				}
			}
		}
		succChild->color = Color::BLACK;

		delete succ;
		return true;
	}
	return false;
}

/**
 * £adowanie danych z pliku
 *
 * @param currentOption Obecnie wyró¿niona opcja w menu
 */
bool RBTree::loadFile(std::fstream* file)
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
					this->push(std::stoi(line));
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

/**
 * Rysowanie menu dla struktury
 *
 * @param currentOption Obecnie wyró¿niona opcja w menu
 */
void RBTree::drawMenu(int currentOption)
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
void RBTree::handleInput(int currentOption)
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
		std::cout << "Podaj wartosc do wstawienia: ";
		value = Menu::cinInt();
		this->push(value);
		std::cout << "Operacja przebiegla pomyslnie\n";
		this->display();
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

/**
 * Wyœwietlanie zawartoœci struktury w odpowiedniej formie
 */
void RBTree::display()
{
	if (this->root != this->NIL) {
		size_t treeLevel = this->treeLevel(this->root);
		int* spaceArray = new int[treeLevel - 1];
		bool* levelDrown = new bool[treeLevel];
		memset(spaceArray, 0, (treeLevel-1) * sizeof(int));
		memset(levelDrown, false, (treeLevel) * sizeof(bool));
		this->fillSpaceArray(spaceArray, this->root);
		if (treeLevel > 1) {
			spaceArray[0]--;
		}
		this->drawTree(spaceArray, levelDrown, this->root);
		delete[] spaceArray;
		delete[] levelDrown;
	}
}

/**
 * Iloœæ elementów menu dla struktury wykorzystywana w klasie Menu do obs³ugi menu za pomoc¹ klawiszy
 *
 * @return Indeks ostatniego elementu w menu
 */
int RBTree::maxMenu()
{
	return RBTree::MAX_MENU;
}

/**
 * Destruktor klasy RBTree usuwaj¹cy wszystkie elementy w strukturze.
 */
RBTree::~RBTree()
{
	this->erase();
}

/**
 * Konstruktor klasy Node uzupe³niaj¹cy automatycznie podstawowe wartoœci
 *
 * @param data wartosc przechowywana przez wêze³
 * @param parent wskaŸnik na rodzica wêz³a
 */
Node::Node(int value, Node* parent, Node *NIL)
{
	this->value = value;
	if (NIL == nullptr) {
		this->left = this->right = this;
		this->color = Color::BLACK;
	}
	else {
		this->left = this->right = NIL;
		this->color = Color::RED;
	}
	this->parent = parent;
}
