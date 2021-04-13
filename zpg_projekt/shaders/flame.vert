/*#version 420   // 330

out vec3 fragPos;				// ex_worldPosition;
out vec3 normal;				// ex_worldNormal;
out vec2 textureCoords;			// textureCoords = uv

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 uv;

//layout(location=1) in vec3 vertexColor; //  // Notice that the 1 here equals the 1 in glVertexAttribPointer

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;			// camera.getViewMatrix()
uniform mat4 projectionMatrix;		// perspective or orthographic effect

void main () {
	gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vertexPosition, 1.0);  // gl_position became normalized device coordinates (ranging from -1 to +1)
	fragPos = vec4(modelMatrix * vec4(vertexPosition, 1.0f)).xyz;
	normal = vertexNormal;	// normal = normalize( N * vertexNormal ); // N = normal matrix
	textureCoords = uv;
};*/


#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;	// modelspace vertex position
layout(location = 1) in vec2 vertexUV;			// texture coordinates
layout(location = 2) in vec3 vertexNormal;		// model space normal in vertex


// Output data ; will be interpolated for each fragment.
out vec2 texCoordUV;
out vec3 fragPos;					// Position in world space
out vec3 normal;					// Surface normal in world space

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;			// camera.getViewMatrix()
uniform mat4 projectionMatrix;		// perspective or orthographic effect

void main()
{
	gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vertexPosition, 1.0);
	
	// UV of the vertex. No special space for this one.
	texCoordUV = vertexUV;
	fragPos = vec3(modelMatrix * vec4(vertexPosition, 1.0f));
	mat3 normal_matrix = transpose(inverse(mat3(modelMatrix)));
	normal = normalize(normal_matrix * vertexNormal);	// normal = normalize( N * vertexNormal ); // N = normal matrix
	// normal = normalize(mat3(viewMatrix) * normal_matrix * vertexNormal); // computes the normal surface in view space
}