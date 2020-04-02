#pragma once
#include <string>
/*
Klasa bazowa dla struktur aby mo¿na by³o je trzymaæ w jednym miejscu w klasie Menu
*/
class Container
{
private:
	std::string structureName;
protected:
	size_t size;
public:
	Container(std::string name);
	std::string name();
	size_t returnSize();
	virtual void drawMenu(int currentOption) = 0;
	virtual void handleInput(int currentOption) = 0;
	virtual void display() = 0;
	virtual int maxMenu() = 0;
	virtual ~Container() = default;
};

