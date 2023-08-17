#pragma once
#include <glm/glm.hpp>
class Texture;

struct MaterialValue {
	glm::vec4 mAlbedo = {1.0, 1.0, 1.0, 1.0};

	// mMetalRoughness_MapORValue.r is metalRoughnessFactor
	// mMetalRoughness_MapORValue.g > 0, albedo value from map; mMetalRoughness_MapORValue.g < 0, albedo value from albedoFactor
	// mMetalRoughness_MapORValue.b > 0, metalRoughness value from map; mMetalRoughness_MapORValue.g < 0, metalRoughness value from metalRoughnessFactor
	// mMetalRoughness_MapORValue.a > 0, normal map exist; mMetalRoughness_MapORValue.a < 0, no normal map
	glm::vec4 mMetalRoughness_MapORValue = {1.0f, 1.0f, 1.0f, 1.0f};
};

struct MaterialTexture {
	// Albedo
	Texture* mAlbedo = nullptr;

	// Metallic-Roughness
	Texture* mMetalRoughness = nullptr;

	// Normal
	Texture* mNormal = nullptr;
};

class PBRMaterial
{
public:
	PBRMaterial();
	virtual ~PBRMaterial();

public:

	MaterialValue mValues;

	MaterialTexture mTextures;
};