#pragma once

#include <vector>
#include <memory>
#include <array>

#include <glm/glm.hpp>

#include "MeshLoader.hpp"
#include "Texture.hpp"
#include "Material.hpp"
#include "VAO.hpp"
#include "VBO.hpp"

// Z-buffer pipeline 142 stran prezentaci
// https://www.cs.utexas.edu/~fussell/courses/cs384g-spring2017/lectures/Lecture9-Zbuffer_pipeline.pdf

// Material -> presunout ho do samostatneho souboru a jen ho importovat
/*struct Material {
	glm::vec3 diffuse{ 0.f };
	glm::vec3 specular{ 0.f };
	glm::vec3 ambient{ 0.f };
	std::string diffuseMap;
	std::string specularMap;
	std::string heightMap;
	float shininess = 0.f;
};*/

class Mesh
{
public:
	Mesh(
		std::vector<std::unique_ptr<Material>>&& material,
		std::vector<std::shared_ptr<struct MeshLoader::MeshEntry>>&& meshEntries, 
		//std::vector<>&& meshEntries,
		unsigned int elementCount,
		std::vector<std::shared_ptr<Texture>>&& textures
		//std::unique_ptr<VAO> m_VAO,
		//std::array<std::unique_ptr<VBO>, 4> m_VBO
	);
	~Mesh();

	/**
	 *	Render this Mesh
	 */
	void render();
	void render(bool b);

	int getTextureCount() const;
	void setShader(std::shared_ptr<Shader> shader);

private:
	std::vector<std::unique_ptr<Material>> m_material;
	std::vector<std::shared_ptr<struct MeshLoader::MeshEntry>> m_meshEntries;
	std::vector<std::shared_ptr<Texture>> m_textures;

	std::unique_ptr<VAO> m_VAO = std::make_unique<VAO>();
	std::array<std::unique_ptr<VBO>, 4> m_VBO;
	
	GLuint m_elementCount = 0;
	std::shared_ptr<Shader> m_shader;

	static enum BUFFERS {
		VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER
	};
};
