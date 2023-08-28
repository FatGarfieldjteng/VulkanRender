//
#version 460

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(binding = 0) uniform UniformBufferObject 
{
    mat4 mvp;

} lightMVP;


void main()
{
	gl_Position = lightMVP.mvp * vec4(inPosition, 1.0);
}
