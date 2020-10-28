#version 420   // 330

out vec3 ex_worldPosition;
out vec3 ex_worldNormal;
//out vec3 fragmentColor;
//out vec3 lightPosition;

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vertNormal;
//layout(location=1) in vec3 vertexColor; //  // Notice that the 1 here equals the 1 in glVertexAttribPointer

uniform mat4 modelMatrix;           // = mat4(1.0f);
uniform mat4 viewMatrix;			// = mat4(1.0f);	// camera.getViewMatrix()
uniform mat4 projectionMatrix;		// = mat4(1.0f);   // perspective or orthographic effect
//uniform vec3 fragColor;
//uniform vec3 newLightPos;


void main () {
// gl_Position = mProj * mView * mModel * vec4(position, 1.0);
   gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vp, 1.0);  // gl_position became normalized device coordinates (ranging from -1 to +1)
   ex_worldPosition = vec4(modelMatrix * vec4(vp, 1.0f)).xyz;
   ex_worldNormal = vertNormal;

   //fragmentColor = fragColor;
   //lightPosition = newLightPos;
};