/*
*	ZPG - Zaklady programovani grafiky 2020
*   author: Vooja "Vojtìch Vávra"
*	https://open.gl/transformations
*/

#include <cstdlib>
#include "Application.hpp"

int main()
{
	Application* app = nullptr;
	app = app->getInstance();
	app->setWindowTitle("Zabijak je jeskynni troll");
	//app->setWindowSize(1024, 768);	// [800,600]
	app->init();

	return(EXIT_SUCCESS);
}
