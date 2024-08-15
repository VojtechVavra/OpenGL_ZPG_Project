#pragma once

// this code taken and edited from: https://badvertex.com/2014/04/13/loading-meshes-using-assimp-in-opengl.html
#include <vector>
#include <array>
#include <memory>
#include <cstddef>					// std::size_t
#include <string>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Material.hpp"
#include "Shader.hpp"
//#include "Mesh.hpp"
#include "VAO.hpp"
#include "VBO.hpp"

class Mesh;

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

class MeshLoader
{
private:
	struct MeshEntry {
		enum BUFFERS : int {
			VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER
		};

		//GLuint vao;
		//GLuint vbo[4];
		GLuint vao;
		GLuint vbo[4];

		
		//std::shared_ptr<VAO> m_VAO;
		//std::array<std::shared_ptr<VBO>, 4> m_VBO;
		//VAO m_VAO;
		//VBO m_VBO[4];
		
		
		unsigned int m_numVertices = 0;

		unsigned int elementCount;

		MeshEntry(aiMesh* mesh);
		~MeshEntry();

		int materialIndex = 0;	// added

		//void load(aiMesh* mesh);
		void render();
	};

	std::vector<std::shared_ptr<struct MeshLoader::MeshEntry>> meshEntries;
	//std::vector<struct MeshLoader::MeshEntry*> meshEntries;

public:
	MeshLoader(const char* filename, std::shared_ptr<Mesh>& out_mesh);
	~MeshLoader(void);

	std::vector<std::unique_ptr<Material>> material;
	std::unique_ptr<Material> getMaterial(const aiMaterial* mat, const char* filename);

	void render();
	void renderFlame();
	void render2(); // TODO: Smazat tuto funkci a udelat ji obecne
// pridat do teto classy textury a materialy
	glm::mat4 ModelMatrix; // added
	GLuint shaderProgramID; // added

	std::string path;
	std::string fileName;

private:
	void SaveFilenameAndPath(const std::string& filename);
};
