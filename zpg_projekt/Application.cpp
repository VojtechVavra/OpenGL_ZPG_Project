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
	m_window = std::make_shared<Window>("ZPG - OpenGL engine");
	m_window->VSyncEnable(false);

	srand(time(nullptr));
}

void Application::init()
{
	printInfo();
	// after window creation we can set callbacks
	callbackFunctions();
	draw();
}

void Application::setWindowSize(int width, int height) const
{
	this->m_window->setSize(width, height);
}

void Application::setWindowTitle(const std::string& newTitle) const
{
	this->m_window->setWindowTitle(newTitle);
}

void Application::printInfo()
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

void Application::callbackFunctions() const
{
	Callback::setWindow(m_window);

	glfwSetErrorCallback([](int error, const char* description) { Callback::error_callback(error, description); });
	glfwSetCursorPosCallback(m_window->getGLFWwindow(), Callback::cursor_pos_callback);
	glfwSetKeyCallback(m_window->getGLFWwindow(), [](GLFWwindow* window, int key, int scancode, int action, int mods) { 
		Callback::key_callback(window, key, scancode, action, mods);
		});
	glfwSetMouseButtonCallback(m_window->getGLFWwindow(), [](GLFWwindow* window, int button, int action, int mode) { 
		Callback::button_callback(window, button, action, mode);
		});
	//glfwSetWindowFocusCallback(this->GetWindow().get(), [](GLFWwindow* window, int focused) { Callback::window_focus_callback(window, focused); });
	glfwSetWindowFocusCallback(m_window->getGLFWwindow(), Callback::window_focus_callback);
	glfwSetWindowIconifyCallback(m_window->getGLFWwindow(), [](GLFWwindow* window, int iconified) { 
		Callback::window_iconify_callback(window, iconified); 
		});
	glfwSetWindowUserPointer(m_window->getGLFWwindow(), m_window->getWindowSizePtr()/*&windSize*/);
	glfwSetWindowSizeCallback(m_window->getGLFWwindow(), Callback::window_size_callback);
	
	// Nastavení callbacku pro zmìnu velikosti framebufferu
	glfwSetFramebufferSizeCallback(m_window->getGLFWwindow(), Callback::framebuffer_size_callback);

	glfwSetScrollCallback(m_window->getGLFWwindow(), Callback::scroll_callback);
}

void Application::draw()
{
	m_scene = std::make_shared<Scene>();
	Callback::scene = m_scene;
	m_renderer.renderScene(m_scene, m_window);
}

Application::~Application()
{
	printf("Destructor ~Application\n");
}
