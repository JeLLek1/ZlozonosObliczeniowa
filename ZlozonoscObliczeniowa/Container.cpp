#include "Container.h"

//Konstruktor klasy bazowej z name-nazwa struktury w menu i this->size-wiekoœæ struktury
Container::Container(std::string name)
{
	this->structureName = name;
	this->size = 0;
}
//zwraca nazwê struktruy
std::string Container::name()
{
	return this->structureName;
}
//zwraca wielkoœæ sktruktury
size_t Container::returnSize()
{
	return this->size;
}
