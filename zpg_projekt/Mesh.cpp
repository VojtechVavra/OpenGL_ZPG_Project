#include "Mesh.hpp"
#include "Shader.hpp"
#include "ObjLoader.hpp"    // added
#include "TextureManager.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned short> indices, std::vector<std::shared_ptr<Texture>> texture) //std::vector<Texture> textures
{
    this->vertices = vertices;
    this->indices = indices;
    this->texture = texture;

    VAO = VBO = IBO = 0;

    setupMesh();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned short> indices, std::vector<Material> material) //std::vector<Texture> textures
{
    this->vertices = vertices;
    this->indices = indices;
    //this->texture = texture;
    this->material = material;


    VAO = VBO = IBO = 0;

    setupMesh();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned short> indices, std::vector<Material> material, std::vector<glm::vec3>& vert) //std::vector<Texture> textures
{
    this->vertices = vertices;
    this->indices = indices;
    //this->texture = texture;
    this->material = material;
    this->vert = vert;

    VAO = VBO = IBO = 0;

    setupMesh();
}

Mesh::Mesh() {}

void Mesh::Draw(GLuint shaderProgram)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < texture.size(); i++)
    {
        // activate proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);   // glActiveTexture(GL_TEXTURE0 + 0);
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = "texture_diffuse"; //textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);


        Shader::sendUniform(shaderProgram, "myTextureSampler", (GLint)0);  // 0 instead of i
        //Shader::sendUniform(shaderProgram, "textureUnitID", 0);

        //shader.setFloat(("material." + name + number).c_str(), i);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        // Bind our texture in Texture Unit 0
        //Shader::sendUniform(shaderProgram, )
        glBindTexture(GL_TEXTURE_2D, texture[i]->getTextureId());
    }
    //glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glBindVertexArray(VAO);
    //
    //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}

void Mesh::Draw2(GLuint shaderProgram)
{
    // TODO: viz odkaz - zkusit
    // https://stackoverflow.com/questions/39069591/trouble-loading-and-drawing-a-obj-model-using-assimp-and-opengl

    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    // Load the texture
    auto textureManager = TextureManager::getInstance();

    for (unsigned int i = 0; i < material.size(); i++)
    {
        // activate proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);   // glActiveTexture(GL_TEXTURE0 + 0);
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = "texture_diffuse"; //textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);


        Shader::sendUniform(shaderProgram, "myTextureSampler", (GLint)i);  // 0 instead of i
        //Shader::sendUniform(shaderProgram, "textureUnitID", 0);

        std::shared_ptr<Texture> texture2;
        try
        {
            texture2 = textureManager->getTexture("..\\" + material[i].diffuseMap);   // "..\\models\\cube\\test.png"
        }
        catch (const std::exception& e)
        {
            printf(" a standard exception was caught, with message '%s'\n", e.what());
            //std::cout << " a standard exception was caught, with message '" << e.what() << "'\n";
            getchar();
        }
        
        glBindTexture(GL_TEXTURE_2D, texture2->getTextureId());
    }
    //glActiveTexture(GL_TEXTURE0);

    // draw mesh - Bind the VAO
    glBindVertexArray(VAO);
    //
    //Bind the ELement Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_TRIANGLES, 0, this->vertices.size());   // vertices
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_SHORT, NULL);

    //Unbind the VAO
    //glBindVertexArray(0);
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);  // uvBuffer - texture indices

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // GL_ARRAY_BUFFER

    //glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // funguje
    //glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * this->vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * this->vertices.size(), &vertices[0], GL_DYNAMIC_DRAW); // GL_STATIC_DRAW
    //

    //glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * indices.size(),
        &indices[0], GL_STATIC_DRAW);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // vertex normals
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    // vertex texture coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
}
