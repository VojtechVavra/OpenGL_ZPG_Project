#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <array>
#include "Callback.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Object.hpp"


class Application {
public:
    Application* getInstance();
    ~Application();
    inline GLFWwindow* GetWindow() { return window; }
    void callBackFunctions();
    void setWindowSize(int width, int height);
    void init();
private:
    static Application* single;
    Application();
    GLFWwindow* window;
    Callback callbacks;
    int windowWidth, windowHeight;
   
    // timing
    float deltaTime;	// time between current frame and last frame
    float lastFrame;
    Shader shaders;
    Camera camera;

    void CreateWindow(int width = 800, int height = 600);
    void PrintInfo();
    void draw();
};

#endif