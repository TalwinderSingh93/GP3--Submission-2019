#version 330 core

out vec4 colorFrag;


uniform vec3 lightPos;
uniform vec3 PosView;

in vec3 Normal;
in vec2 TexCoord;
in vec3 Position;
in mat3 TBN;

uniform sampler2D diffuse;


vec3 colorLight =vec3(1.f, 1.f, 1.f);

void main()
{
	//ambient light
	float strengthAmbient = 0.1f;
    vec3 lightingAmbient = strengthAmbient * colorLight;
	
	//diffuse light
	vec3 normalB = normalize(Normal);
	normalB = texture(diffuse, TexCoord).rgb;
	normalB = normalize(normalB * 2.0 - 1.0);
	normalB = normalize(TBN * normalB); 
	vec3 targetDirection = normalize(lightPos - Position);
	float diff = max(dot(normalB, targetDirection), 0.0);
	vec3 lightingDiffuse = diff * colorLight;
	
	//Specular light
	float specularStr = 1;
	vec3 viewDir = TBN * normalize(PosView - Position);
	vec3 halfwayDir = TBN * normalize(targetDirection + viewDir);
	float spec = pow(max(dot(normalB, halfwayDir), 0.0), 32);
	vec3 lightingSpecular = specularStr * spec * colorLight;  

	//blinn Phong light
    vec3 result = (lightingAmbient + lightingDiffuse + lightingSpecular);
	float gamma = 2.2;
    colorFrag =  texture2D(diffuse, TexCoord) * vec4(result, 1.0);
	colorFrag.rgb = pow(colorFrag.rgb, vec3(1.0/gamma));
}