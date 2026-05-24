#include "SkyBox.hpp"
#include "Camera.hpp"
#include "SOIL.h"

#include <filesystem>
#include <iostream>
#include <stdexcept>


SkyBox::SkyBox(const std::string& imagePath, const std::string& imageExtension, float size)
{
    initMesh(size);
    initCubemap(imagePath, imageExtension);
}

SkyBox::~SkyBox()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vboVertices);
    glDeleteBuffers(1, &vboIndices);
    glDeleteTextures(1, &cubemapTexture);
}

void SkyBox::initMesh(float size)
{
    const GLfloat vertices[] = {
         size,  size,  size,
         size, -size,  size,
         size,  size, -size,
         size, -size, -size,
        -size, -size, -size,
        -size,  size, -size,
        -size, -size,  size,
        -size,  size,  size
    };

    const GLuint indices[] = {
        0,1,3, 3,2,0,
        0,1,7, 7,6,1,
        1,3,6, 6,4,3,
        3,2,4, 4,2,5,
        5,4,6, 6,5,7,
        7,5,2, 2,0,7
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vboVertices);
    glGenBuffers(1, &vboIndices);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void SkyBox::initCubemap(const std::string& imagePath, const std::string& ext)
{
    const std::vector<std::string> faces = {
        (imagePath + "\\right."  + ext),
        (imagePath + "\\left."   + ext),
        (imagePath + "\\up."    + ext),     // top
        (imagePath + "\\down." + ext),      // bottom
        (imagePath + "\\back."   + ext),
        (imagePath + "\\front."  + ext),
    };

    cubemapTexture = loadCubemap(faces);
}
#include <windows.h>
GLuint SkyBox::loadCubemap(const std::vector<std::string>& faces)
{
    GLuint textureID = 0;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, channels;
    for (GLuint i = 0; i < faces.size(); ++i)
    {
        GLubyte* data = SOIL_load_image(faces[i].c_str(), &width, &height, &channels, SOIL_LOAD_RGB);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            SOIL_free_image_data(data);
        }
        else
        {
            SOIL_free_image_data(data);
            glDeleteTextures(1, &textureID);
            std::cerr << "SOIL error: " << SOIL_last_result() << std::endl;
            throw std::runtime_error("Cubemap face failed to load: " + faces[i]);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void SkyBox::draw(const ShaderProgram& shader, const std::shared_ptr<Camera>& camera) const
{
    glUseProgram(shader.shaderProgram);
    Shader::sendUniform(shader.shaderProgram, "viewMatrix",       camera->getCamera());
    Shader::sendUniform(shader.shaderProgram, "projectionMatrix", camera->getProjectionMatrix());
    Shader::sendUniform(shader.shaderProgram, "skybox", 0);

    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawElements(GL_TRIANGLE_STRIP, 36, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
