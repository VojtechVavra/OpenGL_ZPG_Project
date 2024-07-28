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

	int getTextureCount() const;
	void setShader(Shader shader);

private:
	std::vector<std::unique_ptr<Material>> m_material;
	std::vector<std::shared_ptr<struct MeshLoader::MeshEntry>> m_meshEntries;
	std::vector<std::shared_ptr<Texture>> m_textures;

	std::unique_ptr<VAO> m_VAO = std::make_unique<VAO>();
	std::array<std::unique_ptr<VBO>, 4> m_VBO;
	
	GLuint m_elementCount = 0;
	Shader m_shader;

	static enum BUFFERS {
		VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER
	};
};
