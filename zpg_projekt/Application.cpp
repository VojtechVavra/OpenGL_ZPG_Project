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
//#include "Shader.hpp"
//#include "Object.hpp"
#include "Callback.hpp"
//#include "models/1/bedna.hpp"
//#include "sphere.hpp"
//#include "Light.hpp"
//#include "Model.hpp"
#include "Scene.hpp"
#include "Renderer.hpp"


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
	//deltaTime = 0.0f;
	//lastFrame = 0.0f;
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
	glfwGetFramebufferSize(window.get(), &width, &height);
	//float ratio = width / (float)height;
	glViewport(0, 0, width, height);

	// 3 Vertex Buffer Object, Vertex Array Object
	//VboInit();
	//VaoInit();



	draw();
}

void Application::setWindowSize(int width, int height)
{
	this->windowWidth = width;
	this->windowHeight = height;
}

void Application::CreateWindow(int width, int height)
{
	this->window = std::shared_ptr<GLFWwindow>(glfwCreateWindow(width, height, "ZPG", NULL, NULL));

	if (!window.get())
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
		std::cout << "Error in creating window on line: " << __LINE__ << std::endl;
	}

	glfwMakeContextCurrent(window.get());
	glfwSwapInterval(1);


	// we will tell GLFW that it should hide the cursor and capture it. Capturing a cursor means that, once the application has focus, the mouse cursor stays within the center of the window (unless the application loses focus or quits). We can do this with one simple configuration call:
	// After this call, wherever we move the mouse it won't be visible and it should not leave the window.
	glfwSetInputMode(window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
	glfwSetCursorPosCallback(window.get(), Callback::cursor_pos_callback);
	glfwSetKeyCallback(this->GetWindow().get(), [](GLFWwindow* window, int key, int scancode, int action, int mods) { Callback::key_callback(window, key, scancode, action, mods); });
	glfwSetMouseButtonCallback(this->GetWindow().get(), [](GLFWwindow* window, int button, int action, int mode) { Callback::button_callback(window, button, action, mode); });
	glfwSetWindowFocusCallback(this->GetWindow().get(), [](GLFWwindow* window, int focused) { Callback::window_focus_callback(window, focused); });
	glfwSetWindowIconifyCallback(this->GetWindow().get(), [](GLFWwindow* window, int iconified) { Callback::window_iconify_callback(window, iconified); });
	//glfwSetWindowSizeCallback(this->GetWindow(), [](GLFWwindow* window, int width, int height) { Callback::window_size_callback(window, width, height); });
	glfwSetWindowSizeCallback(window.get(), Callback::window_size_callback);
	glfwSetScrollCallback(window.get(), Callback::scroll_callback);
}

void Application::draw()
{
	/*
	//shaders = Shader(&camera);
	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));
	Callback::setCamera(&camera);
	//GLuint shaderProgram = shaders.CreateAndCompile();

	std::vector<Shader> shader;
	shader.push_back(Shader(&camera, ShaderType::AMBIENT));
	shader.push_back(Shader(&camera, ShaderType::DIFFUSE));
	shader.push_back(Shader(&camera, ShaderType::SPECULAR));
	shader.push_back(Shader(&camera, ShaderType::PHONG));
	for (int i = 4; i < 7; i++)
	{
		shader.push_back(Shader(&camera, ShaderType::PHONG));
	}
	std::vector<GLuint> shaderProgram;
	for (int i = 0; i < 7; i++)
	{
		shaderProgram.push_back(shader[i].getShader());
	}

	camera.setPerspectiveCamera();

	glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	Shader shaderLight = Shader(&camera, ShaderType::AMBIENT);
	GLuint shaderLightProgram = shaderLight.getShader();
	std::vector<Object> object;
	
	std::unique_ptr<Shader> pshader222;
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);

	Model plain("plain");
	Model sphere("sphere");
	Model suzi_smooth("suzi_smooth");
	Model suzi_flat("suzi_flat");
	Model lightModel("sphere");
	// light
	Light light(glm::vec3(0.0f, 0.0f, 0.0f), lightModel, glm::vec3(1.0f, 1.0f, 1.0f), shaderLightProgram, glm::vec3(1.0f, 1.0f, 1.0f));

	object.push_back(Object(glm::vec3(-0.25f, 0.f, 0.25f), sphere, glm::vec3(1.10f, 1.10f, 1.10f), shaderProgram[0]));	// horni leva koule
	object.push_back(Object(glm::vec3(0.25f, 0.f, 0.25f), sphere, glm::vec3(0.0f, 0.0f, 0.9f), shaderProgram[1]));		// horni prava koule
	object.push_back(Object(glm::vec3(-0.25f, 0.f, -0.25f), sphere, glm::vec3(0.7f, 0.0f, 0.5f), shaderProgram[2]));	// dolni leva koule
	object.push_back(Object(glm::vec3(0.25f, 0.f, -0.25f), sphere, glm::vec3(0.8f, 0.0f, 0.0f), shaderProgram[3]));		// dolni prava koule

	object.push_back(Object(pos, plain, shaderProgram[4]));
	object.push_back(Object(pos, suzi_smooth, shaderProgram[5]));
	object.push_back(Object(pos, suzi_flat, shaderProgram[6]));
	// light
	//object.push_back(Object(pos, light, shaderLightProgram));

	glm::vec3 sphereScale = glm::vec3(0.2f, 0.2f, 0.2f);
	object[0].Scale(sphereScale);
	object[1].Scale(sphereScale);
	object[2].Scale(sphereScale);
	object[3].Scale(sphereScale);

	bool first = true;

	// object translate and scale
	object[4].Translate(glm::vec3(0.25f, 0.0f, 0.25f));
	object[4].Scale(glm::vec3(0.10f, 0.10f, 0.10f));
	object[5].Translate(glm::vec3(-1.0f, 0.0f, 0.25f));
	object[5].Scale(glm::vec3(0.10f, 0.10f, 0.10f));
	object[6].Translate(glm::vec3(0.0, -0.30, 0.0));
	object[6].Scale(glm::vec3(0.10f, 0.10f, 0.10f));
	// end translate, scale
	*/

	std::shared_ptr<Scene> scene = std::shared_ptr<Scene>(new Scene());
	Renderer renderer = Renderer();

	renderer.renderScene(scene, window);


	/*

	glEnable(GL_DEPTH_TEST);
	// vytvorit renderer, predat mu tridu scenu, scena udrzuje objekty
	while (!glfwWindowShouldClose(window.get()))
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


		//lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		//lightPos.z = sin(glfwGetTime() / 2.0f) * 1.0f;

		// draw model 1 - sphere
		//glBindVertexArray(0);
		//glBindVertexArray(object[0].getModel().getVAO());




		if (first)
		{
			first = false;
			
			// draw models - sphere
			for (int i = 0; i < 7; i++) {
				glBindVertexArray(0);
				glBindVertexArray(object[0].getModel().getVAO());
				object[i].useShader();		//glUseProgram(shaderProgram);
				if (shader[i].getType() == ShaderType::AMBIENT)
				{
					// vertex shader uniforms
					shader[i].sendUniform("modelMatrix", object[i].getMatrix());
					shader[i].sendUniform("viewMatrix", camera.getCamera());
					shader[i].sendUniform("projectionMatrix", camera.getProjectionMatrix());

					// fragment shader uniforms
					shader[i].sendUniform("fragmentColor", object[i].getColor());
					//shader[i].sendUniform("viewPos", camera.getPosition());
					// posilat jen pri zmnene
					//shader[i].sendUniform("newLightPos", light.getPosition());
					//shader[i].sendUniform("lightColor", light.lightColor);
				}
				else if (shader[i].getType() == ShaderType::DIFFUSE)
				{
					// vertex shader uniforms
					shader[i].sendUniform("modelMatrix", object[i].getMatrix());
					shader[i].sendUniform("viewMatrix", camera.getCamera());
					shader[i].sendUniform("projectionMatrix", camera.getProjectionMatrix());

					// fragment shader uniforms
					shader[i].sendUniform("fragmentColor", object[i].getColor());
					shader[i].sendUniform("lightPosition", light.getPosition());
					shader[i].sendUniform("lightColor", light.lightColor);
				}
				else if (shader[i].getType() == ShaderType::SPECULAR)
				{
					// vertex shader uniforms
					shader[i].sendUniform("modelMatrix", object[i].getMatrix());
					shader[i].sendUniform("viewMatrix", camera.getCamera());
					shader[i].sendUniform("projectionMatrix", camera.getProjectionMatrix());

					// fragment shader uniforms
					shader[i].sendUniform("fragmentColor", object[i].getColor());
					shader[i].sendUniform("lightPosition", light.getPosition());
					shader[i].sendUniform("lightColor", light.lightColor);
					shader[i].sendUniform("viewPos", camera.getPosition());
					shader[i].sendUniform("specularStrength", 0.5f);
				}
				else if (shader[i].getType() == ShaderType::PHONG)
				{
					shader[i].sendUniform("modelMatrix", object[i].getMatrix());
					shader[i].sendUniform("fragmentColor", object[i].getColor());
					shader[i].sendUniform("viewPos", camera.getPosition());
					// posilat jen pri zmnene
					shader[i].sendUniform("lightPosition", light.getPosition());
					shader[i].sendUniform("lightColor", light.lightColor);
					shader[i].sendUniform("viewMatrix", camera.getCamera());
					shader[i].sendUniform("projectionMatrix", camera.getProjectionMatrix());
				}
				
				object[i].getModel().render();
			}



			// draw model 7 - light
			glBindVertexArray(light.getModel().getVAO());
			//glm::mat4 trmat5 = glm::translate(glm::mat4(1.0f), lightPos);
			//glm::mat4 vysledek5 = glm::scale(trmat5, glm::vec3(0.05f, 0.05f, 0.05f));
			light.useShader();

			//object[7].Translate(lightPos);
			light.Scale(glm::vec3(0.05f, 0.05f, 0.05f));
			shaderLight.sendUniform("viewMatrix", camera.getCamera());
			shaderLight.sendUniform("modelMatrix", light.getMatrix());
			shaderLight.sendUniform("fragmentColor", light.getColor());
			//shaderLight.sendUniform("lightColor", light.lightColor);
			shaderLight.sendUniform("ambientStrength", 1.0f);
			//shader2.sendUniform("specularStrength", 0.0f);
			//shaderLight.sendUniform("lightPosition", light.getPosition());
			//shaderLight.sendUniform("viewPos", camera.getPosition());
			shaderLight.sendUniform("projectionMatrix", camera.getProjectionMatrix());
			light.getModel().render();	//	glDrawArrays(GL_TRIANGLES, 0, 2880);
		}
		else
		{
		// objects
			for (int i = 0; i < 7; i++)
			{
				glBindVertexArray(object[i].getModel().getVAO());
				object[i].useShader();

				if (shader[i].getType() == ShaderType::AMBIENT)
				{
					shader[i].sendUniform("modelMatrix", object[i].getMatrix());
					shader[i].sendUniform("viewMatrix", camera.getCamera());
				}
				else if (shader[i].getType() == ShaderType::DIFFUSE)
				{
					shader[i].sendUniform("modelMatrix", object[i].getMatrix());
					shader[i].sendUniform("viewMatrix", camera.getCamera());
				}
				else if (shader[i].getType() == ShaderType::SPECULAR)
				{
					shader[i].sendUniform("modelMatrix", object[i].getMatrix());
					shader[i].sendUniform("viewMatrix", camera.getCamera());
					shader[i].sendUniform("viewPos", camera.getPosition());
				}
				else if (shader[i].getType() == ShaderType::PHONG)
				{
					shader[i].sendUniform("modelMatrix", object[i].getMatrix());
					shader[i].sendUniform("viewMatrix", camera.getCamera());
					shader[i].sendUniform("viewPos", camera.getPosition());
				}
				

				//shader[i].sendUniform("projectionMatrix", camera.getProjectionMatrix());
				//shader[i].sendUniform("fragColor", object[i].getColor());
				object[i].getModel().render();
			}

			// light
			glBindVertexArray(light.getModel().getVAO());
			light.useShader();
			shaderLight.sendUniform("modelMatrix", light.getMatrix());
			shaderLight.sendUniform("viewMatrix", camera.getCamera());
			light.getModel().render();
		}
		

		//glDrawElements(GL_TRIANGLES, sizeof(idxs)/sizeof(idxs[0]), GL_UNSIGNED_SHORT, idxs);
		//glPopMatrix();
		// end added

		// update other events like input handling
		glfwPollEvents();
		// put the stuff we’ve been drawing onto the display
		glfwSwapBuffers(window.get());
	}
	*/



	//glfwDestroyWindow(window.get());
	//glfwTerminate();
}

Application::~Application()
{
	printf("Destructor ~Application \n");
}
