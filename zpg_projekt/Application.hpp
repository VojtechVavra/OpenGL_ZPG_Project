#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <stdio.h>
#include <array>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"


class Application {
public:
    Application* getInstance();
    ~Application();
    
    void callbackFunctions();
    void setWindowSize(int width, int height) const;
    void setWindowTitle(const std::string& newTitle) const;
    void init();

private:
    static Application* m_single;
    Application();
    std::shared_ptr<Window> m_window; // application window
    //int windowWidth, windowHeight;
    //int* windSize[2];

    //void CreateWindow(int width = 800, int height = 600);
    void PrintInfo();
    void draw();
};

#endif