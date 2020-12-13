#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "Texture.hpp"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Mesh
{

public:
    // mesh data
    std::vector<Vertex>       vertices;
    std::vector<unsigned short> indices;
    std::shared_ptr<Texture> texture;
    //std::vector<Texture>      textures;
    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned short> indices, std::shared_ptr<Texture> texture); // std::vector<Texture> textures
    void Draw(GLuint shaderProgram);    
    //void Draw(Shader& shader);
private:
    //  render data
    unsigned int VAO, VBO, EBO;

    void setupMesh();
};

