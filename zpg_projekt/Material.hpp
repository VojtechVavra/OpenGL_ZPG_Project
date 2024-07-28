#pragma once

#include <string>

#include <GL/glew.h>	    // must be always included first!
#include <glm/vec3.hpp>// glm::vec3

// Material table example at:
// http://devernay.free.fr/cours/opengl/materials.html
// Another example site
// http://www.it.hiof.no/~borres/j3d/explain/light/p-materials.html
// https://learnopengl.com/Lighting/Materials



struct Material {
    // materials
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    // texture names
    std::string diffuseMap;
    std::string specularMap;
    std::string heightMap;
};