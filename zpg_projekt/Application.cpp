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


Application* Application::m_single = nullptr;

Application* Application::getInstance()
{
	if (Application::m_single == nullptr) {
		Application::m_single = new Application();
	}
	return Application::m_single;
}

Application::Application()
{
	m_window = std::make_shared<Window>("ZPGSS");
	//windowWidth = windowHeight = 0;
	//windSize[0] = &this->windowWidth;
	//windSize[1] = &this->windowHeight;
	srand(time(nullptr));
}

void Application::init()
{
	//if (!glfwInit()) {
	//	fprintf(stderr, "ERROR: could not start GLFW3\n");
		/** 
		 * Funkce exit()
		 * Okamžité ukonèení programu z jakéhokoliv místa v programu
		 * Nepovolá destruktory lokálních objektù, což mùže vést k únikùm pamìti.
		 * Funkce registrované pomocí atexit() budou volány, stejnì jako destruktory statických objektù.
		 */
	//	exit(EXIT_FAILURE);
	//}

	// Make the window's context current
	//glfwMakeContextCurrent(m_window->getRawPtrWindow());
	//glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	/*if (windowWidth <= 0 || windowHeight <= 0) {
		CreateWindow();
	}
	else {
		CreateWindow(windowWidth, windowHeight);
	}*/
	
	PrintInfo();

	// after window creation can be set callbacks
	callbackFunctions();
	draw();
}

void Application::setWindowSize(int width, int height) const
{
	this->m_window->setSize(width, height);
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
	//std::shared_ptr<GLFWwindow*> a = std::make_shared<GLFWwindow*>(m_window->getWindow());
	//auto sharedWindow = std::shared_ptr<GLFWwindow>(m_window->getWindow(), glfwDestroyWindow);
	Callback::setWindow(m_window);

	glfwSetErrorCallback([](int error, const char* description) { Callback::error_callback(error, description); });
	//glfwSetCursorPosCallback(this->GetWindow(), [](GLFWwindow* window, double mouseXPos, double mouseYPos) -> void { Callback::cursor_pos_callback(window, mouseXPos, mouseYPos); });
	glfwSetCursorPosCallback(m_window->getRawPtrWindow(), Callback::cursor_pos_callback);
	glfwSetKeyCallback(m_window->getRawPtrWindow(), [](GLFWwindow* window, int key, int scancode, int action, int mods) { Callback::key_callback(window, key, scancode, action, mods); });
	glfwSetMouseButtonCallback(m_window->getRawPtrWindow(), [](GLFWwindow* window, int button, int action, int mode) { Callback::button_callback(window, button, action, mode); });
	//glfwSetWindowFocusCallback(this->GetWindow().get(), [](GLFWwindow* window, int focused) { Callback::window_focus_callback(window, focused); });
	glfwSetWindowFocusCallback(m_window->getRawPtrWindow(), Callback::window_focus_callback);
	glfwSetWindowIconifyCallback(m_window->getRawPtrWindow(), [](GLFWwindow* window, int iconified) { Callback::window_iconify_callback(window, iconified); });

	//int* windSize[2]{ &this->windowWidth, &this->windowHeight };
	//glfwSetWindowSizeCallback(window.get(), [&](GLFWwindow* window, int width, int height) { Callback::window_size_callback(window, width, height, &this->windowWidth, &this->windowHeight); });
	//glfwSetWindowUserPointer(window.get(), Callback::window_size_callback, &this->windowWidth, &this->windowHeight);
	
	//void (*my_fptr_setWindowSizeCallback)(int& winWidth, int& winHeight) = Callback::window_size_modify_callback;
	//my_fptr_setWindowSizeCallback
	glfwSetWindowUserPointer(m_window->getRawPtrWindow(), m_window->getWindowSizePtr()/*&windSize*/);
	//glfwSetWindowUserPointer
	//glfwSetWindowSizeCallback(window.get(), Callback::window_size_callback);

	glfwSetWindowSizeCallback(m_window->getRawPtrWindow(), Callback::window_size_callback);
	// Nastavení callbacku pro zmìnu velikosti framebufferu
	glfwSetFramebufferSizeCallback(m_window->getRawPtrWindow(), Callback::framebuffer_size_callback);

	glfwSetScrollCallback(m_window->getRawPtrWindow(), Callback::scroll_callback);
}

void Application::draw()
{
	std::shared_ptr<Scene> scene = std::shared_ptr<Scene>(new Scene());
	Renderer renderer = Renderer();
	Callback::scene = scene;

	renderer.renderScene(scene, m_window->getWindow());
}

Application::~Application()
{
	printf("Destructor ~Application\n");
}
