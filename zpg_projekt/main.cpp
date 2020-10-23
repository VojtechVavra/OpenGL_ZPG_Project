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

	exit(EXIT_SUCCESS);
}


//  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL); // rezani
//				id pointeru, kolik hodnot si ma uriznout, datovy typ, 