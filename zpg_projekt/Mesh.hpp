#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "Texture.hpp"

// https://stackoverflow.com/questions/21652546/what-is-the-role-of-glbindvertexarrays-vs-glbindbuffer-and-what-is-their-relatio

// Material mam duplicitni v ObjLoader.hpp -> presunout ho do samostatneho souboru a jen ho importovat
struct Material;/* {
    glm::vec3 diffuse{ 0.f };
    glm::vec3 specular{ 0.f };
    glm::vec3 ambient{ 0.f };
    std::string diffuseMap;
    std::string specularMap;
    std::string heightMap;
    float shininess = 0.f;
};*/

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
    std::vector<std::shared_ptr<Texture>> texture;
    std::vector<Material> material;

    // smazat
    std::vector<glm::vec3> vert;
    //

    //std::vector<Texture>      textures;
    Mesh();
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned short> indices, std::vector<std::shared_ptr<Texture>> texture); // std::vector<Texture> textures
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned short> indices, std::vector<Material> material);

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned short> indices, std::vector<Material> material, std::vector<glm::vec3>& vert);
    
    void Draw(GLuint shaderProgram);    
    void Draw2(GLuint shaderProgram);  // used for material input
    //void Draw(Shader& shader);
private:
    //  render data
    unsigned int VAO, VBO, EBO;
    unsigned int IBO;

    void setupMesh();
};

