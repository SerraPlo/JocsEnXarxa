#pragma once
#include "TextureCache.h"

namespace SerraPlo {
	class ResourceManager {
		static TextureCache m_textureCache;
	public:
		static GLTexture getTexture(std::string texturePath);
	};
}