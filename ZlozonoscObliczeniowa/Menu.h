#pragma once
#include <string>
#include "Heap.h"
#include "List.h"
#include "Array.h"
#include "RBTree.h"
#include "Avl.h"
#include "TimeMeasure.h"

class Menu
{
private:
	static const size_t STRUCTURES_COUND;
	static const std::string INFORMATION;
	int option;
	size_t structure;
	char input = 0;
	//Struktury
	Container** conatainers;

public:
	static const int MAX_RAND = 1000;
	static int numberLength(int number);
	Menu();
	static int cinInt();
	static size_t cinSize_t();
	void loop();
	void drawMenu();
	void handleInput();
	~Menu();
};

