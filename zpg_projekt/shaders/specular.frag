#version 400

out vec4 out_color; // original name: frag_color

in vec3 fragPos;    //ex_worldPosition; // fragPos
in vec3 normal;     //ex_worldNormal

uniform vec3 fragmentColor;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform float specularStrength = 0.5f;  // 0.5


void main() {
    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 lightDir = normalize(lightPosition - fragPos);
    vec3 norm = normalize(normal);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);   // h = 32
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = specular * fragmentColor;   // objectColor;
    out_color = vec4(result, 1.0f);
};
