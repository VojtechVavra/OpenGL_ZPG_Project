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
	object.push_back(objectFactory.createObject("sphere", ShaderType::AMBIENT, glm::vec3(-0.25f, 0.f, 0.25f), glm::vec3(1.10f, 1.10f, 1.10f), camera[0]));	// horni leva 
	object.push_back(objectFactory.createObject("sphere", ShaderType::DIFFUSE, glm::vec3(0.25f, 0.f, 0.25f), glm::vec3(0.2f, 0.1f, 1.0f), camera[0]));	// horni prava 
	object.push_back(objectFactory.createObject("sphere", ShaderType::SPECULAR, glm::vec3(-0.25f, 0.f, -0.25f), glm::vec3(0.7f, 0.0f, 0.5f), camera[0]));	// dolni leva
	object.push_back(objectFactory.createObject("sphere", ShaderType::PHONG, glm::vec3(0.25f, 0.f, -0.25f), glm::vec3(0.8f, 0.0f, 0.0f), camera[0]));	// dolni prava

	object.push_back(objectFactory.createObject("plain", ShaderType::PHONG, pos, glm::vec3(0.2f, 1.0f, 0.2f), camera[0]));
	object.push_back(objectFactory.createObject("suzi_smooth", ShaderType::PHONG, pos, glm::vec3(1.0f, 1.0f, 0.2f), camera[0]));
	object.push_back(objectFactory.createObject("suzi_flat", ShaderType::PHONG, pos, glm::vec3(0.2f, 0.5f, 1.0f), camera[0]));



	// TODO: pouzit statickou funkci Shader::createShader() pro vytvoreni shaderu
	// a ziskanit GLunit shaderProgramID a ulozeni teto hodnoty do vytvoreneho objektu.
	// Dale pak volani v rendereru na shader bude pres Staticke funkce
	// v objektu draw()
	/*shader.push_back(Shader(&camera[0], ShaderType::AMBIENT));
	shader.push_back(Shader(&camera[0], ShaderType::DIFFUSE));
	shader.push_back(Shader(&camera[0], ShaderType::SPECULAR));
	shader.push_back(Shader(&camera[0], ShaderType::PHONG));
	*/

	/*camera[0].registerObserver(std::make_shared<Shader>(shader[0]));
	camera[0].registerObserver(std::make_shared<Shader>(shader[1]));
	camera[0].registerObserver(std::make_shared<Shader>(shader[2]));
	camera[0].registerObserver(std::make_shared<Shader>(shader[3]));
	*/
	/*camera[0]->registerObserver(std::make_shared<Object>(object[0]));
	camera[0]->registerObserver(std::make_shared<Object>(object[1]));
	camera[0]->registerObserver(std::make_shared<Object>(object[2]));
	camera[0]->registerObserver(std::make_shared<Object>(object[3]));
	*/
	camera[0]->registerObserver(object[0]);
	camera[0]->registerObserver(object[1]);
	camera[0]->registerObserver(object[2]);
	camera[0]->registerObserver(object[3]);
	

	/*shader.push_back(Shader(&camera[0], ShaderType::PHONG));
	camera[0].registerObserver(std::make_shared<Shader>(shader[4]));
	shader.push_back(Shader(&camera[0], ShaderType::PHONG));
	camera[0].registerObserver(std::make_shared<Shader>(shader[5]));
	*/

	for (int i = 4; i < 7; i++)
	{
		//shader.push_back(Shader(&camera[0], ShaderType::PHONG));
		//object.push_back(objectFactory.createObject("sphere", ShaderType::PHONG, glm::vec3(0.5f, 0.f, -0.5f), glm::vec3(0.8f, 0.0f, 0.0f), camera[0]));
		camera[0]->registerObserver(object[i]);
	}

	/*for (int i = 0; i < 7; i++)
	//for (int i = 0; i < 4; i++)
	{
		shaderProgram.push_back(shader[i].getShader());
	}*/

	camera[0]->setPerspectiveCamera();

	//shaderLight = Shader(&camera[0], ShaderType::AMBIENT);
	
	//camera[0].registerObserver(std::make_shared<Shader>(shaderLight));
	

	//std::unique_ptr<Shader> pshader222;
	

	/*Model plain("plain");
	Model sphere("sphere");
	Model suzi_smooth("suzi_smooth");
	Model suzi_flat("suzi_flat");
	Model lightModel("sphere");*/

	// light
	Model lightModel("sphere");
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	shaderLightProgram = Shader::createShader(ShaderType::AMBIENT, 2);	//shaderLight.getShader();

	light = std::unique_ptr<Light>(new Light(glm::vec3(0.0f, 0.0f, 0.0f), lightModel, glm::vec3(1.0f, 1.0f, 1.0f), shaderLightProgram, lightColor, ShaderType::AMBIENT, camera[0])); // glm::vec3(1.0f, 1.0f, 1.0f)
	light->Scale(glm::vec3(0.05f, 0.05f, 0.05f));
	camera[0]->registerObserver(*light);


	/*object.push_back(Object(glm::vec3(-0.25f, 0.f, 0.25f), sphere, glm::vec3(1.10f, 1.10f, 1.10f), shaderProgram[0]));	// horni leva koule
	object.push_back(Object(glm::vec3(0.25f, 0.f, 0.25f), sphere, glm::vec3(1.0f, 1.0f, 0.9f), shaderProgram[1])); // glm::vec3(0.0f, 0.0f, 0.9f)		// horni prava koule
	object.push_back(Object(glm::vec3(-0.25f, 0.f, -0.25f), sphere, glm::vec3(0.7f, 0.0f, 0.5f), shaderProgram[2]));	// dolni leva koule
	object.push_back(Object(glm::vec3(0.25f, 0.f, -0.25f), sphere, glm::vec3(0.8f, 0.0f, 0.0f), shaderProgram[3]));		// dolni prava koule
	
	object.push_back(Object(pos, plain, glm::vec3(0.2f, 1.0f, 0.2f), shaderProgram[4]));
	object.push_back(Object(pos, suzi_smooth, shaderProgram[5]));
	object.push_back(Object(pos, suzi_flat, shaderProgram[6]));
	*/
	// light
	//object.push_back(Object(pos, light, shaderLightProgram));

	// 4 spheres
	glm::vec3 sphereScale = glm::vec3(0.2f, 0.2f, 0.2f);
	object[0].Scale(sphereScale);
	object[1].Scale(sphereScale);
	object[2].Scale(sphereScale);
	object[3].Scale(sphereScale);


	// object translate and scale
	// plane
	object[4].Translate(glm::vec3(0.0f, -0.20f, 0.0f));
	object[4].Scale(glm::vec3(1.0f, 1.0f, 1.0f));
	// monkeys
	object[5].Translate(glm::vec3(-1.0f, 0.0f, 0.25f));
	object[5].Scale(glm::vec3(0.10f, 0.10f, 0.10f));
	object[6].Translate(glm::vec3(-3.0, -0.30, 0.0));
	object[6].Scale(glm::vec3(0.10f, 0.10f, 0.10f));
	// end translate, scale
}