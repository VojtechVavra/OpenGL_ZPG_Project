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

TextureManager::TextureManager() : TBaseCollectionManager<Texture>() {}


std::shared_ptr<Texture> TextureManager::createTexture(std::string textureName)
{
	std::string texturePath = "textures\\" + textureName;	// "textures\\floor\\floor1.jpg"
	GLuint textureID = SOIL_load_OGL_texture(
        texturePath.c_str(), 
        SOIL_LOAD_RGBA, 
        SOIL_CREATE_NEW_ID, 
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_MIPMAPS
    );

    if (textureID == 0) {
        // Pokud se na�ten� nepoda�ilo, nastav�me v�choz� texturu
        // 
        //printf("[Texture manager/loader] \"%s\" failed to load texture!\n", texturePath.c_str());

        //int r = rand() % 2 + 1;
        const char* rand_texture_choice = DEFAULT_NOTEXTURE2;   // r == 1 ? DEFAULT_NOTEXTURE1 : DEFAULT_NOTEXTURE2;
        //printf("[Texture model manager/loader] setting up default texture \"%s\"\n", rand_texture_choice);
        textureID = SOIL_load_OGL_texture(
            rand_texture_choice, 
            SOIL_LOAD_RGBA, 
            SOIL_CREATE_NEW_ID, 
            SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_MIPMAPS
        );

        texturePath = std::string(rand_texture_choice);
    }
    
    // Vytvo� novou instanci textury a p�idej ji do mapy
	std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(textureName, textureID);
	//textures.insert({ textureName, newTexture });
    addData(newTexture);

	return newTexture;
}

std::shared_ptr<Texture> TextureManager::createNoRepeatTexture(std::string textureName)
{
    //Bind the first texture to the first texture unit.
    //glActiveTexture(GL_TEXTURE0);

    std::string texturePath = textureName;	// "textures\\floor\\floor1.jpg"
    GLuint textureID = SOIL_load_OGL_texture(
        texturePath.c_str(), 
        SOIL_LOAD_RGBA, 
        SOIL_CREATE_NEW_ID, 
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS
    );

    if (textureID == 0) {
        // Pokud se na�ten� nepoda�ilo, nastav�me v�choz� texturu
        
        //printf("[Texture model manager/loader] \"%s\" failed to load texture!\n", texturePath.c_str());

        //int r = rand() % 2 + 1;
        const char* rand_texture_choice = DEFAULT_NOTEXTURE2;   // r == 1 ? DEFAULT_NOTEXTURE1 : DEFAULT_NOTEXTURE2;
        //printf("[Texture model manager/loader] setting up default texture \"%s\"\n", rand_texture_choice);
        textureID = SOIL_load_OGL_texture(
            rand_texture_choice, 
            SOIL_LOAD_RGBA, 
            SOIL_CREATE_NEW_ID, 
            SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS
        );

        texturePath = std::string(rand_texture_choice);
    }
    
    // Vytvo� novou instanci textury a p�idej ji do mapy
    std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(texturePath, textureID);
    //textures.insert({ texturePath, newTexture });
    addData(newTexture);

    return newTexture;
}

std::shared_ptr<Texture> TextureManager::getTexture(std::string textureName, bool repeat)
{
    std::shared_ptr<Texture> texture;

    if ((texture = getData(textureName)) == nullptr) {
        if (!repeat) {
            return createNoRepeatTexture(textureName);
        }
        return createTexture(textureName);
    }

    return texture;

	//auto it = textures.find(textureName);

	/*if (textures.empty())
	{
		return createTexture(textureName);
	}*/
	/*if (it == textures.end())
	{
        if (!repeat) {
            return createNoRepeatTexture(textureName);
        }
		return createTexture(textureName);
	}

	return it->second;*/
    
}


std::shared_ptr<Texture> TextureManager::createModelTexture(std::string textureName)
{
    //Bind the first texture to the first texture unit.
    //glActiveTexture(GL_TEXTURE0);

    std::string texturePath = textureName;	// "textures\\floor\\floor1.jpg"
    GLuint textureID = SOIL_load_OGL_texture(
        texturePath.c_str(), 
        SOIL_LOAD_RGBA, 
        SOIL_CREATE_NEW_ID, 
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_MIPMAPS
    );

    if (textureID == 0) {
        //printf("[Texture model manager/loader] \"%s\" failed to load texture!\n", texturePath.c_str());

        //int r = rand() % 2 + 1;
        const char* rand_texture_choice = DEFAULT_NOTEXTURE2;   // r == 1 ? DEFAULT_NOTEXTURE1 : DEFAULT_NOTEXTURE2;
        //printf("[Texture model manager/loader] setting up default texture \"%s\"\n", rand_texture_choice);
        textureID = SOIL_load_OGL_texture(
            rand_texture_choice, 
            SOIL_LOAD_RGBA, 
            SOIL_CREATE_NEW_ID, 
            SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_MIPMAPS
        );
        texturePath = std::string(rand_texture_choice);
    }
    
    //glBindTexture(GL_TEXTURE_2D, textureID);

    std::shared_ptr<Texture> newTexture = std::make_shared<Texture>(texturePath, textureID);
    //textures.insert({ texturePath, newTexture });
    addData(newTexture);

    return newTexture;
}

std::shared_ptr<Texture> TextureManager::getModelTexture(std::string textureName)
{
    if (textureName == "no_texture") {
        textureName = "models\\textures\\default2.png";
    }

    std::shared_ptr<Texture> texture;

    if ((texture = getData(textureName)) == nullptr) {
        return createModelTexture(textureName);
    }

    return texture;

    /*auto it = textures.find(textureName);
    //if (textures.empty())
    //{
    //    return createTexture(textureName);
    //}
    if (it == textures.end())
    {
        return createModelTexture(textureName);
    }

    return it->second;*/
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
    addData(newTexture);

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
    addData(newTexture);

    return newTexture;
}

void TextureManager::deleteAllTextureFromGraphicsMemory()
{
    //When you're done using the texture, delete it. This will set texname to 0 and
    //delete all of the graphics card memory associated with the texture. If you
    //don't call this method, the texture will stay in graphics card memory until you
    //close the application.
    //it = mymap.find('b');
    
    deleteAll();

    /*std::unordered_map<std::string, std::shared_ptr<Texture>>::iterator it;
    //for (auto texture : textures) {
    for (it = textures.begin(); it != textures.end(); ++it) {
        std::cout << "Deleting texture: " << it->first << "\n";
        it->second->Delete();
        //it = mymap.find('b');
        textures.erase(it);
    }*/
}

void TextureManager::deleteTextureByName(const std::string& textureName)
{
    //When you're done using the texture, delete it. This will set texname to 0 and
    //delete all of the graphics card memory associated with the texture. If you
    //don't call this method, the texture will stay in graphics card memory until you
    //close the application.
    //it = mymap.find('b');


    // Lambda podm�nka pro odstran�n� prvk� rovn� dataToDelete
    auto condition = [textureName](std::shared_ptr<Texture> texture) { return texture->getName() == textureName; };

    deleteByConditionLambda(condition);
    std::cout << "Deleting single texture: " << textureName << "\n";
    
    /* stary spatny pristup, pres lambdu s template typem mnohem 
       univerzalnejsi i pro ostatni datove typy */
    //deleteOne(textureName);
}
