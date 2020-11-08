#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>		// tolower(int)
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>	// glm::lookAt, glm::radians
#include "Callback.hpp"


std::shared_ptr<Camera> Callback::camera = nullptr;

float Callback::lastX = 800 / 2.0f;
float Callback::lastY = 600 / 2.0f;
bool Callback::firstMouse = true;

void Callback::setCamera(std::shared_ptr<Camera> camera) {
	Callback::camera = camera;
}

/*Callback::Callback(Camera* camera) {
	Callback::camera = camera;
}*/
/*Callback::Callback(GLFWwindow* window) {
	this->window = window;

	glfwSetErrorCallback([](int error, const char* description) { Callback::error_callback(error, description); });

	glfwSetCursorPosCallback(this->GetWindow(), [](GLFWwindow* window, double mouseXPos, double mouseYPos) -> void { Callback::cursor_pos_callback(window, mouseXPos, mouseYPos); });

	glfwSetKeyCallback(this->GetWindow(), [](GLFWwindow* window, int key, int scancode, int action, int mods) { Callback::key_callback(window, key, scancode, action, mods); } );

	glfwSetMouseButtonCallback(this->GetWindow(), [](GLFWwindow* window, int button, int action, int mode) { Callback::button_callback(window, button, action, mode);});

	glfwSetWindowFocusCallback(this->GetWindow(), [](GLFWwindow* window, int focused) { Callback::window_focus_callback(window, focused); });

	glfwSetWindowIconifyCallback(this->GetWindow(), [](GLFWwindow* window, int iconified) { Callback::window_iconify_callback(window, iconified); });

	//glfwSetWindowSizeCallback(this->GetWindow(), [](GLFWwindow* window, int width, int height) { Callback::window_size_callback(window, width, height); });
	glfwSetWindowSizeCallback(this->GetWindow(), Callback::window_size_callback);
}*/
//Callback::Callback(){}
/*GLFWwindow* Callback::GetWindow() {
	return this->window;
}*/

// callback functions
void Callback::error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void Callback::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	printf("cursor_pos_callback ax:%d, ay:%d\n", (int)xpos, (int)ypos);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	Callback::camera->processMouseMovement(xoffset, yoffset);
}

void Callback::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
	//if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	//	glfwSetWindowShouldClose(window, true);


	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_W:
			Callback::camera->setMoveDir(FORWARD);
			break;
		case GLFW_KEY_S:
			Callback::camera->setMoveDir(BACKWARD);
			break;
		case GLFW_KEY_A:
			Callback::camera->setMoveDir(STRAFE_LEFT);
			break;
		case GLFW_KEY_D:
			Callback::camera->setMoveDir(STRAFE_RIGHT);
			break;
		default:
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_W:
			Callback::camera->unSetMoveDir(FORWARD);
			break;
		case GLFW_KEY_S:
			Callback::camera->unSetMoveDir(BACKWARD);
			break;
		case GLFW_KEY_A:
			Callback::camera->unSetMoveDir(STRAFE_LEFT);
			break;
		case GLFW_KEY_D:
			Callback::camera->unSetMoveDir(STRAFE_RIGHT);
			break;
		default:
			break;
		}
	}
}

void Callback::window_focus_callback(GLFWwindow* window, int focused) {
	printf("window_focus_callback \n");
}

void Callback::window_iconify_callback(GLFWwindow* window, int iconified) {
	printf("window_iconify_callback \n");
}

// original one
void Callback::window_size_callback(GLFWwindow* window, int width, int height) {
	printf("resize %d, %d \n", width, height);
	glViewport(0, 0, width, height);

	int* windSize = (int*)glfwGetWindowUserPointer(window);
	windSize[0] = width;
	windSize[1] = height;

	Callback::camera->setPerspectiveCamera(width, height);

	//printf("windWidth: %d\n", my_data[0]);
	//printf("windHeight: %d\n", my_data[1]);
}

// new
/*void Callback::window_size_modify_callback(int& windowWidth, int& windowHeight) {
	//printf("resize %d, %d \n", width, height);
	//glViewport(0, 0, width, height);
	printf("My window resize function\n");
	//Callback::camera->
}*/

void Callback::cursor_callback(GLFWwindow* window, double x, double y) {
	printf("cursor_callback \n");
}

void Callback::button_callback(GLFWwindow* window, int button, int action, int mode) {
	if (action == GLFW_PRESS) {
		printf("button_callback [%d,%d,%d]\n", button, action, mode);
	}

	printf("button_callback up? [%d,%d,%d]\n", button, action, mode);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void Callback::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Callback::camera->processMouseScroll(yoffset);
}