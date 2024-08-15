#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>		// tolower(int)

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>	// glm::lookAt, glm::radians

#include "Callback.hpp"
#include "Window.hpp"

struct Resolution;

std::shared_ptr<Camera> Callback::camera = nullptr;
std::shared_ptr<Window> Callback::window = nullptr;
GLFWwindow* Callback::glfwWindow = nullptr;
std::shared_ptr<Scene> Callback::scene = nullptr;

float Callback::lastX = 0;
float Callback::lastY = 0;
bool Callback::firstMouse = true;

bool Callback::isObjectHold = false;
unsigned int Callback::indexObject = 0;
bool Callback::fullscreen = false;
int Callback::width = 640;
int Callback::height = 480;
int Callback::refreshRate = 60;

void Callback::setCamera(std::shared_ptr<Camera> camera) {
	Callback::camera = camera;

	int width, height;
	glfwGetWindowSize(Callback::glfwWindow, &width, &height);

	Camera::Resolution resolution{ width, height };
	camera->setResolution(resolution);
}

void Callback::setWindow(std::shared_ptr<Window> window) {
	Callback::window = window;
	Callback::glfwWindow = window->getGLFWwindow();
}

void Callback::setWindow(GLFWwindow* glfwWindow) {
	Callback::glfwWindow = glfwWindow;
}

// callback functions
void Callback::error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void Callback::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
	//printf("cursor_pos_callback ax:%d, ay:%d\n", (int)xpos, (int)ypos);
	if (firstMouse)
	{
		int width, height;
		glfwGetWindowSize(Callback::glfwWindow, &width, &height);

		Callback::lastX = width / 2.0f;
		Callback::lastY = height / 2.0f;
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;
	
	Callback::camera->processMouseMovement(xoffset, yoffset);
}

void Callback::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	//printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
			case GLFW_KEY_W:
				Callback::camera->setMoveDir(FORWARD);
				break;
			case GLFW_KEY_S:
				Callback::camera->setMoveDir(BACKWARD);
				break;
			case GLFW_KEY_A:
				Callback::camera->setMoveDir(STRAFE_LEFT);
				break;
			case GLFW_KEY_D:
				Callback::camera->setMoveDir(STRAFE_RIGHT);
				break;
			case GLFW_KEY_E:
				// Pick up item / drop down
				if (isObjectHold) {
					isObjectHold = false;
					//scene->setLastColor(indexObject);

					printf("Object %u dropped\n", indexObject - 1);
					//camera->dropObject();
					camera->dropObject2();
					indexObject = 0;
				}
				else if (!isObjectHold) {
					int width, height;
					glfwGetWindowSize(window, &width, &height);

					// naètení ID a pozice ve svìtových souøadnicích
					GLbyte color[4];
					GLfloat depth;
					GLuint index = 0;

					GLint x = width / 2;
					GLint y = height / 2;

					glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
					glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
					glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
					indexObject = index;

					if (indexObject > 0) {
						printf("Clicked on pixel %d, %d, color %02hhx %02hhx %02hhx %02hhx, depth %f, stencil index %u\n", x, y, color[0], color[1], color[2], color[3], depth, index);

						//scene->setNewColor(indexObject);
						printf("Object %u taken\n", indexObject - 1);

						// TODO: Prepracovat drzeni objektu pred kamerou
						/*
						if (indexObject == 12) {
							isObjectHold = true;
							std::shared_ptr<MeshLoader> holdedObject = scene->meshObjects[4];
							Callback::camera->setHoldObject2(holdedObject);
						}
						else if (indexObject == 13) {
							isObjectHold = true;
							std::shared_ptr<MeshLoader> holdedObject = scene->meshObjects[5];
							Callback::camera->setHoldObject2(holdedObject);
						}
						else if (indexObject == 14) {
							isObjectHold = true;
							std::shared_ptr<MeshLoader> holdedObject = scene->meshObjects[6];
							Callback::camera->setHoldObject2(holdedObject);
						}*/			
					}
					else {
						indexObject = 0;
					}
				}
				break;
			case GLFW_KEY_F:
				// Flashlight
				scene->camera[0]->flashLightOnOff();
				break;
			case GLFW_KEY_J:
			{
				printf("ViewMatrix:\n");
				glm::mat4 ViewMatrix = scene->camera[0]->getCamera();
				for (unsigned int i = 0; i < 4; ++i) {
					for (unsigned int j = 0; j < 4; ++j)
						printf("%f, ", ViewMatrix[i][j]);
					printf("\n");
				}

				glm::vec3 camPos = scene->camera[0]->getPosition();
				printf("camPos: %f, %f, %f\n", camPos.x, camPos.y, camPos.z);
				break;
			}
			case GLFW_KEY_DELETE:
			{
				if (indexObject != -1) {
					scene->deleteObject(indexObject);
					//scene->camera[0]->dropObject();
					
					printf("Object %d deleted\n", indexObject - 1);

					isObjectHold = false;
					indexObject = -1;
				}
				break;
			}
			case GLFW_KEY_F10:
			{
				// TODO: Important bug, after press KEY F10, game stop responding
				if (fullscreen)
				{
					// leave full screen by setting previous width and height of the game window
					glfwSetWindowMonitor(window, NULL, 30, 30, width, height, GLFW_DONT_CARE);
					fullscreen = false;
				}
				else
				{
					// Save current width and height, so after user leave full screen we can resize window size back to previos size
					//glfwGetWindowPos(window, &xPos, &_wndPos[1]);
					glfwGetWindowSize(window, &width, &height);
					printf("saving width: %d, height: %d\n", width, height);

					// get resolution, refresh rate of monitor
					GLFWmonitor* monitor = glfwGetPrimaryMonitor();
					const GLFWvidmode* mode = glfwGetVideoMode(monitor);

					// switch to full screen
					glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
					
					fullscreen = true;
				}
				break;
			}
			case GLFW_KEY_F2:
			{
				// Draw / hide detail of textures over them
				if (Callback::camera == nullptr) {
					break;	// return
				}
				bool texture_detail = Callback::camera->getTextureDetail();
				if (!texture_detail)
				{
					Callback::camera->setShowTextureDetail(true);
				}
				else
				{
					Callback::camera->setShowTextureDetail(false);
				}
				printf("Texture detail: %s\n", texture_detail ? "on" : "off");
				break;
			}
			case GLFW_KEY_F1:
			{
				// Enable / disable fly camera
				if (Callback::camera == nullptr) {
					break;	// return
				}
				if (Callback::camera->fly) {
					Callback::camera->fly = false;
				}
				else {
					Callback::camera->fly = true;
				}
				printf("Fly mode: %s\n", Callback::camera->fly ? "on" : "off");
				break;
			}
			default:
				break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		// Camera movement
		switch (key)
		{
		case GLFW_KEY_W:
			Callback::camera->unSetMoveDir(FORWARD);
			break;
		case GLFW_KEY_S:
			Callback::camera->unSetMoveDir(BACKWARD);
			break;
		case GLFW_KEY_A:
			Callback::camera->unSetMoveDir(STRAFE_LEFT);
			break;
		case GLFW_KEY_D:
			Callback::camera->unSetMoveDir(STRAFE_RIGHT);
			break;
		default:
			break;
		}
	}
}

void Callback::window_focus_callback(GLFWwindow* window, int focused) {
	printf("window_focus_callback \n");
}

void Callback::window_iconify_callback(GLFWwindow* window, int iconified) {
	printf("window_iconify_callback \n");
}

// original one
void Callback::window_size_callback(GLFWwindow* window, int width, int height) {
	printf("resize %d, %d \n", width, height);
	//glViewport(0, 0, width, height);

	int* windSize = (int*)glfwGetWindowUserPointer(window);
	windSize[0] = width;
	windSize[1] = height;
	//Callback::window->printWindowSize();

	Callback::camera->setPerspectiveCamera(width, height);
}

void Callback::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	std::cout << "Framebuffer resized to " << width << " x " << height << std::endl;
	glViewport(0, 0, width, height); // Aktualizace viewportu
}

// new
/*void Callback::window_size_modify_callback(int& windowWidth, int& windowHeight) {
	//printf("resize %d, %d \n", width, height);
	//glViewport(0, 0, width, height);
	printf("My window resize function\n");
	//Callback::camera->
}*/

void Callback::cursor_callback(GLFWwindow* window, double x, double y) {
	printf("cursor_callback \n");
}

void Callback::button_callback(GLFWwindow* window, int button, int action, int mode) {
	if (action == GLFW_PRESS) {
		printf("button_callback [%d,%d,%d]\n", button, action, mode);

		int width, height;
		glfwGetWindowSize(window, &width, &height);
		//printf("window size [%d,%d]\n", width, height);

		// naètení ID a pozice ve svìtových souøadnicích
		GLbyte color[4];
		GLfloat depth;
		GLuint index = 0;

		//getting cursor position
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		///
		printf("x: %d\ny: %d\n", camera->getResolution().x, camera->getResolution().y);
		GLint x = width / 2;
		GLint y = height / 2;

		glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
		glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
		glReadPixels(x, y, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

		printf("Clicked on pixel %d, %d, color %02hhx %02hhx %02hhx %02hhx, depth %f, stencil index %u\n", x, y, color[0], color[1], color[2], color[3], depth, index);
		
		if (button == GLFW_MOUSE_BUTTON_1)
		{
			if (index == 10) {
				scene->zatahlyZaves = false;
			}
			else if (index == 11) {
				scene->zatahlyZaves = true;
			}
		}
		/*else if (button == GLFW_MOUSE_BUTTON_2)
		{ // spawnovani objektu
			//Application::getInstance()->getScene()->getRenderer()->setSelect(index-1);

			glm::vec3 screenX = glm::vec3(x, y, depth);
			glm::mat4 view = camera->getCamera();
			glm::mat4 projection = camera->getProjectionMatrix();
			glm::vec4 viewPort = glm::vec4(0, 0, camera->getResolution().x, camera->getResolution().y);
			glm::vec3 pos = glm::unProject(screenX, view, projection, viewPort);

			printf("unProject[%f, %f, %f]\n", pos.x, pos.y, pos.z);

			glm::vec3 newPosition = glm::vec3(pos.x, pos.y, pos.z);
			glm::vec3 sphereScale = glm::vec3(0.2f, 0.2f, 0.2f);

			//scene->object.push_back(objectFactory.createObject);
			scene->addObject("sphere", ShaderType::PHONG, newPosition, glm::vec3(1.0f, 1.0f, 0.5f), camera, sphereScale);
		}*/
	}

	printf("button_callback up? [%d,%d,%d]\n", button, action, mode);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void Callback::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Callback::camera->processMouseScroll(yoffset);
}
