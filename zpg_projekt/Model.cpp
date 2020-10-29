#include <GL/glew.h> // must be always included first!
#include <GLFW/glfw3.h> // this will include <GL/gl.h>

#include "Model.hpp"
//#include "Shader.hpp"


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

    if (modelName == "plain") {
        vertexCount = 6;
        glBufferData(GL_ARRAY_BUFFER, sizeof(plain), plain, GL_STATIC_DRAW);
    }
	else if (modelName == "sphere")
	{
		vertexCount = 2880; // 2880
        glBufferData(GL_ARRAY_BUFFER, sizeof(sphere), sphere, GL_STATIC_DRAW);
	}
    else if (modelName == "suzi_smooth") {
        vertexCount = 2904;
        glBufferData(GL_ARRAY_BUFFER, sizeof(suziSmooth), suziSmooth, GL_STATIC_DRAW);
    }
    else if (modelName == "suzi_flat") {
        vertexCount = 2904;
        glBufferData(GL_ARRAY_BUFFER, sizeof(suziFlat), suziFlat, GL_STATIC_DRAW);
    }
    
    glBindVertexArray(VAO);

    // position attribute
    // Vezni 3 hodnoty z pole (typu float) z sesti. Zacni brat od nulteho prvku.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);   //enable vertex attributes
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
    // Vezni 3 hodnoty typu float z sesti hodnot. Zacni od 3. pozice tzn 4. hodnota
    glEnableVertexAttribArray(1);
}

void Model::render() {
	// render the cube
	//glBindVertexArray(VAO); 
	glDrawArrays(GL_TRIANGLES, 0, vertexCount); // mode, first, count
}

/*GLuint Model::getVAO()
{
	return VAO;
}*/

void Model::bindVAO()
{
    //glBindVertexArray(0);
    glBindVertexArray(VAO);
}