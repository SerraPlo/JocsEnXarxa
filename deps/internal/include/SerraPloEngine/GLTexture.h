#pragma once
#include <GL\glew.h>
#include <SOIL2/SOIL2.h>
#pragma comment(lib, "SOIL.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

namespace SerraPlo {

#define EMPTY_TEXTURE -1

	struct GLTexture {
		GLuint id;
		explicit GLTexture() = default;
		explicit GLTexture(const char* filePath) {
			if (filePath == nullptr) { id = EMPTY_TEXTURE; return; }
			// Load, create texture and generate mipmaps
			id = SOIL_load_OGL_texture(filePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
			// Set texture filtering
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Set texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so not accidentily mess up our texture
		}
	};

}

