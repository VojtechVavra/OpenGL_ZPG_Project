#include <cctype>
#include <iostream>
#include <GL/glew.h>		// must be always included first!
#include <GLFW/glfw3.h>		// this will include <GL/gl.h>

#include <stdlib.h>
//#include <GL/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include "Application.hpp"
#include "Shader.hpp"
#include "Object.hpp"
#include "Callback.hpp"
//#include "models/1/bedna.hpp"
#include "sphere.hpp"
#include "Light.hpp"
#include "Model.hpp"



Application* Application::single = nullptr;

Application* Application::getInstance()
{
	if (Application::single == nullptr) {
		Application::single = new Application();
	}
	return Application::single;
}

Application::Application()
{
	windowWidth = windowHeight = 0;
	deltaTime = 0.0f;
	lastFrame = 0.0f;
}

void Application::init()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		exit(EXIT_FAILURE);
	}

	if (windowWidth <= 0 || windowHeight <= 0) {
		CreateWindow();
	}
	else {
		CreateWindow(windowWidth, windowHeight);
	}
	
	// after window creatin can be set callbacks
	callBackFunctions();
	// 2
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stdout, "Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	PrintInfo();
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	//float ratio = width / (float)height;
	glViewport(0, 0, width, height);

	// 3 Vertex Buffer Object, Vertex Array Object
	//VboInit();
	//VaoInit();

	shaders = Shader(&camera);
	camera = Camera(&shaders, glm::vec3(0.0f, 0.0f, 0.0f));
	Callback::setCamera(&camera);

	draw();
}

void Application::setWindowSize(int width, int height)
{
	this->windowWidth = width;
	this->windowHeight = height;
}

void Application::CreateWindow(int width, int height)
{
	this->window = glfwCreateWindow(width, height, "ZPG", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
		std::cout << "Error in creating window on line: " << __LINE__ << std::endl;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);


	// we will tell GLFW that it should hide the cursor and capture it. Capturing a cursor means that, once the application has focus, the mouse cursor stays within the center of the window (unless the application loses focus or quits). We can do this with one simple configuration call:
	// After this call, wherever we move the mouse it won't be visible and it should not leave the window.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Application::PrintInfo()
{
	// get version info
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
	printf("Vendor %s\n", glGetString(GL_VENDOR));
	printf("Renderer %s\n", glGetString(GL_RENDERER));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	printf("Using GLFW %i.%i.%i\n", major, minor, revision);
}

void Application::callBackFunctions() {
	glfwSetErrorCallback([](int error, const char* description) { Callback::error_callback(error, description); });
	//glfwSetCursorPosCallback(this->GetWindow(), [](GLFWwindow* window, double mouseXPos, double mouseYPos) -> void { Callback::cursor_pos_callback(window, mouseXPos, mouseYPos); });
	glfwSetCursorPosCallback(window, Callback::cursor_pos_callback);
	glfwSetKeyCallback(this->GetWindow(), [](GLFWwindow* window, int key, int scancode, int action, int mods) { Callback::key_callback(window, key, scancode, action, mods); });
	glfwSetMouseButtonCallback(this->GetWindow(), [](GLFWwindow* window, int button, int action, int mode) { Callback::button_callback(window, button, action, mode); });
	glfwSetWindowFocusCallback(this->GetWindow(), [](GLFWwindow* window, int focused) { Callback::window_focus_callback(window, focused); });
	glfwSetWindowIconifyCallback(this->GetWindow(), [](GLFWwindow* window, int iconified) { Callback::window_iconify_callback(window, iconified); });
	//glfwSetWindowSizeCallback(this->GetWindow(), [](GLFWwindow* window, int width, int height) { Callback::window_size_callback(window, width, height); });
	glfwSetWindowSizeCallback(window, Callback::window_size_callback);
	glfwSetScrollCallback(window, Callback::scroll_callback);
}

void Application::draw()
{
	//GLuint shaderProgram = shaders.CreateAndCompile();
	GLuint shaderProgram = shaders.getShader();

	//camera.cameraDirection(0.0f, 0.0f);
	camera.setPerspectiveCamera();
	//Light light(glm::vec3(1.0, 0.0, 0.0));	// 10.0,10.0,10.0
	glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	Shader shader2 = Shader(&camera);
	GLuint shaderProgram2 = shader2.getShader();
	std::vector<Object> object;
	
	std::unique_ptr<Shader> pshader222;
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);

	Model plain("plain");
	Model sphere("sphere");
	Model suzi_smooth("suzi_smooth");
	Model suzi_flat("suzi_flat");

	object.push_back(Object(glm::vec3(-0.25, 0, 0.25), sphere, shaderProgram));	// horni leva koule
	object.push_back(Object(glm::vec3(0.25, 0, 0.25), sphere, shaderProgram));	// horni prava koule
	object.push_back(Object(glm::vec3(-0.25, 0, -0.25), sphere, shaderProgram)); // dolni leva koule
	object.push_back(Object(glm::vec3(0.25, 0, -0.25), sphere, shaderProgram)); // dolni prava koule

	object.push_back(Object(pos, plain, shaderProgram));
	object.push_back(Object(pos, suzi_smooth, shaderProgram));
	object.push_back(Object(pos, suzi_flat, shaderProgram));
	
	glm::vec3 sphereScale = glm::vec3(0.2f, 0.2f, 0.2f);
	object[0].Scale(sphereScale);
	object[1].Scale(sphereScale);
	object[2].Scale(sphereScale);
	object[3].Scale(sphereScale);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		camera.processKeyboard(deltaTime);

		// clear color and depth buffer
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// space color (gray)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		
		// draw model 1 - sphere
		glBindVertexArray(0);
		glBindVertexArray(object[0].getModel().getVAO());
		object[0].useShader();		//glUseProgram(shaderProgram);

		//glm::mat4 scalemat0 = glm::scale(object[0].getMatrix(), glm::vec3(0.10f, 0.10f, 0.10f));
		shaders.sendUniform("modelMatrix", object[0].getMatrix());
		shaders.sendUniform("fragColor", glm::vec3(0.10f, 0.10f, 0.10f));
		shaders.sendUniform("newLightPos", lightPos);
		shaders.sendUniform("lightColor", lightColor);
		shaders.sendUniform("viewPos", camera.getPosition());
		shaders.sendUniform("viewMatrix", camera.getCamera());
		shaders.sendUniform("projectionMatrix", camera.getProjectionMatrix());
		object[0].getModel().render();

		// draw model 2 - sphere
		//glBindVertexArray(0);
		glBindVertexArray(object[1].getModel().getVAO());
		object[1].useShader();		//glUseProgram(object[0].getShader());
		//glm::mat4 scalemat1 = glm::scale(object[1].getMatrix(), glm::vec3(0.1f, 0.1f, 0.1f));
		shaders.sendUniform("modelMatrix", object[1].getMatrix());
		shaders.sendUniform("fragColor", glm::vec3(0.0f, 0.0f, 0.9f));
		shaders.sendUniform("newLightPos", lightPos);
		shaders.sendUniform("viewPos", camera.getPosition());
		shaders.sendUniform("viewMatrix", camera.getCamera());
		shaders.sendUniform("projectionMatrix", camera.getProjectionMatrix());
		object[1].getModel().render();

		// draw model 3 - sphere
		//glBindVertexArray(0);
		glBindVertexArray(object[2].getModel().getVAO());
		object[2].useShader();		//glUseProgram(object[0].getShader());
		//-glm::mat4 scalemat2 = glm::scale(object[2].getMatrix(), glm::vec3(0.10f, 0.10f, 0.10f));
		shaders.sendUniform("modelMatrix", object[2].getMatrix());
		shaders.sendUniform("fragColor", glm::vec3(0.7f, 0.0f, 0.5f));
		shaders.sendUniform("newLightPos", lightPos);
		shaders.sendUniform("viewPos", camera.getPosition());
		shaders.sendUniform("viewMatrix", camera.getCamera());
		shaders.sendUniform("projectionMatrix", camera.getProjectionMatrix());
		object[2].getModel().render();

		// draw model 4 - sphere
		//glBindVertexArray(0);
		glBindVertexArray(object[3].getModel().getVAO());
		object[3].useShader();		//glUseProgram(object[0].getShader());
		//glm::mat4 scalemat3 = glm::scale(object[3].getMatrix(), glm::vec3(0.10f, 0.10f, 0.10f));
		shaders.sendUniform("modelMatrix", object[3].getMatrix());
		shaders.sendUniform("fragColor", glm::vec3(0.8f, 0.0f, 0.0f));
		shaders.sendUniform("newLightPos", lightPos);
		shaders.sendUniform("viewPos", camera.getPosition());
		shaders.sendUniform("viewMatrix", camera.getCamera());
		shaders.sendUniform("projectionMatrix", camera.getProjectionMatrix());
		object[3].getModel().render();

		// draw model 5 - plain
		//glUseProgram(shaderProgram);
		glBindVertexArray(0);
		glBindVertexArray(object[4].getModel().getVAO());
		object[4].useShader();		//glUseProgram(object[0].getShader());

		glm::mat4 trmat = glm::translate(glm::mat4(1.0f), glm::vec3(0.25f, 0.0f, 0.25f));
		glm::mat4 scmat = glm::scale(glm::mat4(1.0f), glm::vec3(0.10f, 0.10f, 0.10f));
		glm::mat4 vysledek = trmat * scmat;//glm::mat4(1.0f); //trmat * scmat;
		//light.render();

		/* funguje
		Sphere2.shader.sendUniform("viewMatrix", camera.getCamera());
		Sphere2.shader.sendUniform("modelMatrix", vysledek);
		Sphere2.shader.sendUniform("fragColor", glm::vec3(0.0f, 0.0f, 1.0f));
		Sphere2.shader.sendUniform("projectionMatrix", camera.getProjectionMatrix());
		*/
		shaders.sendUniform("modelMatrix", vysledek);
		shaders.sendUniform("fragColor", glm::vec3(0.0f, 0.0f, 1.0f));
		shaders.sendUniform("newLightPos", lightPos);
		shaders.sendUniform("viewPos", camera.getPosition());
		shaders.sendUniform("viewMatrix", camera.getCamera());
		shaders.sendUniform("projectionMatrix", camera.getProjectionMatrix());


		//Sphere.shader.sendUniform("newLightPosition", light.getPosition());
		//Sphere.shader.sendUniform("projectionMatrix", camera.getProjectionMatrix());
		//shaders.Update();
		//Sphere2.render();

		object[4].getModel().render();
		//glDrawArrays(GL_TRIANGLES, 0, 2880);	// funguje
		
		// draw model 6 - suzie smooth
		glBindVertexArray(0);
		glBindVertexArray(object[5].getModel().getVAO());
		object[5].useShader();	   // glUseProgram(object[1].getShader());

		glm::mat4 trmat3 = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0f, 0.0f, 0.25f));
		glm::mat4 scmat3 = glm::scale(glm::mat4(1.0f), glm::vec3(0.10f, 0.10f, 0.10f));
		glm::mat4 vysledek3 = trmat3 * scmat3;
		//vysledek2 = glm::translate(vysledek2, glm::vec3(-0.50f, 0.0f, 0.50f));
		//GLuint sphereShader3 = Sphere3.shader.getShader();
		//glUseProgram(sphereShader3);
		
		shaders.sendUniform("viewMatrix", camera.getCamera());
		shaders.sendUniform("modelMatrix", vysledek3);
		shaders.sendUniform("fragColor", glm::vec3(0.10f, 1.0f, 0.4f));
		shaders.sendUniform("newLightPos", lightPos);
		shaders.sendUniform("viewPos", camera.getPosition());
		shaders.sendUniform("projectionMatrix", camera.getProjectionMatrix());
		object[5].getModel().render();

	
		// draw model 6 - suzie flat
		glm::mat4 trmat4 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, -0.30, 0.0));
		glm::mat4 vysledek4 = glm::scale(trmat4, glm::vec3(0.10f, 0.10f, 0.10f));
		glBindVertexArray(object[6].getModel().getVAO());
		object[6].useShader();
		shaders.sendUniform("viewMatrix", camera.getCamera());
		shaders.sendUniform("modelMatrix",vysledek4);
		shaders.sendUniform("fragColor", glm::vec3(1.0f, 1.0f, 0.4f));
		shaders.sendUniform("newLightPos", lightPos);
		shaders.sendUniform("viewPos", camera.getPosition());
		shaders.sendUniform("projectionMatrix", camera.getProjectionMatrix());
		object[6].getModel().render();  //	glDrawArrays(GL_TRIANGLES, 0, 2904);

		/*
		// draw model 5 - Suzi_smooth
		glBindVertexArray(object[7].getModel().getVAO());
		object[7].useShader();
		
		glm::mat4 trmat5 = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, -0.5f));
		glm::mat4 vysledek5 = glm::scale(trmat5, glm::vec3(0.10f, 0.10f, 0.10f));
		object[7].useShader();
		shaders.sendUniform("viewMatrix", camera.getCamera());
		shaders.sendUniform("modelMatrix", vysledek5);
		shaders.sendUniform("fragColor", glm::vec3(0.8f, 0.2f, 0.8f));
		shaders.sendUniform("projectionMatrix", camera.getProjectionMatrix());
		object[7].getModel().render();	//	glDrawArrays(GL_TRIANGLES, 0, 2880);
		*/

		//glDrawElements(GL_TRIANGLES, sizeof(idxs)/sizeof(idxs[0]), GL_UNSIGNED_SHORT, idxs);
		//glPopMatrix();
		// end added

		// update other events like input handling
		glfwPollEvents();
		// put the stuff we’ve been drawing onto the display
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}

Application::~Application()
{
	printf("Destructor ~Application \n");
}
