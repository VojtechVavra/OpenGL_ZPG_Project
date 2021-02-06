#include <ctime>

#include "Renderer.hpp"
#include "FPSCounter.hpp"
#include "LoadModel.hpp"	// smazat
#include "ObjLoader.hpp"

Renderer::Renderer()
{
	currentFrame = 0.0f;
	deltaTime = 0.0f;
	lastFrame = 0.0f;
}

void Renderer::renderScene(std::shared_ptr<Scene> scene, std::shared_ptr<GLFWwindow> window)
{
	this->scene = scene;
	this->window = window;

	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_DEPTH_TEST);
	//renderInit();
	renderLoop();
}

void Renderer::renderInit()
{
	// draw model 7 - light
	/*for (int i = 0; i < scene->light.size(); i++) {
		scene->light[i]->getModel().bindVAO();
		scene->light[i]->useShader();

		Shader::sendUniform(scene->shaderLightProgram, "viewMatrix", scene->camera[0]->getCamera());
		Shader::sendUniform(scene->shaderLightProgram, "modelMatrix", scene->light[i]->getMatrix());
		Shader::sendUniform(scene->shaderLightProgram, "fragmentColor", scene->light[i]->lightColor);	// getColor()
		Shader::sendUniform(scene->shaderLightProgram, "ambientStrength", 1.0f);
		Shader::sendUniform(scene->shaderLightProgram, "projectionMatrix", scene->camera[0]->getProjectionMatrix());
		scene->light[i]->getModel().render();	//	glDrawArrays(GL_TRIANGLES, 0, 2880);
	}*/


	// draw models
	for (int i = 4; i < scene->object.size(); i++) {
		glBindVertexArray(0);
		scene->object[i]->getModel().bindVAO();
		scene->object[i]->useShader();		//glUseProgram(scene->shaderProgram);
		ShaderType shadType = scene->object[i]->getShaderType();

		if (shadType == ShaderType::AMBIENT)
		{
			// vertex scene->shader uniforms
			Shader::sendUniform(scene->object[i]->getShader(), "modelMatrix", scene->object[i]->getMatrix());
			Shader::sendUniform(scene->object[i]->getShader(), "viewMatrix", scene->camera[0]->getCamera());
			Shader::sendUniform(scene->object[i]->getShader(), "projectionMatrix", scene->camera[0]->getProjectionMatrix());

			// fragment scene->shader uniforms
			Shader::sendUniform(scene->object[i]->getShader(), "fragmentColor", scene->object[i]->getColor());
		}
		else if (shadType == ShaderType::DIFFUSE)
		{
			// vertex scene->shader uniforms
			Shader::sendUniform(scene->object[i]->getShader(), "modelMatrix", scene->object[i]->getMatrix());
			Shader::sendUniform(scene->object[i]->getShader(), "viewMatrix", scene->camera[0]->getCamera());
			Shader::sendUniform(scene->object[i]->getShader(), "projectionMatrix", scene->camera[0]->getProjectionMatrix());

			// fragment scene->shader uniforms
			Shader::sendUniform(scene->object[i]->getShader(), "fragmentColor", scene->object[i]->getColor());

			Shader::sendUniform(scene->object[i]->getShader(), "lightPosition", scene->light[0]->getPosition());
			Shader::sendUniform(scene->object[i]->getShader(), "lightColor", scene->light[0]->lightColor);

			/*Shader::sendUniform(scene->object[i]->getShader(), "lightCount", static_cast<GLint>(scene->light.size()));
			char buff[100] = { 0 };


			for (int lightIndex = 0; lightIndex < scene->light.size(); lightIndex++) {
				sprintf_s(buff, sizeof buff, "lights[%d].position", lightIndex);
				//glUniform3f(glGetUniformLocation(scene->object[i]->getShader(), "lights[0].position"), scene->light[lightIndex]->getPosition().x, scene->light[lightIndex]->getPosition().y, scene->light[lightIndex]->getPosition().z);

				Shader::sendUniform(scene->object[i]->getShader(), buff, scene->light[lightIndex]->getPosition());
				//glUniform3fv(loc, scene->light.size(), scene->light);
				memset(buff, 0, sizeof buff);

				sprintf_s(buff, sizeof buff, "lights[%d].color", lightIndex);
				Shader::sendUniform(scene->object[i]->getShader(), buff, scene->light[lightIndex]->lightColor);
			}*/
		}
		else if (shadType == ShaderType::SPECULAR)
		{
			// vertex scene->shader uniforms
			Shader::sendUniform(scene->object[i]->getShader(), "modelMatrix", scene->object[i]->getMatrix());
			Shader::sendUniform(scene->object[i]->getShader(), "viewMatrix", scene->camera[0]->getCamera());
			Shader::sendUniform(scene->object[i]->getShader(), "projectionMatrix", scene->camera[0]->getProjectionMatrix());

			// fragment scene->shader uniforms
			Shader::sendUniform(scene->object[i]->getShader(), "fragmentColor", scene->object[i]->getColor());

			Shader::sendUniform(scene->object[i]->getShader(), "lightPosition", scene->light[0]->getPosition());
			Shader::sendUniform(scene->object[i]->getShader(), "lightColor", scene->light[0]->lightColor);
			/*Shader::sendUniform(scene->object[i]->getShader(), "lightCount", static_cast<GLint>(scene->light.size()));
			char buff[100] = { 0 };

			for (int i = 0; i < scene->light.size(); i++) {
				sprintf_s(buff, sizeof buff, "lights[%d].position", i);
				Shader::sendUniform(scene->object[i]->getShader(), buff, scene->light[i]->getPosition());

				memset(buff, 0, sizeof buff);

				sprintf_s(buff, sizeof buff, "lights[%d].color", i);
				Shader::sendUniform(scene->object[i]->getShader(), buff, scene->light[i]->lightColor);
			}*/

			Shader::sendUniform(scene->object[i]->getShader(), "viewPos", scene->camera[0]->getPosition());
			Shader::sendUniform(scene->object[i]->getShader(), "specularStrength", 0.5f);
		}
		else if (shadType == ShaderType::PHONG)
		{
			Shader::sendUniform(scene->object[i]->getShader(), "modelMatrix", scene->object[i]->getMatrix());
			Shader::sendUniform(scene->object[i]->getShader(), "fragmentColor", scene->object[i]->getColor());
			Shader::sendUniform(scene->object[i]->getShader(), "viewPos", scene->camera[0]->getPosition());

			// posilat jen pri zmnene

			//Shader::sendUniform(scene->object[i]->getShader(), "lightPosition", scene->light->getPosition());
			//Shader::sendUniform(scene->object[i]->getShader(), "lightColor", scene->light->lightColor);
			Shader::sendUniform(scene->object[i]->getShader(), "lightCount", static_cast<GLint>(scene->light.size()));
			char buff[100] = { 0 };


			// Directional light
			/*sun_direction {
				UP_DOWN,
			};*/
			// z kama kam to sviti
			glm::vec3 direction_up_down = glm::vec3(0.0f, 1.0f, 0.0f);
			glm::vec3 direction_down_up = glm::vec3(0.0f, -1.0f, 0.0f);
			glm::vec3 direction_right_to_left = glm::vec3(0.0f, 0.0f, 1.0f);
			glm::vec3 direction_left_to_right = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 direction_front_to_back = glm::vec3(1.0f, 0.0f, 0.0f);
			glm::vec3 direction_back_to_front = glm::vec3(-1.0f, 0.0f, 0.0f);

			Shader::sendUniform(scene->object[i]->getShader(), "dirLight.direction", direction_front_to_back);
			//glUniform3f(glGetUniformLocation(scene->object[i]->getShader(), "dirLight.direction"), 0.0f, 1.0f, 0.0f);
			glUniform3f(glGetUniformLocation(scene->object[i]->getShader(), "dirLight.ambient"), 0.1f, 0.1f, 0.1f);
			glUniform3f(glGetUniformLocation(scene->object[i]->getShader(), "dirLight.diffuse"), 0.2f, 0.2f, 0.2);
			glUniform3f(glGetUniformLocation(scene->object[i]->getShader(), "dirLight.specular"), 0.2f, 0.2f, 0.2f);

			// spotLight
			Shader::sendUniform(scene->object[i]->getShader(), "spotLight[0].position", scene->camera[0]->getPosition());
			Shader::sendUniform(scene->object[i]->getShader(), "spotLight[0].direction", scene->camera[0]->target);
			Shader::sendUniform(scene->object[i]->getShader(), "spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			Shader::sendUniform(scene->object[i]->getShader(), "spotLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
			Shader::sendUniform(scene->object[i]->getShader(), "spotLight[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
			Shader::sendUniform(scene->object[i]->getShader(), "spotLight[0].constant", 1.0f);
			Shader::sendUniform(scene->object[i]->getShader(), "spotLight[0].linear", 0.09f);
			Shader::sendUniform(scene->object[i]->getShader(), "spotLight[0].quadratic", 0.032f);
			Shader::sendUniform(scene->object[i]->getShader(), "spotLight[0].cutOff", glm::cos(glm::radians(12.5f)));
			Shader::sendUniform(scene->object[i]->getShader(), "spotLight[0].outerCutOff", glm::cos(glm::radians(15.0f)));

			// FlashLight
			Shader::sendUniform(scene->object[i]->getShader(), "flashLight.position", scene->camera[0]->getPosition());
			Shader::sendUniform(scene->object[i]->getShader(), "flashLight.direction", scene->camera[0]->target);
			Shader::sendUniform(scene->object[i]->getShader(), "flashLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
			Shader::sendUniform(scene->object[i]->getShader(), "flashLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
			Shader::sendUniform(scene->object[i]->getShader(), "flashLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
			Shader::sendUniform(scene->object[i]->getShader(), "flashLight.constant", 1.0f);
			Shader::sendUniform(scene->object[i]->getShader(), "flashLight.linear", 0.09f);
			Shader::sendUniform(scene->object[i]->getShader(), "flashLight.quadratic", 0.032f);
			Shader::sendUniform(scene->object[i]->getShader(), "flashLight.cutOff", glm::cos(glm::radians(12.5f)));
			Shader::sendUniform(scene->object[i]->getShader(), "flashLight.outerCutOff", glm::cos(glm::radians(15.0f)));

			for (int lightIndex = 0; lightIndex < scene->light.size() - 1; lightIndex++) {
				/*sprintf_s(buff, sizeof buff, "lights[%d].position", lightIndex);
				//glUniform3f(glGetUniformLocation(scene->object[i]->getShader(), "lights[0].position"), scene->light[lightIndex]->getPosition().x, scene->light[lightIndex]->getPosition().y, scene->light[lightIndex]->getPosition().z);

				Shader::sendUniform(scene->object[i]->getShader(), buff, scene->light[lightIndex]->getPosition());
				//glUniform3fv(loc, scene->light.size(), scene->light);
				memset(buff, 0, sizeof buff);

				sprintf_s(buff, sizeof buff, "lights[%d].color", lightIndex);
				Shader::sendUniform(scene->object[i]->getShader(), buff, scene->light[lightIndex]->lightColor);

				memset(buff, 0, sizeof buff);
				sprintf_s(buff, sizeof buff, "lights[%d].lightType", lightIndex);
				Shader::sendUniform(scene->object[i]->getShader(), buff, (int)scene->light[lightIndex]->getLightType());

				memset(buff, 0, sizeof buff);
				sprintf_s(buff, sizeof buff, "lights[%d].direction", lightIndex);
				Shader::sendUniform(scene->object[i]->getShader(), buff, glm::vec3(0, 0, 1));
				*/

				/*Shader::sendUniform(scene->object[i]->getShader(), ("pointLight[" + std::to_string(lightIndex) + "].lightType").c_str(), scene->light[lightIndex]->lightType);
				Shader::sendUniform(scene->object[i]->getShader(), ("pointLight[" + std::to_string(lightIndex) + "].position").c_str(), scene->light[lightIndex]->position);
				Shader::sendUniform(scene->object[i]->getShader(), ("pointLight[" + std::to_string(lightIndex) + "].constant").c_str(), 1.0f);
				Shader::sendUniform(scene->object[i]->getShader(), ("pointLight[" + std::to_string(lightIndex) + "].linear").c_str(), 0.09f);
				Shader::sendUniform(scene->object[i]->getShader(), ("pointLight[" + std::to_string(lightIndex) + "].quadratic").c_str(), 0.032f);
				*/


				/*
				Shader::sendUniform(scene->object[i]->getShader(), ("lights[" + std::to_string(lightIndex) + "].lightType").c_str(), scene->light2[lightIndex]->lightType);
				Shader::sendUniform(scene->object[i]->getShader(), ("lights[" + std::to_string(lightIndex) + "].position").c_str(), scene->light2[lightIndex]->position);
				Shader::sendUniform(scene->object[i]->getShader(), ("lights[" + std::to_string(lightIndex) + "].color").c_str(), scene->light2[lightIndex]->color);
				Shader::sendUniform(scene->object[i]->getShader(), ("lights[" + std::to_string(lightIndex) + "].attenuation").c_str(), scene->light2[lightIndex]->attenuation);
				Shader::sendUniform(scene->object[i]->getShader(), ("lights[" + std::to_string(lightIndex) + "].ambientCoefficient").c_str(), scene->light2[lightIndex]->ambientCoefficient);
				Shader::sendUniform(scene->object[i]->getShader(), ("lights[" + std::to_string(lightIndex) + "].coneAngle").c_str(), scene->light2[lightIndex]->coneAngle);
				Shader::sendUniform(scene->object[i]->getShader(), ("lights[" + std::to_string(lightIndex) + "].coneDirection").c_str(), scene->light2[lightIndex]->coneDirection);
				*/
			}

			Shader::sendUniform(scene->object[i]->getShader(), "viewMatrix", scene->camera[0]->getCamera());
			Shader::sendUniform(scene->object[i]->getShader(), "projectionMatrix", scene->camera[0]->getProjectionMatrix());
		}

		scene->object[i]->getModel().render();
	}

}


void Renderer::renderLoop()
{
	FPSCounter fpsCounter = FPSCounter();

	glEnable(GL_DEPTH_TEST);

	// Deprecated way how to render alpha channel image. Instead of this is implemented in fragment shader, to discard alpha channel for example if value is < 0.1
	// https://stackoverflow.com/questions/24302152/opengl-alpha-test-how-to-replace-alphafunc-deprecated/24312267
	// This function render the image with alpha blending to display only the alpha pixels.
	//glEnable(GL_BLEND);
	//glEnable(GL_ALPHA_TEST);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glAlphaFunc(GL_GREATER, 0.1f);
	// end alpha rendering

	//p�id�n� ID do stencil bufferu
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	
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
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// space color (gray)
		glClearStencil(0); // this is the default value
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glStencilFunc(GL_ALWAYS, -1, 0xFF);
		glDepthMask(GL_FALSE);
		scene->skybox->draw(scene->skyboxshader, scene->camera[0]);
		glDepthMask(GL_TRUE);
		//glClear(GL_DEPTH_BUFFER_BIT);

		// render objects
		for (int i = 0; i < scene->object.size(); i++)
		{
			glStencilFunc(GL_ALWAYS, i + 1, 0xFF);
			//glStencilFunc(GL_ALWAYS, scene->object[i]->getID(), 0xFF);
			renderObject(scene->object[i]);
		}

		// render 3D objects
		//renderModel();

		// 2021 new
		//glDepthMask(GL_FALSE);

		renderModel2(1);
		renderModel2(2);
		//glDisable(GL_ALPHA_TEST);
		//glDepthMask(GL_TRUE);
		//scene->meshModel1->render();

		// update other events like input handling
		glfwPollEvents();
		// put the stuff we�ve been drawing onto the display
		glfwSwapBuffers(window.get());
	}

	glfwDestroyWindow(window.get());
	glfwTerminate();
}

void Renderer::renderObject(std::shared_ptr<Object> object)
{
	object->render();
	/*object->getModel().bindVAO();
	object->useShader();
	object->getModel().render();		// glDrawArrays()*/
}


void Renderer::renderModel()
{
	glUseProgram(scene->programID);
	//scene->newMeshModel->Draw2(scene->programID);

	for (auto mesh : scene->meshes) {
		mesh->Draw2(scene->programID);
	}
	
	//scene->meshes[1]->Draw2(scene->programID);
	//scene->meshes[2]->Draw2(scene->programID);

	glm::mat4 ProjectionMatrix = scene->camera[0]->getProjectionMatrix();
	glm::mat4 ViewMatrix = scene->camera[0]->getCamera();
	glm::mat4 ModelMatrix = glm::mat4(1.0);

	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(scene->programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(scene->programID, "viewMatrix"), 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(scene->programID, "projectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

}

void Renderer::renderModel2(int model)
{
	glUseProgram(scene->programID);

	glm::mat4 ProjectionMatrix = scene->camera[0]->getProjectionMatrix();
	glm::mat4 ViewMatrix = scene->camera[0]->getCamera();
	glm::mat4 ModelMatrix = glm::mat4(1.0);

	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));

	if (model == 1) {
		scene->meshModel1->render();
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
	}	
	else {
		scene->meshModel2->render2();
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.1f, 0.0f));
	}
		

	Shader::sendUniform(scene->programID, "myTextureSampler", (GLint)0);
	/*for (auto mesh : scene->meshes) {
		mesh->Draw2(scene->programID);
	}*/

	//scene->meshes[1]->Draw2(scene->programID);
	//scene->meshes[2]->Draw2(scene->programID);

	
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(scene->programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(scene->programID, "viewMatrix"), 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(scene->programID, "projectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);

	//Shader::sendUniform(scene->programID, "lightPosition", scene->light[0]->getPosition());
	//Shader::sendUniform(scene->programID, "lightColor", scene->light[0]->lightColor);

	// FlashLight
	Shader::sendUniform(scene->programID, "flashLight.position", scene->camera[0]->getPosition() + scene->camera[0]->flashLight->getPosition());
	Shader::sendUniform(scene->programID, "flashLight.direction", scene->camera[0]->target);
	Shader::sendUniform(scene->programID, "flashLight.color", scene->camera[0]->flashLight->lightColor);

	Shader::sendUniform(scene->programID, "flashLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	Shader::sendUniform(scene->programID, "flashLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	Shader::sendUniform(scene->programID, "flashLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	Shader::sendUniform(scene->programID, "flashLight.cutOff", glm::cos(glm::radians(12.5f)));
	Shader::sendUniform(scene->programID, "flashLight.outerCutOff", glm::cos(glm::radians(15.0f)));
}

Renderer::~Renderer()
{
	std::cout << "Destructor of renderer" << std::endl;
}