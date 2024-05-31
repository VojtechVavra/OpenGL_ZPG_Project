#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Window
{
public:
	Window(const char* title, int width = 800, int height = 600);

	GLFWwindow* getWindow() const;
	GLFWwindow* getRawPtrWindow() const;
	//std::shared_ptr<GLFWwindow> getWindow() const;
	//GLFWwindow* getRawPtrWindow() const;
	void createWindow(const char* title, int width, int height);
	
	void setSize(int width, int height);
	int* getWindowSizePtr() { return windowSize; };
	void printWindowSize();
private:
	//std::shared_ptr<GLFWwindow> m_glfwWindow;
	GLFWwindow* m_glfwWindow;

	int windowWidth, windowHeight;
	int windowSize[2] = { 0, 0 };
};

#endif // !WINDOW_HPP