#include <iostream>
#include <memory>
#include <filesystem>

#include "SOIL.h"

#include "Scene.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Object.hpp"
#include "Light.hpp"
#include "Model.hpp"
#include "Callback.hpp"
#include "ObjectFactory.hpp"

#include "SpotLight.hpp"
#include "DirectionalLight.hpp"
#include "TextureManager.hpp"
#include "ShaderLoader.hpp"

//#include "ObjLoader.hpp"

#include "glm/gtc/matrix_transform.hpp"

Scene::Scene()
{
	InitializeScene();
	std::cout << "Scene created." << std::endl;
}

Scene::~Scene()
{
	std::cout << "Deleting scene." << std::endl;
}

void Scene::InitializeScene()
{
	// Camera creation
	cameraSection();

	// Lights creation
	lightSection();

	// Objects creation
	objectSection();

	// Objects transformations and scales
	objectTransformSection();

	// 3D models from import
	modelSection();

	// Skybox creation
	skyboxSection();

	// Register to Observer only used shaders. Unused shaders we will skip. This will prevent us from manually registering shaders.
	registerUsedShaders();
	
}

void Scene::registerUsedShaders()
{
	Shader::getShader(ShaderType::FLAME);

	shaderPrograms = Shader::getShaderPrograms();

	for (ShaderProgram shaderProgram : shaderPrograms)
	{
		if (shaderProgram.shaderProgram > 0) {
			camera[0]->registerObserver(std::make_shared<ShaderProgram>(shaderProgram));
			printf("shadeprogram type register: %d\n", shaderProgram.shaderType);
		}
	}
}

void Scene::cameraSection()
{
	camera.push_back(std::shared_ptr<Camera>(new Camera(glm::vec3(0.0f, 0.6f, 0.0f))));		// glm::vec3(0.0f, 0.0f, 0.0f)
	camera[0]->setPerspectiveCamera();
	Callback::setCamera(camera[0]);
}

void Scene::lightSection()
{
	// setup lights
	std::shared_ptr<ObjectFactory> objectFactory = ObjectFactory::getInstance();

	glm::vec3 position = glm::vec3(0, 0, 1);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	float attenuation = 0.1f;
	float ambientCoefficient = 0.0f;
	float coneAngle = 15.0f;
	glm::vec3 coneDirection = glm::vec3(0.f, 0.f, -1.f);

	std::shared_ptr<Light> flashLight = objectFactory->createSpotLight(position, ShaderType::AMBIENT, lightColor, attenuation, ambientCoefficient, coneAngle, coneDirection);
	camera[0]->flashLight = std::static_pointer_cast<SpotLight>(flashLight);



	/*auto spotlight = std::shared_ptr<Light2>(new Light2());
	spotlight->lightType = 1;
	spotlight->position = glm::vec4(0, 0, 0, 1);
	spotlight->color = glm::vec3(1, 1, 1); //strong white light
	spotlight->attenuation = 0.1f;
	spotlight->ambientCoefficient = 0.0f; //no ambient light
	spotlight->coneAngle = 15.0f;
	spotlight->coneDirection = glm::vec3(0, 0, -1);
	*/


	// directional light directions
	// z kama kam to sviti
	glm::vec3 direction_up_down = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 direction_down_up = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 direction_right_to_left = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 direction_left_to_right = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 direction_front_to_back = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 direction_back_to_front = glm::vec3(-1.0f, 0.0f, 0.0f);

	// directional light
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientCoefficient = 0.0f;
	std::shared_ptr<Light> directionalLightSun = objectFactory->createDirectionalLight(ShaderType::AMBIENT, lightColor, direction_front_to_back + glm::vec3(0.0f, 0.1f, 0.0f), ambientCoefficient);
	auto newDirectionalLightSun = std::static_pointer_cast<DirectionalLight>(directionalLightSun);
	directionalLight.push_back(newDirectionalLightSun);


	/*auto directionalLight = std::shared_ptr<Light2>(new Light2());
	directionalLight->lightType = 0;
	directionalLight->position = glm::vec4(0.0, 0.0, 0.0, 0.0); //w == 0 indications a directional light
	directionalLight->color = glm::vec3(0.4, 0.3, 1.0); //weak yellowish light
	directionalLight->ambientCoefficient = 0.06;
	
	//light2.push_back(spotlight);	// flashlight
	light2.push_back(directionalLight);
	*/


	// light
	// Point light
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	attenuation = 0.1f;
	ambientCoefficient = 0.1f;
	glm::vec3 objectColor(1.0f, 1.0f, 1.0f);

	std::shared_ptr<Light> pointLight1 = objectFactory->createPointLight("sphere", ShaderType::AMBIENT, position, objectColor, lightColor, attenuation, ambientCoefficient);
	pointLight1->Scale(glm::vec3(0.02f, 0.02f, 0.02f));

	auto pointLight1p = std::static_pointer_cast<PointLight>(pointLight1);
	//object.push_back(pointLight1p);
	//camera[0]->registerObserver(pointLight1p);
	light.push_back(pointLight1p);


	/*Model lightModel("sphere");
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	shaderLightProgram = Shader::createShader(ShaderType::AMBIENT);

	attenuation = 0.1f;
	ambientCoefficient = 0.06;
	auto light1 = std::shared_ptr<Light>(new Light(glm::vec3(0.0f, 0.0f, 0.0f), lightModel, glm::vec3(1.0f, 1.0f, 1.0f), shaderLightProgram, ShaderType::AMBIENT, lightColor, ambientCoefficient)); // glm::vec3(1.0f, 1.0f, 1.0f)
	//light1->Scale(glm::vec3(0.05f, 0.05f, 0.05f));
	light1->Scale(glm::vec3(0.02f, 0.02f, 0.02f));

	//object.push_back(light1);
	light.push_back(light1);*/


	//camera[0]->registerObserver(light1);

	// light 2 - spot light
	position = glm::vec3(1.0f, 0.3f, 0.0f);
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	attenuation = 0.1f;
	ambientCoefficient = 0.1f;
	coneAngle = 15.0f;
	coneDirection = glm::vec3(0.0f, -1.0f, 0.0f); // direction_up_down;
	objectColor = glm::vec3(0.5f, 1.0f, 0.5f);

	std::shared_ptr<Light> newSpotLight1 = objectFactory->createSpotLight("cube", ShaderType::AMBIENT, position, objectColor, lightColor, attenuation, ambientCoefficient, coneAngle, coneDirection);
	newSpotLight1->Scale(glm::vec3(0.03f, 0.01f, 0.03f));
	auto newSpotLight11 = std::static_pointer_cast<SpotLight>(newSpotLight1);
	spotLight.push_back(newSpotLight11);
	//object.push_back(newSpotLight11);	// added
	//camera[0]->registerObserver(newSpotLight11);


	
	for (int i = 0; i < spotLight.size(); i++) {
		glBindVertexArray(0);
		spotLight[i]->getModel().bindVAO();
		Shader::use(spotLight[i]->getShader());
		//spotLight[i]->useShader();

		Shader::sendUniform(spotLight[i]->getShader(), "viewMatrix", camera[0]->getCamera());
		Shader::sendUniform(spotLight[i]->getShader(), "modelMatrix", spotLight[i]->getMatrix());
		Shader::sendUniform(spotLight[i]->getShader(), "fragmentColor", spotLight[i]->lightColor);	// getColor()
		Shader::sendUniform(spotLight[i]->getShader(), "ambientStrength", 1.0f);
		Shader::sendUniform(spotLight[i]->getShader(), "projectionMatrix", camera[0]->getProjectionMatrix());
		spotLight[i]->getModel().render();	//	glDrawArrays(GL_TRIANGLES, 0, 2880);
	}

	/*Model lightModel2("cube");
	GLuint shaderLightProgram2 = Shader::createShader(ShaderType::AMBIENT);
	auto light2 = std::shared_ptr<Light>(new Light(glm::vec3(1.0f, 0.0f, 0.0f), lightModel2, glm::vec3(1.0f, 1.0f, 1.0f), shaderLightProgram2, ShaderType::AMBIENT, lightColor, ambientCoefficient)); // glm::vec3(1.0f, 1.0f, 1.0f)
	light2->Scale(glm::vec3(0.05f, 0.05f, 0.05f));

	//object.push_back(light2);
	light.push_back(light2);
	camera[0]->registerObserver(light2);*/


	for (int i = 0; i < light.size(); i++) {
		glBindVertexArray(0);
		light[i]->getModel().bindVAO();
		light[i]->useShader();

		Shader::sendUniform(light[i]->getShader(), "viewMatrix", camera[0]->getCamera());	// shaderLightProgram
		Shader::sendUniform(light[i]->getShader(), "modelMatrix", light[i]->getMatrix());
		Shader::sendUniform(light[i]->getShader(), "fragmentColor", light[i]->lightColor);	// getColor()
		Shader::sendUniform(light[i]->getShader(), "ambientStrength", 1.0f);
		Shader::sendUniform(light[i]->getShader(), "projectionMatrix", camera[0]->getProjectionMatrix());
		light[i]->getModel().render();	//	glDrawArrays(GL_TRIANGLES, 0, 2880);
	}
}

void Scene::objectSection()
{
	/*glm::vec3 pos = glm::vec3(-0.6f, 0.f, 0.25f);	// glm::vec3 pos = glm::vec3(-0.25f, 0.f, 0.25f);
	glm::vec3 color = glm::vec3(0.8f, 0.8f, 0.8f);
	addObject("sphere", ShaderType::AMBIENT, pos, color, camera[0], glm::vec3(1.0f));

	pos = glm::vec3(0.25f, 0.f, 0.25f);
	color = glm::vec3(0.2f, 0.1f, 1.0f);
	addObject("sphere", ShaderType::DIFFUSE, pos, color, camera[0], glm::vec3(1.0f));

	pos = glm::vec3(-0.25f, 0.f, -0.25f);
	color = glm::vec3(0.7f, 0.0f, 0.5f);
	addObject("sphere", ShaderType::SPECULAR, pos, color, camera[0], glm::vec3(1.0f));

	pos = glm::vec3(0.25f, 0.f, -0.25f);
	color = glm::vec3(0.8f, 0.0f, 0.0f);
	addObject("sphere", ShaderType::PHONG, pos, color, camera[0], glm::vec3(1.0f));

	pos = glm::vec3(0.0f, 0.0f, 0.0f);
	color = glm::vec3(0.2f, 1.0f, 0.2f);
	//addObject("plain", ShaderType::PHONG, pos, color, camera[0], glm::vec3(1.0f));
	std::string texture1 = "floor\\floor2.jpg";
	addObject("plainTextured", ShaderType::PHONG, glm::vec3(0.0f, -10.0f, 0.0f), color, camera[0], glm::vec3(1.0f), texture1);	// podlaha

	addObject("suzi_smooth", ShaderType::PHONG, pos, glm::vec3(1.0f, 1.0f, 0.2f), camera[0], glm::vec3(1.0f));

	addObject("suzi_flat", ShaderType::PHONG, pos, glm::vec3(0.2f, 0.5f, 1.0f), camera[0], glm::vec3(1.0f));

	std::string texture2 = "floor\\floor1.jpg";
	addObject("plainTextured", ShaderType::PHONG, pos, glm::vec3(0.2f, 0.1f, 1.0f), camera[0], glm::vec3(1.0f), texture2);	// texture1

	//std::string texture1 = "floor\\floor1.jpg";
	//addObject("bedna", ShaderType::PHONG, pos, glm::vec3(0.2f, 0.1f, 1.0f), camera[0], glm::vec3(1.0f), texture1);

	addObject("worker", ShaderType::PHONG, pos, glm::vec3(0.2f, 0.3f, 0.7f), camera[0], glm::vec3(1.0f));

	//std::string texture3 = "floor\\floor1.jpg";
	//addObject("cube", ShaderType::SKYBOX, pos, color, camera[0], glm::vec3(1.0f));
	*/
}

void Scene::objectTransformSection()
{
	/*
	// 4 spheres
	glm::vec3 sphereScale = glm::vec3(0.2f, 0.2f, 0.2f);
	object[0]->Scale(sphereScale);
	object[1]->Scale(sphereScale);
	object[2]->Scale(sphereScale);
	object[3]->Scale(sphereScale);


	// object translate and scale
	// plane
	object[4]->Translate(glm::vec3(0.0f, -0.20f, 0.0f));
	object[4]->Scale(glm::vec3(10.0f, 1.0f, 10.0f));

	// monkeys
	object[5]->Translate(glm::vec3(-1.0f, 0.0f, 0.25f));
	object[5]->Scale(glm::vec3(0.10f, 0.10f, 0.10f));

	object[6]->Translate(glm::vec3(-2.0, 0, 0.0));
	object[6]->Scale(glm::vec3(0.10f, 0.10f, 0.10f));

	// bedna
	object[7]->Translate(glm::vec3(1.0, 0, 1.0));
	object[7]->Scale(glm::vec3(0.20f, 0.20f, 0.20f));

	// worker
	object[8]->Translate(glm::vec3(1.5, 0, 1.0));
	object[8]->Scale(glm::vec3(0.20f, 0.20f, 0.20f));
	// end translate, scale
	*/

	object.push_back(light[0]);	// <- odkomentovat
	//camera[0]->registerObserver(light[0]);

	object.push_back(spotLight[0]);	// added
	//camera[0]->registerObserver(spotLight[0]);

	object.push_back(directionalLight[0]);
	//camera[0]->registerObserver(directionalLight[0]);

	// Apply initial transformations and scales on objects
	camera[0]->notifyObservers(camera[0].get(), camChange::MOVE_ROTATE);
}

void Scene::addObject(std::string modelName, ShaderType shaderType, glm::vec3 position, glm::vec3 color, std::shared_ptr<Camera> camera, glm::vec3 scale, std::string texturePath)
{	// pouzito pro vytvareni objektu za runtimu
	//ObjectFactory objectFactory;
	std::shared_ptr<ObjectFactory> objectFactory = ObjectFactory::getInstance();
	std::shared_ptr<Object> newObject;
	if (texturePath == "")
	{
		newObject = objectFactory->createObject(modelName, shaderType, position, color);
	}
	else	// textured model
	{
		newObject = objectFactory->createObject(modelName, shaderType, position, texturePath);
	}

	object.push_back(newObject);
	

	object.back()->Scale(scale);
	//object.back()->Translate(glm::vec3(0.f, 0.f, -1.25f));

	glBindVertexArray(0);
	object.back()->getModel().bindVAO();
	object.back()->useShader();		//glUseProgram(scene->shaderProgram);


	GLuint objShaderProgram = object.back()->getShader();

	ShaderType shadType = object.back()->getShaderType();
	if (shadType == ShaderType::AMBIENT)
	{
		// vertex scene->shader uniforms
		Shader::sendUniform(objShaderProgram, "modelMatrix", object.back()->getMatrix());
		Shader::sendUniform(objShaderProgram, "viewMatrix", camera->getCamera());
		Shader::sendUniform(objShaderProgram, "projectionMatrix", camera->getProjectionMatrix());

		// fragment scene->shader uniforms
		Shader::sendUniform(objShaderProgram, "fragmentColor", object.back()->getColor());
	}
	else if (shadType == ShaderType::DIFFUSE)
	{
		// vertex scene->shader uniforms
		Shader::sendUniform(objShaderProgram, "modelMatrix", object.back()->getMatrix());
		Shader::sendUniform(objShaderProgram, "viewMatrix", camera->getCamera());
		Shader::sendUniform(objShaderProgram, "projectionMatrix", camera->getProjectionMatrix());

		// fragment scene->shader uniforms
		Shader::sendUniform(objShaderProgram, "fragmentColor", object.back()->getColor());

		Shader::sendUniform(objShaderProgram, "lightPosition", light[0]->getPosition());
		Shader::sendUniform(objShaderProgram, "lightColor", light[0]->lightColor);


		// added
		// direction light
		for (int dirIndex = 0; dirIndex < directionalLight.size(); dirIndex++) {
			Shader::sendUniform(objShaderProgram, "dirLight.direction", directionalLight[dirIndex]->direction);
			Shader::sendUniform(objShaderProgram, "dirLight.color", directionalLight[dirIndex]->lightColor);
			Shader::sendUniform(objShaderProgram, "dirLight.ambient", directionalLight[dirIndex]->ambientCoefficient);
			Shader::sendUniform(objShaderProgram, "dirLight.diffuse", glm::vec3(0.2f, 0.2f, 0.2f));	// glm::vec3(0.2f, 0.2f, 0.2f)
			Shader::sendUniform(objShaderProgram, "dirLight.specular", glm::vec3(0.2f, 0.2f, 0.2f));
		}

		// spotLight
		Shader::sendUniform(objShaderProgram, "spotLightCount", static_cast<GLint>(spotLight.size()));

		for (int spotIndex = 0; spotIndex < spotLight.size(); spotIndex++) {
			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].position").c_str(), spotLight[spotIndex]->getPosition());
			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].direction").c_str(), spotLight[spotIndex]->coneDirection);
			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].color").c_str(), spotLight[spotIndex]->lightColor);
			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].ambient").c_str(), glm::vec3(0.0f, 0.0f, 0.0f));
			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].diffuse").c_str(), glm::vec3(0.5f, 0.0f, 0.5f));
			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].specular").c_str(), glm::vec3(0.5f, 0.0f, 0.5f));

			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].constant").c_str(), 1.0f);
			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].linear").c_str(), 0.09f);
			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].quadratic").c_str(), 0.032f);

			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].cutOff").c_str(), glm::cos(glm::radians(12.5f)));
			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].outerCutOff").c_str(), glm::cos(glm::radians(15.0f)));
		}

		// FlashLight
		Shader::sendUniform(objShaderProgram, "flashLight.position", camera->getPosition() + camera->flashLight->getPosition());
		Shader::sendUniform(objShaderProgram, "flashLight.direction", camera->target);
		Shader::sendUniform(objShaderProgram, "flashLight.color", camera->flashLight->lightColor);

		Shader::sendUniform(objShaderProgram, "flashLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		Shader::sendUniform(objShaderProgram, "flashLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		Shader::sendUniform(objShaderProgram, "flashLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		Shader::sendUniform(objShaderProgram, "flashLight.constant", 1.0f);
		Shader::sendUniform(objShaderProgram, "flashLight.linear", 0.09f);
		Shader::sendUniform(objShaderProgram, "flashLight.quadratic", 0.032f);

		Shader::sendUniform(objShaderProgram, "flashLight.cutOff", glm::cos(glm::radians(12.5f)));
		Shader::sendUniform(objShaderProgram, "flashLight.outerCutOff", glm::cos(glm::radians(15.0f)));


		Shader::sendUniform(objShaderProgram, "pointLightCount", static_cast<GLint>(light.size()));

		// point lights
		for (int lightIndex = 0; lightIndex < light.size(); lightIndex++) {
			Shader::sendUniform(objShaderProgram, ("pointLight[" + std::to_string(lightIndex) + "].lightType").c_str(), static_cast<int>(light[lightIndex]->getLightType()));
			Shader::sendUniform(objShaderProgram, ("pointLight[" + std::to_string(lightIndex) + "].position").c_str(), light[lightIndex]->getPosition());
			Shader::sendUniform(objShaderProgram, ("pointLight[" + std::to_string(lightIndex) + "].color").c_str(), light[lightIndex]->lightColor);
			Shader::sendUniform(objShaderProgram, ("pointLight[" + std::to_string(lightIndex) + "].constant").c_str(), 1.0f);
			Shader::sendUniform(objShaderProgram, ("pointLight[" + std::to_string(lightIndex) + "].linear").c_str(), 0.09f);
			Shader::sendUniform(objShaderProgram, ("pointLight[" + std::to_string(lightIndex) + "].quadratic").c_str(), 0.032f);
		}
		// end added
	}
	else if (shadType == ShaderType::SPECULAR)
	{
		// vertex scene->shader uniforms
		Shader::sendUniform(objShaderProgram, "modelMatrix", object.back()->getMatrix());
		Shader::sendUniform(objShaderProgram, "viewMatrix", camera->getCamera());
		Shader::sendUniform(objShaderProgram, "projectionMatrix", camera->getProjectionMatrix());

		// fragment scene->shader uniforms
		Shader::sendUniform(objShaderProgram, "fragmentColor", object.back()->getColor());

		Shader::sendUniform(objShaderProgram, "lightPosition", light[0]->getPosition());
		Shader::sendUniform(objShaderProgram, "lightColor", light[0]->lightColor);

		Shader::sendUniform(objShaderProgram, "viewPos", camera->getPosition());
		Shader::sendUniform(objShaderProgram, "specularStrength", 0.5f);
	}
	else if (shadType == ShaderType::PHONG)
	{
		Shader::sendUniform(objShaderProgram, "modelMatrix", object.back()->getMatrix());
		Shader::sendUniform(objShaderProgram, "fragmentColor", object.back()->getColor()); //glm::vec3(1.f, 0.f, 0.f)); // 
		std::cout << "Fragment color: " << object.back()->getColor().x << " " << object.back()->getColor().y << " " << object.back()->getColor().z << "\n";

		Shader::sendUniform(objShaderProgram, "viewPos", camera->getPosition());

		Shader::sendUniform(objShaderProgram, "viewMatrix", camera->getCamera());
		Shader::sendUniform(objShaderProgram, "projectionMatrix", camera->getProjectionMatrix());


		// direction light
		for (int dirIndex = 0; dirIndex < directionalLight.size(); dirIndex++) {
			Shader::sendUniform(objShaderProgram, "dirLight.direction", directionalLight[dirIndex]->direction);
			Shader::sendUniform(objShaderProgram, "dirLight.color", directionalLight[dirIndex]->lightColor);
			Shader::sendUniform(objShaderProgram, "dirLight.ambient", directionalLight[dirIndex]->ambientCoefficient);
			Shader::sendUniform(objShaderProgram, "dirLight.diffuse", glm::vec3(0.2f, 0.2f, 0.2f));	// glm::vec3(0.2f, 0.2f, 0.2f)
			Shader::sendUniform(objShaderProgram, "dirLight.specular", glm::vec3(0.2f, 0.2f, 0.2f));
		}
		/*Shader::sendUniform(object.back()->getShader(), "dirLight.direction", direction_front_to_back);
		Shader::sendUniform(object.back()->getShader(), "dirLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		Shader::sendUniform(object.back()->getShader(), "dirLight.diffuse", glm::vec3(0.2f, 0.2f, 0.2));
		Shader::sendUniform(object.back()->getShader(), "dirLight.specular", glm::vec3(0.2f, 0.2f, 0.2f));*/

		// spotLight
		Shader::sendUniform(objShaderProgram, "spotLightCount", static_cast<GLint>(spotLight.size()));

		for (int spotIndex = 0; spotIndex < spotLight.size(); spotIndex++) {
			Shader::sendUniform(object.back()->getShader(), ("spotLight[" + std::to_string(spotIndex) + "].position").c_str(), spotLight[spotIndex]->getPosition());
			Shader::sendUniform(object.back()->getShader(), ("spotLight[" + std::to_string(spotIndex) + "].direction").c_str(), spotLight[spotIndex]->coneDirection);
			Shader::sendUniform(object.back()->getShader(), ("spotLight[" + std::to_string(spotIndex) + "].color").c_str(), spotLight[spotIndex]->lightColor);
			Shader::sendUniform(object.back()->getShader(), ("spotLight[" + std::to_string(spotIndex) + "].ambient").c_str(), glm::vec3(0.0f, 0.0f, 0.0f));
			Shader::sendUniform(object.back()->getShader(), ("spotLight[" + std::to_string(spotIndex) + "].diffuse").c_str(), glm::vec3(0.5f, 0.0f, 0.5f));
			Shader::sendUniform(object.back()->getShader(), ("spotLight[" + std::to_string(spotIndex) + "].specular").c_str(), glm::vec3(0.5f, 0.0f, 0.5f));

			Shader::sendUniform(object.back()->getShader(), ("spotLight[" + std::to_string(spotIndex) + "].constant").c_str(), 1.0f);
			Shader::sendUniform(object.back()->getShader(), ("spotLight[" + std::to_string(spotIndex) + "].linear").c_str(), 0.09f);
			Shader::sendUniform(object.back()->getShader(), ("spotLight[" + std::to_string(spotIndex) + "].quadratic").c_str(), 0.032f);

			Shader::sendUniform(object.back()->getShader(), ("spotLight[" + std::to_string(spotIndex) + "].cutOff").c_str(), glm::cos(glm::radians(12.5f)));
			Shader::sendUniform(object.back()->getShader(), ("spotLight[" + std::to_string(spotIndex) + "].outerCutOff").c_str(), glm::cos(glm::radians(15.0f)));
		}

		// FlashLight
		Shader::sendUniform(object.back()->getShader(), "flashLight.position", camera->getPosition() + camera->flashLight->getPosition());
		Shader::sendUniform(object.back()->getShader(), "flashLight.direction", camera->target);
		Shader::sendUniform(object.back()->getShader(), "flashLight.color", camera->flashLight->lightColor);

		Shader::sendUniform(object.back()->getShader(), "flashLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
		Shader::sendUniform(object.back()->getShader(), "flashLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		Shader::sendUniform(object.back()->getShader(), "flashLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		Shader::sendUniform(object.back()->getShader(), "flashLight.constant", 1.0f);
		Shader::sendUniform(object.back()->getShader(), "flashLight.linear", 0.09f);
		Shader::sendUniform(object.back()->getShader(), "flashLight.quadratic", 0.032f);
		Shader::sendUniform(object.back()->getShader(), "flashLight.cutOff", glm::cos(glm::radians(12.5f)));
		Shader::sendUniform(object.back()->getShader(), "flashLight.outerCutOff", glm::cos(glm::radians(15.0f)));


		Shader::sendUniform(object.back()->getShader(), "pointLightCount", static_cast<GLint>(light.size()));

		// point lights
		for (int lightIndex = 0; lightIndex < light.size(); lightIndex++) {
			Shader::sendUniform(object.back()->getShader(), ("pointLight[" + std::to_string(lightIndex) + "].lightType").c_str(), static_cast<int>(light[lightIndex]->getLightType()));
			Shader::sendUniform(object.back()->getShader(), ("pointLight[" + std::to_string(lightIndex) + "].position").c_str(), light[lightIndex]->getPosition());
			Shader::sendUniform(object.back()->getShader(), ("pointLight[" + std::to_string(lightIndex) + "].color").c_str(), light[lightIndex]->lightColor);
			Shader::sendUniform(object.back()->getShader(), ("pointLight[" + std::to_string(lightIndex) + "].constant").c_str(), 1.0f);
			Shader::sendUniform(object.back()->getShader(), ("pointLight[" + std::to_string(lightIndex) + "].linear").c_str(), 0.09f);
			Shader::sendUniform(object.back()->getShader(), ("pointLight[" + std::to_string(lightIndex) + "].quadratic").c_str(), 0.032f);
		}

		// texture load
		if (newObject->hasTexture())
		{
			newObject->loadTexture();
			Shader::sendUniform(newObject->getShader(), "hasTexture", 1);
			//Set texture unit to fragment shader

			// newObject->texture->getID()
			Shader::sendUniform(newObject->getShader(), "textureUnitID", 0);
			//GLint uniformID = glGetUniformLocation(object.back()->getShader(), "textureUnitID");
			//glUniform1i(uniformID, 0);
			//glUniform1i(uniformID, 0);
			//newObject->hasTexture = true;
		}
		else
		{
			Shader::sendUniform(object.back()->getShader(), "hasTexture", 0);
		}
		// end added
	}

	//camera->registerObserver(object.back());	// funguje s timto
	object.back()->getModel().render();
}

void Scene::skyboxSection()
{
	//std::unordered_map<GLenum, std::string> shaders;
	//shaders[GL_VERTEX_SHADER] = "../shaders/sky_vertex.glsl";
	//shaders[GL_FRAGMENT_SHADER] = "../shaders/sky_fragment.glsl";

	//skybox = std::make_shared<SkyBox>("jpg", "countryside2");
	skybox = std::make_shared<SkyBox>("tga", "env", 1.0f);
	//skybox->InitJpg(10, "countryside2");
	//skybox->InitTga(10, "env");
	GLuint shaderProgram = Shader::getShader(ShaderType::SKYBOX);
	ShaderProgram new_skyboxshader(ShaderType::SKYBOX, shaderProgram);
	skyboxshader = new_skyboxshader;
	

	//auto of = ObjectFactory::getInstance();
	//skybox2 = of->createSkybox("countryside2");

	//auto of = ObjectFactory::getInstance();
	//skybox2 = of->createSkybox("countryside2");
}

void Scene::modelSection()
{
	ShaderLoader sl;
	// Create and compile our GLSL program from the shaders
	//programID = sl.loadShader("shaders\\TransformVertexShader.vert", "shaders\\TextureFragmentShader.frag");
	
	//programID = Shader::getShader(ShaderType::DIFFUSE_MODEL);	// uncomment this, this worked well
	//Shader::use(programID);

	//ShaderProgram spModelID = ShaderProgram(ShaderType::DIFFUSE_MODEL, programID);
	//camera[0]->registerObserver(std::make_shared<ShaderProgram>(spModelID));

	//std::shared_ptr<ShaderProgram> shadPrg = std::shared_ptr<ShaderProgram>(new ShaderProgram(ShaderType::DIFFUSE_MODEL, programID));
	//camera[0]->registerObserver(shadPrg);

	//programID = sl.loadShader("shaders\\vertex.glsl", "shaders\\phong.frag");


	// Read our .obj file

	//bool res = loadOBJ("models\\cube\\test.obj", vertices, uvs, normals);	// "smazat\\cube.obj"
	//bool res2 = loadAssImp("models\\cube\\2\\Crate1.obj", indices, vertices, uvs, normals, material);	// "smazat\\cube.obj"

	//bool res2 = loadAssImp("models\\cube\\dum2\\dum2.obj", indices, vertices, uvs, normals, material, meshes);	// funguje - "models\\cube\\test.obj"
	//bool res2 = loadAssImp("models\\cube\\test.obj", indices, vertices, uvs, normals, material, meshes);
	//bool res2 = loadAssImp("models\\downloaded\\Indoor_plant_3\\Low-Poly Plant_.obj", indices, vertices, uvs, normals, material, meshes); // pouzite dnes 6. 2. 2021


	//meshModel1 = new MeshLoader("models\\downloaded\\Grass\\Grass.obj");


	// exporting from Blender into .obj with right coordinates:
	// Z forward
	// Y up
		   // .3DS
	//MeshLoader* newMeshModel = new MeshLoader("models\\downloaded\\open3dmodel\\Aylin\\Aylin.obj");

	//MeshLoader* newMeshModel = new MeshLoader("models\\downloaded\\open3dmodel\\Character_A1008A325\\mm3.obj");

	/*MeshLoader* newMeshModel = new MeshLoader("models\\chair\\chair.obj");
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 0.65f, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
	newMeshModel->ModelMatrix = ModelMatrix;
	//ModelMatrix = glm::rotate(ModelMatrix, 90.0f, glm::vec3(-1, 1, 1));
	meshObjects.push_back(newMeshModel);
	
	newMeshModel = new MeshLoader("models\\vase\\vase.obj");
	ModelMatrix = glm::mat4(1.0);
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-1.5f, 0.0f, 1.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
	newMeshModel->ModelMatrix = ModelMatrix;
	meshObjects.push_back(newMeshModel);*/

	MeshLoader* newMeshModel = new MeshLoader("models\\scena\\kniha_scena6.obj");
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0, 0.0f, 2.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));	// glm::vec3(0.1f, 0.1f, 0.1f)
	newMeshModel->ModelMatrix = ModelMatrix;
	//meshObjects.push_back(newMeshModel);
	meshObjects.push_back(std::shared_ptr<MeshLoader>(newMeshModel));
	zatahlyZaves = true;

	newMeshModel = new MeshLoader("models\\zavesy\\zaves_zatahly.obj");
	//newMeshModel->stencilNumber = 10;
	ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0f, 2.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
	newMeshModel->ModelMatrix = ModelMatrix;
	//meshObjects.push_back(newMeshModel);
	meshObjects.push_back(std::shared_ptr<MeshLoader>(newMeshModel));

	newMeshModel = new MeshLoader("models\\zavesy\\zaves_odtahly.obj");
	ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0f, 2.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
	newMeshModel->ModelMatrix = ModelMatrix;
	//meshObjects.push_back(newMeshModel);
	meshObjects.push_back(std::shared_ptr<MeshLoader>(newMeshModel));

	newMeshModel = new MeshLoader("models\\scena\\krb.obj");
	ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0f, 2.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));	// glm::vec3(0.1f, 0.1f, 0.1f)
	newMeshModel->ModelMatrix = ModelMatrix;
	//meshObjects.push_back(newMeshModel);
	meshObjects.push_back(std::shared_ptr<MeshLoader>(newMeshModel));

	//newMeshModel = new MeshLoader("models\\chair\\chair.obj");
	newMeshModel = new MeshLoader("models\\scena\\vetev2.obj");
	ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-1.25f, 0.26f, 0.6f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));
	newMeshModel->ModelMatrix = ModelMatrix;
	//newMeshModel->shaderProgramID = 12;
	//meshObjects.push_back(newMeshModel);
	meshObjects.push_back(std::shared_ptr<MeshLoader>(newMeshModel));

	newMeshModel = new MeshLoader("models\\scena\\flame.obj");
	ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0f, 2.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.05f, 0.05f, 0.05f));	// glm::vec3(0.1f, 0.1f, 0.1f)
	newMeshModel->ModelMatrix = ModelMatrix;
	meshObjects.push_back(std::shared_ptr<MeshLoader>(newMeshModel));


	/*newMeshModel = new MeshLoader("models\\downloaded\\Indoor_plant_3\\Low-Poly Plant_.obj");
	//ModelMatrix = glm::translate(ModelMatrix, glm::vec3(1.0f, 0.2f, 0.0f));
	ModelMatrix = glm::scale(glm::mat4(1.0), glm::vec3(0.6f, 0.6f, 0.6f));
	newMeshModel->ModelMatrix = ModelMatrix;
	meshObjects.push_back(newMeshModel);*/

	/*newMeshModel = new MeshLoader("models\\downloaded\\Indoor_plant_3\\Indoor plant_3_BI_blend2.obj");
	ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(-1.9f, 0.07f, 3.8f)); // (dopredu/dozadu, nahoru/dolu, doprava/doleva)
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.7f, 0.7f, 0.7f));
	newMeshModel->ModelMatrix = ModelMatrix;
	meshObjects.push_back(newMeshModel);*/

	/*newMeshModel = new MeshLoader("models\\cube\\dum2\\dum2.obj");
	ModelMatrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, -0.4f, 0.0f));
	newMeshModel->ModelMatrix = ModelMatrix;
	//meshObjects.push_back(newMeshModel);	// uncomment to import house into scene
	*/

	//meshModel1 = new MeshLoader("models\\downloaded\\Grass\\Grass.obj");
	
	// z kama kam to sviti
	glm::vec3 direction_up_down = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 direction_down_up = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 direction_right_to_left = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 direction_left_to_right = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 direction_front_to_back = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 direction_back_to_front = glm::vec3(-1.0f, 0.0f, 0.0f);

	//glm::vec3 direction_front_to_back_up_half_down = glm::vec3(1.0f, 1.0f, 0.0f);
	glm::vec3 direction_front_to_back_up_half_down = glm::vec3(1.0f, 1.0f, 0.0f);

	Shader::sendUniform(newMeshModel->shaderProgramID, "dirLight.direction", direction_front_to_back_up_half_down);
	Shader::sendUniform(newMeshModel->shaderProgramID, "dirLight.color", glm::vec3(1.0f, 1.0f, 1.0f));
	Shader::sendUniform(newMeshModel->shaderProgramID, "dirLight.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	Shader::sendUniform(newMeshModel->shaderProgramID, "dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5));
	Shader::sendUniform(newMeshModel->shaderProgramID, "dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	//Shader::sendUniform(newMeshModel->shaderProgramID, "viewMatrix2", camera[0]->getProjectionMatrix());
	
}

//void Scene::skybox

void Scene::deleteObject(int stencilID)
{
	camera[0]->dropObject();

	for (int i = 0; i < object.size(); i++) {
		if (object[i]->getID() == (stencilID - 1)) {
			//camera[0]->removeObserver(object[i]);
			object.erase(object.begin() + i);
			return;
		}
	}
}

/*void Scene::rotateObject(int index, float rotxangle, glm::vec3 axis)
{
	//this->object[index]->setPosition(newPosition);
	this->object[index]->Rotate(rotxangle, axis);
}

void Scene::moveObject(int index, glm::vec3 newPosition)
{
	
	this->object[index]->Translate(newPosition);
	this->object[index]->update(&*camera[0], camChange::MOVE_ROTATE);

	this->object[index]->getModel().bindVAO();
	this->object[index]->useShader();
	this->object[index]->getModel().render();
}*/

void Scene::setNewColor(int _index)
{
	if (_index <= 0) {
		return;
	}
	if (object.size() > _index - 1) {
		std::cout << _index << std::endl;
		int index = _index - 1;// -1;
		this->object[index]->setNewColor(glm::vec3(1.0f, 0.0f, 0.0f));

		this->object[index]->useShader();	 // added mb smazat
		Shader::sendUniform(this->object[index]->getShader(), "fragmentColor", this->object[index]->getColor());
	}
	else {
		int index = _index - 1;
		//meshObjects[index].;

	}
	/*if (std::dynamic_pointer_cast<Light>(this->object[index]))
	{
		Shader::sendUniform(this->object[index]->getShader(), "lightColor", glm::vec3(1.0f, 0.0f, 0.0f));

		camera[0]->notifyObservers(&*camera[0], camChange::MOVE_ROTATE);
	}*/
}

void Scene::setLastColor(int _index)
{
	if (_index <= 0) {
		return;
	}
	if (object.size() > _index - 1) {
		int index = _index - 1; // -1;
		this->object[index]->setLastColor();

		this->object[index]->useShader();	 // added mb smazat
		Shader::sendUniform(this->object[index]->getShader(), "fragmentColor", this->object[index]->getColor());
	}
}

