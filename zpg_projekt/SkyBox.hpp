#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include <GL/glew.h> // must be always included first!
#include <glm/vec3.hpp>                 // glm::vec3
#include <glm/vec4.hpp>                 // glm::vec4
#include <glm/mat4x4.hpp>               // glm::mat4

#include "Texture.hpp"
#include "ShaderProgram.hpp"


class SkyBox : public Model
{
public:
    //SkyBox(std::shared_ptr<Texture> cubeMap);
    SkyBox(const std::string imageType, const std::string skybox = "countryside2", const float size = 1);

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

    //void initVbo();
    //void initVao();
};

