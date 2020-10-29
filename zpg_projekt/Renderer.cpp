#include <ctime>

#include "Renderer.hpp"
#include "FPSCounter.hpp"

Renderer::Renderer()
{
	currentFrame = 0.0f;
	deltaTime = 0.0f;
	lastFrame = 0.0f;
}


void Renderer::renderScene(std::shared_ptr<Scene> scene, std::shared_ptr<GLFWwindow> window)
{
	this->scene = scene;
	//this->scene = std::move(scene);
	this->window = window;

	glEnable(GL_DEPTH_TEST);

	first = true;
	renderInit();
	renderLoop();
}

void Renderer::renderInit()
{
	// draw model 7 - light
	scene->light->getModel().bindVAO();
	scene->light->useShader();

	Shader::sendUniform(scene->shaderLightProgram, "viewMatrix", scene->camera[0]->getCamera());
	Shader::sendUniform(scene->shaderLightProgram, "modelMatrix", scene->light->getMatrix());
	Shader::sendUniform(scene->shaderLightProgram, "fragmentColor", scene->light->lightColor);	// getColor()
	Shader::sendUniform(scene->shaderLightProgram, "ambientStrength", 1.0f);
	Shader::sendUniform(scene->shaderLightProgram, "projectionMatrix", scene->camera[0]->getProjectionMatrix());
	scene->light->getModel().render();	//	glDrawArrays(GL_TRIANGLES, 0, 2880);


	// draw models
	for (int i = 0; i < scene->object.size(); i++) {
		glBindVertexArray(0);
		scene->object[i].getModel().bindVAO();
		scene->object[i].useShader();		//glUseProgram(scene->shaderProgram);
		ShaderType shadType = scene->object[i].getShaderType();

		if (shadType == ShaderType::AMBIENT)
		{
			// vertex scene->shader uniforms
			Shader::sendUniform(scene->object[i].getShader(), "modelMatrix", scene->object[i].getMatrix());
			Shader::sendUniform(scene->object[i].getShader(), "viewMatrix", scene->camera[0]->getCamera());
			Shader::sendUniform(scene->object[i].getShader(), "projectionMatrix", scene->camera[0]->getProjectionMatrix());

			// fragment scene->shader uniforms
			Shader::sendUniform(scene->object[i].getShader(), "fragmentColor", scene->object[i].getColor());
		}
		else if (shadType == ShaderType::DIFFUSE)
		{
			// vertex scene->shader uniforms
			Shader::sendUniform(scene->object[i].getShader(), "modelMatrix", scene->object[i].getMatrix());
			Shader::sendUniform(scene->object[i].getShader(), "viewMatrix", scene->camera[0]->getCamera());
			Shader::sendUniform(scene->object[i].getShader(), "projectionMatrix", scene->camera[0]->getProjectionMatrix());

			// fragment scene->shader uniforms
			Shader::sendUniform(scene->object[i].getShader(), "fragmentColor", scene->object[i].getColor());
			Shader::sendUniform(scene->object[i].getShader(), "lightPosition", scene->light->getPosition());
			Shader::sendUniform(scene->object[i].getShader(), "lightColor", scene->light->lightColor);
		}
		else if (shadType == ShaderType::SPECULAR)
		{
			// vertex scene->shader uniforms
			Shader::sendUniform(scene->object[i].getShader(), "modelMatrix", scene->object[i].getMatrix());
			Shader::sendUniform(scene->object[i].getShader(), "viewMatrix", scene->camera[0]->getCamera());
			Shader::sendUniform(scene->object[i].getShader(), "projectionMatrix", scene->camera[0]->getProjectionMatrix());

			// fragment scene->shader uniforms
			Shader::sendUniform(scene->object[i].getShader(), "fragmentColor", scene->object[i].getColor());
			Shader::sendUniform(scene->object[i].getShader(), "lightPosition", scene->light->getPosition());
			Shader::sendUniform(scene->object[i].getShader(), "lightColor", scene->light->lightColor);
			Shader::sendUniform(scene->object[i].getShader(), "viewPos", scene->camera[0]->getPosition());
			Shader::sendUniform(scene->object[i].getShader(), "specularStrength", 0.5f);
		}
		else if (shadType == ShaderType::PHONG)
		{
			Shader::sendUniform(scene->object[i].getShader(), "modelMatrix", scene->object[i].getMatrix());
			Shader::sendUniform(scene->object[i].getShader(), "fragmentColor", scene->object[i].getColor());
			Shader::sendUniform(scene->object[i].getShader(), "viewPos", scene->camera[0]->getPosition());
			// posilat jen pri zmnene
			Shader::sendUniform(scene->object[i].getShader(), "lightPosition", scene->light->getPosition());
			Shader::sendUniform(scene->object[i].getShader(), "lightColor", scene->light->lightColor);
			Shader::sendUniform(scene->object[i].getShader(), "viewMatrix", scene->camera[0]->getCamera());
			Shader::sendUniform(scene->object[i].getShader(), "projectionMatrix", scene->camera[0]->getProjectionMatrix());
		}

		scene->object[i].getModel().render();
	}

}


void Renderer::renderLoop()
{
	FPSCounter fpsCounter = FPSCounter();

	glEnable(GL_DEPTH_TEST);
	// vytvorit renderer, predat mu tridu scenu, scena udrzuje objekty


	while (!glfwWindowShouldClose(window.get()))
	{
		// per-frame time logic
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;	// rozdil casu, kolik ubehlo mezi timto a predchozim casem
		lastFrame = currentFrame;

		// FPSCounter
		fpsCounter.drawFps(currentFrame);

		// input
		scene->camera[0]->processKeyboard(deltaTime);

		// clear color and depth buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// space color (gray)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// render objects
		for (auto it = scene->object.begin(); it != scene->object.end(); ++it)
		{
			render(&*it);
		}

		// render lights
		scene->light->getModel().bindVAO();
		scene->light->useShader();
		//Shader::sendUniform(scene->shaderLightProgram, "viewMatrix", scene->camera[0]->getCamera());
		//Shader::sendUniform(scene->shaderLightProgram, "modelMatrix", scene->light->getMatrix());

		//scene->shaderLight.sendUniform("modelMatrix", scene->light->getMatrix());
		//scene->shaderLight.sendUniform("viewMatrix", scene->camera[0]->getCamera());
		scene->light->getModel().render();


		// update other events like input handling
		glfwPollEvents();
		// put the stuff we’ve been drawing onto the display
		glfwSwapBuffers(window.get());
	}

	glfwDestroyWindow(window.get());
	glfwTerminate();
}

void Renderer::render(Object* object)
{
	object->getModel().bindVAO();
	object->useShader();
	object->getModel().render();		// glDrawArrays()
}

Renderer::~Renderer()
{
	std::cout << "Destructor of renderer" << std::endl;
}