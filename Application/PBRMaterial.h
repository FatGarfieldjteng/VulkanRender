#pragma once
#include <glm/glm.hpp>
class Texture;

class PBRMaterial
{
public:
	PBRMaterial();
	virtual ~PBRMaterial();

public:

	// Albedo
	glm::vec4 mAlbedoFactor = { 1.0f, 1.0f, 1.0f, 1.0f };
	Texture* mTexAlbedo = nullptr;

	// Metallic-Roughness
	Texture* mTexMetalRoughness = nullptr;
	float mMetalRoughnessFactor = 1.0f;

	// Normal
	Texture* mTexNormal = nullptr;
};