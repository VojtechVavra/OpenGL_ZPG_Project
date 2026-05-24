#pragma once

#include <memory>
#include <string>
#include <vector>

#include <GL/glew.h>            // must be always included first!
#include "ShaderProgram.hpp"

class Camera;

// IMPORTANT: All cubemap images must be square and the same size (e.g. 1024x1024)
class SkyBox
{
public:
    SkyBox(const std::string& imagePath, const std::string& imageExtension, float size = 1.0f);
    ~SkyBox();

    // Non-copyable (owns OpenGL resources)
    SkyBox(const SkyBox&) = delete;
    SkyBox& operator=(const SkyBox&) = delete;

    void draw(const ShaderProgram& shader, const std::shared_ptr<Camera>& camera) const;

private:
    GLuint vao = 0;
    GLuint vboVertices = 0;
    GLuint vboIndices = 0;
    GLuint cubemapTexture = 0;

    void initMesh(float size);
    void initCubemap(const std::string& imagePath, const std::string& imageExtension);
    GLuint loadCubemap(const std::vector<std::string>& faces);
};
