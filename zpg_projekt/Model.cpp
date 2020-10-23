
#include "Model.hpp"
#include "Shader.hpp"
#include <GLFW/glfw3.h>

Model::Model()
{
	// camera
}
Model::Model(std::string modelName/*, Shader shader*/, GLuint vertCount)
{
   // this->shader = shader; //Shader(1);

    VAO = VBO = 0;

    // first, configure the cube's VAO (and then VBO)
    glGenVertexArrays(1, &VAO); //generate the VAO
    glGenBuffers(1, &VBO);      // generate one VBO buffer

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

	if (modelName == "koule")
	{
		vertexCount = 2880;
        glBufferData(GL_ARRAY_BUFFER, sizeof(sphere), sphere, GL_STATIC_DRAW);
	}
    else if (modelName == "suzi_smooth") {
        vertexCount = 2904;
        glBufferData(GL_ARRAY_BUFFER, sizeof(suziSmooth), suziSmooth, GL_STATIC_DRAW);
    }


    glBindVertexArray(VAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);   //enable vertex attributes
    // normal attribute
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);
}

void Model::render() {
	// render the cube
	//glBindVertexArray(VAO); 
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

GLuint Model::getVAO()
{
	return VAO;
}