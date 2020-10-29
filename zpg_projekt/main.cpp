/*
*	ZPG - Zaklady programovani grafiky 2020
*	https://open.gl/transformations
*/

#include <cstdlib>
#include "Application.hpp"

int main()
{
	Application* app = nullptr;
	app = app->getInstance();
	app->setWindowSize(800, 600);
	app->init();

	return(EXIT_SUCCESS);
	//exit(EXIT_SUCCESS);
}
