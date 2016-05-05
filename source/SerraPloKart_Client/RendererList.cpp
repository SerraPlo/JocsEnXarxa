#include "RendererList.h"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

void RendererList::Add(SerraPlo::GameObject * newObject) {
	m_objectList.push_back(newObject);
}

void RendererList::Add(SerraPlo::BaseLight * newLight) {
	m_lightList.push_back(newLight);
}

void RendererList::DrawObjects(SerraPlo::ShaderProgram &program) {
	for (auto gameObject : m_objectList) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gameObject->texture.id);
		glUniform1i(program.getUniformLocation("diffuseSampler"), 0);

		Transform &transformTemp = gameObject->transform;
		glm::mat4 model = glm::translate(glm::mat4(), transformTemp.position);
		model = glm::rotate(model, glm::radians(transformTemp.rotation.x), glm::vec3{ 1,0,0 });
		model = glm::rotate(model, glm::radians(transformTemp.rotation.y), glm::vec3{ 0,1,0 });
		model = glm::rotate(model, glm::radians(transformTemp.rotation.z), glm::vec3{ 0,0,1 });
		model = glm::scale(model, transformTemp.scale);
		glUniformMatrix4fv(program.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

		const glm::vec4 white(1);
		const glm::vec4 black(0);
		const glm::vec4 ambient(0.1f, 0.1f, 0.1f, 1.0f);
		// Light properties.
		auto g_uniformLightPosW = program.getUniformLocation("LightPosW");
		auto g_uniformLightColor = program.getUniformLocation("LightColor");
		glUniform4fv(g_uniformLightPosW, 1, glm::value_ptr(model[3]));
		glUniform4fv(g_uniformLightColor, 1, glm::value_ptr(white));

		// Global ambient.
		auto g_uniformAmbient = program.getUniformLocation("Ambient");
		glUniform4fv(g_uniformAmbient, 1, glm::value_ptr(ambient));

		// Material properties.
		auto g_uniformMaterialEmissive = program.getUniformLocation("MaterialEmissive");
		auto g_uniformMaterialDiffuse = program.getUniformLocation("MaterialDiffuse");
		auto g_uniformMaterialSpecular = program.getUniformLocation("MaterialSpecular");
		auto g_uniformMaterialShininess = program.getUniformLocation("MaterialShininess");
		glUniform4fv(g_uniformMaterialEmissive, 1, glm::value_ptr(black));
		glUniform4fv(g_uniformMaterialDiffuse, 1, glm::value_ptr(white));
		glUniform4fv(g_uniformMaterialSpecular, 1, glm::value_ptr(white));
		glUniform1f(g_uniformMaterialShininess, 50.0f);

		auto g_uniformEyePosW = program.getUniformLocation("EyePosW");
		glm::vec4 eyePosW = glm::vec4(glm::vec3{ 0,0,0 }, 1);
		glUniform4fv(g_uniformEyePosW, 1, glm::value_ptr(eyePosW));

		glBindVertexArray(gameObject->mesh.vao);
		glDrawElements(GL_TRIANGLES, gameObject->mesh.elements, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void RendererList::DrawLights(SerraPlo::ShaderProgram & program) {
	for (auto gameLight : m_lightList) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 1);
		glUniform1i(program.getUniformLocation("diffuseSampler"), 0);

		glm::mat4 model = glm::translate(glm::mat4(), {0,0,0});
		glUniformMatrix4fv(program.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

		const glm::vec4 white(1);
		const glm::vec4 black(0);
		const glm::vec4 ambient(0.1f, 0.1f, 0.1f, 1.0f);
		// Light properties.
		auto g_uniformLightPosW = program.getUniformLocation("LightPosW");
		auto g_uniformLightColor = program.getUniformLocation("LightColor");
		glUniform4fv(g_uniformLightPosW, 1, glm::value_ptr(model[3]));
		glUniform4fv(g_uniformLightColor, 1, glm::value_ptr(white));

		// Global ambient.
		auto g_uniformAmbient = program.getUniformLocation("Ambient");
		glUniform4fv(g_uniformAmbient, 1, glm::value_ptr(ambient));

		// Material properties.
		auto g_uniformMaterialEmissive = program.getUniformLocation("MaterialEmissive");
		auto g_uniformMaterialDiffuse = program.getUniformLocation("MaterialDiffuse");
		auto g_uniformMaterialSpecular = program.getUniformLocation("MaterialSpecular");
		auto g_uniformMaterialShininess = program.getUniformLocation("MaterialShininess");
		glUniform4fv(g_uniformMaterialEmissive, 1, glm::value_ptr(black));
		glUniform4fv(g_uniformMaterialDiffuse, 1, glm::value_ptr(white));
		glUniform4fv(g_uniformMaterialSpecular, 1, glm::value_ptr(white));
		glUniform1f(g_uniformMaterialShininess, 50.0f);

		auto g_uniformEyePosW = program.getUniformLocation("EyePosW");
		glm::vec4 eyePosW = glm::vec4(glm::vec3{ 0,0,0 }, 1);
		glUniform4fv(g_uniformEyePosW, 1, glm::value_ptr(eyePosW));

		glBindVertexArray(gameLight->vao);
		glDrawElements(GL_TRIANGLES, gameLight->elements, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
