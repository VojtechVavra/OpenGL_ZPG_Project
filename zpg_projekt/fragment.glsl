#version 400

in vec3 fragmentColor;
in vec3 ex_worldPosition; // fragPos
in vec3 ex_worldNormal;   // normal
in vec3 lightPosition;
//uniform vec3 lightPosition;   //the shader has light position
//uniform vec3 viewPos;
uniform vec3 lightColor = vec3(1.0, 0.0, 0.5);
//out vec4 frag_colour;
out vec4 out_Color; // original name: frag_colour
void main () {
//ambient
   float ambientStrength = 1.0f;
   vec3 ambient = ambientStrength * lightColor;
//diffuse
   vec3 norm = normalize(ex_worldNormal);
   vec3 lightDir = normalize(lightPosition - ex_worldPosition);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = diff * lightColor;
//result
//    vec3 result = (ambient + diffuse) * objectColor;
   vec3 result = (ambient + diffuse) * fragmentColor;
   out_Color = vec4(result, 1.0);
};