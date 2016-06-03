#include "GLSprite.h"
#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include "PathLoader.h"

namespace SerraPlo {

	GLSprite::GLSprite() {
		GLfloat vertices[] = {
			// Positions          // Colors           // Texture Coords
			0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Top Right
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // Bottom Right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // Top Left 
		};
		GLuint indices[] = {  // Note that we start from 0!
			0, 1, 3, // First Triangle
			1, 2, 3  // Second Triangle
		};

		///Create VAO
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		///Create VBO
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		///Create EBO
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glBindVertexArray(0);
	}

	void GLSprite::SetImage(const std::string& path) {
		texture.Load(GetPathToAsset(path).c_str());
	}

	void GLSprite::Draw(ShaderProgram &program, GLCamera &camera) const {
		program.Bind();
		glUniformMatrix4fv(program.getUniformLocation("camera"), 1, GL_FALSE, glm::value_ptr(camera.PVMatrix()));
		glm::mat4 model;
		model = glm::translate(model, position);
		model = glm::rotate(model, glm::radians(rotation.x), { 1,0,0 });
		model = glm::rotate(model, glm::radians(rotation.y), { 0,1,0 });
		model = glm::rotate(model, glm::radians(rotation.z-180), { 0,0,1 });
		model = glm::scale(model, scale);
		glUniformMatrix4fv(program.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

		glDisable(GL_CULL_FACE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.id);

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); // Draw plane with 36 vertices
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(0);
		glEnable(GL_CULL_FACE);

	}

}