#pragma once

#include <GL/glew.h>

class VBO
{
public:
    VBO();
    ~VBO();
    void bind();
    void unbind();
    void setData(GLsizeiptr size, const void* data, GLenum usage = GL_STATIC_DRAW);

private:
    GLuint m_VBO;
};
