//The fragment shader operates on each pixel in a given polygon
#version 330 core

struct Material {
	vec3 ambient;
	vec3 emissive;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
 
in vec4 fragPosition; // Position in world space.
in vec4 fragNormal; // Surface normal in world space.
in vec2 fragUV; // Texture coordinates
 
uniform vec3 viewerPosition; // Eye position in world space.
uniform vec3 lightPosition; // Light's position in world space.
uniform vec3 lightColor; // Light's diffuse and specular contribution.
 
uniform bool lightingEnabled;
uniform bool isALightSource;
 
uniform Material material;
 
uniform sampler2D diffuseSampler;
 
layout (location=0) out vec4 finalColor;

void main()
{
	if (!isALightSource) {

		finalColor = texture(diffuseSampler,fragUV);

		if (lightingEnabled) {
				// Emissive component
			vec3 emissive = material.emissive;
				// Ambient component
			vec3 ambient = material.ambient;
				// Diffuse component
			vec3 norm = normalize(fragNormal.xyz);
			vec3 lightDir = normalize(lightPosition - fragPosition.xyz);  
			float diff = max(dot(norm, lightDir), 0.0f);
			vec3 diffuse = lightColor * diff * material.diffuse;
				// Specular component
			vec3 viewDir = normalize(viewerPosition - fragPosition.xyz);
			vec3 reflectDir = reflect(-lightDir, norm);  
			float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
			vec3 specular = lightColor * spec * material.specular;  

			finalColor = vec4((emissive + ambient + diffuse + specular), 1.0f) * finalColor;
		}
	} else {
		if (lightingEnabled) {
			finalColor = vec4(lightColor, 1.0f);
		} else {
			finalColor = vec4(0.0f,0.0f,0.0f,0.0f);
		}
	}

}

	/*
    // Compute the emissive term.
    vec4 Emissive = MaterialEmissive;
 
    // Compute the diffuse term.
    vec4 N = normalize( fragNormal );
    vec4 L = normalize( LightPosW - fragPosition );
    float NdotL = max( dot( N, L ), 0 );
    vec4 Diffuse =  NdotL * LightColor * MaterialDiffuse;
     
    // Compute the specular term.
    vec4 V = normalize( EyePosW - fragPosition );
    vec4 H = normalize( L + V );
    vec4 R = reflect( -L, N );
    float RdotV = max( dot( R, V ), 0 );
    float NdotH = max( dot( N, H ), 0 );
    vec4 Specular = pow( RdotV, MaterialShininess ) * LightColor * MaterialSpecular;
     
    finalColor = ( Emissive + Ambient + Diffuse + Specular ) * texture( diffuseSampler, fragUV );*/