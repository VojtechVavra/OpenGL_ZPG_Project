#pragma once

#include <string>

#include <GL/glew.h> // must be always included first!

class Texture
{
public:
	Texture(std::string texturePath, GLuint textureId);
	~Texture();

	void Load() const;
	void LoadCubemap() const;

	GLuint getTextureId() const;
	std::string getName() const;
	std::string getPath() const;

	// operator == to compare Texture object with std::string name
	bool operator==(const std::string& rhs) {
		return this->texturePath == rhs;
	}

	// Operátor < potøebný pro std::set (pouzito v Mesh.cpp)
	bool operator<(const Texture& other) const {
		return textureId < other.textureId;
	}

	void Bind() const;
	
	void Delete();
private:
	std::string getLastNameAfterDelimiter(const std::string& path);
	

private:
	GLuint textureId;
	std::string texturePath;

	// TODO: this param name is not used, but it should also be set
	std::string m_name;
};
