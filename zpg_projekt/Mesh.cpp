#include "Mesh.hpp"
#include "Shader.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned short> indices, std::shared_ptr<Texture> texture) //std::vector<Texture> textures
{
    this->vertices = vertices;
    this->indices = indices;
    this->texture = texture;

    setupMesh();
}

Mesh::Mesh() {}

void Mesh::Draw(GLuint shaderProgram)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    //for (unsigned int i = 0; i < textures.size(); i++)
    //{
        glActiveTexture(GL_TEXTURE0 + 0); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = "texture_diffuse"; //textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);


        Shader::sendUniform(shaderProgram, "myTextureSampler", 0);
        //shader.setFloat(("material." + name + number).c_str(), i);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        // Bind our texture in Texture Unit 0
        //Shader::sendUniform(shaderProgram, )
        glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
    //}
    //glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);

    //

    //


    //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);  // uvBuffer - texture indices

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // vertex normals
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}
