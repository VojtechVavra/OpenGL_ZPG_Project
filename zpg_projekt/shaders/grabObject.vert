#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;	// modelspace vertex position
layout(location = 1) in vec2 vertexUV;			// texture coordinates
layout(location = 2) in vec3 vertexNormal;		// model space normal in vertex


// Output data ; will be interpolated for each fragment.
out vec2 texCoordUV;
out vec3 fragPos;					// Position in world space
out vec3 normal;					// Surface normal in world space
out vec4 textureCoordProj;			//  the texture coordinates
out vec4 textureCoordProj2;			//  the texture coordinates
//out mat4 textureCoordProj;		//  the texture coordinates

// Values that stay constant for the whole mesh.
//uniform mat4 MVP;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;			// camera.getViewMatrix()
uniform mat4 projectionMatrix;		// perspective or orthographic effect
uniform mat4 viewMatrix2;
uniform mat4 textureMatrix;
// The model-view-projection matrix for the projector
		// This matrix is used to calculate the texture coordinates for the vertex. 
//uniform mat4x4 textureMatrix;

void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position = (projectionMatrix * modelMatrix) * vec4(vertexPosition, 1.0);
	//gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vertexPosition, 1.0);
	
	// UV of the vertex. No special space for this one.
	texCoordUV = vertexUV;
	fragPos = vec3(modelMatrix * vec4(vertexPosition, 1.0f));
	mat3 normal_matrix = transpose(inverse(mat3(modelMatrix)));
	normal = normalize(normal_matrix * vertexNormal);	// normal = normalize( N * vertexNormal ); // N = normal matrix
	// normal = normalize(mat3(viewMatrix) * normal_matrix * vertexNormal); // computes the normal surface in view space

	// Calculates the texture coordinates for the vertex
	// Project the vertex to a 2D picture and then take the 2D coordinates as 
	// the texture coordinates.
	//mat4x4 textureMatrix;
	//textureMatrix = mat4x4(vec4(1.0, 0.0, 0.0, 0.0), vec4(0.0, 1.0, 0.0, 0.0), vec4(0.0, 0.0, 1.0, 0.0), vec4(0.0, 0.0, 0.0, 1.0));
	//textureMatrix = mat4(1.0);
	//textureCoordProj = textureMatrix * modelMatrix * vec4(vertexPosition, 1.0);
	
	textureCoordProj = viewMatrix2 /*viewMatrix*/ * textureMatrix /** modelMatrix*/ * vec4(vertexPosition, 1.0f);	//  (viewMatrix * modelMatrix * vec4(vertexPosition, 1.0f); 
	//textureCoordProj = vec4(clamp(textureCoordProj.x, 1.0, 0.0), clamp(textureCoordProj.y, 1.0, 0.0), textureCoordProj.z, 1.0);
	//textureCoordProj = vec4(vertexPosition, 1.0);
	
	// If modelMatrix is removed from this calculation, the projected texture will
		// "stick" to the object, rather than be fixed in space. This is called texture decal. 
		// "   textureCoordProj = textureMatrix * vPos;"



	textureCoordProj2 = vec4(vertexPosition, 1.0f);	//  * modelMatrix 

}






/*
#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;	// modelspace vertex position
layout(location = 1) in vec2 vertexUV;			// texture coordinates
layout(location = 2) in vec3 vertexNormal;		// model space normal in vertex


// Output data ; will be interpolated for each fragment.
out vec2 texCoordUV;
out vec3 fragPos;					// Position in world space
out vec3 normal;					// Surface normal in world space
out vec4 textureCoordProj;			//  the texture coordinates

// Values that stay constant for the whole mesh.
//uniform mat4 MVP;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;			// camera.getViewMatrix()
uniform mat4 projectionMatrix;		// perspective or orthographic effect
uniform mat4 textureMatrix;


void main(){

	// Output position of the vertex, in clip space : MVP * position
	gl_Position = (projectionMatrix * modelMatrix) * vec4(vertexPosition, 1.0);
	//gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vertexPosition, 1.0);
	
	// UV of the vertex. No special space for this one.
	texCoordUV = vertexUV;
	fragPos = vec3(modelMatrix * vec4(vertexPosition, 1.0f));
	mat3 normal_matrix = transpose(inverse(mat3(modelMatrix)));
	normal = normalize(normal_matrix * vertexNormal);	// normal = normalize( N * vertexNormal ); // N = normal matrix
	
	textureCoordProj = viewMatrix * modelMatrix * vec4(vertexPosition, 1.0f);	//  (viewMatrix * modelMatrix * vec4(vertexPosition, 1.0f); 
}*/