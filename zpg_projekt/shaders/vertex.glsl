#version 330 core  // 330 , 420

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
uniform bool isHeld;

void main () {
	// gl_Position = mProj * mView * mModel * vec4(position, 1.0);
	
	if (isHeld) {
		gl_Position = (projectionMatrix * modelMatrix) * vec4(vertexPosition, 1.0);
	}
	else {
		gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vertexPosition, 1.0);  // gl_position became normalized device coordinates (ranging from -1 to +1)
	}
	fragPos = vec4(modelMatrix * vec4(vertexPosition, 1.0f)).xyz;
	normal = vertexNormal;	// normal = normalize( N * vertexNormal ); // N = normal matrix
	textureCoords = uv;
};
