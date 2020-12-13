#pragma once

#include <string>

#include <GL/glew.h> // must be always included first!

class Texture
{
public:
	Texture(std::string texturePath, GLuint textureId);
	void Load() const;
	void LoadCubemap() const;
	GLuint getTextureId();
private:
	GLuint textureId;
	std::string texturePath;
};

