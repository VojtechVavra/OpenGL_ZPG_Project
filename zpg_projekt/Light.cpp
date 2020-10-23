#include <GL/glew.h>
#include <GLFW/glfw3.h>  
#include "Light.hpp"

Light::Light(glm::vec3 position) : Object(position) {
	//this->position = position;
	this->m_matrix = glm::translate(m_matrix, position);
	VAO = 0;
	glGenVertexArrays(1, &VAO);
	//glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO); // pokud bych chtel pozit VBO z jinho objektu treba krychle, pro vykresleni lampy
	// set the vertex attributes (only position data for our lamp)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	render();
}

Light::~Light() {

}

void Light::render() {
	//glBindVertexArray(VAO);
}