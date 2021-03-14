#pragma once

#include <unordered_map>
#include <memory>

#include "Texture.hpp"

class TextureManager
{
public:
	static std::shared_ptr<TextureManager> getInstance();

	std::shared_ptr<Texture> getTexture(std::string textureName);
	std::shared_ptr<Texture> createTexture(std::string texturePath);

	std::shared_ptr<Texture> getModelTexture(std::string textureName);
	std::shared_ptr<Texture> createModelTexture(std::string texturePath);

	std::shared_ptr<Texture> createCubemap(std::vector<std::string> faces, const std::string sky);

	std::shared_ptr<Texture> createCubemap2(std::vector<std::string> faces, const std::string sky);

	//std::unordered_map<std::string, Texture> textures;
	std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
private:
	static std::shared_ptr<TextureManager> instance;
	TextureManager();

	const char* DEFAULT_NOTEXTURE1 = "models\\textures\\default1.png";
	const char* DEFAULT_NOTEXTURE2 = "models\\textures\\default2.png";
};

