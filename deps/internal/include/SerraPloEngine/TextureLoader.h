#pragma once
#include <string>
#include "GLTexture.h"
#include "picoPNG.h"
#include "IOManager.h"
#include "ErrorManager.h"

namespace SerraPlo {
	class TextureLoader {
	public:
		static GLTexture loadPNG(std::string filePath) {
			GLTexture texture = {};

			std::vector<unsigned char> out, in;
			unsigned long width, height;

			if (IOManager::loadFileToBuffer(filePath, in) == false) SP_THROW_ERROR("Failed to load PNG file to buffer");
			auto errorCode = decodePNG(out, width, height, &(in[0]), in.size());
			if (errorCode != 0) SP_THROW_ERROR("decodePNG failed with error" + std::to_string(errorCode));

			glGenTextures(1, &(texture.id));
			glBindTexture(GL_TEXTURE_2D, texture.id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);

			texture.width = width;
			texture.height = height;

			return texture;
		}
	};
}

