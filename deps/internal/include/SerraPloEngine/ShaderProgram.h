#pragma once
#include "ErrorManager.h"
#include <GL\glew.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")

namespace SerraPlo {

	class ShaderProgram {
		GLuint programID			{ 0 };
		GLuint m_vertexShaderID		{ 0 };
		GLuint m_fragmentShaderID	{ 0 };
		int m_numAttributes			{ 0 };

		static void compileShader(const char* source, const std::string &name, GLuint id);
	public:
		explicit ShaderProgram();
		~ShaderProgram();

		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
		void linkShaders() const;

		GLint getUniformLocation(const std::string &uniformName) const;
		GLint getAttribLocation(const std::string &attribName) const;

		void bind() const;
		void unbind() const;
	};

}

