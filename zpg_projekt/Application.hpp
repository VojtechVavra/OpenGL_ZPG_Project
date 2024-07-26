#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <stdio.h>
#include <array>
#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.hpp"
#include "Renderer.hpp"


class Application {
public:
    Application* getInstance();
    ~Application();
    
    void callbackFunctions() const;
    void setWindowSize(int width, int height) const;
    void setWindowTitle(const std::string& newTitle) const;
    void init();

private:
    static Application* m_single;
    Application();
    std::shared_ptr<Window> m_window;
    void PrintInfo();
    void draw();

    std::shared_ptr<class Scene> m_scene;
    Renderer m_renderer;
};

#endif
