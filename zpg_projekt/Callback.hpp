#ifndef CALLBACK_HPP
#define CALLBACK_HPP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Camera.hpp"


class Callback {
public:
    //Callback(Camera* camera);
    static void setCamera(Camera* camera);
private:
    //static GLFWwindow* window;
    static Camera* camera;
public:
    static float lastX, lastY;
    static bool firstMouse;

    static void error_callback(int error, const char* description);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);  // presmerovat informace na objekt
    static void window_focus_callback(GLFWwindow* window, int focused);
    static void window_iconify_callback(GLFWwindow* window, int iconified);
    static void window_size_callback(GLFWwindow* window, int width, int height);
    static void cursor_callback(GLFWwindow* window, double x, double y);
    static void button_callback(GLFWwindow* window, int button, int action, int mode);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};


#endif