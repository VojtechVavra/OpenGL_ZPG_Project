/*
*	ZPG - Zaklady programovani grafiky 2020
*   VŠB - Vysoká Škola Báòská - Technická Univerzita Ostrava
*   https://edison.sso.vsb.cz/cz.vsb.edison.edu.study.prepare.web/SubjectVersion.faces?version=460-2021/02&subjectBlockAssignmentId=299169&studyFormId=1&studyPlanId=19954&locale=cs&back=true   
*   Author: Vooja "Vojtìch Vávra"
*   This project passed in university, 
*   now I continue on this project by expanding it and refactoring
*	https://open.gl/transformations
*/

#include <cstdlib>
#include "Application.hpp"

int main()
{
	Application* app = nullptr;
	app = app->getInstance();
	app->setWindowTitle("ZPG - OpenGL engine");
	//app->setWindowSize(1024, 768);	// [800,600]
	app->init();

	return(EXIT_SUCCESS);
}
