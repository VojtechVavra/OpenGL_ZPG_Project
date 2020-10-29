#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>

#include "Shader.hpp"
#include "Object.hpp"
#include "Light.hpp"
#include "Model.hpp"
#include "Camera.hpp"
//class Camera;

class Scene
{
public:
	Scene();
	~Scene();
private:
public:
	std::vector<Object> object;
	std::vector<Shader> shader;
	std::vector<std::shared_ptr<Camera>> camera;
	std::vector<Model> model;
	std::vector<GLuint> shaderProgram; // asi smazu

	Shader shaderLight;
	GLuint shaderLightProgram;

	std::unique_ptr<Light> light;

	void InitializeScene();
};

#endif // !SCENE_HPP