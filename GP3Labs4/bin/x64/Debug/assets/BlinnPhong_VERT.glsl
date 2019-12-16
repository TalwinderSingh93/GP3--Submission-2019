#version 420
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;


uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;


out vec2 TexCoord;
out vec3 normal;
out vec3 fragPos;


void main()
{
	TexCoord = aTexCoord;
	fragPos = vec3(model * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(model))) * aNormal;  
    
    gl_Position = projection * view * vec4(fragPos, 1.0);
}