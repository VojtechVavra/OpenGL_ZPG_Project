#version 330 core

// Interpolated values from the vertex shaders
// Data coming from the vertex shader
in vec2 texCoordUV;
in vec3 fragPos;			// added
in vec3 normal;			    // added

// Ouput data
out vec4 gl_FragColor;      // output color

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;
uniform sampler2D texture_diffuse2;

#define MAX_LIGHTS 6

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
uniform vec3 fragmentColor = vec3(1.0f, 1.0f, 1.0f);

// constant values modifiers
uniform float ambientStrength = 0.2f;   // constatnt color - good 0.2f - 0.1f
uniform float specularStrength = 0.5f;  // 0.5


float getAttenuation(vec3 lightPosition, vec3 _fragPos);
vec4 CalcFlashLight(SpotLight light, vec3 normal, vec3 fragPos);

vec3 calculateDiffuse(vec3 position, vec3 _lightColor);

void main() {
	vec3 norm = normalize(normal);
	vec4 result = vec4(0.0f, 0.0f, 0.0f, 0.0f);

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
    if(flashLight.isActive == 1) {
        result += CalcFlashLight(flashLight, norm, fragPos);
    }
    else {
        vec4 val = texture(myTextureSampler, texCoordUV);
        if (val.a < 0.1) {
            discard;
        }
        //result += val * vec4(0.3f, 0.3f, 0.3f, 0.0f);
        result += val * 0.7f; //ambientStrength;
    }

    //vec4 val = texture2D(myTextureSampler, texCoordUV);
    /*vec4 val = texture(result, texCoordUV);    // UV = texCoord; texture2D is deprecated, use texture
    if (val.a < 0.1) {
        discard;
    }
    gl_FragColor = val;*/
    gl_FragColor = result;

	// Output color = color of the texture at the specified UV
	//color = texture( myTextureSampler, UV ); // texture().rgba
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
}*/


float getAttenuation(vec3 _lightPosition, vec3 _fragPos)
{
    // light attenuation coefficient
    float light_constant = 1.0f;
    float light_linear = 0.09f;
    float light_quadratic = 0.032f;

    // attenuation
    float distance = length(_lightPosition - fragPos);
    float attenuation = 1.0 / (light_constant + light_linear * distance + light_quadratic * (distance * distance));

    return attenuation;
}

vec4 CalcFlashLight(SpotLight light, vec3 normal, vec3 fragPos)
{
    //light.position = viewPos;

    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    //float diff = max(dot(normal, lightDir), 0.0);
    
    float attenuation = getAttenuation(light.position, fragPos);
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.3, 1.0);
    // combine results

    vec4 ambient, diffuse;
    
    vec4 val = texture(myTextureSampler, texCoordUV);
    //ambient = ambientStrength * texture(myTextureSampler, texCoordUV) * vec4(fragmentColor * light.color, 1.0); // TexCoords == uv
    if (val.a < 0.1) {
        discard;
    }
    ambient = ambientStrength * val * vec4(light.color, 1.0);
    if(diff == 0.0)
    {
        ambient = val;
    }
    //diffuse = diff * texture(myTextureSampler, texCoordUV) * vec4(fragmentColor * light.color, 1.0);

    diffuse = diff * val * vec4(light.color, 1.0);
    
    //ambient  = ambientStrength * fragmentColor * light.color;
    //diffuse  = diff * fragmentColor * light.color;

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
