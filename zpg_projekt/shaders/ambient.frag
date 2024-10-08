#version 330 core // 400

out vec4 out_color;

uniform vec3 fragmentColor;
uniform float ambientStrength = 0.2;

void main()
{
	out_color = vec4(ambientStrength * fragmentColor, 1.0);
}
