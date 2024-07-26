#pragma once

#include <GL/glew.h>

class VAO
{
public:
	VAO();
	~VAO();
	void bind() const;
	void unbind() const;

private:
	GLuint m_VAO;
};
