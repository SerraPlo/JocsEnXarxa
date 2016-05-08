#include "ShaderProgram.h"
#include "IOManager.h"
#include <iostream>

namespace SerraPlo {

	ShaderProgram::ShaderProgram() {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Specifies the alignment requirements for the start of each pixel row in memory
	}

	ShaderProgram::~ShaderProgram() {
		if (m_vertexShaderID) glDeleteShader(m_vertexShaderID);
		if (m_fragmentShaderID) glDeleteShader(m_fragmentShaderID);
		if (programID) glDeleteProgram(programID);
	}

	void ShaderProgram::compileShader(const char* source, const std::string &name, GLuint id) {
		glShaderSource(id, 1, &source, nullptr);
		glCompileShader(id);
		auto success = 0; glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (success == GL_FALSE) {
			auto maxLength = 0; glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
			glDeleteShader(id);
			SP_THROW_ERROR("Shader " + name + " failed to be compiled. Error Log: " + &errorLog[0]);
		}
	}

	void ShaderProgram::LoadShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
		std::string vertSource, fragSource;

		IOManager::loadFileToBuffer(vertexShaderFilePath, vertSource);
		IOManager::loadFileToBuffer(fragmentShaderFilePath, fragSource);

		programID = glCreateProgram();
		m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (m_vertexShaderID == 0) SP_THROW_ERROR("Vertex shader failed to be created.");
		m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (m_fragmentShaderID == 0) SP_THROW_ERROR("Fragment shader failed to be created.");

		compileShader(vertSource.c_str(), "Vertex Shader", m_vertexShaderID);
		compileShader(fragSource.c_str(), "Fragment Shader", m_fragmentShaderID);
		linkShaders();
	}

	void ShaderProgram::linkShaders() const {
		glAttachShader(programID, m_vertexShaderID); glAttachShader(programID, m_fragmentShaderID);
		glLinkProgram(programID);
		auto isLinked = 0; glGetProgramiv(programID, GL_LINK_STATUS, static_cast<int*>(&isLinked));
		if (isLinked == GL_FALSE) {
			auto maxLength = 0; glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(programID, maxLength, &maxLength, &errorLog[0]);
			glDeleteProgram(programID); glDeleteShader(m_vertexShaderID); glDeleteShader(m_fragmentShaderID);
			SP_THROW_ERROR("Program failed to be compiled. Error Log: " + std::string(&errorLog[0]));
		}
		glDetachShader(programID, m_vertexShaderID);
		glDetachShader(programID, m_fragmentShaderID);
		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);
	}

	GLint ShaderProgram::getUniformLocation(const std::string & uniformName) const {
		GLint location = glGetUniformLocation(programID, uniformName.c_str()); // Get the location of a uniform value from the GLSL program
		if (location == GL_INVALID_INDEX) { SP_THROW_ERROR("Uniform " + uniformName + " not found on shader"); return 0; }
		return location;
	}

	GLint ShaderProgram::getAttribLocation(const std::string &attribName) const {
		GLint attrib = glGetAttribLocation(programID, attribName.c_str()); // Get the location of a variable from the GLSL program
		if (attrib == GL_INVALID_INDEX) { SP_THROW_ERROR("Attribute " + attribName + " not found on shader"); return 0; }
		return attrib;
	}

	void ShaderProgram::bind() const {
		glUseProgram(programID);
	}

	void ShaderProgram::unbind() const {
		glUseProgram(0);
	}
}
