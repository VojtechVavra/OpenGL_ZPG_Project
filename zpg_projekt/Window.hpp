#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Window
{
public:
	Window(const char* title, int width = 800, int height = 600);
	
	GLFWwindow* getGLFWwindow() const;
	void createWindow(const char* title, int width, int height);
	void VSyncEnable(bool enabled);

	void setSize(int width, int height);
	void setWindowTitle(const std::string& newTitle);
	int* getWindowSizePtr() { return windowSize; };
	void printWindowSize();
private:
	GLFWwindow* m_glfwWindow;

	int windowWidth, windowHeight;
	int windowSize[2] = { 0, 0 };
};

#endif // !WINDOW_HPP