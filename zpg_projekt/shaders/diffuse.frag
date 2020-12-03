#version 400

out vec4 out_color;


in vec3 fragPos;    // ex_worldPosition
in vec3 normal;     // ex_worldNormal

uniform vec3 fragmentColor;      // objectColor
uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform float ambientStrength = 0.0;

void main() 
{
    //diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosition - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);  // dot product - jak jsou si 2 vektory podobne
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambientStrength + diffuse) * fragmentColor; // fragmentColor = objectColor

    out_color = vec4(result, 1.0f);
};
