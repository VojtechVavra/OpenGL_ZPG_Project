#version 330 core

// Interpolated values from the vertex shaders
// Data coming from the vertex shader
in vec2 texCoordUV;
in vec3 fragPos;			
in vec3 normal;			    
in vec4 textureCoordProj;   // added
//in mat4 textureCoordProj;   // added
in vec4 textureCoordProj2;   // show detail texture - added


// Ouput data
out vec4 gl_FragColor;      // output color

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;     // Regular texture image 
uniform int hasTexture;
//uniform sampler2D texture_diffuse2;
uniform sampler2D texProj;              // Projected texture image
uniform sampler2D texProj2;             // show detail texture
uniform bool showTextureDetail;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Material meshMaterial;

struct DirLight {
    int lightType;

    vec3 direction;
    vec3 color;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;

#define MAX_LIGHTS 6

uniform int pointLightCount;
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
} pointLight [MAX_LIGHTS];

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
} spotLight [MAX_LIGHTS];


uniform SpotLight flashLight;
//uniform vec3 fragmentColor = vec3(1.0f, 1.0f, 1.0f);

// constant values modifiers
vec4 scene_ambient = vec4(0.2, 0.2, 0.2, 1.0);
uniform float ambientStrength = 0.2f;   // constatnt color - good 0.2f - 0.1f
uniform float specularStrength = 0.5f;  // 0.5

vec4 CalcDirLight(DirLight light, vec3 normal);
float getAttenuation(vec3 lightPosition, vec3 _fragPos);
float getAttenuation2(vec3 _lightPosition, vec3 _fragPos);
vec4 CalcFlashLight(SpotLight light, vec3 normal, vec3 fragPos);
vec4 CalcFlashLight2(SpotLight light, vec3 normal, vec3 fragPos);

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos);
vec3 calculateDiffuse(vec3 position, vec3 _lightColor);
vec3 calculateSpecular(vec3 position, vec3 _lightColor, vec3 viewPos);
bool calculateTextureShadow();

// https://stackoverflow.com/questions/22952742/creating-a-rectangular-light-source-in-opengl
float time = 30.0;
vec2 uSpotSize = vec2(0.3, 0.6);
//vec3 lp = vec3(0.0, 0.0, 7.0 + cos(time) * 5.0); // Light world-space position
//vec3 lp = vec3(3.0, 0.0, 10.0); // (doprava --, doleva ++ podel zdi; y - vyska zasazenych objektu; z - dopredu/ dozadu)
vec3 lp = vec3(3.0, 0.5, 1.3);
vec3 lz = vec3(-1.0, -1.0, 0.0); // Light direction (Z vector)    // vec3(0.0, 0.0, -1.0)
// Light radius (for attenuation calculation)
float lr = 1;     // 3.0

void main() {
	vec3 norm = normalize(normal);
	vec4 result = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    // phase 1: Directional lighting
    //result = CalcDirLight(dirLight, norm) * 0.5f;

	// phase 2: Point lights
    for(int i = 0; i < pointLightCount; i++) {
        //result += vec4(CalcPointLight(pointLight[i], norm, fragPos), 1.0);
        //result += CalcPointLight(pointLight[i], norm, fragPos);
    }
    // phase 3: Spot lights
    //for(int i = 0; i < spotLightCount; i++) {
        //result += CalcSpotLight(spotLight[i], norm, fragPos);
    //}
    // phase 4: FlashLight
    if(flashLight.isActive == 1) {
        result += CalcFlashLight2(flashLight, norm, fragPos);
        //result *= vec4(0.5, 0.5, 0.5, 1.0);
        vec4 val = texture(myTextureSampler, texCoordUV);
            if (val.a < 0.1) {
                discard;
            }
            result += val * vec4(meshMaterial.ambient + meshMaterial.diffuse, 1.0);
    }
    else {
            vec4 val = texture(myTextureSampler, texCoordUV);
            if (val.a < 0.1) {
                discard;
            }
            result += val * vec4(meshMaterial.ambient + meshMaterial.diffuse, 1.0);
    }

    
    

    gl_FragColor = texture(myTextureSampler, texCoordUV); //result;

	// Output color = color of the texture at the specified UV
	//color = texture( myTextureSampler, UV ); // texture().rgba
}




// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // attenuation
    float distance = length(light.position - fragPos);
    //float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032f * (distance * distance));  
    // combine results
    
    vec3 ambient, diffuse;
    ambient = vec3(scene_ambient) * meshMaterial.ambient * light.color;
    diffuse = diff * attenuation * meshMaterial.diffuse * light.color;

    if(hasTexture == 1) {
        vec4 val = texture(myTextureSampler, texCoordUV);
        if (val.a < 0.1) {
            discard;    // odstranime podle alpha kanalu texturu
        }
        return ambient * diffuse * vec3(val.xyz);   // (ambient + diffuse) * vec3(val.xyz);

        //ambient = ambientStrength * vec3(texture(myTextureSampler, texCoordUV)) * light.color; // TexCoords = uv
        //diffuse = diff * vec3(texture(myTextureSampler, texCoordUV)) * light.color;
    }
    else {
        return (ambient * diffuse) * 2;
        //return vec3(scene_ambient) * meshMaterial.ambient * meshMaterial.diffuse * diffuse;
        //return diff * attenuation * light.color * meshMaterial.ambient * meshMaterial.diffuse;
        //return ambient * diffuse * meshMaterial.diffuse;
        //ambient = ambientStrength * meshMaterial.ambient * light.color;
        //diffuse = calculateDiffuse(light.position, vec3(1.0f, 1.0f, 1.0f)) * meshMaterial.diffuse * light.color;
    }

    //ambient *= attenuation;
    //diffuse *= attenuation;

    //return (ambient + diffuse) * 0.5;
}


// https://learnopengl.com/Lighting/Multiple-lights
// https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/6.multiple_lights/multiple_lights.cpp
// https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/6.multiple_lights/6.multiple_lights.fs
vec4 CalcDirLight(DirLight light, vec3 normal)
{
    //diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.direction);
    float diff = max(dot(norm, lightDir), 0.0);  // dot product?

    vec3 ambient, diffuse, specular;
    vec4 val = texture(myTextureSampler, texCoordUV);

    if (hasTexture == 1 && val.a < 0.1) {
        discard;            // odstranime podle alpha kanalu texturu
    }

    ambient = vec3(scene_ambient) * meshMaterial.ambient * light.color;
    diffuse = diff * meshMaterial.diffuse * light.color;
    specular = calculateSpecular(dirLight.direction, light.color, vec3(0.0, 0.0, 0.0)) * meshMaterial.specular * light.color;
    // diffuse = diff * vec3(texture(myTextureSampler, UV)) * light.color;

    //return vec4(meshMaterial.diffuse, 1.0);
    //if(vec3(val) == vec3(0.0, 0.0, 0.0))
    if(hasTexture == 0)
    {
        return vec4(ambient * diffuse, 1.0);
    }

    return vec4(ambient * diffuse, 1.0) * val;
}



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

float getAttenuation2(vec3 _lightPosition, vec3 _fragPos)
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
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    if(diff == 0.0){
        diff = max(dot(-normal, lightDir), 0.0);
    }

    float attenuation = getAttenuation(light.position, fragPos);
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    

    vec4 textureColorProj = textureProj(texProj, textureCoordProj);


    // combine results
    vec4 ambient, diffuse, specular;
    
    vec4 val = texture(myTextureSampler, texCoordUV);
    //ambient = ambientStrength * texture(myTextureSampler, texCoordUV) * vec4(fragmentColor * light.color, 1.0); // TexCoords == uv
    if (val.a < 0.1) {
        discard;    // odstranime podle alpha kanalu texturu
    }

    ambient = scene_ambient * vec4(meshMaterial.ambient, 1.0) * vec4(light.color, 1.0);
    diffuse = diff * attenuation * intensity * vec4(meshMaterial.diffuse, 1.0) * vec4(light.color, 1.0);
    specular = vec4(calculateSpecular(light.position, light.color, light.position), 1.0) * attenuation * intensity * vec4(meshMaterial.specular, 1.0) * vec4(light.color, 1.0);

    if(intensity <= 0.0) {
        return (ambient + diffuse + specular) * val;
    }
    return mix((ambient + diffuse + specular) * val, textureColorProj, 0.3);
}


vec4 CalcFlashLight2(SpotLight light, vec3 normal, vec3 fragPos)
{
    vec3 spotLightWindowPos = vec3(5.950044, 0.895011, 1.963056);   // 5.750044, 0.695011, 0.763056
    vec3 spotLightPos = vec3(4.944598, 1.174669, 1.755617);     // vec3 spotLightPos = vec3(5.944598, 0.874669, 1.755617);
    vec3 lightDir = normalize(spotLightWindowPos - fragPos);    // vec3(5.821190, 2.376043, 1.707771)
    //vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    if(diff == 0.0){
        diff = max(dot(-normal, lightDir), 0.0);
    }

    float attenuation = getAttenuation(spotLightPos, fragPos);
    float attenuation2 = getAttenuation2(spotLightWindowPos, fragPos);
    //float attenuation = getAttenuation(light.position, fragPos);

    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    //epsilon = 0.4;
    theta = dot(lightDir, normalize(vec3(1,0.5,0))); 
    //float intensity = clamp((0.7 - 0.5) / epsilon, 0.0, 1.0);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec4 textureColorProj = textureProj(texProj, textureCoordProj);
    //intensity = textureColorProj;


    vec3 L = lp - fragPos;
    // Project L on the YZ / XZ plane
    vec3 LX = normalize(vec3(L.x, 0.0, L.z));
    vec3 LY = normalize(vec3(0.0, L.y, L.z));

    // Calculate the angle on X and Y axis using projected vectors just above
    float ax = dot(LX, -lz);
    float ay = dot(LY, -lz);
    
        
    // combine results
    vec4 ambient, diffuse, specular;
    
    //specular = vec4(calculateSpecular(light.position, light.color), 1.0);

    vec4 val = texture(myTextureSampler, texCoordUV);
    //ambient = ambientStrength * texture(myTextureSampler, texCoordUV) * vec4(fragmentColor * light.color, 1.0); // TexCoords == uv
    if (val.a < 0.1) {
        discard;    // odstranime podle alpha kanalu texturu
    }

    // vec3 ambientLighting = vec3(scene_ambient) * vec3(frontMaterial.ambient);
    ambient = scene_ambient * vec4(meshMaterial.ambient, 1.0) * vec4(light.color, 1.0);
    diffuse = diff * attenuation * intensity * vec4(meshMaterial.diffuse, 1.0) * vec4(light.color, 1.0);
    specular = vec4(calculateSpecular(light.position, light.color, light.position), 1.0) * attenuation * intensity * vec4(meshMaterial.specular, 1.0) * vec4(light.color, 1.0);

      // cerny stin

        //return vec4(0.6, 0.3, 0.0, 1.0);
        //return (ambient + diffuse + specular + vec4(0.6, 0.3, 0.0, 1.0) * intensity * attenuation2) * val;

        //return (ambient + diffuse + vec4(0.6, 0.3, 0.0, 1.0) * attenuation2 * 2 + specular) * val;
    


    return (ambient + diffuse + specular) ;
    //return mix((ambient + diffuse + specular) * val, textureColorProj, 0.3);
}

vec3 calculateDiffuse(vec3 position, vec3 _lightColor)
{
    //diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);  // dot product = cos
    return diff * _lightColor;
}

vec3 calculateSpecular(vec3 position, vec3 _lightColor, vec3 viewPos)
{
    // specular
    float h = 32.0f; // default is 32
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(position - fragPos);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), h);
    //return specularStrength * spec * _lightColor;
    //return spec;
    return specularStrength * spec * _lightColor;
}

bool calculateTextureShadow()
{
    //float intensity = clamp((theta - 0.5f) / 0.1, 0.0, 1.0); // new added ctvercove stinitko, v budoucnu predelat na texturove stinitko
    //intensity = clamp((theta - light.outerCutOff), 0.0, 1.0);
    //vec4 position_lightClipSpace = vec4(0.5, 0.5, 0.5, 1.0);
    // clipping against the light frustum
    //bool isInsideX = ( position_lightClipSpace.x <= position_lightClipSpace.w && position_lightClipSpace.x >= -position_lightClipSpace.w );
    //bool isInsideY = ( position_lightClipSpace.y <= position_lightClipSpace.w && position_lightClipSpace.y >= -position_lightClipSpace.w );
    //bool isInsideZ = ( position_lightClipSpace.z <= position_lightClipSpace.w && position_lightClipSpace.z >= -position_lightClipSpace.w );
    //bool isInside = isInsideX && isInsideY && isInsideZ;
    //intensity = isInside ? 1.0 : 0.0;


    // For perspective projection effect
	vec4 textureColorProj = textureProj(texProj, textureCoordProj);

    return textureColorProj.w < 0.01 ? false : true;
}