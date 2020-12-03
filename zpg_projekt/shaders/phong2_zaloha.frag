#version 400

#define MAX_LIGHTS 64

out vec4 out_Color; // original name: frag_colour

in vec3 ex_worldPosition; // fragPos
in vec3 ex_worldNormal;   // normal

uniform vec3 fragmentColor;
//uniform vec3 lightPosition;                              // lights.position
//uniform vec3 lightColor; // = vec3(1.0, 0.0, 0.5);      // lights.color
uniform vec3 viewPos = vec3(0.0, 0.0, 0.0);
uniform float ambientStrength = 0.1f;   // constatnt color - good 0.2f - 0.1f
uniform float specularStrength = 0.5f;  // 0.5
//uniform vec3 lightPosition;   //the shader has light position
//uniform vec3 viewPos;
//out vec4 frag_colour;




/*struct Light2
{
    vec3 position;  //  ***
    vec3 direction;
    float cutoff;
    int lightType;
    vec3 color;     // ***
};*/

struct Light
{
    vec3 position;
    vec3 color;
    vec3 direction;
    int lightType;
};

uniform Light lights[MAX_LIGHTS];
uniform int lightCount;


//void calculatePointLight(vec3 specular, vec3 diffuse);



vec3 calculateAmbient(Light light);
vec3 calculateDiffuse(Light light);
vec3 calculateSpecular(Light light);
vec3 calculatePhong(vec3 ambient, vec3 diffuse, vec3 specular);

vec3 spotLight(Light light, vec3 l_spotDir, vec3 lightDirection, float cuttOff, vec3 lightNormal);

//vec3 pointLight(Light light);
vec3 pointLight(vec3 ambient, vec3 diffuse, vec3 specular);
vec3 spotLight(Light light);
vec3 directionalLight(inout vec3 ambient, inout vec3 diffuse, inout vec3 specular, Light light);


// http://ogldev.atspace.co.uk/
// https://learnopengl.com/Lighting/Multiple-lights
void main () {
    vec3 lightDir;

    vec3 ambient = vec3(0.0, 0.0, 0.0);
    vec3 specular = vec3(0.0, 0.0, 0.0);
    vec3 diffuse = vec3(0.0, 0.0, 0.0);

    vec3 result = vec3(0.0, 0.0, 0.0);
    vec3 spotDir = vec3(-1, 0, 0);
    //vec3 lightDir = vec3(-1, 0, 0);
    float cuttOff = 180.0f;
    vec3 lightNormal = vec3(0, 0, 1);

    for(int i = 0; i < lightCount; i++) {
        ambient = calculateAmbient(lights[i]);  // +=
        diffuse = calculateDiffuse(lights[i]);
        specular = calculateSpecular(lights[i]);

        //lightDir = normalize(-lights[0].direction);

        //result += pointLight(ambient, diffuse, specular);   // pointlight
        result = directionalLight(ambient, diffuse, specular, lights[i]);

        //result += spotLight(lights[i], spotDir, lightDir, cuttOff, lightNormal);
    }

    //vec3 result = calculatePhong(ambient, diffuse, specular);

    


    out_Color = vec4(result, 1.0f);

    //out_Color = vec4(result, 1.0f);
    
/*
//ambient
    //float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
//diffuse
    vec3 norm = normalize(ex_worldNormal);
    vec3 lightDir = normalize(lightPosition - ex_worldPosition);
    float diff = max(dot(norm, lightDir), 0.0);  // dot product?
    vec3 diffuse = diff * lightColor;
// specular
    //float specularStrength = 0.5f;  // 0.5
    vec3 viewDir = normalize(viewPos - ex_worldPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);   // h = 32

    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * fragmentColor; // objectColor;
    out_Color = vec4(result, 1.0f);
*/
//result
    //vec3 result = (ambient + diffuse) * objectColor;
    //vec3 result = (ambient + diffuse) * fragmentColor;
    //out_Color = vec4(result, 1.0);
};

vec3 calculateAmbient(Light light)
{
    // ambient - constant
    return ambientStrength * light.color;
}

vec3 calculateDiffuse(Light light)
{
    //diffuse
    vec3 norm = normalize(ex_worldNormal);
    vec3 lightDir = normalize(light.position - ex_worldPosition);
    float diff = max(dot(norm, lightDir), 0.0);  // dot product?
    return diff * light.color;
}

vec3 calculateSpecular(Light light)
{
    // specular
    int h = 32;
    vec3 norm = normalize(ex_worldNormal);
    vec3 lightDir = normalize(light.position - ex_worldPosition);

    vec3 viewDir = normalize(viewPos - ex_worldPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);   // h = 32
    return specularStrength * spec * light.color;
}

vec3 calculatePhong(vec3 ambient, vec3 diffuse, vec3 specular)
{
    return (ambient + diffuse + specular) * fragmentColor; // objectColor = fragmentColor;
}

vec3 pointLight(vec3 ambient, vec3 diffuse, vec3 specular)
{
    return (ambient + diffuse + specular) * fragmentColor;
}

vec3 spotLight(Light light, vec3 l_spotDir, vec3 lightDirection, float cuttOff, vec3 lightNormal)
{
   float intensity = 0.0;
    vec3 spec = vec3(0.0);
 
    vec3 ld = normalize(lightDirection);    // light.direction
    vec3 sd = normalize(vec3(-l_spotDir));  
 
    // inside the cone (kuzel)?
    if (dot(sd,ld) > cuttOff) {    // light.cutoff
 
        vec3 n = normalize(lightNormal);   // light.normal
        intensity = max(dot(n,ld), 0.0);
 
        if (intensity > 0.0) {
            vec3 eye = normalize(light.position); // DataIn.eye
            vec3 h = normalize(ld + eye);
            float intSpec = max(dot(h,n), 0.0);
            //spec = specular * pow(intSpec, shininess);
            spec = vec3(calculateSpecular(light));
        }
    }
 
    return max(intensity * calculateDiffuse(light) + spec, calculateAmbient(light));
}

vec3 directionalLight(inout vec3 ambient, inout vec3 diffuse, inout vec3 specular, Light light)
{
    //ambient += calculateAmbient(light);
    //diffuse += calculateDiffuse(light);
    //specular += calculateSpecular(light);

    ///
    // ambient
    ambient = calculateAmbient(light) * fragmentColor;
  	
    // diffuse 
    vec3 norm = normalize(ex_worldNormal);
    // vec3 lightDir = normalize(light.position - FragPos);
    vec3 lightDir = normalize(-light.direction);  
    float diff = max(dot(norm, lightDir), 0.0);
    diffuse = calculateDiffuse(light) * diff * fragmentColor;
    
    vec3 fragPos = ex_worldPosition;
    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    specular = calculateSpecular(light) * fragmentColor;
        
    return ambient + diffuse + specular;
    //
    //return (ambient + diffuse) * fragmentColor; // + specular;
}


/*void calculatePointLight(vec3 specular, vec3 diffuse)
{
//ambient
    //float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
//diffuse
    vec3 norm = normalize(ex_worldNormal);
    vec3 lightDir = normalize(lightPosition - ex_worldPosition);
    float diff = max(dot(norm, lightDir), 0.0);  // dot product?
    diffuse = diff * lightColor;
// specular
    //float specularStrength = 0.5f;  // 0.5
    vec3 viewDir = normalize(viewPos - ex_worldPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);   // h = 32

    specular = specularStrength * spec * lightColor;

    
}*/



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