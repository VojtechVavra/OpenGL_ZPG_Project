#pragma once

// this code taken and edited from: https://badvertex.com/2014/04/13/loading-meshes-using-assimp-in-opengl.html
#include <vector>
#include <memory>
#include <cstddef>         // std::size_t
#include <string>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Material.hpp"
#include "Shader.hpp"


class MeshLoader
{
public:
	struct MeshEntry {
		static enum BUFFERS {
			VERTEX_BUFFER, TEXCOORD_BUFFER, NORMAL_BUFFER, INDEX_BUFFER
		};
		GLuint vao;
		GLuint vbo[4];

		unsigned int elementCount;

		MeshEntry(aiMesh* mesh);
		~MeshEntry();

		int materialIndex;	// added

		void load(aiMesh* mesh);
		void render();
	};

	std::vector<MeshEntry*> meshEntries;

public:
	MeshLoader(const char* filename);
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

