#include <iostream>
#include <memory>

#include "Scene.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Object.hpp"
#include "Light.hpp"
#include "Model.hpp"
#include "Callback.hpp"
#include "ObjectFactory.hpp"

#include "PointLight.hpp"
#include "SpotLight.hpp"


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
	camera.push_back(std::shared_ptr<Camera>(new Camera(glm::vec3(0.0f, 0.0f, 0.0f))));
	camera[0]->setPerspectiveCamera();
	Callback::setCamera(camera[0]);

	// Light creation
	// setup lights
	auto spotlight = std::shared_ptr<Light2>(new Light2());
	spotlight->lightType = 1;
	spotlight->position = glm::vec4(0, 0, 0, 1);
	spotlight->color = glm::vec3(1, 1, 1); //strong white light
	spotlight->attenuation = 0.1f;
	spotlight->ambientCoefficient = 0.0f; //no ambient light
	spotlight->coneAngle = 15.0f;
	spotlight->coneDirection = glm::vec3(0, 0, -1);

	auto directionalLight = std::shared_ptr<Light2>(new Light2());
	directionalLight->lightType = 0;
	directionalLight->position = glm::vec4(0.0, 0.0, 0.0, 0.0); //w == 0 indications a directional light
	directionalLight->color = glm::vec3(0.4, 0.3, 1.0); //weak yellowish light
	directionalLight->ambientCoefficient = 0.06;

	light2.push_back(spotlight);
	light2.push_back(directionalLight);



	// light
	Model lightModel("sphere");
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	shaderLightProgram = Shader::createShader(ShaderType::AMBIENT, 2);

	auto light1 = std::shared_ptr<Light>(new Light(glm::vec3(0.0f, 0.0f, 0.0f), lightModel, glm::vec3(1.0f, 1.0f, 1.0f), shaderLightProgram, lightColor, ShaderType::AMBIENT, camera[0])); // glm::vec3(1.0f, 1.0f, 1.0f)
	light1->Scale(glm::vec3(0.05f, 0.05f, 0.05f));

	//object.push_back(light1);

	light.push_back(light1);

	camera[0]->registerObserver(light1);

	// light 2
	Model lightModel2("cube");
	GLuint shaderLightProgram2 = Shader::createShader(ShaderType::AMBIENT, 2);
	auto light2 = std::shared_ptr<Light>(new Light(glm::vec3(1.0f, 0.0f, 0.0f), lightModel2, glm::vec3(1.0f, 1.0f, 1.0f), shaderLightProgram2, lightColor, ShaderType::AMBIENT, camera[0])); // glm::vec3(1.0f, 1.0f, 1.0f)
	light2->Scale(glm::vec3(0.05f, 0.05f, 0.05f));

	//object.push_back(light2);
	light.push_back(light2);

	camera[0]->registerObserver(light2);

	//PointLight pl = PointLight()

	for (int i = 0; i < light.size(); i++) {
		light[i]->getModel().bindVAO();
		light[i]->useShader();

		Shader::sendUniform(shaderLightProgram, "viewMatrix", camera[0]->getCamera());
		Shader::sendUniform(shaderLightProgram, "modelMatrix", light[i]->getMatrix());
		Shader::sendUniform(shaderLightProgram, "fragmentColor", light[i]->lightColor);	// getColor()
		Shader::sendUniform(shaderLightProgram, "ambientStrength", 1.0f);
		Shader::sendUniform(shaderLightProgram, "projectionMatrix", camera[0]->getProjectionMatrix());
		light[i]->getModel().render();	//	glDrawArrays(GL_TRIANGLES, 0, 2880);
	}



	// Object creation

	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);

	ObjectFactory objectFactory;

	addObject("sphere", ShaderType::AMBIENT, glm::vec3(-0.25f, 0.f, 0.25f), glm::vec3(1.0f, 1.0f, 1.0f), camera[0], glm::vec3(1.0f));
	//object.push_back(objectFactory.createObject("sphere", ShaderType::AMBIENT, glm::vec3(-0.25f, 0.f, 0.25f), glm::vec3(1.0f, 1.0f, 1.0f)));	// horni leva - bila

	addObject("sphere", ShaderType::DIFFUSE, glm::vec3(0.25f, 0.f, 0.25f), glm::vec3(0.2f, 0.1f, 1.0f), camera[0], glm::vec3(1.0f));
	//object.push_back(objectFactory.createObject("sphere", ShaderType::DIFFUSE, glm::vec3(0.25f, 0.f, 0.25f), glm::vec3(0.2f, 0.1f, 1.0f)));	// horni prava - modra

	addObject("sphere", ShaderType::SPECULAR, glm::vec3(-0.25f, 0.f, -0.25f), glm::vec3(0.7f, 0.0f, 0.5f), camera[0], glm::vec3(1.0f));
	//object.push_back(objectFactory.createObject("sphere", ShaderType::SPECULAR, glm::vec3(-0.25f, 0.f, -0.25f), glm::vec3(0.7f, 0.0f, 0.5f)));	// dolni leva - 

	addObject("sphere", ShaderType::PHONG, glm::vec3(0.25f, 0.f, -0.25f), glm::vec3(0.8f, 0.0f, 0.0f), camera[0], glm::vec3(1.0f));
	//object.push_back(objectFactory.createObject("sphere", ShaderType::PHONG, glm::vec3(0.25f, 0.f, -0.25f), glm::vec3(0.8f, 0.0f, 0.0f)));	// dolni prava - cervena

	addObject("plain", ShaderType::PHONG, pos, glm::vec3(0.2f, 1.0f, 0.2f), camera[0], glm::vec3(1.0f));
	//object.push_back(objectFactory.createObject("plain", ShaderType::PHONG, pos, glm::vec3(0.2f, 1.0f, 0.2f)));


	addObject("suzi_smooth", ShaderType::PHONG, pos, glm::vec3(1.0f, 1.0f, 0.2f), camera[0], glm::vec3(1.0f));
	//object.push_back(objectFactory.createObject("suzi_smooth", ShaderType::PHONG, pos, glm::vec3(1.0f, 1.0f, 0.2f)));

	addObject("suzi_flat", ShaderType::PHONG, pos, glm::vec3(0.2f, 0.5f, 1.0f), camera[0], glm::vec3(1.0f));
	//object.push_back(objectFactory.createObject("suzi_flat", ShaderType::PHONG, pos, glm::vec3(0.2f, 0.5f, 1.0f)));

	addObject("bedna", ShaderType::PHONG, pos, glm::vec3(0.2f, 0.1f, 1.0f), camera[0], glm::vec3(1.0f));
	//object.push_back(objectFactory.createObject("bedna", ShaderType::PHONG, pos, glm::vec3(0.2f, 0.1f, 1.0f)));

	addObject("worker", ShaderType::PHONG, pos, glm::vec3(0.2f, 0.3f, 0.7f), camera[0], glm::vec3(1.0f));
	//object.push_back(objectFactory.createObject("worker", ShaderType::PHONG, pos, glm::vec3(0.2f, 0.3f, 0.7f)));


	// TODO: volani v rendereru na shader bude pres Staticke funkce
	// v objektu draw()
	

	/*
	for (int i = 0; i < this->object.size(); i++)
	{
		camera[0]->registerObserver(object[i]);
	}*/

	//camera[0]->setPerspectiveCamera();



	// Object transformation and scales

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
}


void Scene::addObject(std::string modelName, ShaderType shaderType, glm::vec3 position, glm::vec3 color, std::shared_ptr<Camera> camera, glm::vec3 scale)
{	// pouzito pro vytvareni objektu za runtimu
	ObjectFactory objectFactory;
	std::shared_ptr<Object> newObject = objectFactory.createObject(modelName, shaderType, position, color);
	object.push_back(newObject);
	

	object.back()->Scale(scale);
	//object.back()->Translate(glm::vec3(0.f, 0.f, -1.25f));

	glBindVertexArray(0);
	object.back()->getModel().bindVAO();
	object.back()->useShader();		//glUseProgram(scene->shaderProgram);
	ShaderType shadType = object.back()->getShaderType();
	
	if (shadType == ShaderType::AMBIENT)
	{
		// vertex scene->shader uniforms
		Shader::sendUniform(object.back()->getShader(), "modelMatrix", object.back()->getMatrix());
		Shader::sendUniform(object.back()->getShader(), "viewMatrix", camera->getCamera());
		Shader::sendUniform(object.back()->getShader(), "projectionMatrix", camera->getProjectionMatrix());

		// fragment scene->shader uniforms
		Shader::sendUniform(object.back()->getShader(), "fragmentColor", object.back()->getColor());
	}
	else if (shadType == ShaderType::DIFFUSE)
	{
		// vertex scene->shader uniforms
		Shader::sendUniform(object.back()->getShader(), "modelMatrix", object.back()->getMatrix());
		Shader::sendUniform(object.back()->getShader(), "viewMatrix", camera->getCamera());
		Shader::sendUniform(object.back()->getShader(), "projectionMatrix", camera->getProjectionMatrix());

		// fragment scene->shader uniforms
		Shader::sendUniform(object.back()->getShader(), "fragmentColor", object.back()->getColor());

		Shader::sendUniform(object.back()->getShader(), "lightPosition", light[0]->getPosition());
		Shader::sendUniform(object.back()->getShader(), "lightColor", light[0]->lightColor);
	}
	else if (shadType == ShaderType::SPECULAR)
	{
		// vertex scene->shader uniforms
		Shader::sendUniform(object.back()->getShader(), "modelMatrix", object.back()->getMatrix());
		Shader::sendUniform(object.back()->getShader(), "viewMatrix", camera->getCamera());
		Shader::sendUniform(object.back()->getShader(), "projectionMatrix", camera->getProjectionMatrix());

		// fragment scene->shader uniforms
		Shader::sendUniform(object.back()->getShader(), "fragmentColor", object.back()->getColor());

		Shader::sendUniform(object.back()->getShader(), "lightPosition", light[0]->getPosition());
		Shader::sendUniform(object.back()->getShader(), "lightColor", light[0]->lightColor);

		Shader::sendUniform(object.back()->getShader(), "viewPos", camera->getPosition());
		Shader::sendUniform(object.back()->getShader(), "specularStrength", 0.5f);
	}
	else if (shadType == ShaderType::PHONG)
	{
		Shader::sendUniform(object.back()->getShader(), "modelMatrix", object.back()->getMatrix());
		Shader::sendUniform(object.back()->getShader(), "fragmentColor", object.back()->getColor());
		Shader::sendUniform(object.back()->getShader(), "viewPos", camera->getPosition());

		// posilat jen pri zmnene

		Shader::sendUniform(object.back()->getShader(), "lightCount", static_cast<GLint>(light.size()));
		char buff[100] = {0};

		for (int i = 0; i < light.size(); i++) {
			sprintf_s(buff, sizeof buff, "lights[%d].position", i);
			Shader::sendUniform(object.back()->getShader(), buff, light[i]->getPosition());

			memset(buff, 0, sizeof buff);

			sprintf_s(buff, sizeof buff, "lights[%d].color", i);
			Shader::sendUniform(object.back()->getShader(), buff, light[i]->lightColor);
		}

		Shader::sendUniform(object.back()->getShader(), "viewMatrix", camera->getCamera());
		Shader::sendUniform(object.back()->getShader(), "projectionMatrix", camera->getProjectionMatrix());



		/// ////
		Shader::sendUniform(object.back()->getShader(), "lightCount", static_cast<GLint>(light.size()));

		// direction light
		// z kama kam to sviti
		glm::vec3 direction_up_down = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 direction_down_up = glm::vec3(0.0f, -1.0f, 0.0f);
		glm::vec3 direction_right_to_left = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 direction_left_to_right = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 direction_front_to_back = glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 direction_back_to_front = glm::vec3(-1.0f, 0.0f, 0.0f);

		Shader::sendUniform(object.back()->getShader(), "dirLight.direction", direction_front_to_back);
		Shader::sendUniform(object.back()->getShader(), "dirLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		Shader::sendUniform(object.back()->getShader(), "dirLight.diffuse", glm::vec3(0.2f, 0.2f, 0.2));
		Shader::sendUniform(object.back()->getShader(), "dirLight.specular", glm::vec3(0.2f, 0.2f, 0.2f));

		// spotLight
		Shader::sendUniform(object.back()->getShader(), "spotLight[0].position", camera->getPosition());
		Shader::sendUniform(object.back()->getShader(), "spotLight[0].direction", camera->target);
		Shader::sendUniform(object.back()->getShader(), "spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		Shader::sendUniform(object.back()->getShader(), "spotLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		Shader::sendUniform(object.back()->getShader(), "spotLight[0].specular", glm::vec3(1.0f, 1.0f, 1.0f));
		Shader::sendUniform(object.back()->getShader(), "spotLight[0].constant", 1.0f);
		Shader::sendUniform(object.back()->getShader(), "spotLight[0].linear", 0.09f);
		Shader::sendUniform(object.back()->getShader(), "spotLight[0].quadratic", 0.032f);
		Shader::sendUniform(object.back()->getShader(), "spotLight[0].cutOff", glm::cos(glm::radians(12.5f)));
		Shader::sendUniform(object.back()->getShader(), "spotLight[0].outerCutOff", glm::cos(glm::radians(15.0f)));

		// FlashLight
		Shader::sendUniform(object.back()->getShader(), "flashLight.position", camera->getPosition());
		Shader::sendUniform(object.back()->getShader(), "flashLight.direction", camera->target);
		Shader::sendUniform(object.back()->getShader(), "flashLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		Shader::sendUniform(object.back()->getShader(), "flashLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		Shader::sendUniform(object.back()->getShader(), "flashLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		Shader::sendUniform(object.back()->getShader(), "flashLight.constant", 1.0f);
		Shader::sendUniform(object.back()->getShader(), "flashLight.linear", 0.09f);
		Shader::sendUniform(object.back()->getShader(), "flashLight.quadratic", 0.032f);
		Shader::sendUniform(object.back()->getShader(), "flashLight.cutOff", glm::cos(glm::radians(12.5f)));
		Shader::sendUniform(object.back()->getShader(), "flashLight.outerCutOff", glm::cos(glm::radians(15.0f)));


		// point lights
		for (int lightIndex = 0; lightIndex < light.size() - 1; lightIndex++) {
			Shader::sendUniform(object.back()->getShader(), ("pointLight[" + std::to_string(lightIndex) + "].lightType").c_str(), light2[lightIndex]->lightType);
			Shader::sendUniform(object.back()->getShader(), ("pointLight[" + std::to_string(lightIndex) + "].position").c_str(), light2[lightIndex]->position);
			Shader::sendUniform(object.back()->getShader(), ("pointLight[" + std::to_string(lightIndex) + "].constant").c_str(), 1.0f);
			Shader::sendUniform(object.back()->getShader(), ("pointLight[" + std::to_string(lightIndex) + "].linear").c_str(), 0.09f);
			Shader::sendUniform(object.back()->getShader(), ("pointLight[" + std::to_string(lightIndex) + "].quadratic").c_str(), 0.032f);
		}
	}

	camera->registerObserver(object.back());
	object.back()->getModel().render();
}

void Scene::deleteObject(int stencilID)
{
	camera[0]->dropObject();

	for (int i = 0; i < object.size(); i++) {
		if (object[i]->getID() == (stencilID - 1)) {
			camera[0]->removeObserver(object[i]);
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
	std::cout << _index << std::endl;
	int index = _index - 1;// -1;
	this->object[index]->setNewColor(glm::vec3(1.0f, 0.0f, 0.0f));
	Shader::sendUniform(this->object[index]->getShader(), "fragmentColor", this->object[index]->getColor());

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

	int index = _index - 1;// -1;
	this->object[index]->setLastColor();
	Shader::sendUniform(this->object[index]->getShader(), "fragmentColor", this->object[index]->getColor());
}