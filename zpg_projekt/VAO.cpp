#include "VAO.hpp"
//#include <GL/glew.h>

VAO::VAO()
{
    //generate one VAO
    glGenVertexArrays(1, &m_VAO);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &m_VAO);
}

void VAO::bind() const
{
	glBindVertexArray(m_VAO);
}

void VAO::unbind() const
{
    glBindVertexArray(0);
}
