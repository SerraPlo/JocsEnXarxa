#include "RendererList.h"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

void RendererList::Add(SerraPlo::GameObject * newObject) {
	m_objectList.push_back(newObject);
}

void RendererList::Add(SerraPlo::BaseLight * newLight) {
	m_lightList.push_back(newLight);
}

void RendererList::DrawObjects(SerraPlo::ShaderProgram &program, Camera &camera) {
	for (auto gameObject : m_objectList) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gameObject->texture.id);
		glUniform1i(program.getUniformLocation("diffuseSampler"), 0);

		// Send camera matrix to shader (projection + view)
		glUniformMatrix4fv(program.getUniformLocation("camera"), 1, GL_FALSE, glm::value_ptr(camera.PVMatrix()));

		Transform &transformTemp = gameObject->transform;
		glm::mat4 model = glm::translate(glm::mat4(), transformTemp.position);
		model = glm::rotate(model, glm::radians(transformTemp.rotation.x), glm::vec3{ 1,0,0 });
		model = glm::rotate(model, glm::radians(transformTemp.rotation.y), glm::vec3{ 0,1,0 });
		model = glm::rotate(model, glm::radians(transformTemp.rotation.z), glm::vec3{ 0,0,1 });
		model = glm::scale(model, transformTemp.scale);
		glUniformMatrix4fv(program.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(program.getUniformLocation("lightingEnabled"), GL_TRUE);
		glUniform1i(program.getUniformLocation("isALightSource"), GL_FALSE);

		const glm::vec3 white(1);
		const glm::vec3 black(0);
		const glm::vec3 ambient(0.1f, 0.1f, 0.1f);
		// Light properties.
		auto g_uniformLightPosW = program.getUniformLocation("lightPosition");
		auto g_uniformLightColor = program.getUniformLocation("lightColor");
		glUniform3fv(g_uniformLightPosW, 1, glm::value_ptr(model));
		glUniform3fv(g_uniformLightColor, 1, glm::value_ptr(white));

		// Global ambient.
		auto g_uniformAmbient = program.getUniformLocation("material.ambient");
		glUniform3fv(g_uniformAmbient, 1, glm::value_ptr(ambient));

		// Material properties.
		auto g_uniformMaterialEmissive = program.getUniformLocation("material.emissive");
		auto g_uniformMaterialDiffuse = program.getUniformLocation("material.diffuse");
		auto g_uniformMaterialSpecular = program.getUniformLocation("material.specular");
		auto g_uniformMaterialShininess = program.getUniformLocation("material.shininess");
		glUniform3fv(g_uniformMaterialEmissive, 1, glm::value_ptr(black));
		glUniform3fv(g_uniformMaterialDiffuse, 1, glm::value_ptr(white));
		glUniform3fv(g_uniformMaterialSpecular, 1, glm::value_ptr(white));
		glUniform1f(g_uniformMaterialShininess, 50.0f);

		glUniform3fv(program.getUniformLocation("viewerPosition"), 1, glm::value_ptr(camera.position));

		glBindVertexArray(gameObject->mesh.vao);
		glDrawElements(GL_TRIANGLES, gameObject->mesh.elements, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void RendererList::DrawLights(SerraPlo::ShaderProgram & program, Camera &camera) {
	for (auto gameLight : m_lightList) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 1);
		glUniform1i(program.getUniformLocation("diffuseSampler"), 0);

		// Send camera matrix to shader (projection + view)
		glUniformMatrix4fv(program.getUniformLocation("camera"), 1, GL_FALSE, glm::value_ptr(camera.PVMatrix()));

		glm::mat4 model = glm::translate(glm::mat4(), {0,0,0});
		glUniformMatrix4fv(program.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

		glUniform1i(program.getUniformLocation("lightingEnabled"), GL_TRUE);
		glUniform1i(program.getUniformLocation("isALightSource"), GL_TRUE);

		const glm::vec3 white(1);
		const glm::vec3 black(0);
		const glm::vec3 ambient(0.1f, 0.1f, 0.1f);
		// Light properties.
		auto g_uniformLightPosW = program.getUniformLocation("lightPosition");
		auto g_uniformLightColor = program.getUniformLocation("lightColor");
		glUniform3fv(g_uniformLightPosW, 1, glm::value_ptr(model));
		glUniform3fv(g_uniformLightColor, 1, glm::value_ptr(white));

		// Global ambient.
		auto g_uniformAmbient = program.getUniformLocation("material.ambient");
		glUniform3fv(g_uniformAmbient, 1, glm::value_ptr(ambient));

		// Material properties.
		auto g_uniformMaterialEmissive = program.getUniformLocation("material.emissive");
		auto g_uniformMaterialDiffuse = program.getUniformLocation("material.diffuse");
		auto g_uniformMaterialSpecular = program.getUniformLocation("material.specular");
		auto g_uniformMaterialShininess = program.getUniformLocation("material.shininess");
		glUniform3fv(g_uniformMaterialEmissive, 1, glm::value_ptr(black));
		glUniform3fv(g_uniformMaterialDiffuse, 1, glm::value_ptr(white));
		glUniform3fv(g_uniformMaterialSpecular, 1, glm::value_ptr(white));
		glUniform1f(g_uniformMaterialShininess, 50.0f);

		glUniform3fv(program.getUniformLocation("viewerPosition"), 1, glm::value_ptr(camera.position));

		glBindVertexArray(gameLight->vao);
		glDrawElements(GL_TRIANGLES, gameLight->elements, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
