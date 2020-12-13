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


struct Material;

class Scene
{
public:
	Scene();
	~Scene();

	std::vector<std::shared_ptr<Object>> object;
	std::vector<std::shared_ptr<Camera>> camera;
	std::vector<std::shared_ptr<Light>> light;
	std::vector<ShaderProgram> shaderPrograms;

	std::shared_ptr<SkyBox> skybox;
	ShaderProgram skyboxshader;
	std::shared_ptr<Object> skybox2;

	std::vector<std::shared_ptr<SpotLight>> spotLight;
	std::vector<std::shared_ptr<DirectionalLight>> directionalLight;
	
	//GLuint modelShader;
	//GLuint newModel;
	//GLint indicesCount;


	//
	GLuint VertexArrayID;
	GLuint programID;
	GLuint MatrixID;
	//std::shared_ptr<Texture> texture;
	GLuint texture;
	GLuint TextureID;

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> vertices;
	std::vector<Vertex> vertices2;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.
	Material material;
	
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;

	Mesh* newMeshModel;
	//

	//std::unique_ptr<Light> light;

	//Shader shaderLight;
	//GLuint shaderLightProgram;

	void InitializeScene();
	void addObject(std::string modelName, ShaderType shaderType, glm::vec3 position, glm::vec3 color, std::shared_ptr<Camera> camera, glm::vec3 scale = glm::vec3(1.0f), std::string texturePath = "");
	void deleteObject(int id);
	//void rotateObject(int index, float rotxangle, glm::vec3 axis);
	//void moveObject(int index, glm::vec3 newPosition);
	void setNewColor(int index);
	void setLastColor(int index);

private:
	void cameraSection();
	void lightSection();
	void objectSection();
	void objectTransformSection();
	void skyboxSection();
	void modelSection();
};


#endif // !SCENE_HPP