#include "Window.hpp"

#include <iostream>


Window::Window(const char* title, int width, int height)
{
	createWindow(title, width, height);
}

void Window::createWindow(const char* title, int width, int height)
{
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
		std::cout << "Error in creating window on line: " << __LINE__ << std::endl;
		glfwTerminate();
		//throw std::runtime_error("Failed to create GLFW window");
		exit(EXIT_FAILURE);
	}

	// Make the window's context current
	glfwMakeContextCurrent(this->m_glfwWindow);
	
	VSyncEnable(true);

	// we will tell GLFW that it should hide the cursor and capture it. Capturing a cursor means that, once the application has focus, the mouse cursor stays within the center of the window (unless the application loses focus or quits). We can do this with one simple configuration call:
	// After this call, wherever we move the mouse it won't be visible and it should not leave the window.
	glfwSetInputMode(this->m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* Start GLEW extension handler
	*  -----------------------------------------------------------------
	*  GLEW (OpenGL Extension Wrangler Library)
	*  je knihovna pro spr�vu roz���en� OpenGL. Poskytuje p��stup k
	*  modern�m funkc�m OpenGL, kter� nemus� b�t dostupn� ve standardn�
	*  implementaci OpenGL na dan�m syst�mu.
	*/
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		// glewInit failed, something is seriously wrong.
		fprintf(stdout, "Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
	
	// set frame buffer size
	glViewport(0, 0, width, height);
}

/*
*  Enable or disable V-Sync
*  @param enable - true (1), disable - false (0)
*/
void Window::VSyncEnable(bool enabled)
{
	// Disable 0, Enable 1 
	glfwSwapInterval(enabled);
}

/*
*   Get raw ptr to glfwWindow
*/
GLFWwindow* Window::getGLFWwindow() const
{
	return m_glfwWindow;
}

void Window::setSize(int width, int height)
{
	this->windowWidth = width;
	this->windowHeight = height;
}

void Window::setWindowTitle(const std::string& newTitle)
{
	// Change the window title
	glfwSetWindowTitle(m_glfwWindow, newTitle.c_str());
}

void Window::printWindowSize()
{
	std::cout << windowSize[0] << " " << windowSize[1] << std::endl;
}
