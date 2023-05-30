//
#version 460

layout(location = 0) in vec3 worldPos;
layout(location = 1) in vec2 tc;
layout(location = 2) in vec3 normal;

layout(location = 0) out vec4 out_FragColor;

layout(binding = 0) uniform UniformBuffer
{
	mat4 mvp;
	vec4 cameraPos;
} ubo;


layout(binding = 3) uniform sampler2D texAlbedo;
layout(binding = 4) uniform sampler2D texMetalRoughness;
layout(binding = 5) uniform sampler2D texNormal;

layout(binding = 6) uniform samplerCube texEnvMap;
layout(binding = 7) uniform samplerCube texEnvMapIrradiance;

layout(binding = 8) uniform sampler2D texBRDF_LUT;

#include "PBR.sp""

void main()
{
	vec4 Kd  = texture(texAlbedo, tc);
	vec2 MeR = texture(texMetalRoughness, tc).yz;

	vec3 normalSample = texture(texNormal, tc).xyz;

	// world-space normal
	vec3 n = normalize(normal);

	// normal mapping
	n = perturbNormal(n, normalize(ubo.cameraPos.xyz - worldPos), normalSample, tc);

	vec4 mrSample = texture(texMetalRoughness, tc);

	PBRInfo pbrInputs;
	Ke.rgb = SRGBtoLINEAR(Ke).rgb;
	// image-based lighting
	vec3 color = calculatePBRInputsMetallicRoughness(Kd, n, ubo.cameraPos.xyz, worldPos, mrSample, pbrInputs);
	// one hardcoded light source
	color += calculatePBRLightContribution( pbrInputs, normalize(vec3(-1.0, -1.0, -1.0)), vec3(1.0) );
	
	out_FragColor = vec4(color, 1.0);
}
