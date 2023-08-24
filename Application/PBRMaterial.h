#pragma once
#include <glm/glm.hpp>
class Texture;

class PBRMaterial
{
public:
	PBRMaterial();
	virtual ~PBRMaterial();

	static const unsigned int HAS_ALBEDO_MAP = (0x1 << 0);
	static const unsigned int HAS_ROUGHNESS_METALLIC_MAP = (0x1 << 1);
	static const unsigned int HAS_NORMAL_MAP = (0x1 << 2);

	struct MaterialValue {
		glm::vec4 mAlbedo = { 1.0, 1.0, 1.0, 1.0 };

		float mMetallic = 0.0f;
		float mRoughness = 0.0f;
		unsigned int   mMapOrValue = 0;
		float mPadding = 0.0f;
	};

	struct MaterialTexture {
		// Albedo
		Texture* mAlbedo = nullptr;

		// Metallic-Roughness
		Texture* mMetalRoughness = nullptr;

		// Normal
		Texture* mNormal = nullptr;
	};

public:

	MaterialValue mValues;

	MaterialTexture mTextures;
};