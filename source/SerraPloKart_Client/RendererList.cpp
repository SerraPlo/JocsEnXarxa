#include "RendererList.h"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void RendererList::Add(GameObject * newObject) {
	m_objectList.push_back(newObject);
}

void RendererList::Add(BaseLight newLight) {
	m_lightList.push_back(newLight);
}

void RendererList::DrawObjects(ShaderProgram &program, Camera &camera) {
	// Send camera matrix to shader (projection + view)
	glUniformMatrix4fv(program.getUniformLocation("camera"), 1, GL_FALSE, glm::value_ptr(camera.PVMatrix()));
	// Set camera position as the viewer
	glUniform3fv(program.getUniformLocation("viewerPosition"), 1, glm::value_ptr(camera.position));

	// Directional light properties
	glUniform3f(program.getUniformLocation("dirLight.direction"), -0.2f, -1.0f, -0.3f);
	glUniform3f(program.getUniformLocation("dirLight.ambient"), 0.0f, 0.0f, 0.0f); //0.05f, 0.05f, 0.05f
	glUniform3f(program.getUniformLocation("dirLight.diffuse"), 0.0f, 0.0f, 0.0f); //0.4f, 0.4f, 0.4f
	glUniform3f(program.getUniformLocation("dirLight.specular"), 0.0f, 0.0f, 0.0f); //0.5f, 0.5f, 0.5f

	// Point light properties
	glUniform3f(program.getUniformLocation("pointLights[0].position"), 10, 0, 0); //temp pos !!!!!!!!!!!
	glUniform3f(program.getUniformLocation("pointLights[0].ambient"), 0.5f, 0.0f, 0.5f);
	glUniform3f(program.getUniformLocation("pointLights[0].diffuse"), 1.0f, 0.0f, 1.0f);
	glUniform3f(program.getUniformLocation("pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
	glUniform1f(program.getUniformLocation("pointLights[0].constant"), 1.0f);
	glUniform1f(program.getUniformLocation("pointLights[0].linear"), 0.09f);
	glUniform1f(program.getUniformLocation("pointLights[0].quadratic"), 0.032f);

	// Spot light properties
	glUniform3fv(program.getUniformLocation("spotLights[0].position"), 1, glm::value_ptr(glm::vec3{ -10,5,0 }));
	glUniform3fv(program.getUniformLocation("spotLights[0].direction"), 1, glm::value_ptr(glm::vec3{ 0,-1,0 }));
	glUniform1f(program.getUniformLocation("spotLights[0].cutOff"), glm::cos(glm::radians(40.0f)));
	glUniform1f(program.getUniformLocation("spotLights[0].outerCutOff"), glm::cos(glm::radians(45.0f)));
	glUniform3fv(program.getUniformLocation("spotLights[0].diffuse"), 1, glm::value_ptr(COLOR_GREEN));
	glUniform3fv(program.getUniformLocation("spotLights[0].ambient"), 1, glm::value_ptr(glm::vec3{ 0.0f,0.5f,0.0f }));
	glUniform3fv(program.getUniformLocation("spotLights[0].specular"), 1, glm::value_ptr(COLOR_WHITE));
	glUniform1f(program.getUniformLocation("spotLights[0].constant"), 1.0f);
	glUniform1f(program.getUniformLocation("spotLights[0].linear"), 0.09f);
	glUniform1f(program.getUniformLocation("spotLights[0].quadratic"), 0.032f);

	glUniform1i(program.getUniformLocation("material.diffuse"), 0);
	glUniform1i(program.getUniformLocation("material.normal"), 1);
	glUniform1i(program.getUniformLocation("material.specular"), 2);

	for (auto gameObject : m_objectList) {
		// Material properties
		if (gameObject->material.diffuse.id != EMPTY_TEXTURE)
			glActiveTexture(GL_TEXTURE0), glBindTexture(GL_TEXTURE_2D, gameObject->material.diffuse.id);
		if (gameObject->material.normal.id != EMPTY_TEXTURE)
			glActiveTexture(GL_TEXTURE1), glBindTexture(GL_TEXTURE_2D, gameObject->material.normal.id);
		if (gameObject->material.specular.id != EMPTY_TEXTURE)
			glActiveTexture(GL_TEXTURE2),glBindTexture(GL_TEXTURE_2D, gameObject->material.specular.id);
		glUniform1f(program.getUniformLocation("material.shininess"), gameObject->material.shininess);

		// Transform properties
		Transform &transformTemp = gameObject->transform;
		glm::mat4 model = glm::translate(glm::mat4(), transformTemp.position);
		model = glm::rotate(model, glm::radians(transformTemp.rotation.x), glm::vec3{ 1,0,0 });
		model = glm::rotate(model, glm::radians(transformTemp.rotation.y), glm::vec3{ 0,1,0 });
		model = glm::rotate(model, glm::radians(transformTemp.rotation.z), glm::vec3{ 0,0,1 });
		model = glm::scale(model, transformTemp.scale);
		glUniformMatrix4fv(program.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

		// Draw game object
		glBindVertexArray(gameObject->mesh.vao);
		glDrawElements(GL_TRIANGLES, gameObject->mesh.elements, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void RendererList::DrawLights(ShaderProgram & program, Camera &camera) {
	static DebugLight debugLight;
	for (auto gameLight : m_lightList) {
		// Send camera matrix to shader (projection + view)
		glUniformMatrix4fv(program.getUniformLocation("camera"), 1, GL_FALSE, glm::value_ptr(camera.PVMatrix()));

		glm::mat4 model = glm::translate(glm::mat4(), gameLight.position);
		glUniformMatrix4fv(program.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

		glUniform3fv(program.getUniformLocation("lightColor"), 1, glm::value_ptr(gameLight.diffuse));

		glBindVertexArray(debugLight.vao);
		glDrawElements(GL_TRIANGLES, debugLight.elements, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
