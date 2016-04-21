#pragma once
#include <map>
#include "TextureLoader.h"

namespace SerraPlo {
	class TextureCache {
		std::map<std::string, GLTexture> m_textureMap;
	public:
		explicit TextureCache() = default;
		~TextureCache() = default;
		GLTexture getTexture(std::string texturePath) {
			//look up the texture and check if is in the map
			auto mit = m_textureMap.find(texturePath);
			//if its not inside the map
			if (mit == m_textureMap.end()) {
				//load png into new GLTexture
				GLTexture newTexture = TextureLoader::loadPNG(texturePath);
				//insert it in the map
				//std::pair<std::string, GLTexture> newPair(texturePath, newTexture);
				m_textureMap.insert(std::make_pair(texturePath, newTexture));
				//std::cout << "New cache texture loaded" << std::endl;
				return newTexture;
			}
			//std::cout << "Texture loaded" << std::endl;
			return mit->second;
		}
	};
}
