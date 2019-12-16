#version 420
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

uniform mat4 MVP;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoord;
out vec3 normal;
out vec3 fragPos;
out vec3 viewPos;

void main()
{
	gl_Position = MVP * vec4(aPos, 1.0);
	TexCoord = aTexCoord;
	normal = aNormal;
	fragPos = vec3(model * vec4(aPos, 1.0));
	viewPos = vec3(view * vec4(aPos, 1.0));


}