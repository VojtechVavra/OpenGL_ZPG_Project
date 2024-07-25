#include <GL/glew.h> // must be always included first!
#include <GLFW/glfw3.h> // this will include <GL/gl.h>

#include "Model.hpp"
//#include "Shader.hpp"

// https://www.cs.utexas.edu/~fussell/courses/cs384g-spring2017/lectures/Lecture9-Zbuffer_pipeline.pdf

Model::Model()
{
	// camera
}
Model::Model(std::string modelName/*, Shader shader*/, GLuint vertCount)
{
    bool  isTextured = false;
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
    else if (modelName == "plainTextured") {
        isTextured = true;
        vertexCount = 6;
        glBufferData(GL_ARRAY_BUFFER, sizeof(plainTextured), plainTextured, GL_STATIC_DRAW);
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
    else if (modelName == "bedna") {
        vertexCount = 522;
        glBufferData(GL_ARRAY_BUFFER, sizeof(bedna), bedna, GL_STATIC_DRAW);
    }
    else if (modelName == "worker") {
        vertexCount = 30681;
        glBufferData(GL_ARRAY_BUFFER, sizeof(workerVertices), workerVertices, GL_STATIC_DRAW);
    }
    else if (modelName == "cube") {
        vertexCount = 36;
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_simple2), cube_simple2, GL_STATIC_DRAW);

        glBindVertexArray(VAO);

        // position attribute
        // Vezni 3 hodnoty z pole (typu float) z sesti. Zacni brat od nulteho prvku.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);   //enable vertex attributes
        // normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
        // Vezni 3 hodnoty typu float z sesti hodnot. Zacni od 3. pozice tzn 4. hodnota
        glEnableVertexAttribArray(1);

        return;
    }
    
    if (isTextured)
    {
        glBindVertexArray(VAO);

        //enable vertex attributes
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        
        // vertex position
        // vem 3 floaty z osmi a bude to ze zacatku od nulteho prvku
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)0);
        // normal attribute
        // vezne 3 floaty z osmi ale vezne je s posunem vuci zacatku
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
        // texture coordinates
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));

        // position attribute
        // Vezni 3 hodnoty z pole (typu float) z sesti. Zacni brat od nulteho prvku.
        //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        //glEnableVertexAttribArray(0);   //enable vertex attributes
        // normal attribute
        //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
        // Vezni 3 hodnoty typu float z sesti hodnot. Zacni od 3. pozice tzn 4. hodnota
        //glEnableVertexAttribArray(1);
        
        return;
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


Model::Model(std::string modelName, bool _isTextured)
{
    bool  isTextured = _isTextured;

    VAO = VBO = 0;
    vertexCount = 36;

    // first, configure the cube's VAO (and then VBO)
    //glGenVertexArrays(1, &VAO); //generate the VAO
    glGenBuffers(1, &VBO);      // generate one VBO buffer

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, cubemapPoints.size() * sizeof(cubemapPoints[0]), cubemapPoints.data(), GL_STATIC_DRAW);

    /*if (modelName == "cube") {
        vertexCount = 36;
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_simple2), cube_simple2, GL_STATIC_DRAW);

        glBindVertexArray(VAO);

        // position attribute
        // Vezni 3 hodnoty z pole (typu float) z sesti. Zacni brat od nulteho prvku.
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);   //enable vertex attributes
        return;
    }*/

    // vao
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //constexpr int typeSize = sizeof(cubemapPoints[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(cubemapPoints[0]), nullptr);
}

void Model::render() {
	// render the cube
	//glBindVertexArray(VAO); 
	glDrawArrays(GL_TRIANGLES, 0, vertexCount); // mode, first, count
    glBindVertexArray(0);
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




const std::vector<float> Model::cubemapPoints = {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
};
