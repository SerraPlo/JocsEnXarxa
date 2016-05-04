#pragma once
#include <GL\glew.h>
#include <SOIL/SOIL.h>
#pragma comment(lib, "SOIL.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

namespace SerraPlo {
	struct GLTexture {
		GLuint id;
		int width, height;
		explicit GLTexture() = default;
		explicit GLTexture(const char* filePath) {
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id); // All upcoming GL_TEXTURE_2D operations now have effect on texture object
			// Set texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Set texture wrapping to GL_REPEAT
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// Set texture filtering
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Load, create texture and generate mipmaps
			id = SOIL_load_OGL_texture(filePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
			glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so not accidentily mess up our texture
		}
	};
}

