#pragma once
#include "Container.h"
struct ListSegment {
	int value;
	ListSegment* prev,* next;
};
class List : public Container
{
private:
	ListSegment* tail,* head;
	ListSegment* search(size_t index);
	void popElement(ListSegment* element);
public:
	static const int MAX_MENU = 5;
	List(std::string name);
	bool popFront();
	bool popBack();
	bool pop(size_t index);
	bool popValue(int value);
	void pushFront(int value);
	void pushBack(int value);
	bool push(int value, size_t index);
	void random(size_t size);
	void erase();
	bool find(int value);
	bool loadFile(std::fstream* file);
	virtual void drawMenu(int currentOption);
	virtual void handleInput(int currentOption);
	virtual void display();
	virtual int maxMenu();
	~List();
};

