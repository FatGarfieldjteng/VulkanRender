//
#version 460

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outTexCoord;

layout(binding = 0) uniform UniformBufferObject 
{
    mat4 mvp;
	vec4 cameraPos;
} WVPCameraPos;


void main()
{
	gl_Position = WVPCameraPos.mvp * vec4(inPosition, 1.0);

	outPos = inPosition;
	outNormal = inNormal;
	outTexCoord = inTexCoord;
}
