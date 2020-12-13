
//uniform vec3 lightPosition;                              // lights.position
//uniform vec3 lightColor; // = vec3(1.0, 0.0, 0.5);      // lights.color

/////////////////////////////////////
#version 400

//uniform mat4 model;
uniform vec3 viewPos;
///
in vec3 fragPos; //     // ex_worldPosition
in vec3 normal;   // ex_worldNormal

uniform vec3 fragmentColor;

// texture
uniform int hasTexture = 0;
uniform sampler2D textureUnitID;
in vec2 textureCoords;

//uniform vec3 viewPos = vec3(0.0, 0.0, 0.0);
uniform float ambientStrength = 0.1f;   // constatnt color - good 0.2f - 0.1f
uniform float specularStrength = 0.5f;  // 0.5



#define MAX_LIGHTS 64
uniform int pointLightCount;
uniform int spotLightCount;

uniform struct PointLight {
    int lightType;
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}pointLight [MAX_LIGHTS];
//uniform PointLight pointLight;

struct DirLight {
    int lightType;

    vec3 direction;
    vec3 color;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;

uniform struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;

    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    int isActive;
}spotLight [MAX_LIGHTS];

uniform SpotLight flashLight;

//vec3 calculateAmbient(Light light);
vec3 calculateDiffuse(vec3 position, vec3 _lightColor);
vec3 calculateSpecular(vec3 position, vec3 _lightColor);


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcFlashLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

//in vec2 fragTexCoord;
//n vec3 ex_worldNormal; // fragNormal
//in vec3 fragVert;

out vec4 out_Color;


void main()
{
    // properties
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, normalize(viewPos));
    // phase 2: Point lights
    for(int i = 0; i < pointLightCount; i++) {
        result += CalcPointLight(pointLight[i], norm, fragPos, viewDir);
    }
    // phase 3: Spot lights
    for(int i = 0; i < spotLightCount; i++) {
        result += CalcSpotLight(spotLight[i], norm, fragPos, viewDir);
    }
    // phase 4: FlashLight
    if(flashLight.isActive == 1) {
        result += CalcFlashLight(flashLight, norm, fragPos, viewDir);
    }
     
    //result = vec3(0.8, 0, 0);
    out_Color = vec4(result, 1.0);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    
    vec3 ambient, diffuse, specular;

    if(hasTexture == 1) {
        ambient = ambientStrength * vec3(texture(textureUnitID, textureCoords)) * light.color; // TexCoords = uv
        diffuse = /*light.diffuse **/ diff * vec3(texture(textureUnitID, textureCoords)) * light.color;
        specular = /*light.specular **/ spec * vec3(texture(textureUnitID, textureCoords)) * light.color;
    }
    else {
        
        ambient = ambientStrength * fragmentColor * light.color;
        diffuse = calculateDiffuse(light.position, vec3(1.0f, 1.0f, 1.0f)) * fragmentColor * light.color;
        specular = calculateSpecular(light.position, vec3(1.0f, 1.0f, 1.0f)) * fragmentColor * light.color;
    }
    
    //vec3 diffuse  = light.diffuse  * diff * fragmentColor;
    //vec3 specular = light.specular * spec * fragmentColor;
    //vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    //vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    //vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}


// https://learnopengl.com/Lighting/Multiple-lights
// https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/6.multiple_lights/multiple_lights.cpp
// https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/6.multiple_lights/6.multiple_lights.fs
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    //diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.direction);
    float diff = max(dot(norm, lightDir), 0.0);  // dot product?

    vec3 ambient, diffuse;

    if(hasTexture == 1) {
        ambient = ambientStrength * vec3(texture(textureUnitID, textureCoords)) * light.color;          // TexCoords == uv
        diffuse = /*light.diffuse * */diff * vec3(texture(textureUnitID, textureCoords)) * light.color;    // light.diffuse * diff *
    }
    else {
        ambient = ambientStrength * fragmentColor * light.color;
        diffuse = diff *fragmentColor * light.color;
    }
    return (ambient + diffuse);  // * fragmentColor
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    //light.position = viewPos;

    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results

    vec3 ambient, diffuse, specular;

    if(hasTexture == 1) {
        ambient = ambientStrength * vec3(texture(textureUnitID, textureCoords)); // TexCoords = uv
        diffuse =  /*light.color */light.diffuse * diff * vec3(texture(textureUnitID, textureCoords));
        specular = /*light.specular * */ spec * vec3(texture(textureUnitID, textureCoords));
    }
    else {
        ambient  = ambientStrength * fragmentColor; //vec3(1.0f, 1.0f, 1.0f); // light.color // * fragmentColor;
        diffuse  = light.diffuse  * diff * fragmentColor; // * fragmentColor;
        specular = light.specular * spec * fragmentColor; // * fragmentColor;
    }

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular); // * fragmentColor;
}

vec3 CalcFlashLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    //light.position = viewPos;

    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results

    vec3 ambient, diffuse, specular;

    if(hasTexture == 1) {
        ambient = ambientStrength * vec3(texture(textureUnitID, textureCoords)) * light.color; // TexCoords == uv
        diffuse =  diff * vec3(texture(textureUnitID, textureCoords)) * light.color;
        specular = spec * vec3(texture(textureUnitID, textureCoords)) * light.color;
    }
    else {
        ambient  = ambientStrength * fragmentColor * light.color;
        diffuse  = diff * fragmentColor * light.color;
        specular = spec * fragmentColor * light.color;
    }

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return (ambient + diffuse + specular);
}

/*vec3 calculateAmbient(Light light)
{
    // ambient - constant
    return ambientStrength * light.color;
}*/

vec3 calculateDiffuse(vec3 position, vec3 _lightColor)
{
    //diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);  // dot product?
    return diff * _lightColor;
}

vec3 calculateSpecular(vec3 position, vec3 _lightColor)
{
    // specular
    int h = 32;
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(position - fragPos);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);   // h = 32
    return specularStrength * spec * _lightColor;
}
