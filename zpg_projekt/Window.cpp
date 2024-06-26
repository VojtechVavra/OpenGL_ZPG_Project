#include "Window.hpp"

#include <iostream>


Window::Window(const char* title, int width, int height)
{
	createWindow(title, width, height);
}

void Window::createWindow(const char* title, int width, int height)
{
	//this->m_glfwWindow = std::make_shared<GLFWwindow>(glfwCreateWindow(width, height, "ZPG", NULL, NULL));

	/**
	 * GLFW (Graphics Library Framework)
	 * GLFW je knihovna pro spr�vu oken a vstup�, kter� je prim�rn� ur�ena pro pou�it� s OpenGL. Poskytuje
	 * jednoduch� API pro vytv��en� oken, kontext� OpenGL a zpracov�n� u�ivatelsk�ch vstup�, jako je
	 * kl�vesnice, my� a dal�� za��zen�.
	 */
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		/**
		 * Funkce exit()
		 * Okam�it� ukon�en� programu z jak�hokoliv m�sta v programu
		 * Nepovol� destruktory lok�ln�ch objekt�, co� m��e v�st k �nik�m pam�ti.
		 * Funkce registrovan� pomoc� atexit() budou vol�ny, stejn� jako destruktory statick�ch objekt�.
		 */
		exit(EXIT_FAILURE);
	}

	

	this->m_glfwWindow = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!m_glfwWindow)
	{
		glfwTerminate();
		
		std::cout << "Error in creating window on line: " << __LINE__ << std::endl;
		//throw std::runtime_error("Failed to create GLFW window");
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(getRawPtrWindow());
	glfwSwapInterval(1);

	// we will tell GLFW that it should hide the cursor and capture it. Capturing a cursor means that, once the application has focus, the mouse cursor stays within the center of the window (unless the application loses focus or quits). We can do this with one simple configuration call:
	// After this call, wherever we move the mouse it won't be visible and it should not leave the window.
	glfwSetInputMode(getRawPtrWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);



	// start GLEW extension handler
	// GLEW (OpenGL Extension Wrangler Library)
	/*
	* je knihovna pro spr�vu roz���en� OpenGL. Poskytuje p��stup k
	* modern�m funkc�m OpenGL, kter� nemus� b�t dostupn� ve standardn�
	* implementaci OpenGL na dan�m syst�mu.
	*/
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stdout, "Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
	
	//int width, height;
	//glfwGetFramebufferSize(m_glfwWindow, &width, &height);
	//float ratio = width / (float)height;
	
	// set frame buffer size
	glViewport(0, 0, width, height);
}

/*std::shared_ptr<GLFWwindow> Window::getWindow() const
{
	return m_glfwWindow;
}*/

/*GLFWwindow* Window::getRawPtrWindow() const
{
	return m_glfwWindow.get();
}*/

GLFWwindow* Window::getWindow() const
{
	return m_glfwWindow;
}

GLFWwindow* Window::getRawPtrWindow() const
{
	std::cout << windowSize[0] << " " << windowSize[1] << std::endl;
	return m_glfwWindow;
}

void Window::setSize(int width, int height)
{
	this->windowWidth = width;
	this->windowHeight = height;
}

void Window::printWindowSize()
{
	std::cout << windowSize[0] << " " << windowSize[1] << std::endl;
}


