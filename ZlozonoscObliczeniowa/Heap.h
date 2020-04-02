#pragma once
#include "Container.h"
#include <string>
class Heap : public Container
{
private:
	static const size_t HEAP_MAX_SIZE = 100000;
	int* heap;
	static size_t heapLevel(size_t index);
	static size_t powerTwo(size_t index);
	void drawHeap(int* spaceArray, size_t index, int* levelDrawn, int level);
	size_t getParent(size_t children);
	size_t getChildLeft(size_t parent);
	size_t getChildRight(size_t parent);
	void floydConvert();
	void repairDown(size_t index);
	void repairUp(size_t index);
public:
	Heap(std::string name);
	static const int MAX_MENU = 5;
	bool push(int value);
	void erase();
	bool random(size_t size);
	bool find(int value);
	bool popRoot();
	bool popValue(int value);
	bool loadFile(std::fstream* file);
	virtual void drawMenu(int currentOption);
	virtual void handleInput(int currentOption);
	virtual void display();
	virtual int maxMenu();
	~Heap();
};

