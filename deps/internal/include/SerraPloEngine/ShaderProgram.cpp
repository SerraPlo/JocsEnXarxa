#include "ShaderProgram.h"
#include "IOManager.h"
#include <iostream>

namespace SerraPlo {

	ShaderProgram::ShaderProgram() :
		programID(0), 
		m_vertexShaderID(0), 
		m_fragmentShaderID(0),
		m_numAttributes(0) {
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	ShaderProgram::~ShaderProgram() {
		if (m_vertexShaderID) glDeleteShader(m_vertexShaderID);
		if (m_fragmentShaderID) glDeleteShader(m_fragmentShaderID);
		if (programID) glDeleteProgram(programID);
	}

	void ShaderProgram::compileShader(const char* source, const std::string &name, GLuint id) {
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

	void ShaderProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
		std::string vertSource;
		std::string fragSource;

		IOManager::loadFileToBuffer(vertexShaderFilePath, vertSource);
		IOManager::loadFileToBuffer(fragmentShaderFilePath, fragSource);

		compileShadersFromSource(vertSource.c_str(), fragSource.c_str());
	}

	void ShaderProgram::compileShadersFromSource(const char* vertexSource, const char* fragmentSource) {
			programID = glCreateProgram();
			//if (m_programID == 0) fatalError("GLProgram failed to be created.");
			m_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
			if (m_vertexShaderID == 0) SP_THROW_ERROR("Vertex shader failed to be created.");
			m_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
			if (m_fragmentShaderID == 0) SP_THROW_ERROR("Fragment shader failed to be created.");

			compileShader(vertexSource, "Vertex Shader", m_vertexShaderID);
			compileShader(fragmentSource, "Fragment Shader", m_fragmentShaderID);
		}

	void ShaderProgram::linkShaders() const {
		glAttachShader(programID, m_vertexShaderID);
		glAttachShader(programID, m_fragmentShaderID);
		glLinkProgram(programID);

		auto isLinked = 0;
		glGetProgramiv(programID, GL_LINK_STATUS, static_cast<int*>(&isLinked));
		if (isLinked == GL_FALSE) {
			auto maxLength = 0;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<char> errorLog(maxLength);
			glGetProgramInfoLog(programID, maxLength, &maxLength, &errorLog[0]);
			glDeleteProgram(programID);
			glDeleteShader(m_vertexShaderID);
			glDeleteShader(m_fragmentShaderID);
			SP_THROW_ERROR("Program failed to be compiled. Error Log: " + std::string(&errorLog[0]));
		}

		glDetachShader(programID, m_vertexShaderID);
		glDetachShader(programID, m_fragmentShaderID);
		glDeleteShader(m_vertexShaderID);
		glDeleteShader(m_fragmentShaderID);
	}

	void ShaderProgram::addAttribute(const std::string& attributeName) {
		glBindAttribLocation(programID, m_numAttributes++, attributeName.c_str()); // Bind the variables used in the GLSL source code with our program data
	}

	GLint ShaderProgram::getUniformLocation(const std::string & uniformName) const {
		GLint location = glGetUniformLocation(programID, uniformName.c_str()); // Get the location of a uniform value from the GLSL program
		/*if (location == GL_INVALID_INDEX) {
			SP_THROW_ERROR("Uniform " + uniformName + " not found on shader");
			return 0;
		}*/
		return location;
	}

	GLint ShaderProgram::getAttribLocation(const std::string &attribName) const {
		GLint attrib = glGetAttribLocation(programID, attribName.c_str()); // Get the location of a variable from the GLSL program
		if (attrib == GL_INVALID_INDEX) {
			SP_THROW_ERROR("Attribute " + attribName + " not found on shader");
			return 0;
		}
		return attrib;
	}

	void ShaderProgram::bind() const {
		glUseProgram(programID);
		for (auto i = 0; i < m_numAttributes; ++i) {
			glEnableVertexAttribArray(i);
		}
	}

	void ShaderProgram::unbind() const {
		glUseProgram(0);
		for (auto i = 0; i < m_numAttributes; ++i) {
			glDisableVertexAttribArray(i);
		}
	}

	void ShaderProgram::dispose() const {
		if (programID) glDeleteProgram(programID);
	}
}
