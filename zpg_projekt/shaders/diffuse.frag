#version 400

out vec4 out_color;

in vec3 fragPos;    // ex_worldPosition
in vec3 normal;     // ex_worldNormal

uniform vec3 fragmentColor;      // objectColor
uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform float ambientStrength = 0.0;


// added
#define MAX_LIGHTS 12
uniform int pointLightCount;
uniform int spotLightCount;

uniform struct PointLight {
    int lightType;
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}pointLight [MAX_LIGHTS];

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
//vec3 CalcDirLight(DirLight light, vec3 normal);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos);
vec3 CalcFlashLight(SpotLight light, vec3 normal, vec3 fragPos);

// end added


void main() 
{

    vec3 norm = normalize(normal);

    vec3 result = vec3(0.0f, 0.0f, 0.0f);

    // phase 2: Point lights
    for(int i = 0; i < pointLightCount; i++) {
        result += CalcPointLight(pointLight[i], norm, fragPos);
    }
    // phase 3: Spot lights
    for(int i = 0; i < spotLightCount; i++) {
        result += CalcSpotLight(spotLight[i], norm, fragPos);
    }
    // phase 4: FlashLight
    if(flashLight.isActive == 1) {
        result += CalcFlashLight(flashLight, norm, fragPos);
    }

    out_color = vec4(result, 1.0f);
};


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    
    vec3 ambient = ambientStrength * vec3(1.0f, 1.0f, 1.0f) * fragmentColor;
    vec3 diffuse = calculateDiffuse(light.position, vec3(1.0f, 1.0f, 1.0f)) * fragmentColor;

    ambient *= attenuation;
    diffuse *= attenuation;

    return (ambient + diffuse);
}

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
    vec3 ambient  = ambientStrength * vec3(1.0f, 1.0f, 1.0f); // light.color // * fragmentColor;
    vec3 diffuse  = light.diffuse  * diff; // * fragmentColor;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;

    return (ambient + diffuse) * fragmentColor;
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
    vec3 ambient  = ambientStrength * vec3(1.0f, 1.0f, 1.0f); // light.color // * fragmentColor;
    vec3 diffuse  = light.diffuse  * diff; // * fragmentColor;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;

    return (ambient + diffuse) * fragmentColor;
}

vec3 calculateDiffuse(vec3 position, vec3 _lightColor)
{
    //diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    return diff * _lightColor;
}