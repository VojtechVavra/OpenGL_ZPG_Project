#version 330

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform vec3 fragColor;

layout(location=0) in vec3 vp;

out vec3 fragmentColor;

void main () {
	gl_Position = (projectionMatrix * viewMatrix * modelMatrix) * vec4(vp, 1.0);
	fragmentColor = fragColor;
}