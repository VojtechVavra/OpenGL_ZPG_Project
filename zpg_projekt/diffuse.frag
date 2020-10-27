#version 400

out vec4 out_color;


in vec3 ex_worldPosition;   // fragPos
in vec3 ex_worldNormal;     // normal

uniform vec3 fragmentColor;      // objectColor
uniform vec3 lightPosition;
uniform vec3 lightColor;     // = vec3(1.0, 0.0, 0.5);


void main() 
{
    //diffuse
    vec3 norm = normalize(ex_worldNormal);
    vec3 lightDir = normalize(lightPosition - ex_worldPosition);
    float diff = max(dot(norm, lightDir), 0.0);  // dot product - jak jsou si 2 vektory podobne
    vec3 diffuse = diff * lightColor;

    vec3 result = diffuse * fragmentColor; // fragmentColor = objectColor
    out_color = vec4(result, 1.0f);
};
