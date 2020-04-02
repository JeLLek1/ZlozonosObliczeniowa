// ZlozonoscObliczeniowa.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#include "Menu.h"
#include <windows.h>
int main()
{

	Menu* menu = new Menu();
	menu->loop();

	delete menu;
}
