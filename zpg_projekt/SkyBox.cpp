#include "SkyBox.hpp"
#include "Camera.hpp"

#include "SOIL.h"


/*SkyBox::SkyBox(std::shared_ptr<Texture> cubeMap)
{
}*/

SkyBox::SkyBox(const std::string imageType, const std::string skybox, const float size) /*: Model("cube", true)*/ {
    if (imageType == "jpg") {
        InitJpg(size, skybox);
    }
    else if(imageType == "tga") {
        InitTga(size, skybox);
    }  
};

//skybox path = /textures/skybox/countryside/
unsigned int SkyBox::loadCubemap(std::vector<std::string> faces, const std::string sky)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    std::string path = std::string("textures\\skybox\\") + sky + std::string("\\");
    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        std::string file = path + faces[i];
        std::cerr << file << std::endl;
        GLubyte* data = SOIL_load_image(file.c_str(), &width, &height, &nrChannels, SOIL_LOAD_AUTO);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            // glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
            SOIL_free_image_data(data);
            // glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            SOIL_free_image_data(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    

    return textureID;
}

void SkyBox::InitJpg(const float size, const std::string sky) {
    GLfloat sky_vertices[] = {
        size,  size,  size,
        size, -size,  size,
        size,  size, -size,
        size, -size, -size,
       -size, -size, -size,
       -size,  size, -size,
       -size, -size,  size,
       -size,  size,  size
    };

    GLuint sky_indices[] = {
        0, 1, 3,
        3, 2, 0,
        0, 1, 7,
        7, 6, 1,
        1, 3, 6,
        6, 4, 3,
        3, 2, 4,
        4, 2, 5,
        5, 4, 6,
        6, 5, 7,
        7, 5, 2,
        2, 0, 7
    };

    // IMPORTANT
    // Images must be in cube format all in same size.
    // for example 1024x1024 each, otherwise skybox will be black
    std::vector<std::string> faces
    {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "back.jpg",
        "front.jpg"
    };
    

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &VBOvertices);
    glGenBuffers(1, &VBOindices);

    cubemapTexture = loadCubemap(faces, sky);
    glBindVertexArray(vao);

    //passing the vertex coordinates attribute to the shader program
    glBindBuffer(GL_ARRAY_BUFFER, VBOvertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sky_vertices), sky_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //passing indices to the shader program
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOindices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sky_indices), sky_indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void SkyBox::InitTga(const float size, const std::string sky) {
    GLfloat sky_vertices[] = {
        size,  size,  size,
        size, -size,  size,
        size,  size, -size,
        size, -size, -size,
       -size, -size, -size,
       -size,  size, -size,
       -size, -size,  size,
       -size,  size,  size
    };

    GLuint sky_indices[] = {
        0, 1, 3,
        3, 2, 0,
        0, 1, 7,
        7, 6, 1,
        1, 3, 6,
        6, 4, 3,
        3, 2, 4,
        4, 2, 5,
        5, 4, 6,
        6, 5, 7,
        7, 5, 2,
        2, 0, 7
    };

    // IMPORTANT
    // Images must be in cube format all in same size.
    // for example 1024x1024 each, otherwise skybox will be black
    std::vector<std::string> faces
    {
        "right.tga",
        "left.tga",
        "up.tga",
        "down.tga",
        "back.tga",
        "front.tga"
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &VBOvertices);
    glGenBuffers(1, &VBOindices);

    cubemapTexture = loadCubemap(faces, sky);
    glBindVertexArray(vao);

    //passing the vertex coordinates attribute to the shader program
    glBindBuffer(GL_ARRAY_BUFFER, VBOvertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sky_vertices), sky_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //passing indices to the shader program
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOindices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sky_indices), sky_indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void SkyBox::draw(const ShaderProgram& shader, const std::shared_ptr<Camera>& camera)
{
    //glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    //shader.StartUseShader();
    //glDepthMask(GL_FALSE);
    
    glUseProgram(shader.shaderProgram);

    Shader::sendUniform(shader.shaderProgram, "viewMatrix", camera->getCamera());
    Shader::sendUniform(shader.shaderProgram, "projectionMatrix", camera->getProjectionMatrix());
    //Shader::sendUniform(shader.shaderProgram, "viewPos", camera->getPosition());
    //shader.SetUniform("campos", campos);

    glBindVertexArray(vao);   // funguje
    //bindVAO();

    //glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    //shader.SetUniform("skybox", 0);
    Shader::sendUniform(shader.shaderProgram, "skybox", 0);
    
    //render();
    glDrawElements(GL_TRIANGLE_STRIP, 36, GL_UNSIGNED_INT, nullptr); // funguje
    //glBindVertexArray(0);
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    //glDepthMask(GL_TRUE);
    //glDepthFunc(GL_LESS); // set depth function back to default
}
