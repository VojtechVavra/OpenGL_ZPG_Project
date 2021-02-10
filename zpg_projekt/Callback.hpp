#ifndef CALLBACK_HPP
#define CALLBACK_HPP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.hpp"
#include "Scene.hpp"


class Callback {
public:
    //Callback(Camera* camera);
    static void setCamera(std::shared_ptr<Camera> camera);
    static void setWindow(std::shared_ptr<GLFWwindow> window);
    static std::shared_ptr<GLFWwindow> window;
private:
    //static GLFWwindow* window;
    static std::shared_ptr<Camera> camera;
    static bool isObjectHold;
    static unsigned int indexObject;
    static bool fullscreen;
    static int width, height, refreshRate;
public:
    static std::shared_ptr<Scene> scene;
    static float lastX, lastY;
    static bool firstMouse;

    static void error_callback(int error, const char* description);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);  // presmerovat informace na objekt
    static void window_focus_callback(GLFWwindow* window, int focused);
    static void window_iconify_callback(GLFWwindow* window, int iconified);

    static void window_size_callback(GLFWwindow* window, int width, int height);
    //static void window_size_modify_callback(int& windowWidth, int& windowHeight);

    static void cursor_callback(GLFWwindow* window, double x, double y);
    static void button_callback(GLFWwindow* window, int button, int action, int mode);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};


#endif