#include "Renderer.hpp"
#include <ctime>
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

	// draw models - sphere
	for (int i = 0; i < scene->object.size(); i++) {
		glBindVertexArray(0);
		scene->object[i].getModel().bindVAO();
		scene->object[i].useShader();		//glUseProgram(scene->shaderProgram);
		ShaderType shadType = scene->shader[i].getType();

		if (shadType == ShaderType::AMBIENT)
		{
			// vertex scene->shader uniforms
			scene->shader[i].sendUniform("modelMatrix", scene->object[i].getMatrix());
			scene->shader[i].sendUniform("viewMatrix", scene->camera[0].getCamera());
			scene->shader[i].sendUniform("projectionMatrix", scene->camera[0].getProjectionMatrix());

			// fragment scene->shader uniforms
			scene->shader[i].sendUniform("fragmentColor", scene->object[i].getColor());
		}
		else if (shadType == ShaderType::DIFFUSE)
		{
			// vertex scene->shader uniforms
			scene->shader[i].sendUniform("modelMatrix", scene->object[i].getMatrix());
			scene->shader[i].sendUniform("viewMatrix", scene->camera[0].getCamera());
			scene->shader[i].sendUniform("projectionMatrix", scene->camera[0].getProjectionMatrix());

			// fragment scene->shader uniforms
			scene->shader[i].sendUniform("fragmentColor", scene->object[i].getColor());
			scene->shader[i].sendUniform("lightPosition", scene->light->getPosition());
			scene->shader[i].sendUniform("lightColor", scene->light->lightColor);
		}
		else if (shadType == ShaderType::SPECULAR)
		{
			// vertex scene->shader uniforms
			scene->shader[i].sendUniform("modelMatrix", scene->object[i].getMatrix());
			scene->shader[i].sendUniform("viewMatrix", scene->camera[0].getCamera());
			scene->shader[i].sendUniform("projectionMatrix", scene->camera[0].getProjectionMatrix());

			// fragment scene->shader uniforms
			scene->shader[i].sendUniform("fragmentColor", scene->object[i].getColor());
			scene->shader[i].sendUniform("lightPosition", scene->light->getPosition());
			scene->shader[i].sendUniform("lightColor", scene->light->lightColor);
			scene->shader[i].sendUniform("viewPos", scene->camera[0].getPosition());
			scene->shader[i].sendUniform("specularStrength", 0.5f);
		}
		else if (shadType == ShaderType::PHONG)
		{
			scene->shader[i].sendUniform("modelMatrix", scene->object[i].getMatrix());
			scene->shader[i].sendUniform("fragmentColor", scene->object[i].getColor());
			scene->shader[i].sendUniform("viewPos", scene->camera[0].getPosition());
			// posilat jen pri zmnene
			scene->shader[i].sendUniform("lightPosition", scene->light->getPosition());
			scene->shader[i].sendUniform("lightColor", scene->light->lightColor);
			scene->shader[i].sendUniform("viewMatrix", scene->camera[0].getCamera());
			scene->shader[i].sendUniform("projectionMatrix", scene->camera[0].getProjectionMatrix());
		}

		scene->object[i].getModel().render();
	}


	// draw model 7 - light
	scene->light->getModel().bindVAO();
	scene->light->useShader();

	scene->shaderLight.sendUniform("viewMatrix", scene->camera[0].getCamera());
	scene->shaderLight.sendUniform("modelMatrix", scene->light->getMatrix());
	scene->shaderLight.sendUniform("fragmentColor", scene->light->getColor());
	//scene->shaderLight.sendUniform("lightColor", scene->light->lightColor);
	scene->shaderLight.sendUniform("ambientStrength", 1.0f);
	//scene->shader2.sendUniform("specularStrength", 0.0f);
	//scene->shaderLight.sendUniform("lightPosition", scene->light->getPosition());
	//scene->shaderLight.sendUniform("viewPos", scene->camera[0].getPosition());
	scene->shaderLight.sendUniform("projectionMatrix", scene->camera[0].getProjectionMatrix());
	scene->light->getModel().render();	//	glDrawArrays(GL_TRIANGLES, 0, 2880);
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
		scene->camera[0].processKeyboard(deltaTime);

		// clear color and depth buffer
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// space color (gray)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (true)
		{
			// scene->objects
			for (int i = 0; i < scene->object.size(); i++)
			{
				glBindVertexArray(0);
				scene->object[i].getModel().bindVAO();
				scene->object[i].useShader();

				ShaderType shadType = scene->shader[i].getType();
				if (shadType == ShaderType::AMBIENT)
				{
					//scene->shader[i].sendUniform("modelMatrix", scene->object[i].getMatrix());
					//scene->shader[i].sendUniform("viewMatrix", scene->camera[0].getCamera());
				}
				else if (shadType == ShaderType::DIFFUSE)
				{
					//scene->shader[i].sendUniform("modelMatrix", scene->object[i].getMatrix());
					//scene->shader[i].sendUniform("viewMatrix", scene->camera[0].getCamera());
				}
				else if (shadType == ShaderType::SPECULAR)
				{
					//scene->shader[i].sendUniform("modelMatrix", scene->object[i].getMatrix());
					//scene->shader[i].sendUniform("viewMatrix", scene->camera[0].getCamera());
					//scene->shader[i].sendUniform("viewPos", scene->camera[0].getPosition());
				}
				else if (shadType == ShaderType::PHONG)
				{
					//scene->shader[i].sendUniform("modelMatrix", scene->object[i].getMatrix());
					//scene->shader[i].sendUniform("viewMatrix", scene->camera[0].getCamera());
					//scene->shader[i].sendUniform("viewPos", scene->camera[0].getPosition());
				}
				else if (shadType == ShaderType::BLINN)
				{
					//scene->shader[i].sendUniform("modelMatrix", scene->object[i].getMatrix());
					//scene->shader[i].sendUniform("viewMatrix", scene->camera[0].getCamera());
					//scene->shader[i].sendUniform("viewPos", scene->camera[0].getPosition());
				}

				scene->object[i].getModel().render();	// glDrawArrays()
			}

			// light
			scene->light->getModel().bindVAO();
			scene->light->useShader();
			scene->shaderLight.sendUniform("modelMatrix", scene->light->getMatrix());
			scene->shaderLight.sendUniform("viewMatrix", scene->camera[0].getCamera());
			scene->light->getModel().render();
		}
		


		// update other events like input handling
		glfwPollEvents();
		// put the stuff we’ve been drawing onto the display
		glfwSwapBuffers(window.get());
	}

	glfwDestroyWindow(window.get());
	glfwTerminate();
}








/*void Renderer::renderLoop()
{
	// vytvorit renderer, predat mu tridu scenu, scena udrzuje objekty
	while (!glfwWindowShouldClose(window.get()))
	{
		// per-frame time logic
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		scene->scene->camera[0][0].processKeyboard(deltaTime);
		
		// clear color and depth buffer
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// space color (gray)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render();

		// update other events like input handling
		glfwPollEvents();
		// put the stuff we’ve been drawing onto the display
		glfwSwapBuffers(window.get());
	}

	
	glfwDestroyWindow(window.get());
	glfwTerminate();
}

void Renderer::render()
{
	if (first)
	{
		first = false;

		// draw models - sphere
		for (int i = 0; i < 7; i++) {
		//for (int i = 0; i < 4; i++) {
			glBindVertexArray(0);
			scene->scene->object[i].getModel().bindVAO();
			scene->scene->object[i].useShader();		//glUseProgram(scene->shaderProgram);
			if (scene->scene->shader[i].getType() == ShaderType::AMBIENT)
			{
				// vertex scene->shader uniforms
				scene->scene->shader[i].sendUniform("modelMatrix", scene->scene->object[i].getMatrix());
				scene->scene->shader[i].sendUniform("viewMatrix", scene->scene->camera[0][0].getCamera());
				scene->scene->shader[i].sendUniform("projectionMatrix", scene->scene->camera[0][0].getProjectionMatrix());

				// fragment scene->shader uniforms
				scene->scene->shader[i].sendUniform("fragmentColor", scene->scene->object[i].getColor());
				//scene->shader[i].sendUniform("viewPos", scene->camera[0].getPosition());
				// posilat jen pri zmnene
				//scene->shader[i].sendUniform("newLightPos", light->getPosition());
				//scene->shader[i].sendUniform("lightColor", light->lightColor);
			}
			else if (scene->scene->shader[i].getType() == ShaderType::DIFFUSE)
			{
				// vertex scene->shader uniforms
				scene->scene->shader[i].sendUniform("modelMatrix", scene->scene->object[i].getMatrix());
				scene->scene->shader[i].sendUniform("viewMatrix", scene->scene->camera[0][0].getCamera());
				scene->scene->shader[i].sendUniform("projectionMatrix", scene->scene->camera[0][0].getProjectionMatrix());

				// fragment scene->shader uniforms
				scene->scene->shader[i].sendUniform("fragmentColor", scene->scene->object[i].getColor());
				scene->scene->shader[i].sendUniform("lightPosition", scene->light->getPosition());
				scene->scene->shader[i].sendUniform("lightColor", scene->light->lightColor);
			}
			else if (scene->scene->shader[i].getType() == ShaderType::SPECULAR)
			{
				// vertex scene->shader uniforms
				scene->scene->shader[i].sendUniform("modelMatrix", scene->scene->object[i].getMatrix());
				scene->scene->shader[i].sendUniform("viewMatrix", scene->scene->camera[0][0].getCamera());
				scene->scene->shader[i].sendUniform("projectionMatrix", scene->scene->camera[0][0].getProjectionMatrix());

				// fragment scene->shader uniforms
				scene->scene->shader[i].sendUniform("fragmentColor", scene->scene->object[i].getColor());
				scene->scene->shader[i].sendUniform("lightPosition", scene->light->getPosition());
				scene->scene->shader[i].sendUniform("lightColor", scene->light->lightColor);
				scene->scene->shader[i].sendUniform("viewPos", scene->scene->camera[0][0].getPosition());
				scene->scene->shader[i].sendUniform("specularStrength", 0.5f);
			}
			else if (scene->scene->shader[i].getType() == ShaderType::PHONG)
			{
				scene->scene->shader[i].sendUniform("modelMatrix", scene->scene->object[i].getMatrix());
				scene->scene->shader[i].sendUniform("fragmentColor", scene->scene->object[i].getColor());
				scene->scene->shader[i].sendUniform("viewPos", scene->scene->camera[0][0].getPosition());
				// posilat jen pri zmnene
				scene->scene->shader[i].sendUniform("lightPosition", scene->light->getPosition());
				scene->scene->shader[i].sendUniform("lightColor", scene->light->lightColor);
				scene->scene->shader[i].sendUniform("viewMatrix", scene->scene->camera[0][0].getCamera());
				scene->scene->shader[i].sendUniform("projectionMatrix", scene->scene->camera[0][0].getProjectionMatrix());
			}

			scene->scene->object[i].getModel().render();
		}



		// draw model 7 - light
		scene->light->getModel().bindVAO();
		//scene->light->getModel().bindVAO());
		//glm::mat4 trmat5 = glm::translate(glm::mat4(1.0f), lightPos);
		//glm::mat4 vysledek5 = glm::scale(trmat5, glm::vec3(0.05f, 0.05f, 0.05f));
		scene->light->useShader();

		//scene->object[7].Translate(lightPos);
		scene->light->Scale(glm::vec3(0.05f, 0.05f, 0.05f));
		scene->scene->shaderLight.sendUniform("viewMatrix", scene->scene->camera[0][0].getCamera());
		scene->scene->shaderLight.sendUniform("modelMatrix", scene->light->getMatrix());
		scene->scene->shaderLight.sendUniform("fragmentColor", scene->light->getColor());
		scene->scene->shaderLight.sendUniform("ambientStrength", 1.0f);
		scene->scene->shaderLight.sendUniform("projectionMatrix", scene->scene->camera[0][0].getProjectionMatrix());
		scene->light->getModel().render();	//	glDrawArrays(GL_TRIANGLES, 0, 2880);
	}
	else if(true)
	{
		// scene->objects
		//for (int i = 0; i < 7; i++)
		for (int i = 0; i < 4; i++)
		{
			scene->scene->object[i].getModel().bindVAO();
			scene->scene->object[i].useShader();

			if (scene->scene->shader[i].getType() == ShaderType::AMBIENT)
			{
				scene->scene->shader[i].sendUniform("modelMatrix", scene->scene->object[i].getMatrix());
				scene->scene->shader[i].sendUniform("viewMatrix", scene->scene->camera[0][0].getCamera());
			}
			else if (scene->scene->shader[i].getType() == ShaderType::DIFFUSE)
			{
				scene->scene->shader[i].sendUniform("modelMatrix", scene->scene->object[i].getMatrix());
				scene->scene->shader[i].sendUniform("viewMatrix", scene->scene->camera[0][0].getCamera());
			}
			else if (scene->scene->shader[i].getType() == ShaderType::SPECULAR)
			{
				scene->scene->shader[i].sendUniform("modelMatrix", scene->scene->object[i].getMatrix());
				scene->scene->shader[i].sendUniform("viewMatrix", scene->scene->camera[0][0].getCamera());
				scene->scene->shader[i].sendUniform("viewPos", scene->scene->camera[0][0].getPosition());
			}
			else if (scene->scene->shader[i].getType() == ShaderType::PHONG)
			{
				scene->scene->shader[i].sendUniform("modelMatrix", scene->scene->object[i].getMatrix());
				scene->scene->shader[i].sendUniform("viewMatrix", scene->scene->camera[0][0].getCamera());
				scene->scene->shader[i].sendUniform("viewPos", scene->scene->camera[0][0].getPosition());
			}


			//scene->shader[i].sendUniform("projectionMatrix", scene->camera[0].getProjectionMatrix());
			//scene->shader[i].sendUniform("fragColor", scene->object[i].getColor());
			scene->scene->object[i].getModel().render();
		}

		// light
		//scene->light->getModel().bindVAO());
		scene->light->getModel().bindVAO();
		scene->light->useShader();
		scene->scene->shaderLight.sendUniform("modelMatrix", scene->light->getMatrix());
		scene->scene->shaderLight.sendUniform("viewMatrix", scene->scene->camera[0][0].getCamera());
		scene->light->getModel().render();
	}
	else
	{
		for (int i = 0; i < 7; i++)
		{
			glBindVertexArray(0);
			scene->scene->object[i].getModel().bindVAO();
			scene->scene->object[i].useShader();

			scene->scene->shader[i].sendUniform("modelMatrix", scene->scene->object[i].getMatrix());
			//scene->scene->shader[i].sendUniform("viewPos", scene->scene->camera[0][0].getPosition());

			scene->scene->object[i].getModel().render();
		}
		
	}
}
*/

Renderer::~Renderer()
{
	std::cout << "Destructor of renderer" << std::endl;
}