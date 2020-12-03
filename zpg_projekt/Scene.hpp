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
	struct Light2 {
		int lightType;
		glm::vec4 position;
		glm::vec3 color; //a.k.a. the color of the light
		float attenuation;
		float ambientCoefficient;
		float coneAngle; // new
		glm::vec3 coneDirection; // new
	};
	//std::vector<Object> object;
	std::vector<std::shared_ptr<Object>> object;
	std::vector<std::shared_ptr<Camera>> camera;
	std::vector<std::shared_ptr<Light>> light;
	std::vector<std::shared_ptr<Light2>> light2;
	//std::unique_ptr<Light> light;

	Shader shaderLight;
	GLuint shaderLightProgram;

	void InitializeScene();
	void addObject(std::string modelName, ShaderType shaderType, glm::vec3 position, glm::vec3 color, std::shared_ptr<Camera> camera, glm::vec3 scale = glm::vec3(1.0f));
	void deleteObject(int id);
	//void rotateObject(int index, float rotxangle, glm::vec3 axis);
	//void moveObject(int index, glm::vec3 newPosition);
	void setNewColor(int index);
	void setLastColor(int index);
};

#endif // !SCENE_HPP