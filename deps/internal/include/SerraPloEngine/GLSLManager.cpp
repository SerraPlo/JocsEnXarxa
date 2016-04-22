#include "GLSLManager.h"
#include "IOManager.h"
#include <iostream>

namespace SerraPlo {

	GLSLManager::GLSLManager() : 
		m_programID(0), 
		m_vertexShaderID(0), 
		m_fragmentShaderID(0),
		m_numAttributes(0) {}

	GLSLManager::~GLSLManager() {
		if (m_vertexShaderID) glDeleteShader(m_vertexShaderID);
		if (m_fragmentShaderID) glDeleteShader(m_fragmentShaderID);
		if (m_programID) glDeleteProgram(m_programID);
	}

	void GLSLManager::compileShader(const char* source, const std::string &name, GLuint id) {
		glShaderSource(id, 1, &source, nullptr);
		glCompileShader(id);

		auto success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE) {
			auto maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> errorLog(maxLength);
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
			glDeleteShader(id);
			std::cout << &errorLog[0] << std::endl;
			SP_THROW_ERROR("Shader " + name + " failed to be compiled.");
		}
	}

	void GLSLManager::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
		std::string vertSource;
		std::string fragSource;

		IOManager::loadFileToBuffer(vertexShaderFilePath, vertSource);
		IOManager::loadFileToBuffer(fragmentShaderFilePath, fragSource);

		compileShadersFromSource(vertSource.c_str(), fragSource.c_str());
	}

void GLSLManager::compileShadersFromSource(const char* vertexSource, const char* fragmentSource) {
		m_programID = glCreateProgram();
		//if (m_programID == 0) fatalError("GLProgram failed to be created.");
		m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (m_vertexShaderID == 0) SP_THROW_ERROR("Vertex shader failed to be created.");
		m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (m_fragmentShaderID == 0) SP_THROW_ERROR("Fragment shader failed to be created.");

		compileShader(vertexSource, "Vertex Shader", m_vertexShaderID);
		compileShader(fragmentSource, "Fragment Shader", m_fragmentShaderID);
	}

void GLSLManager::linkShaders() const
	{
		glAttachShader(m_programID, m_vertexShaderID);
		glAttachShader(m_programID, m_fragmentShaderID);
		glLinkProgram(m_programID);

		auto isLinked = 0;
		glGetProgramiv(m_programID, GL_LINK_STATUS, static_cast<int*>(&isLinked));
		if (isLinked == GL_FALSE) {
			auto maxLength = 0;
			glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(m_programID, maxLength, &maxLength, &errorLog[0]);
			glDeleteProgram(m_programID);
			glDeleteShader(m_vertexShaderID);
			glDeleteShader(m_fragmentShaderID);
			std::cout << &errorLog[0] << std::endl;
			SP_THROW_ERROR("Program failed to be compiled.");
		}

		glDetachShader(m_programID, m_vertexShaderID);
		glDetachShader(m_programID, m_fragmentShaderID);
		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);
	}

	void GLSLManager::addAttribute(const std::string& attributeName) {
		glBindAttribLocation(m_programID, m_numAttributes++, attributeName.c_str());
	}

	GLint GLSLManager::getUniformLocation(const std::string & uniformName) const
	{
		auto location = glGetUniformLocation(m_programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX) {
			SP_THROW_ERROR("Uniform " + uniformName + " not found on shader");
			return 0;
		}
		return location;
	}

	void GLSLManager::bind() const
	{
		glUseProgram(m_programID);
		for (auto i = 0; i < m_numAttributes; i++) {
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLManager::unbind() const
	{
		glUseProgram(0);
		for (auto i = 0; i < m_numAttributes; i++) {
			glDisableVertexAttribArray(i);
		}
	}

	void GLSLManager::dispose() const {
		if (m_programID) glDeleteProgram(m_programID);
	}
}
