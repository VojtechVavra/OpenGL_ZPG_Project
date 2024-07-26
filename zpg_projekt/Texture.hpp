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

	void Bind() const;
	void Delete();
private:
	GLuint textureId;
	std::string texturePath;

	// TODO: this param name is not used, but it should also be set
	std::string name;
};
