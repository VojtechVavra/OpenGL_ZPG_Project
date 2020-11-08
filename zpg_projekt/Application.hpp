#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <stdio.h>
#include <array>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


class Application {
public:
    Application* getInstance();
    ~Application();
    inline std::shared_ptr<GLFWwindow> GetWindow() { return window; }
    void callbackFunctions();
    void setWindowSize(int width, int height);
    void init();
private:
    static Application* single;
    Application();
    std::shared_ptr<GLFWwindow> window;
    int windowWidth, windowHeight;
    int* windSize[2];

    void CreateWindow(int width = 800, int height = 600);
    void PrintInfo();
    void draw();
};

#endif