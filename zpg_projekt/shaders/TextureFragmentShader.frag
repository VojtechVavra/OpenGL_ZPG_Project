#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
//in vec3 fragPos;			// added
//in vec3 normal;				// added

// Ouput data
out vec3 color;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

/*uniform int hasTexture = 0;

// added
uniform vec3 viewPos;

uniform vec3 fragmentColor;      // objectColor
uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform float ambientStrength = 0.0;


// added
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

vec3 calculateDiffuse(vec3 position, vec3 _lightColor);

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos);
vec3 CalcDirLight(DirLight light, vec3 normal);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos);
vec3 CalcFlashLight(SpotLight light, vec3 normal, vec3 fragPos);
// end
*/

void main() {
	//vec3 norm = normalize(normal);
	//vec3 result = vec3(0.0f, 0.0f, 0.0f);

    // phase 1: Directional lighting
    //vec3 result = CalcDirLight(dirLight, norm);

	// phase 2: Point lights
    //for(int i = 0; i < pointLightCount; i++) {
        //result += CalcPointLight(pointLight[i], norm, fragPos);
    //}
    // phase 3: Spot lights
    //for(int i = 0; i < spotLightCount; i++) {
        //result += CalcSpotLight(spotLight[i], norm, fragPos);
    //}
    // phase 4: FlashLight
    //if(flashLight.isActive == 1) {
        //result += CalcFlashLight(flashLight, norm, fragPos);
    //}

	// Output color = color of the texture at the specified UV
	color = texture( myTextureSampler, UV ).rgb;
    //color = result;
}



/*
// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    
    vec3 ambient, diffuse;

    if(hasTexture == 1) {
        ambient = ambientStrength * vec3(texture(myTextureSampler, UV)) * light.color; // TexCoords = uv
        diffuse = diff * vec3(texture(myTextureSampler, UV)) * light.color;

    }
    else {
        
        ambient = ambientStrength * fragmentColor * light.color;
        diffuse = calculateDiffuse(light.position, vec3(1.0f, 1.0f, 1.0f)) * fragmentColor * light.color;
    }

    ambient *= attenuation;
    diffuse *= attenuation;

    return (ambient + diffuse);
}


// https://learnopengl.com/Lighting/Multiple-lights
// https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/6.multiple_lights/multiple_lights.cpp
// https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/6.multiple_lights/6.multiple_lights.fs
vec3 CalcDirLight(DirLight light, vec3 normal)
{
    //diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.direction);
    float diff = max(dot(norm, lightDir), 0.0);  // dot product?

    vec3 ambient, diffuse;

    if(hasTexture == 1) {
        ambient = ambientStrength * vec3(texture(myTextureSampler, UV)) * light.color;          // TexCoords == uv
        diffuse = diff * vec3(texture(myTextureSampler, UV)) * light.color;    // light.diffuse * diff *
    }
    else {
        ambient = ambientStrength * fragmentColor * light.color;
        diffuse = diff *fragmentColor * light.color;
    }
    return (ambient + diffuse);  // * fragmentColor
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos)
{
    //light.position = viewPos;

    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results

    vec3 ambient, diffuse;

    if(hasTexture == 1) {
        ambient = ambientStrength * vec3(texture(myTextureSampler, UV)); // TexCoords = uv
        diffuse =  light.diffuse * diff * vec3(texture(myTextureSampler, UV));
    }
    else {
        ambient  = ambientStrength * fragmentColor; //vec3(1.0f, 1.0f, 1.0f); // light.color // * fragmentColor;
        diffuse  = light.diffuse  * diff * fragmentColor; // * fragmentColor;
    }

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;

    return (ambient + diffuse); // * fragmentColor;
}

vec3 CalcFlashLight(SpotLight light, vec3 normal, vec3 fragPos)
{
    //light.position = viewPos;

    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results

    vec3 ambient, diffuse;

    if(hasTexture == 1) {
        ambient = ambientStrength * vec3(texture(myTextureSampler, UV)) * light.color; // TexCoords == uv
        diffuse =  diff * vec3(texture(myTextureSampler, UV)) * light.color;
    }
    else {
        ambient  = ambientStrength * fragmentColor * light.color;
        diffuse  = diff * fragmentColor * light.color;
    }

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;

    return (ambient + diffuse);
}

vec3 calculateDiffuse(vec3 position, vec3 _lightColor)
{
    //diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);  // dot product
    return diff * _lightColor;
}
*/