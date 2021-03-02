#include<iostream>

#include "SOIL.h"

#include "TextureManager.hpp"


std::shared_ptr<TextureManager> TextureManager::instance = nullptr;

std::shared_ptr<TextureManager> TextureManager::getInstance()
{
	if (TextureManager::instance == nullptr) {
		TextureManager::instance = std::shared_ptr<TextureManager>(new TextureManager());
		//TextureManager::instance = std::make_shared<TextureManager>();
	}
	return TextureManager::instance;
}

TextureManager::TextureManager() {}


std::shared_ptr<Texture> TextureManager::createTexture(std::string textureName)
{
	//Bind the first texture to the first texture unit.
	glActiveTexture(GL_TEXTURE0);

	std::string texturePath = "textures\\" + textureName;	// "textures\\floor\\floor1.jpg"
	GLuint textureID = SOIL_load_OGL_texture(texturePath.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_MIPMAPS);
    if (textureID == 0) {
        printf("[Texture manager / loader] \"%s\" failed to load!\n", texturePath.c_str());
    }


	glBindTexture(GL_TEXTURE_2D, textureID);

	std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(textureName, textureID);
	textures.insert({ textureName, newTexture });

	return newTexture;
}

std::shared_ptr<Texture> TextureManager::getTexture(std::string textureName)
{
	auto it = textures.find(textureName);
	/*if (textures.empty())
	{
		return createTexture(textureName);
	}*/
	if (it == textures.end())
	{
		return createTexture(textureName);
	}

	return it->second;
}


std::shared_ptr<Texture> TextureManager::createModelTexture(std::string textureName)
{
    //Bind the first texture to the first texture unit.
    glActiveTexture(GL_TEXTURE0);

    std::string texturePath = textureName;	// "textures\\floor\\floor1.jpg"
    GLuint textureID = SOIL_load_OGL_texture(texturePath.c_str(), SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_MIPMAPS);
    if (textureID == 0) {
        printf("[Texture model manager/loader] \"%s\" failed to load texture!\n", texturePath.c_str());
    }


    glBindTexture(GL_TEXTURE_2D, textureID);

    std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(textureName, textureID);
    textures.insert({ textureName, newTexture });

    return newTexture;
}

std::shared_ptr<Texture> TextureManager::getModelTexture(std::string textureName)
{
    auto it = textures.find(textureName);
    /*if (textures.empty())
    {
        return createTexture(textureName);
    }*/
    if (it == textures.end())
    {
        return createModelTexture(textureName);
    }

    return it->second;
}

std::shared_ptr<Texture> TextureManager::createCubemap(std::vector<std::string> faces, const std::string skyboxFolderName)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    std::string path = std::string("textures\\skybox\\") + skyboxFolderName + std::string("\\");
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        std::string file = path + faces[i];
        std::cerr << file << std::endl;
        GLubyte* data = SOIL_load_image(file.c_str(), &width, &height, &nrChannels, SOIL_LOAD_AUTO);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            // glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
            SOIL_free_image_data(data);
            // glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            SOIL_free_image_data(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    
    std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(skyboxFolderName, textureID);
    return newTexture;
}


std::shared_ptr<Texture> TextureManager::createCubemap2(std::vector<std::string> faces, const std::string sky)
{
    GLuint cubeMapTexture = SOIL_load_OGL_cubemap(
        faces[0].c_str(), faces[1].c_str(), faces[2].c_str(),
        faces[3].c_str(), faces[4].c_str(), faces[5].c_str(),
        SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, 0 //SOIL_FLAG_INVERT_Y
    );

    glGenTextures(1, &cubeMapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(sky, cubeMapTexture);
    return newTexture;
}