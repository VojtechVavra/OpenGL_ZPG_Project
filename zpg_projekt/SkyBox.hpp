#pragma once

#include <memory>
#include <vector>

#include <GL/glew.h>            // must be always included first!
#include <glm/vec3.hpp>         // glm::vec3
#include <glm/vec4.hpp>         // glm::vec4
#include <glm/mat4x4.hpp>       // glm::mat4

#include "Texture.hpp"
#include "ShaderProgram.hpp"


// IMPORTANT
// Images must be in cube format all in same size.
// for example 1024x1024 each, otherwise skybox will be black

class SkyBox
{
public:
    SkyBox(const std::string imageType, const std::string skybox, const float size = 1);
    ~SkyBox();

    unsigned int loadCubemap(std::vector<std::string> faces, const std::string sky);
    void InitJpg(const float size, const std::string sky);
    void InitTga(const float size, const std::string sky);

    void draw(const ShaderProgram& shader, const std::shared_ptr<Camera>& camera);
    //void draw() const;
private:
    GLuint VBOvertices, VBOindices;
    unsigned int cubemapTexture;

    std::shared_ptr<Texture> cubeMap;
    //Shader& shader;

    GLuint vbo = 0;
    GLuint vao = 0;

    GLuint textureId;
};
