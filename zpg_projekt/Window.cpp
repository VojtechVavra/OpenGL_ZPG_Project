#include "Window.hpp"

#include <iostream>


Window::Window(const char* title, int width, int height)
{
	createWindow(title, width, height);
	VSyncEnable(false);
}

void Window::createWindow(const char* title, int width, int height)
{
	/**
	 * GLFW (Graphics Library Framework)
	 * GLFW je knihovna pro správu oken a vstupů, která je primárně určena pro použití s OpenGL.
	 * Poskytuje jednoduché API pro vytváření oken, kontext OpenGL a zpracování uživatelských vstupů,
	 * jako je klávesnice, myš a další zařízení.
	 */
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		/**
		 * Funkce exit()
		 * Okamžitě ukončení programu z jakéhokoliv místa v programu
		 * Nepovolí destruktory lokálních objektů, což může vést k únikům paměti.
		 * Funkce registrované pomocí atexit() budou volány, stejně jako destruktory statických objektů.
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

	// we will tell GLFW that it should hide the cursor and capture it. Capturing a cursor means that, once the application has focus, the mouse cursor stays within the center of the window (unless the application loses focus or quits). We can do this with one simple configuration call:
	// After this call, wherever we move the mouse it won't be visible and it should not leave the window.
	glfwSetInputMode(this->m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/* Start GLEW extension handler
	*  -----------------------------------------------------------------
	*  GLEW (OpenGL Extension Wrangler Library)
	*  je knihovna pro správu rozšíření OpenGL. Poskytuje přístup k
	*  moderním funkcím OpenGL, které nemusí být dostupné ve standardní
	*  implementaci OpenGL na daném systému.
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
