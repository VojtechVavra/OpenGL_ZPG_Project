#include <iostream>
#include <memory>

#include "Scene.hpp"
#include "Shader.hpp"
#include "Object.hpp"
#include "Light.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "Callback.hpp"



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
	camera.push_back(Camera(glm::vec3(0.0f, 0.0f, 0.0f)));
	Callback::setCamera(&camera[0]);
	
	shader.push_back(Shader(&camera[0], ShaderType::AMBIENT));
	shader.push_back(Shader(&camera[0], ShaderType::DIFFUSE));
	shader.push_back(Shader(&camera[0], ShaderType::SPECULAR));
	shader.push_back(Shader(&camera[0], ShaderType::PHONG));

	camera[0].registerObserver(std::make_shared<Shader>(shader[0]));
	camera[0].registerObserver(std::make_shared<Shader>(shader[1]));
	camera[0].registerObserver(std::make_shared<Shader>(shader[2]));
	camera[0].registerObserver(std::make_shared<Shader>(shader[3]));

	/*shader.push_back(Shader(&camera[0], ShaderType::PHONG));
	camera[0].registerObserver(std::make_shared<Shader>(shader[4]));
	shader.push_back(Shader(&camera[0], ShaderType::PHONG));
	camera[0].registerObserver(std::make_shared<Shader>(shader[5]));
	*/
	for (int i = 4; i < 7; i++)
	{
		shader.push_back(Shader(&camera[0], ShaderType::PHONG));

		camera[0].registerObserver(std::make_shared<Shader>(shader[i]));
	}

	for (int i = 0; i < 7; i++)
	//for (int i = 0; i < 4; i++)
	{
		shaderProgram.push_back(shader[i].getShader());
	}

	camera[0].setPerspectiveCamera();

	shaderLight = Shader(&camera[0], ShaderType::AMBIENT);
	shaderLightProgram = shaderLight.getShader();
	camera[0].registerObserver(std::make_shared<Shader>(shaderLight));


	//std::unique_ptr<Shader> pshader222;
	glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);

	Model plain("plain");
	Model sphere("sphere");
	Model suzi_smooth("suzi_smooth");
	Model suzi_flat("suzi_flat");
	Model lightModel("sphere");

	// light
	light = std::unique_ptr<Light>(new Light(glm::vec3(0.0f, 0.0f, 0.0f), lightModel, glm::vec3(1.0f, 1.0f, 1.0f), shaderLightProgram, glm::vec3(1.0f, 1.0f, 1.0f)));
	light->Scale(glm::vec3(0.05f, 0.05f, 0.05f));

	object.push_back(Object(glm::vec3(-0.25f, 0.f, 0.25f), sphere, glm::vec3(1.10f, 1.10f, 1.10f), shaderProgram[0]));	// horni leva koule
	object.push_back(Object(glm::vec3(0.25f, 0.f, 0.25f), sphere, glm::vec3(0.0f, 0.0f, 0.9f), shaderProgram[1]));		// horni prava koule
	object.push_back(Object(glm::vec3(-0.25f, 0.f, -0.25f), sphere, glm::vec3(0.7f, 0.0f, 0.5f), shaderProgram[2]));	// dolni leva koule
	object.push_back(Object(glm::vec3(0.25f, 0.f, -0.25f), sphere, glm::vec3(0.8f, 0.0f, 0.0f), shaderProgram[3]));		// dolni prava koule

	object.push_back(Object(pos, plain, glm::vec3(0.2f, 1.0f, 0.2f), shaderProgram[4]));
	object.push_back(Object(pos, suzi_smooth, shaderProgram[5]));
	object.push_back(Object(pos, suzi_flat, shaderProgram[6]));
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