#define USE_ASSIMP
/*#include<assimp/Importer.hpp>// C++ importerinterface
#include<assimp/scene.h>// aiSceneoutputdata structure
#include<assimp/postprocess.h>// Post processingflags

#include "ObjLoader.hpp"

#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include <glm/glm.hpp>

// Very, VERY simple OBJ loader.
// Here is a short list of features a real function would provide : 
// - Binary files. Reading a model should be just a few memcpy's away, not parsing a file at runtime. In short : OBJ is not very great.
// - Animations & bones (includes bones weights)
// - Multiple UVs
// - All attributes should be optional, not "forced"
// - More stable. Change a line in the OBJ file and it crashes.
// - More secure. Change another line and you can inject code.
// - Loading from memory, stream, etc

bool loadOBJ(
	const char* path,
	std::vector<glm::vec3>& out_vertices,
	std::vector<glm::vec2>& out_uvs,
	std::vector<glm::vec3>& out_normals
) {
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE* file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}
	
	while (1) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				fclose(file);
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else {
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}
	fclose(file);
	return true;
}


#ifdef USE_ASSIMP // don't use this #define, it's only for me (it AssImp fails to compile on your machine, at least all the other tutorials still work)

// Include AssImp
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
unsigned int pocetTextur_Diffuser = 0;
unsigned int pocetTextur_Specular = 0;
unsigned int pocetTextur_heightMap = 0;


bool loadAssImp(
	const char* path,
	std::vector<unsigned short>& indices,
	std::vector<glm::vec3>& vertices,
	std::vector<glm::vec2>& uvs,
	std::vector<glm::vec3>& normals,
	Material& material,
	std::vector<Mesh*>& meshes
) {
	Assimp::Importer importer;
	unsigned int importOptions = aiProcess_Triangulate | aiProcess_GenNormals;

	const aiScene* scene = importer.ReadFile(path, importOptions);*/
	//const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate /*0 aiProcess_JoinIdenticalVertices | aiProcess_SortByPType*/);
	/*if (!scene) {
		fprintf(stderr, importer.GetErrorString());
		getchar();
		return false;
	}

	for(int m = 0; m < scene->mNumMeshes; m++)
	{
		const aiMesh* mesh = scene->mMeshes[m]; // In this simple example code we always use the 1rst mesh (in OBJ files there is often only one anyway)
	
		vertices.reserve(mesh->mNumVertices);
		uvs.reserve(mesh->mNumVertices);
		normals.reserve(mesh->mNumVertices);

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			// Fill vertices positions
			if (mesh->HasPositions())
			{
				aiVector3D pos = mesh->mVertices[i];
				vertices.push_back(glm::vec3(pos.x, pos.y, pos.z));
			}
			// Fill vertices texture coordinates
			if (mesh->HasTextureCoords(0))
			{
				aiVector3D UVW = mesh->mTextureCoords[0][i]; // Assume only 1 set of UV coords; AssImp supports 8 UV sets.
				uvs.push_back(glm::vec2(UVW.x, UVW.y));
			}
			// Fill vertices normals
			if (mesh->HasNormals())
			{
				aiVector3D n = mesh->mNormals[i];
				normals.push_back(glm::vec3(n.x, n.y, n.z));
			}
		}

		// Fill face indices
		indices.reserve(3 * mesh->mNumFaces);
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			// Assume the model has only triangles.
			indices.push_back(mesh->mFaces[i].mIndices[0]);
			indices.push_back(mesh->mFaces[i].mIndices[1]);
			indices.push_back(mesh->mFaces[i].mIndices[2]);
		}

		// materials
		aiMaterial* mat = scene->mMaterials[m];	// [1]
		aiColor3D color{ 0.f, 0.f, 0.f };

		mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material.diffuse = glm::vec3{ color.r, color.g, color.b };

		mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
		material.ambient = glm::vec3{ color.r, color.g, color.b };

		mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		material.specular = glm::vec3{ color.r, color.g, color.b };

		mat->Get(AI_MATKEY_SHININESS, material.shininess);

		aiString path_name;


		pocetTextur_Diffuser = mat->GetTextureCount(aiTextureType_DIFFUSE);
		pocetTextur_Specular = mat->GetTextureCount(aiTextureType_SPECULAR);
		pocetTextur_heightMap = mat->GetTextureCount(aiTextureType_HEIGHT);
		
		std::cout << mat->GetTextureCount(aiTextureType_DIFFUSE) << std::endl;
		mat->GetTexture(aiTextureType_DIFFUSE, 0, &path_name);
		
		std::cout << path_name.C_Str() << std::endl;

		if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path_name) == AI_SUCCESS) {
			material.diffuseMap = path_name.C_Str();

			std::string new_path = path;
			new_path = new_path.substr(0, new_path.find_last_of("\\/"));

			char path_buffer[1024] = {0};
			new_path += '\\';
			new_path += path_name.C_Str();
			material.diffuseMap = new_path;
			std::cout << new_path << std::endl;	// TODO: make load this texture path/name
		}
		if (mat->GetTexture(aiTextureType_SPECULAR, 0, &path_name) == AI_SUCCESS) {
			material.specularMap = path_name.C_Str();
		}
		if (mat->GetTexture(aiTextureType_HEIGHT, 0, &path_name) == AI_SUCCESS) {
			material.heightMap = path_name.C_Str();
		}


		Mesh* newMesh = createMesh(indices, vertices, uvs, normals, material);
		meshes.push_back(newMesh);
	}

	// The "scene" pointer will be deleted automatically by "importer"
	return true;
}

Mesh* createMesh(std::vector<unsigned short>& indices,
	std::vector<glm::vec3>& vertices,
	std::vector<glm::vec2>& uvs,
	std::vector<glm::vec3>& normals,
	Material& material)
{
	std::vector<Vertex> vertices2;
	// added mesh
	for (int i = 0; i < vertices.size(); i++)
	{
		Vertex newVertex;

		newVertex.Position = vertices[i];
		if (normals.size() > 0) {
			newVertex.Normal = normals[i];
		}
		if (uvs.size() != 0)
		{
			newVertex.TexCoords = uvs[i];
		}

		vertices2.push_back(newVertex);
	}

	//newMeshModel = new Mesh(vertices2, indices, texture);		// funguje
	std::vector<Material> mat{ material };

	//return new Mesh(vertices2, indices, mat);
	return new Mesh(vertices2, indices, mat, vertices);
}

#endif



void send3DobjUniforms(GLuint objShaderProgram, std::shared_ptr<Camera> camera, std::vector<std::shared_ptr<Light>> light, std::vector<std::shared_ptr<DirectionalLight>> directionalLight, std::vector<std::shared_ptr<SpotLight>> spotLight, GLuint texture, glm::mat4x4 modelMatrix)
{
	if (true)
	{
		Shader::sendUniform(objShaderProgram, "modelMatrix", modelMatrix);
		Shader::sendUniform(objShaderProgram, "fragmentColor", glm::vec3(1.f, 0.f, 0.f)); //glm::vec3(1.f, 0.f, 0.f)); // 
		Shader::sendUniform(objShaderProgram, "viewPos", camera->getPosition());

		Shader::sendUniform(objShaderProgram, "viewMatrix", camera->getCamera());
		Shader::sendUniform(objShaderProgram, "projectionMatrix", camera->getProjectionMatrix());


		// direction light
		for (int dirIndex = 0; dirIndex < directionalLight.size(); dirIndex++) {
			Shader::sendUniform(objShaderProgram, "dirLight.direction", directionalLight[dirIndex]->direction);
			Shader::sendUniform(objShaderProgram, "dirLight.color", directionalLight[dirIndex]->lightColor);
			Shader::sendUniform(objShaderProgram, "dirLight.ambient", directionalLight[dirIndex]->ambientCoefficient);
			Shader::sendUniform(objShaderProgram, "dirLight.diffuse", glm::vec3(0.2f, 0.2f, 0.2f));	// glm::vec3(0.2f, 0.2f, 0.2f)
			Shader::sendUniform(objShaderProgram, "dirLight.specular", glm::vec3(0.2f, 0.2f, 0.2f));
		}

		// spotLight
		Shader::sendUniform(objShaderProgram, "spotLightCount", static_cast<GLint>(spotLight.size()));

		for (int spotIndex = 0; spotIndex < spotLight.size(); spotIndex++) {
			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].position").c_str(), spotLight[spotIndex]->getPosition());
			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].direction").c_str(), spotLight[spotIndex]->coneDirection);
			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].color").c_str(), spotLight[spotIndex]->lightColor);
			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].ambient").c_str(), glm::vec3(0.0f, 0.0f, 0.0f));
			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].diffuse").c_str(), glm::vec3(0.5f, 0.0f, 0.5f));
			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].specular").c_str(), glm::vec3(0.5f, 0.0f, 0.5f));

			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].constant").c_str(), 1.0f);
			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].linear").c_str(), 0.09f);
			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].quadratic").c_str(), 0.032f);

			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].cutOff").c_str(), glm::cos(glm::radians(12.5f)));
			Shader::sendUniform(objShaderProgram, ("spotLight[" + std::to_string(spotIndex) + "].outerCutOff").c_str(), glm::cos(glm::radians(15.0f)));
		}

		// FlashLight
		Shader::sendUniform(objShaderProgram, "flashLight.position", camera->getPosition() + camera->flashLight->getPosition());
		Shader::sendUniform(objShaderProgram, "flashLight.direction", camera->target);
		Shader::sendUniform(objShaderProgram, "flashLight.color", camera->flashLight->lightColor);

		Shader::sendUniform(objShaderProgram, "flashLight.ambient", glm::vec3(1.0f, 0.0f, 0.0f));
		Shader::sendUniform(objShaderProgram, "flashLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		Shader::sendUniform(objShaderProgram, "flashLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		Shader::sendUniform(objShaderProgram, "flashLight.constant", 1.0f);
		Shader::sendUniform(objShaderProgram, "flashLight.linear", 0.09f);
		Shader::sendUniform(objShaderProgram, "flashLight.quadratic", 0.032f);
		Shader::sendUniform(objShaderProgram, "flashLight.cutOff", glm::cos(glm::radians(12.5f)));
		Shader::sendUniform(objShaderProgram, "flashLight.outerCutOff", glm::cos(glm::radians(15.0f)));


		Shader::sendUniform(objShaderProgram, "pointLightCount", static_cast<GLint>(light.size()));

		// point lights
		for (int lightIndex = 0; lightIndex < light.size(); lightIndex++) {
			Shader::sendUniform(objShaderProgram, ("pointLight[" + std::to_string(lightIndex) + "].lightType").c_str(), static_cast<int>(light[lightIndex]->getLightType()));
			Shader::sendUniform(objShaderProgram, ("pointLight[" + std::to_string(lightIndex) + "].position").c_str(), light[lightIndex]->getPosition());
			Shader::sendUniform(objShaderProgram, ("pointLight[" + std::to_string(lightIndex) + "].color").c_str(), light[lightIndex]->lightColor);
			Shader::sendUniform(objShaderProgram, ("pointLight[" + std::to_string(lightIndex) + "].constant").c_str(), 1.0f);
			Shader::sendUniform(objShaderProgram, ("pointLight[" + std::to_string(lightIndex) + "].linear").c_str(), 0.09f);
			Shader::sendUniform(objShaderProgram, ("pointLight[" + std::to_string(lightIndex) + "].quadratic").c_str(), 0.032f);
		}

		// texture load
		if (true)
		{
			//texture->Load();
			glActiveTexture(GL_TEXTURE0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);  // GL_REPEAT
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			glBindTexture(GL_TEXTURE_2D, texture);

			Shader::sendUniform(objShaderProgram, "hasTexture", 1);
			//Set texture unit to fragment shader

			// newObject->texture->getID()
			Shader::sendUniform(objShaderProgram, "textureUnitID", 0);
		}
		else
		{
			Shader::sendUniform(objShaderProgram, "hasTexture", 0);
		}
	// end added
	}
} */



// checks all material textures of a given type and loads the textures if they're not loaded yet.
 // the required info is returned as a Texture struct.
/*std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{   // if texture hasn't been loaded already, load it
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}*/