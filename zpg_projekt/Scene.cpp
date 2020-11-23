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
	Callback::setCamera(camera[0]);
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);

	ObjectFactory objectFactory;
	object.push_back(objectFactory.createObject("sphere", ShaderType::AMBIENT, glm::vec3(-0.25f, 0.f, 0.25f), glm::vec3(1.0f, 1.0f, 1.0f)));	// horni leva - bila
	object.push_back(objectFactory.createObject("sphere", ShaderType::DIFFUSE, glm::vec3(0.25f, 0.f, 0.25f), glm::vec3(0.2f, 0.1f, 1.0f)));	// horni prava - modra
	object.push_back(objectFactory.createObject("sphere", ShaderType::SPECULAR, glm::vec3(-0.25f, 0.f, -0.25f), glm::vec3(0.7f, 0.0f, 0.5f)));	// dolni leva - 
	object.push_back(objectFactory.createObject("sphere", ShaderType::PHONG, glm::vec3(0.25f, 0.f, -0.25f), glm::vec3(0.8f, 0.0f, 0.0f)));	// dolni prava - cervena

	object.push_back(objectFactory.createObject("plain", ShaderType::PHONG, pos, glm::vec3(0.2f, 1.0f, 0.2f)));
	object.push_back(objectFactory.createObject("suzi_smooth", ShaderType::PHONG, pos, glm::vec3(1.0f, 1.0f, 0.2f)));
	object.push_back(objectFactory.createObject("suzi_flat", ShaderType::PHONG, pos, glm::vec3(0.2f, 0.5f, 1.0f)));

	object.push_back(objectFactory.createObject("bedna", ShaderType::PHONG, pos, glm::vec3(0.2f, 0.1f, 1.0f)));
	object.push_back(objectFactory.createObject("worker", ShaderType::PHONG, pos, glm::vec3(0.2f, 0.3f, 0.7f)));


	// TODO: volani v rendereru na shader bude pres Staticke funkce
	// v objektu draw()
	

	for (int i = 0; i < this->object.size(); i++)
	{
		camera[0]->registerObserver(object[i]);
	}

	camera[0]->setPerspectiveCamera();


	// light
	Model lightModel("sphere");
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	shaderLightProgram = Shader::createShader(ShaderType::AMBIENT, 2);

	light = std::shared_ptr<Light>(new Light(glm::vec3(0.0f, 0.0f, 0.0f), lightModel, glm::vec3(1.0f, 1.0f, 1.0f), shaderLightProgram, lightColor, ShaderType::AMBIENT, camera[0])); // glm::vec3(1.0f, 1.0f, 1.0f)
	light->Scale(glm::vec3(0.05f, 0.05f, 0.05f));
	object.push_back(light);


	camera[0]->registerObserver(light);


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
{
	ObjectFactory objectFactory;
	object.push_back(objectFactory.createObject(modelName, shaderType, position, color));
	camera->registerObserver(object.back());

	object.back()->Scale(scale);
	//object.back()->Translate(glm::vec3(0.f, 0.f, -1.25f));

	glBindVertexArray(0);
	object.back()->getModel().bindVAO();
	object.back()->useShader();		//glUseProgram(scene->shaderProgram);
	ShaderType shadType = object.back()->getShaderType();
	
	if (shadType == ShaderType::PHONG)
	{
		Shader::sendUniform(object.back()->getShader(), "modelMatrix", object.back()->getMatrix());
		Shader::sendUniform(object.back()->getShader(), "fragmentColor", object.back()->getColor());
		Shader::sendUniform(object.back()->getShader(), "viewPos", camera->getPosition());
		// posilat jen pri zmnene
		Shader::sendUniform(object.back()->getShader(), "lightPosition", this->light->getPosition());
		Shader::sendUniform(object.back()->getShader(), "lightColor", this->light->lightColor);
		Shader::sendUniform(object.back()->getShader(), "viewMatrix", camera->getCamera());
		Shader::sendUniform(object.back()->getShader(), "projectionMatrix", camera->getProjectionMatrix());
	}

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

void Scene::rotateObject(int index, float rotxangle, glm::vec3 axis)
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
}

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