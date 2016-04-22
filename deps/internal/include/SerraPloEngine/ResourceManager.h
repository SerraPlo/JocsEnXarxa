#pragma once
#include "TextureCache.h"

namespace SerraPlo {
	class ResourceManager {
		static TextureCache m_textureCache;
	public:
		static GLTexture GetTexture(std::string texturePath) {
			return m_textureCache.getTexture(LoadAsset(texturePath));
		}
		static std::string LoadAsset(const std::string &subDir) {
			static std::string basePath = SDL_GetBasePath();
			if (basePath == "") SP_THROW_ERROR("[SDLInterface] Cannot get resource path for subdirectory: " + subDir);
			size_t pos = basePath.rfind("build");
			return basePath.substr(0, pos) + "assets/" + subDir;
		}
	};
	TextureCache ResourceManager::m_textureCache;
}