#include "MeshLoader.hpp"
#include "TextureManager.hpp"

/**
*	Constructor, loading the specified aiMesh
**/
MeshLoader::MeshEntry::MeshEntry(aiMesh* mesh) {
	vbo[VERTEX_BUFFER] = NULL;
	vbo[TEXCOORD_BUFFER] = NULL;
	vbo[NORMAL_BUFFER] = NULL;
	vbo[INDEX_BUFFER] = NULL;

	if (mesh->mMaterialIndex >= 0)
	{
		materialIndex = mesh->mMaterialIndex;
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	elementCount = mesh->mNumFaces * 3;

	if (mesh->HasPositions()) {
		float* vertices = new float[mesh->mNumVertices * 3];
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			vertices[i * 3] = mesh->mVertices[i].x;
			vertices[i * 3 + 1] = mesh->mVertices[i].y;
			vertices[i * 3 + 2] = mesh->mVertices[i].z;
		}

		glGenBuffers(1, &vbo[VERTEX_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(0);

		delete[] vertices;
	}


	if (mesh->HasTextureCoords(0)) {
		float* texCoords = new float[mesh->mNumVertices * 2];
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			texCoords[i * 2] = mesh->mTextureCoords[0][i].x;
			texCoords[i * 2 + 1] = mesh->mTextureCoords[0][i].y;
		}

		glGenBuffers(1, &vbo[TEXCOORD_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 2 * mesh->mNumVertices * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(1);

		delete[] texCoords;
	}


	if (mesh->HasNormals()) {
		float* normals = new float[mesh->mNumVertices * 3];
		for (int i = 0; i < mesh->mNumVertices; ++i) {
			normals[i * 3] = mesh->mNormals[i].x;
			normals[i * 3 + 1] = mesh->mNormals[i].y;
			normals[i * 3 + 2] = mesh->mNormals[i].z;
		}

		glGenBuffers(1, &vbo[NORMAL_BUFFER]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]);
		glBufferData(GL_ARRAY_BUFFER, 3 * mesh->mNumVertices * sizeof(GLfloat), normals, GL_STATIC_DRAW);

		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(2);

		delete[] normals;
	}

	// fill face indices
	if (mesh->HasFaces()) {
		unsigned int* indices = new unsigned int[mesh->mNumFaces * 3];
		for (int i = 0; i < mesh->mNumFaces; ++i) {
			indices[i * 3] = mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}

		glGenBuffers(1, &vbo[INDEX_BUFFER]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * mesh->mNumFaces * sizeof(GLuint), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(3);
		
		delete[] indices;
	}


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}



/**
*	Deletes the allocated OpenGL buffers
**/
MeshLoader::MeshEntry::~MeshEntry() {
	if (vbo[VERTEX_BUFFER]) {
		glDeleteBuffers(1, &vbo[VERTEX_BUFFER]);
	}

	if (vbo[TEXCOORD_BUFFER]) {
		glDeleteBuffers(1, &vbo[TEXCOORD_BUFFER]);
	}

	if (vbo[NORMAL_BUFFER]) {
		glDeleteBuffers(1, &vbo[NORMAL_BUFFER]);
	}

	if (vbo[INDEX_BUFFER]) {
		glDeleteBuffers(1, &vbo[INDEX_BUFFER]);
	}

	glDeleteVertexArrays(1, &vao);
}

/**
*	Renders this MeshEntry
**/
void MeshLoader::MeshEntry::render() {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
}

/**
*	Mesh constructor, loads the specified filename if supported by Assimp
**/
MeshLoader::MeshLoader(const char* filename)
{
	ModelMatrix = glm::mat4(1.0);
	Assimp::Importer importer;
	//unsigned int importOptionFlags = aiProcess_Triangulate | aiProcess_GenNormals;	// default NULL
	//unsigned int importOptionFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
	unsigned int importOptionFlags = 
		  aiProcess_OptimizeMeshes              // slou�en� mal�ch plo�ek
		| aiProcess_JoinIdenticalVertices       // NUTN� jinak hodn� duplikuje
		| aiProcess_Triangulate                 // prevod vsech ploch na trojuhelniky
		| aiProcess_CalcTangentSpace;           // vypocet tangenty, nutny pro spravne pouziti normalove mapy

	const aiScene* scene = importer.ReadFile(filename, importOptionFlags);
	if (!scene) {
		printf("Unable to load mesh: %s\n", importer.GetErrorString());
		return;
	}

	for (int i = 0; i < scene->mNumMeshes; ++i) {
		aiMesh* mesh = scene->mMeshes[i];
		meshEntries.push_back(new MeshLoader::MeshEntry(mesh));
		
		const aiMaterial* pMaterial = scene->mMaterials[i];

		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* pMaterial = scene->mMaterials[mesh->mMaterialIndex];
			std::unique_ptr<Material> newMaterial = getMaterial(pMaterial, filename);
			if (newMaterial->diffuseMap == "error")
			{
				continue;
			}

			
			material.push_back(std::move(newMaterial));
		}
		else
		{
			printf("error\n");	// smazat
		}
		/*if (scene->mMeshes[i]->mMaterialIndex >= 0)
		{
			std::unique_ptr<Material> newMaterial = getMaterial(pMaterial, filename);
			if (newMaterial->diffuseMap == "error")
			{
				continue;
			}

			material.push_back(std::move(newMaterial));
		}*/
	}

	/*for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
	//for(unsigned int i = 0; i < mat->GetTextureCount()) {
		const aiMaterial* pMaterial = scene->mMaterials[i];

		std::unique_ptr<Material> newMaterial = getMaterial(pMaterial, filename);
		if (newMaterial->diffuseMap == "error")
		{
			continue;
		}

		material.push_back(std::move(newMaterial));
	}*/

	//Material material = getMaterial();
}


std::unique_ptr<Material> MeshLoader::getMaterial(const aiMaterial* mat, const char* filename)
{
	// materials
	std::unique_ptr<Material> newMaterial = std::make_unique<Material>();
	aiColor3D color{ 0.f, 0.f, 0.f };

	mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	newMaterial->diffuse = glm::vec3{ color.r, color.g, color.b };

	mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
	newMaterial->ambient = glm::vec3{ color.r, color.g, color.b };

	mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
	newMaterial->specular = glm::vec3{ color.r, color.g, color.b };

	mat->Get(AI_MATKEY_SHININESS, newMaterial->shininess);

	// textures
	aiString path_name;
	
	unsigned int numTextur_Diffuser = mat->GetTextureCount(aiTextureType_DIFFUSE);
	unsigned int numTextur_Specular = mat->GetTextureCount(aiTextureType_SPECULAR);
	unsigned int numTextur_heightMap = mat->GetTextureCount(aiTextureType_HEIGHT);

	//std::cout << mat->GetTextureCount(aiTextureType_DIFFUSE) << std::endl;
	//mat->GetTexture(aiTextureType_DIFFUSE, 0, &path_name);
	
	//std::cout << path_name.C_Str() << std::endl;
	
	bool someTexture = false;

	if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path_name) == AI_SUCCESS) {
		newMaterial->diffuseMap = path_name.C_Str();

		std::string new_path = filename;
		new_path = new_path.substr(0, new_path.find_last_of("\\/"));

		char path_buffer[1024] = { 0 };
		new_path += '\\';
		new_path += path_name.C_Str();
		newMaterial->diffuseMap = new_path;

		someTexture = true;
		//std::cout << new_path << std::endl;	// TODO: make load this texture path/name
	}
	if (mat->GetTexture(aiTextureType_SPECULAR, 0, &path_name) == AI_SUCCESS) {
		newMaterial->specularMap = path_name.C_Str();
		someTexture = true;
	}
	if (mat->GetTexture(aiTextureType_HEIGHT, 0, &path_name) == AI_SUCCESS) {
		newMaterial->heightMap = path_name.C_Str();
		someTexture = true;
	}

	if (!someTexture) {
		newMaterial->diffuseMap = "error";
	}

	return newMaterial;
}

/**
*	Clears all loaded MeshEntries
**/
MeshLoader::~MeshLoader(void)
{
	for (int i = 0; i < meshEntries.size(); ++i) {
		delete meshEntries.at(i);
	}
	meshEntries.clear();
}

/**
*	Renders all loaded MeshEntries
**/
void MeshLoader::render() {
	auto textureManager = TextureManager::getInstance();
	std::vector<std::shared_ptr<Texture>> t;

	//t.push_back(textureManager->getTexture("..\\models\\downloaded\\Indoor_plant_3\\textures\\bpng.png"));
	//t.push_back(textureManager->getTexture("..\\models\\downloaded\\Indoor_plant_3\\textures\\bpng.png"));
	//t.push_back(textureManager->getTexture("..\\models\\downloaded\\Indoor_plant_3\\textures\\Pot textures_col.jpg"));


	//t.push_back(textureManager->getTexture("..\\models\\downloaded\\Indoor_plant_3\\textures\\bpng_Schwaz_weistensel.jpg"));
	//auto t3 = textureManager->getTexture("..\\models\\downloaded\\Indoor_plant_3\\textures\\bpng_Schwaz_weistensel.jpg");
	//auto t = textureManager->getTexture("..\\models\\cube\\test.png");	// "floor\\floor1.jpg"

	//glBindTexture(GL_TEXTURE_2D, t->getTextureId());
	//glBindTexture(GL_TEXTURE_2D + 1, t2->getTextureId());
	glActiveTexture(GL_TEXTURE0);

	for (int j = 0; j < material.size(); j++) {
		t.emplace_back(textureManager->getModelTexture(material[j]->diffuseMap));
		//t.emplace_back(textureManager->getModelTexture("..\\" + material[j]->diffuseMap));
		//std::string a = "..\\" + material[j]->diffuseMap;
	}

	for (int i = 0; i < meshEntries.size(); ++i) {
		/*if (material[i]->diffuseMap == "") {
			continue;
		}*/
		//glBindTexture(GL_TEXTURE_2D, t[i]->getTextureId());
		//meshEntries.at(i)->render();

		if (material.size() - 1 >= i) {
			
			//printf("texture: %s\n", "..\\" + material[i]->diffuseMap);
			//printf("mesh entries size %d\n", meshEntries.size());

			glBindTexture(GL_TEXTURE_2D, t[i]->getTextureId());
		}
		

		meshEntries.at(i)->render();
	}
}

void MeshLoader::render2() {
	auto textureManager = TextureManager::getInstance();
	std::vector<std::shared_ptr<Texture>> t;
	t.push_back(textureManager->getTexture("..\\models\\cube\\dum2\\floor1.jpg"));
	t.push_back(textureManager->getTexture("..\\models\\cube\\dum2\\test.png"));
	t.push_back(textureManager->getTexture("..\\models\\cube\\dum2\\floor1.jpg"));

	for (int i = 0; i < meshEntries.size(); ++i) {
		//printf("mesh entries size %d\n", meshEntries.size());
		glBindTexture(GL_TEXTURE_2D, t[i]->getTextureId());
		meshEntries.at(i)->render();
	}
}
