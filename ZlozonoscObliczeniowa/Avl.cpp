#include "Avl.h"
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
void Avl::ereaseFrom(NodeA* node)
{
	if (node == nullptr) return;

	this->ereaseFrom(node->left);
	this->ereaseFrom(node->right);
	delete node;
}

/*
 * Wyznaczanie wielkoœci drzewa
 *
 * @return wielkosc drzewa
 */
size_t Avl::treeLevel(NodeA* fromNode, size_t level = 0)
{
	if (fromNode == nullptr) return level;

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
void Avl::fillSpaceArray(int* spaceArray, NodeA* node, size_t level = 0)
{
	if (node->left == nullptr && node->right == nullptr) return;
	int space = Menu::numberLength(node->value)+ Menu::numberLength(node->bf)+1;
	if (space > spaceArray[level]) spaceArray[level] = space;

	if (node->left != nullptr) this->fillSpaceArray(spaceArray, node->left, level + 1);
	if (node->right != nullptr) this->fillSpaceArray(spaceArray, node->right, level + 1);
}

/*
 * Rysowanie elementów drzewa inorder
 * Kropka na liœciach wyœwietlana jest aby po przesuniêciu okna nie by³o czerwonych pasków
 *
 * Funckja jest rekurencyjna na podstawie inorder, jednak dla ³adniejszego uk³adu u¿ywa spaceArray, która trzyma d³ugoœci najd³u¿szych liczb z poziomu oraz levelDrown dla sprawdzenia który poziom ma mieæ przed³u¿enie znakiem |
 *
 * @param spaceArray tablica z d³ugoœci¹ spacji na poziom
 * @param levelDrown czy dany poziom zosta³ rozpoczêty, czy nie (dla rysowania znaku |)
 * @param node obecnie sprawdzany wêze³
 * @param level obecnie sprawdzany poziom
 * @param right czy obecny wêze³ jest prawy czy lewy
 */
void Avl::drawTree(int* spaceArray, bool* levelDrown, NodeA* node, int level = 0, bool right = true)
{
	if (node == nullptr) return;
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
		if (i > 0 && levelDrown[i - 1]) {
			line += "|";
		}
		else {
			line += " ";
		}
		for (int j = 0; j < spaceArray[i]; j++) {
			line += " ";
		}
	}
	if (level > 0) {
		if (right) {
			line += static_cast<char>(218);
		}
		else {
			line += static_cast<char>(192);
		}
	}
	//wypisanie klucza wraz z jego wspó³czynnikiem równowagi
	line+= std::to_string(node->value)+":"+std::to_string(node->bf);
	if (node->left != nullptr || node->right != nullptr) {
		for (int i = 0; i < spaceArray[level] - Menu::numberLength(node->value) - Menu::numberLength(node->bf) -1; i++) {
			line+= "-";
		}
		if (node->left != nullptr && node->right != nullptr) {
			line+= static_cast<char>(180);
		}
		else if (node->left != nullptr) {
			line += static_cast<char>(191);
		}
		else {
			line += static_cast<char>(217);
		}
	}
	line += "\n";
	std::cout << line;
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
 * Rotacja pojedyñcza w lewo dla podanego wêz³a
 *
 * @param node wêze³ dla którego ma zostaæ wykonana rotacja
*/
void Avl::rotateRR(NodeA* node)
{
	if (node->parent == nullptr) {
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
	if (node->right != nullptr) {
		node->right->parent = node;
	}
	node->parent->left = node;
	//zmiana wed³ug zasad wspó³czynnika równowagi
	if (node->parent->bf == -1) {
		node->bf = node->parent->bf = 0;
	}
	else {
		node->bf = -1;
		node->parent->bf = 1;
	}
}

/**
 * Rotacja podwójna
 *    node
 *     \
 *      B
 *     /
 *    C
 * rotacja ta jest z³o¿eniem dwóch w jedn¹ krótsz¹ (LL i RR)
 *
 * @param node wêze³ dla którego ma zostaæ wykonana rotacja
*/
void Avl::rotateRL(NodeA* node)
{
	NodeA* B = node->right;
	NodeA* C = B->left;
	if (node->parent == nullptr) {
		this->root = C;
	}
	else {
		if (node == node->parent->left) {
			node->parent->left = C;
		}
		else {
			node->parent->right = C;
		}
	}
	C->parent = node->parent;
	B->left = C->right;
	node->right = C->left;

	if (B->left != nullptr) B->left->parent = B;
	if (node->right != nullptr) node->right->parent = node;

	C->left = node;
	C->right = B;
	node->parent = B->parent = C;

	//zmiana wed³ug zasad wspó³czynnika równowagi
	if (C->bf == -1) {
		node->bf = 1;
	}
	else {
		node->bf = 0;
	}

	if (C->bf == 1) {
		B->bf = -1;
	}
	else {
		B->bf = 0;
	}
}

/**
 * Rotacja podwójna
 *    node
 *   /  
 *  B    
 *   \  
 *    C
 *
 * Rotacja ta jest lustrzanym odbiciem RL
 *
 * @param node wêze³ dla którego ma zostaæ wykonana rotacja
*/
void Avl::rotateLR(NodeA* node)
{
	NodeA* B = node->left;
	NodeA* C = B->right;
	if (node->parent == nullptr) {
		this->root = C;
	}
	else {
		if (node == node->parent->left) {
			node->parent->left = C;
		}
		else {
			node->parent->right = C;
		}
	}
	C->parent = node->parent;

	B->right = C->left;
	node->left = C->right;

	if (B->right != nullptr) B->right->parent = B;
	if (node->left != nullptr) node->left->parent = node;

	C->left = B;
	C->right = node;
	node->parent = B->parent = C;

	//zmiana wed³ug zasad wspó³czynnika równowagi
	if (C->bf == 1) {
		node->bf = -1;
	}
	else {
		node->bf = 0;
	}

	if (C->bf == -1) {
		B->bf = 1;
	}
	else {
		B->bf = 0;
	}
}

/**
 * Rotacja pojedyñcza w prawo dla podanego wêz³a
 *
 * @param node wêze³ dla którego ma zostaæ wykonana rotacja
*/
void Avl::rotateLL(NodeA* node)
{
	if (node->parent == nullptr) {
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
	if (node->left != nullptr) {
		node->left->parent = node;
	}
	node->parent->right = node;

	//zmiana wed³ug zasad wspó³czynnika równowagi
	if (node->parent->bf == 1) {
		node->bf = node->parent->bf = 0;
	}
	else {
		node->bf = 1;
		node->parent->bf = -1;
	}
}

/*
 * Wyszukuje poprzednika wybranego wêz³a
 *
 * @param node wêze³, dla którego poprzednik ma zostaæ znaleziony
 *
 * @return wêze³, który jest nastêpnikiem
 */
NodeA* Avl::findPredecessor(NodeA* node)
{
	if (node != nullptr) {
		if (node->left != nullptr) {
			NodeA* tmp;
			tmp = node->right;
			while (tmp->right != nullptr) {
				tmp = tmp->right;
			}
			return tmp;
		}
		while (node->parent != nullptr && node->parent->right != node) {
			node = node->parent;
		}
		return node->parent;
	}
	return nullptr;
}

/*
 * Konstruktor klasy RBTree
 *
 * @param name Nazwa struktury wykorzystywana podczas wyœwietlania w menu
 */
Avl::Avl(std::string name) : Container(name)
{
	this->root = nullptr;
}

/**
 * Wstawianie do struktury wybranej wartoœci oraz naprawa
 *
 * @param value wartoœæ do wstawienia
 */
void Avl::push(int value)
{

	NodeA* temp;
	//je¿eli nie ma elementów wstaw jako korzeñ
	if (this->root == nullptr) {
		this->root = temp = new NodeA(value, nullptr);
	}
	else {
		//je¿eli s¹ elementy nale¿y wyszukaæ miejsca dla nowego elementu jak w BST
		NodeA* tmp = this->root;
		NodeA* parent = nullptr;
		while (tmp != nullptr) {
			parent = tmp;
			if (value < parent->value) {
				tmp = tmp->left;
			}
			else {
				tmp = tmp->right;
			}
		}

		if (value < parent->value) {
			parent->left = temp = new NodeA(value, parent);
		}
		else {
			parent->right = temp = new NodeA(value, parent);
		}
	
		NodeA* temp1 = temp->parent;
		NodeA *temp2 = temp1->parent;

		//Nastêpnie dokonywana jest naprawa wed³ug zasad
		if (temp1->bf != 0) {
			temp1->bf = 0; //je¿eli rodzic nie ma wspó³czynnika równowagi równego 0 to znaczy ¿e ma -1 lub 1. Po wstawieniu elementu siê wyzeruje
		}
		else {
			if (temp1->left == temp) { //je¿eli jedna z ga³êzi rodzica sta³a siê ciê¿sza to nale¿y zmieniæ wspó³czynnik
				temp1->bf = 1;
			}
			else {
				temp1->bf = -1;
			}
			while (temp2 !=nullptr && temp2->bf==0) //szukamy w górê wêz³a który straci³ równowagê
			{
				if (temp2->bf == 0) {
					if (temp2->left == temp1) {
						temp2->bf = 1;
					}
					else {
						temp2->bf = -1;
					}
					temp1 = temp2;
					temp2 = temp2->parent;
				}
			}
			if (temp2 != nullptr) { //je¿eli taki wêze³ zosta³ znaleziony to w zale¿noœci od strony która jest przeci¹¿ona wykonujemy rotacje
				if (temp2->bf == 1) {
					if (temp2->right == temp1) {
						temp2->bf = 0;
					}
					else if (temp1->bf == -1) {
						this->rotateLR(temp2);
					}
					else {
						rotateLL(temp2);
					}
				}
				else {
					if (temp2->left == temp1) {
						temp2->bf = 0;
					}else if(temp1->bf == 1){
						this->rotateRL(temp2);
					}
					else {
						this->rotateRR(temp2);
					}
				}
			}
		}
	}


	this->size++;


}

/**
 * Usuwanie wszystkich elementów drzewa
 */
void Avl::erase()
{
	this->ereaseFrom(this->root);
	this->root = nullptr;
	this->size = 0;
}

/**
 * Generuje losowe klucze i wstawia je do struktury
 *
 * @param size wielkoœæ struktury
 */
void Avl::random(size_t size)
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
bool Avl::find(int value)
{
	NodeA* tmp = this->root;
	NodeA* parent = nullptr;
	while (tmp != nullptr) {
		parent = tmp;
		if (value < parent->value) {
			tmp = tmp->left;
		}
		else if (value > parent->value) {
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
bool Avl::popValue(int value)
{
	//Wyszukiwanie klucza do usuniêcia
	NodeA* toRemove = this->root;
	while (toRemove != nullptr && toRemove->value != value) {
		if (value < toRemove->value) {
			toRemove = toRemove->left;
		}
		else {
			toRemove = toRemove->right;
		}
	}
	//wywo³anie funkcji usuwaj¹cej element
	NodeA *deletion = popNode(toRemove);
	if (deletion != nullptr) {
		delete deletion;
		return true;
	}
	return false;
}

/**
 * Usuwanie podanego elementu
 *
 * @param element do usuniêcia
 *
 * @return usuwany element
 */
NodeA* Avl::popNode(NodeA* node)
{
	if (node != nullptr) {
		bool nesting;
		NodeA* temp;
		//Przypdaki które mog¹ wyst¹piæ podczas usuwania elementu
		if (node->left != nullptr && node->right != nullptr) { //je¿eli usuwany wêze³ ma obu synów to szukamy nastêpnika i wstawiamy za niego
			temp = this->popNode(this->findPredecessor(node));
			nesting = false;
		}
		else {
			if (node->left != nullptr) { //przypadek gdy wêze³ posiada mniej ni¿ dwóch synów
				temp = node->left;
				node->left;
				node->left = nullptr;
			}
			else {
				temp = node->right;
				node->right = nullptr;
			}
			node->bf = 0; //je¿eli taki syn istnieje to bêdzie mia³ wspó³czynnik zagnie¿d¿enia 0
			nesting = true; //nale¿y sprawdziæ w górê
		}

		if (temp != nullptr) { //przypisanie wszystkich danych z wêz³a do usuniêcia na wêze³ do wstawienia
			temp->bf = node->bf;
			temp->parent = node->parent;
			temp->left = node->left;
			if (temp->left != nullptr) {
				temp->left->parent = temp;
			}
			temp->right = node->right;
			if (temp->right != nullptr) {
				temp->right->parent = temp;
			}
		}

		if (node->parent != nullptr) { //je¿eli usuwany wêze³ nie mia³ rodzica to jest korzeniem
			if (node->parent->right == node) {
				node->parent->right = temp;
			}
			else {
				node->parent->left = temp;
			}
		}
		else {
			this->root = temp;
		}
		//sprawdzenie zagnie¿d¿eñ
		if (nesting) {
			NodeA* temp1 = temp;
			temp = node->parent;
			while (temp != nullptr) {
				//dla kolejnych wêz³ów w górê 
				if (temp->bf == 0) { //w takim wypadku wstawiony element zmodyfikowa³ jedynie wspó³czynnik wywa¿enia ale nie zaburzy³ struktury
					//zamieniamy wspó³czynnik wywa¿enia w zale¿noœci od strony
					if (temp->left == temp1) {
						temp->bf = -1;
					}
					else {
						temp->bf = 1;
					}
					temp = nullptr;
				}
				else {
					//zabrany element sprawi³ ¿e poddrzewo ma wspó³czynnik wywa¿enia 0, jednak mog³o zaburzyæ strukturê wy¿ej
					if ((temp->bf == 1 && temp->left == temp1) || (temp->bf == -1 && temp->right == temp1)) {
						temp->bf = 0;
						temp1 = temp;
						temp = temp->parent;
					}
					else {
						NodeA* tmp;
						if (temp->left == temp1) {
							tmp = temp->right;
						}
						else {
							tmp = temp->left;
						}
						if (tmp->bf == 0) {//wspó³czynnik dziecka w ciê¿szym poddrzewie jest równy 0
							if (temp->bf == 1) {
								this->rotateLL(temp);
							}
							else {
								this->rotateRR(temp);
							}
							temp = nullptr; //rotacja ta naprawia ca³e drzewo wiêc algorytm siê koñczy
						}
						else if (temp->bf == tmp->bf) {//wspó³czynnik temp jest równe jego dziecku
							if (temp->bf == 1) {
								this->rotateLL(temp);
							}
							else {
								this->rotateRR(temp);
							}
							temp1 = tmp;
							temp = tmp->parent;//rotacja ta nie naprawi ca³ej struktury wiêc pêtla idzie dalej
						}
						else {//gdy wspó³czynniki temp i temp1 s¹ przeciwne
							if (temp->bf == 1) {
								this->rotateLR(temp);
							}
							else {
								this->rotateRL(temp);
							}
							temp1 = temp->parent;
							temp = temp1->parent;//rotacja ta nie naprawia ca³ej struktury wiêc pêtla jest kontynuowana
						}
					}
				}
			}
		}
		return node;
	}
	return nullptr;
}

/**
 * £adowanie danych z pliku
 *
 * @param currentOption Obecnie wyró¿niona opcja w menu
 */
bool Avl::loadFile(std::fstream* file)
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
void Avl::drawMenu(int currentOption)
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
void Avl::handleInput(int currentOption)
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
		this->display();
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
void Avl::display()
{
	if (this->root != nullptr) {
		size_t treeLevel = this->treeLevel(this->root);
		int* spaceArray = new int[treeLevel - 1];
		bool* levelDrown = new bool[treeLevel];
		memset(spaceArray, 0, (treeLevel - 1) * sizeof(int));
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
int Avl::maxMenu()
{
	return RBTree::MAX_MENU;
}

/**
 * Destruktor klasy RBTree usuwaj¹cy wszystkie elementy w strukturze.
 */
Avl::~Avl()
{
	this->erase();
}

/**
 * Konstruktor klasy Node uzupe³niaj¹cy automatycznie podstawowe wartoœci
 *
 * @param data wartosc przechowywana przez wêze³
 * @param parent wskaŸnik na rodzica wêz³a
 */
NodeA::NodeA(int value, NodeA* parent)
{
	this->value = value;
	this->right = this->left = nullptr;
	this->parent = parent;
	this->bf = 0;
}
#include "Avl.h"
