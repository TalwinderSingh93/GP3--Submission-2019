#version 420

in vec3 normal;
in vec3 fragPos;
in vec2 TexCoord;

out vec4 fragColor;

uniform sampler2D ourTexture;
uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 PosView;

vec3 lightColor =vec3(1.f, 1.f, 1.f);

void main()
{
	// ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(PosView - fragPos);  
	vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 1.0);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    fragColor = texture(ourTexture, TexCoord) * vec4(result, 1.0);
}