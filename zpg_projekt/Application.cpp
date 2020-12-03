#include <cctype>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include <GL/glew.h>		// must be always included first!
#include <GLFW/glfw3.h>		// this will include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application.hpp"
#include "Callback.hpp"
#include "Scene.hpp"
#include "Renderer.hpp"


Application* Application::single = nullptr;

Application* Application::getInstance()
{
	if (Application::single == nullptr) {
		Application::single = new Application();
	}
	return Application::single;
}

Application::Application()
{
	windowWidth = windowHeight = 0;
	windSize[0] = &this->windowWidth;
	windSize[1] = &this->windowHeight;
}

void Application::init()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		exit(EXIT_FAILURE);
	}

	if (windowWidth <= 0 || windowHeight <= 0) {
		CreateWindow();
	}
	else {
		CreateWindow(windowWidth, windowHeight);
	}
	
	// after window creatin can be set callbacks
	callbackFunctions();

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stdout, "Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	PrintInfo();
	int width, height;
	glfwGetFramebufferSize(window.get(), &width, &height);
	//float ratio = width / (float)height;
	glViewport(0, 0, width, height);

	draw();
}

void Application::setWindowSize(int width, int height)
{
	this->windowWidth = width;
	this->windowHeight = height;
}

void Application::CreateWindow(int width, int height)
{
	this->window = std::shared_ptr<GLFWwindow>(glfwCreateWindow(width, height, "ZPG", NULL, NULL));

	if (!window.get())
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
		std::cout << "Error in creating window on line: " << __LINE__ << std::endl;
	}

	glfwMakeContextCurrent(window.get());
	glfwSwapInterval(1);
	
	// we will tell GLFW that it should hide the cursor and capture it. Capturing a cursor means that, once the application has focus, the mouse cursor stays within the center of the window (unless the application loses focus or quits). We can do this with one simple configuration call:
	// After this call, wherever we move the mouse it won't be visible and it should not leave the window.
	glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::PrintInfo()
{
	// get version info
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
	printf("Vendor %s\n", glGetString(GL_VENDOR));
	printf("Renderer %s\n", glGetString(GL_RENDERER));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	printf("Using GLFW %i.%i.%i\n", major, minor, revision);
}

void Application::callbackFunctions() {
	Callback::setWindow(window);

	glfwSetErrorCallback([](int error, const char* description) { Callback::error_callback(error, description); });
	//glfwSetCursorPosCallback(this->GetWindow(), [](GLFWwindow* window, double mouseXPos, double mouseYPos) -> void { Callback::cursor_pos_callback(window, mouseXPos, mouseYPos); });
	glfwSetCursorPosCallback(window.get(), Callback::cursor_pos_callback);
	glfwSetKeyCallback(this->GetWindow().get(), [](GLFWwindow* window, int key, int scancode, int action, int mods) { Callback::key_callback(window, key, scancode, action, mods); });
	glfwSetMouseButtonCallback(this->GetWindow().get(), [](GLFWwindow* window, int button, int action, int mode) { Callback::button_callback(window, button, action, mode); });
	//glfwSetWindowFocusCallback(this->GetWindow().get(), [](GLFWwindow* window, int focused) { Callback::window_focus_callback(window, focused); });
	glfwSetWindowFocusCallback(window.get(), Callback::window_focus_callback);
	glfwSetWindowIconifyCallback(this->GetWindow().get(), [](GLFWwindow* window, int iconified) { Callback::window_iconify_callback(window, iconified); });

	//int* windSize[2]{ &this->windowWidth, &this->windowHeight };
	//glfwSetWindowSizeCallback(window.get(), [&](GLFWwindow* window, int width, int height) { Callback::window_size_callback(window, width, height, &this->windowWidth, &this->windowHeight); });
	//glfwSetWindowUserPointer(window.get(), Callback::window_size_callback, &this->windowWidth, &this->windowHeight);
	
	//void (*my_fptr_setWindowSizeCallback)(int& winWidth, int& winHeight) = Callback::window_size_modify_callback;
	//my_fptr_setWindowSizeCallback
	glfwSetWindowUserPointer(window.get(), &windSize);
	//glfwSetWindowUserPointer
	//glfwSetWindowSizeCallback(window.get(), Callback::window_size_callback);

	glfwSetWindowSizeCallback(window.get(), Callback::window_size_callback);

	glfwSetScrollCallback(window.get(), Callback::scroll_callback);
}

void Application::draw()
{
	std::shared_ptr<Scene> scene = std::shared_ptr<Scene>(new Scene());
	Renderer renderer = Renderer();
	Callback::scene = scene;

	renderer.renderScene(scene, window);
}

Application::~Application()
{
	printf("Destructor ~Application\n");
}
