#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>

#include "Shader.hpp"
#include "Object.hpp"
#include "Light.hpp"
#include "Model.hpp"
#include "Camera.hpp"


class Scene
{
public:
	Scene();
	~Scene();
private:
public:
	std::vector<Object> object;
	std::vector<std::shared_ptr<Camera>> camera;
	std::unique_ptr<Light> light;

	Shader shaderLight;
	GLuint shaderLightProgram;

	void InitializeScene();
};

#endif // !SCENE_HPP