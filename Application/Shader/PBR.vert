//
#version 460

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outTexCoord;
layout(location = 3) out vec4 outShadowTexCoord;

layout(binding = 0) uniform UniformBufferObject0 
{
    mat4 mvp;
	vec4 cameraPos;
} WVPCameraPos;

layout(binding = 1) uniform UniformBufferObject1
{
    mat4 mvp;

} lightMVP;


const mat4 biasMat = mat4( 
	0.5, 0.0, 0.0, 0.0,
	0.0, 0.5, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.5, 0.5, 0.0, 1.0 );

void main()
{
	gl_Position = WVPCameraPos.mvp * vec4(inPosition, 1.0);

	outPos = inPosition;
	outNormal = inNormal;
	outTexCoord = vec2(inTexCoord.x, inTexCoord.y);

	outShadowTexCoord = (biasMat * lightMVP.mvp) * vec4(inPosition, 1.0);	
}
