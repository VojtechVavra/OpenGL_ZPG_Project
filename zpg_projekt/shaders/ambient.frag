#version 400

out vec4 out_color;

//in vec3 fragmentColor;

uniform vec3 fragmentColor;
uniform float ambientStrength = 0.2;

void main()
{
	out_color = vec4(ambientStrength * fragmentColor, 1.0);
}