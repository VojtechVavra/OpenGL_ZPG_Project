#pragma once

#include <GL/glew.h>

class VAO
{
public:
	VAO();
	~VAO();
	void bind() const;
	void unbind() const;
	operator bool() const;

private:
	GLuint m_VAO;
};
