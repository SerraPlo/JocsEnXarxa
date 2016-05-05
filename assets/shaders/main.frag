//The fragment shader operates on each pixel in a given polygon
#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
	vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular; 
};

#define MAX_POINT_LIGHTS 1
#define MAX_SPOT_LIGHTS 1
 
in vec3 fragPosition; // Position in world space.
in vec3 fragNormal; // Surface normal in world space.
in vec2 fragUV; // Texture coordinates
 
uniform vec3 viewerPosition; // Eye position in world space.
 
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
 
layout (location=0) out vec4 finalColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
		// Properties
    vec3 norm = normalize(fragNormal);
    vec3 viewDir = normalize(viewerPosition - fragPosition);

		// Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
		// Point lights
    for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
        result += CalcPointLight(pointLights[i], norm, fragPosition, viewDir);    
		// Spot lights
	for (int i = 0; i < MAX_SPOT_LIGHTS; ++i)
		result += CalcSpotLight(spotLights[i], norm, fragPosition, viewDir);

	finalColor = vec4(result, 1.0f);
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
		// Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
		// Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		// Combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fragUV));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fragUV));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fragUV));
    return (ambient + diffuse + specular);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPosition);
		// Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
		// Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		// Attenuation
    float distance = length(light.position - fragPosition);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
		// Combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fragUV));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fragUV));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fragUV));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// Calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPosition);
		// Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0f);
		// Specular shading
	vec3 reflectDir = reflect(-lightDir, normal);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		// Attenuation
    float distance = length(light.position - fragPosition);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   
		// Spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);   
		// Combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fragUV));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fragUV));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fragUV));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}





/*

	// Emissive component
	//vec3 emissive = material.emissive;

		// Ambient component
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, fragUV));

		// Diffuse component
	vec3 norm = normalize(fragNormal.xyz);
	vec3 lightDir = normalize(light.position - fragPosition.xyz);  
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fragUV));

		// Specular component
	vec3 viewDir = normalize(viewerPosition - fragPosition.xyz);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, fragUV));

		// Spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    specular *= intensity;

		// Attenuation
    float distance    = length(light.position - fragPosition.xyz);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;

	finalColor = vec4((ambient + diffuse + specular), 1.0f);

*/