#pragma once
#include "Container.h"
#include "Array.h"
#include "List.h"
#include "Heap.h"
#include "RBTree.h"
#include "Avl.h"
#include <ctime>
#include <chrono>
#include <fstream>
class TimeMeasure : public Container
{
private:
	static const int MAX_MENU = 1;
	std::chrono::high_resolution_clock::time_point start;
	void restart();
	long long returnTime();
	bool test(int size);
	Array *array;
	List* list;
	Heap *heap;
	RBTree *rbTree;
	Avl *avl;
	std::fstream* file;

public:
	static const std::string RESULTFILE;

	TimeMeasure(std::string name);
	bool loadFile();
	void erase();
	virtual void drawMenu(int currentOption);
	virtual void handleInput(int currentOption);
	virtual void display();
	virtual int maxMenu();
	virtual ~TimeMeasure();
};

