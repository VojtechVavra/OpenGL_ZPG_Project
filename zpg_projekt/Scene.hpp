#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>

#include "Shader.hpp"
#include "Object.hpp"
#include "Light.hpp"
#include "DirectionalLight.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "ShaderProgram.hpp"
#include "SkyBox.hpp"
#include "LoadModel.hpp"
#include "ObjLoader.hpp"

#include "Mesh.hpp"

// added new
#include "MeshLoader.hpp"

struct Material;

class Scene
{
public:
	Scene();
	~Scene();


	/* Scene contain objects:
	*  objects can be:
	*	 1. 3D Models which contains:
	*		1.1. mesh
	*		1.2. textures
	*		1.3. shaders
	*		1.4. position in 3D world scene
	*    2. Lights
	*	 3. Camera
	*    3. Skybox (which is type of 3D model exatly textured cube)
	*	 4.
	*/

	std::vector<std::shared_ptr<Object>> object;
	std::vector<std::shared_ptr<Camera>> camera;
	std::vector<std::shared_ptr<Light>> light;
	std::vector<ShaderProgram> shaderPrograms;

	std::shared_ptr<SkyBox> skybox;
	ShaderProgram skyboxshader;

	std::vector<std::shared_ptr<SpotLight>> spotLight;
	std::vector<std::shared_ptr<DirectionalLight>> directionalLight;
	

	GLuint programID;
	std::vector<Mesh*> meshes;	// added 2021
	std::vector<std::shared_ptr<MeshLoader>> meshObjects;
	//std::vector<MeshLoader*> meshObjects;
	//Shader shaderLight;
	bool zatahlyZaves;

	void InitializeScene();
	void addObject(std::string modelName, ShaderType shaderType, glm::vec3 position, glm::vec3 color, std::shared_ptr<Camera> camera, glm::vec3 scale = glm::vec3(1.0f), std::string texturePath = "");
	void deleteObject(int id);
	//void rotateObject(int index, float rotxangle, glm::vec3 axis);
	//void moveObject(int index, glm::vec3 newPosition);
	void setNewColor(int index);
	void setLastColor(int index);

private:
	void registerUsedShaders();
	void cameraSection();
	void lightSection();
	void objectSection();
	void objectTransformSection();
	void skyboxSection();
	void modelSection();
};

#endif // !SCENE_HPP
