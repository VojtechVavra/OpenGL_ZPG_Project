#include "VBO.hpp"

VBO::VBO()
{
    // generate one VBO buffer
    glGenBuffers(1, &m_VBO);
}

VBO::~VBO() {
    glDeleteBuffers(1, &m_VBO);
}

void VBO::bind() {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}

void VBO::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::setData(GLsizeiptr size, const void* data, GLenum usage) {
    glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

/* 
 *  Return true if m_VBO buffer exist, else false
*/
VBO::operator bool() const
{
    return glIsBuffer(m_VBO);
}
