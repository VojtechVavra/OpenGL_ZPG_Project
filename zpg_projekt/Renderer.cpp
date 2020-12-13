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

	// added stencil
	//pøidání ID do stencil bufferu
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
		//fpsCounter.drawFps(currentFrame);

		// input
		scene->camera[0]->processKeyboard(deltaTime);

		// clear color and depth buffer
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// space color (gray)
		glClearStencil(0); // this is the default value
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		// render objects
		for (int i = 0; i < scene->object.size(); i++)
		{
			// cubemap oblohy nedavat do stencilu

			/*glStencilFunc(GL_ALWAYS, -1, 0xFF);
			glDepthMask(GL_FALSE);
			scene->skybox->draw(scene->skyboxshader, scene->camera[0]);
			//scene->skybox2->renderSkybox(scene->camera[0]);
			glDepthMask(GL_TRUE);
			*/
			glDepthMask(GL_FALSE);
			scene->skybox->draw(scene->skyboxshader, scene->camera[0]);
			glDepthMask(GL_TRUE);


			glStencilFunc(GL_ALWAYS, i + 1, 0xFF);
			//glStencilFunc(GL_ALWAYS, scene->object[i]->getID(), 0xFF);
			renderObject(scene->object[i]);
			/*if (i == 9)
			{
				glDepthMask(GL_FALSE);
				//scene->skybox->draw(scene->skyboxshader, scene->camera[0]);
				//scene->object[i]->renderSkybox2(scene->camera[0]);
				renderObject(scene->object[i]);
				glDepthMask(GL_TRUE);
			}*/

			
			//  vykresleni pomoci index bufferu
			/**glBindVertexArray(scene->newModel);
			glActiveTexture(GL_TEXTURE0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			glBindTexture(GL_TEXTURE_2D, 0);
			scene->object[3]->loadTexture();
			Shader::sendUniform(scene->object[3]->getShader(), "hasTexture", 1);
			Shader::sendUniform(scene->object[3]->getShader(), "textureUnitID", 0);
			//shader.passUniformLocation("textureUnitID", std::int32_t(0));
			glDrawElements(GL_TRIANGLES, scene->indicesCount, GL_UNSIGNED_INT, NULL);
			glBindVertexArray(0);**/

			//glDepthMask(GL_FALSE);
			renderModel();
			//glDepthMask(GL_TRUE);

			// render the loaded model
			/*glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
			ourShader.setMat4("model", model);
			ourModel.Draw(ourShader);
			*/


			//glDisable(GL_DEPTH_TEST);
			//scene->skybox->draw(scene->skyboxshader, scene->camera[0]);
			//glEnable(GL_DEPTH_TEST);
		}

		// update other events like input handling
		glfwPollEvents();
		// put the stuff we’ve been drawing onto the display
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
	scene->newMeshModel->Draw(scene->programID);
	glm::mat4 ProjectionMatrix = scene->camera[0]->getProjectionMatrix();
	glm::mat4 ViewMatrix = scene->camera[0]->getCamera();
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(scene->programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(scene->programID, "viewMatrix"), 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(scene->programID, "projectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
	return;
	/*
	glBindVertexArray(scene->VertexArrayID);
	// Use our shader
	glUseProgram(scene->programID);

	// Compute the MVP matrix from keyboard and mouse input
	glm::mat4 ProjectionMatrix = scene->camera[0]->getProjectionMatrix();
	glm::mat4 ViewMatrix = scene->camera[0]->getCamera();
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;
	
	glUniformMatrix4fv(glGetUniformLocation(scene->programID, "modelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(scene->programID, "viewMatrix"), 1, GL_FALSE, &ViewMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(scene->programID, "projectionMatrix"), 1, GL_FALSE, &ProjectionMatrix[0][0]);
	*/


	//send3DobjUniforms(scene->programID, scene->camera[0], scene->light, scene->directionalLight, scene->spotLight, scene->texture, ModelMatrix);
	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform

	//glUniformMatrix4fv(scene->MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, scene->texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(scene->TextureID, 0);

	/*
	// vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
	// normal attribute
	// vezne 3 floaty z osmi ale vezne je s posunem vuci zacatku
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
	// uv - texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));
	*/


	// funguje
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, scene->vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : UVs
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, scene->uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	
	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, scene->vertices.size());

	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
}

Renderer::~Renderer()
{
	std::cout << "Destructor of renderer" << std::endl;
}