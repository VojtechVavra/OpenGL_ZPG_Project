#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>		// tolower(int)

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>	// glm::lookAt, glm::radians

#include "Callback.hpp"

struct Resolution;

std::shared_ptr<Camera> Callback::camera = nullptr;
std::shared_ptr<GLFWwindow> Callback::window = nullptr;
std::shared_ptr<Scene> Callback::scene = nullptr;

float Callback::lastX = 0;
float Callback::lastY = 0;
bool Callback::firstMouse = true;

bool Callback::isObjectHold = false;
unsigned int Callback::indexObject = 0;

void Callback::setCamera(std::shared_ptr<Camera> camera) {
	Callback::camera = camera;

	int width, height;
	glfwGetWindowSize(Callback::window.get(), &width, &height);

	Camera::Resolution resolution{ width, height };
	camera->setResolution(resolution);
}

void Callback::setWindow(std::shared_ptr<GLFWwindow> window) {
	Callback::window = window;
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
		glfwGetWindowSize(Callback::window.get(), &width, &height);

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

	/*if (isObjectHold && indexObject > 0 && false)
	{
		printf("object camera move\n");
		glm::vec3 objPos = scene->object[indexObject - 1]->getPosition();
		glm::vec3 camPos = scene->camera[0]->getPosition();

		scene->object[indexObject - 1]->useShader();
		Shader::sendUniform(scene->object[indexObject - 1]->getShader(), "modelMatrix", scene->object[indexObject - 1]->getMatrix());
	}*/

}

void Callback::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	//printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
	//if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	//	glfwSetWindowShouldClose(window, true);


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
				// pick up item / drop down
				if (isObjectHold) {
					isObjectHold = false;
					scene->setLastColor(indexObject);

					printf("Object %u dropped\n", indexObject - 1);
					camera->dropObject();
					indexObject = 0;
				}
				else if (!isObjectHold) {
					int width, height;
					glfwGetWindowSize(window, &width, &height);

					// na�ten� ID a pozice ve sv�tov�ch sou�adnic�ch
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

						scene->setNewColor(indexObject);
						isObjectHold = true;
						printf("Object %u taken\n", indexObject - 1);

						std::shared_ptr<Object> holdedObject = scene->object[indexObject - 1];
						Callback::camera->setHoldObject(holdedObject);

						//holdedObject->setMatrix(glm::inverse(camera->getCamera()) * camera->getMatrix());
					}
					else {
						indexObject = 0;
					}
				}
				break;
			case GLFW_KEY_F:
				scene->camera[0]->flashLightOnOff();
				break;
			case GLFW_KEY_DELETE:
				if (indexObject != -1) {
					scene->deleteObject(indexObject);
					//scene->camera[0]->dropObject();
					
					printf("Object %d deleted\n", indexObject - 1);

					isObjectHold = false;
					indexObject = -1;
				}
				break;
			default:
				break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
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
	glViewport(0, 0, width, height);

	int* windSize = (int*)glfwGetWindowUserPointer(window);
	windSize[0] = width;
	windSize[1] = height;

	Callback::camera->setPerspectiveCamera(width, height);
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

		// na�ten� ID a pozice ve sv�tov�ch sou�adnic�ch
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

		}
		else if (button == GLFW_MOUSE_BUTTON_2)
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
		}
	}

	printf("button_callback up? [%d,%d,%d]\n", button, action, mode);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void Callback::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Callback::camera->processMouseScroll(yoffset);
}