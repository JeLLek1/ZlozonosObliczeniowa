#pragma once
#include <fstream>
#include "Container.h"
class Array : public Container
{
public:
	int* array;
	static const int MAX_MENU = 5;
	Array(std::string name);
	bool popFront();
	bool popBack();
	bool pop(size_t index);
	void pushFront(int value);
	void pushBack(int value);
	bool push(int value, size_t index);
	bool find(int value);
	void random(size_t size);
	void erase();
	bool loadFile(std::fstream *file);
	virtual void drawMenu(int currentOption);
	virtual void handleInput(int currentOption);
	virtual void display();
	virtual int maxMenu();
	~Array();
};

