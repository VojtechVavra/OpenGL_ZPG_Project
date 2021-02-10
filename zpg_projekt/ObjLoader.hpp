#ifndef OBJLOADER_HPP
#define OBJLOADER_HPP

#include <vector>
#include <glm/glm.hpp>

#include "Camera.hpp"
#include "DirectionalLight.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"

#include "Material.hpp"

// Material mam duplicitni v Mesh.hpp -> presunout ho do samostatneho souboru a jen ho importovat
/*struct Material {
	glm::vec3 diffuse{ 0.f };
	glm::vec3 specular{ 0.f };
	glm::vec3 ambient{ 0.f };
	std::string diffuseMap;
	std::string specularMap;
	std::string heightMap;
	float shininess = 0.f;
};*/

bool loadOBJ(
	const char* path,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals
);



bool loadAssImp(
	const char* path,
	std::vector<unsigned short>& indices,
	std::vector<glm::vec3>& vertices,
	std::vector<glm::vec2>& uvs,
	std::vector<glm::vec3>& normals,
	Material& material,
	std::vector<Mesh*>& meshes
);

Mesh* createMesh(std::vector<unsigned short>& indices,
	std::vector<glm::vec3>& vertices,
	std::vector<glm::vec2>& uvs,
	std::vector<glm::vec3>& normals,
	Material& material);

void send3DobjUniforms(GLuint objShaderProgram, std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<Light>> light, std::vector<std::shared_ptr<DirectionalLight>> directionalLight, std::vector<std::shared_ptr<SpotLight>> spotLight, /*std::shared_ptr<Texture>*/ GLuint texture, glm::mat4x4 modelMatrix);

#endif