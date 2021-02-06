#pragma once

// this code taken and edited from: https://badvertex.com/2014/04/13/loading-meshes-using-assimp-in-opengl.html
#include <vector>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <glm/glm.hpp>
#include <GL/glew.h>
//#include "include/GL/glew.h"
//#include "include/GL/glfw3.h"



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

		void load(aiMesh* mesh);
		void render();
	};

	std::vector<MeshEntry*> meshEntries;

public:
	MeshLoader(const char* filename);
	~MeshLoader(void);

	void render();
	void render2(); // TODO: Smazat tuto funkci a udelat ji obecne
// pridat do teto classy textury a materialy
};

