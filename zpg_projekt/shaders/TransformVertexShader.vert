#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;	// modelspace vertex position
layout(location = 1) in vec2 vertexUV;			// texture coordinates
layout(location = 2) in vec3 vertexNormal;		// model space normal in vertex


// Output data ; will be interpolated for each fragment.
out vec2 texCoordUV;
out vec3 fragPos;			// added
out vec3 normal;			// added

// Values that stay constant for the whole mesh.
//uniform mat4 MVP;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;			// camera.getViewMatrix()
uniform mat4 projectionMatrix;		// perspective or orthographic effect

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vertexPosition, 1.0);
	
	// UV of the vertex. No special space for this one.
	texCoordUV = vertexUV;
	fragPos = vec3(modelMatrix * vec4(vertexPosition, 1.0f));
	mat3 normal_matrix = transpose(inverse(mat3(modelMatrix)));
	normal = normalize(normal_matrix * vertexNormal);	// normal = normalize( N * vertexNormal ); // N = normal matrix
	//normal = vertexNormal;
}