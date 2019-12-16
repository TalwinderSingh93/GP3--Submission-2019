#version 330

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec2 VertexTexCoord;
layout (location = 3) in vec3 VertexNormal;



// positions
vec3 pos1 = vec3(-1.0,  1.0, 0.0);
vec3 pos2 = vec3(-1.0, -1.0, 0.0);
vec3 pos3 = vec3( 1.0, -1.0, 0.0);
vec3 pos4 = vec3( 1.0,  1.0, 0.0);
// texture coordinates
vec2 uv1 = vec2(0.0, 1.0);
vec2 uv2 = vec2(0.0, 0.0);
vec2 uv3 = vec2(1.0, 0.0);
vec2 uv4 = vec2(1.0, 1.0);

vec3 tangent1;
vec3 bitangent1;

out vec3 Normal;
out vec2 TexCoord;
out vec3 Position;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec3 edge1 = pos2 - pos1;
	vec3 edge2 = pos3 - pos1;
	vec2 deltaUV1 = uv2 - uv1;
	vec2 deltaUV2 = uv3 - uv1; 

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent1 = normalize(tangent1);

	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent1 = normalize(bitangent1);

	TexCoord = VertexTexCoord;
	Normal = mat3(transpose(inverse(model))) * VertexNormal; 

	Position = vec3(model * vec4(VertexPosition, 1.0));
	gl_Position =projection * view * vec4(Position, 1.0);

	vec3 T = normalize(vec3(model * vec4(tangent1,   0.0)));
	vec3 B = normalize(vec3(model * vec4(bitangent1, 0.0)));
	vec3 N = normalize(vec3(model * vec4(VertexNormal,    0.0)));
	TBN = transpose(mat3(T, B, N));

	Position = TBN * Position;
 
}



