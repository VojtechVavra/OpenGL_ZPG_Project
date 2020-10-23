#version 400

out vec4 out_Color; // original name: frag_colour

in vec3 fragmentColor;
in vec3 lightPosition;
in vec3 ex_worldPosition; // fragPos
in vec3 ex_worldNormal;   // normal

uniform vec3 lightColor; // = vec3(1.0, 0.0, 0.5);
uniform vec3 viewPos = vec3(1.0, 0.0, 0.0);

//uniform vec3 lightPosition;   //the shader has light position
//uniform vec3 viewPos;
//out vec4 frag_colour;

void main () {
//ambient
    float ambientStrength = 0.20f;
    vec3 ambient = ambientStrength * lightColor;
//diffuse
    vec3 norm = normalize(ex_worldNormal);
    vec3 lightDir = normalize(lightPosition - ex_worldPosition);
    float diff = max(dot(norm, lightDir), 0.0);  // dot product?
    vec3 diffuse = diff * lightColor;
// specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - ex_worldPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);   // h = 32
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * fragmentColor; // objectColor;
    out_Color = vec4(result, 1.0f);
//result
    //vec3 result = (ambient + diffuse) * objectColor;
    //vec3 result = (ambient + diffuse) * fragmentColor;
    //out_Color = vec4(result, 1.0);
};

/*
    "//ambient"
    "   float ambientStrength = 1.0f;"
    "   vec3 ambient = ambientStrength * lightColor;"
    "// diffuse"
    "   vec3 norm = normalize(ex_worldNormal);"
    "   vec3 lightDir = normalize(lightPos - ex_worldPosition);"
    "   float diff = max(dot(norm, lightDir), 0.0);"
    "   vec3 diffuse = diff * lightColor;"
    "// specular"
    "   float specularStrength = 0.5f;"
    "   vec3 viewDir = normalize(viewPos - ex_worldPosition);"
    "   vec3 reflectDir = reflect(-lightDir, norm);"
    "   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);"
    "   vec3 specular = specularStrength * spec * lightColor;"

    "   vec3 result = (ambient + diffuse + specular) * objectColor;"
    "   color = vec4(result, 1.0f);"
*/