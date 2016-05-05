#include "SceneObjectList.h"
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>

void SceneObjectList::Add(SerraPlo::GameObject * newObject) {
	m_objectList.push_back(newObject);
}

void SceneObjectList::Draw(SerraPlo::ShaderProgram &program) {
	for (auto gameObject : m_objectList) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gameObject->texture.id);
		glUniform1i(program.getUniformLocation("texture_diffuse"), 0);

		Transform &transformTemp = gameObject->transform;
		glm::mat4 model = glm::translate(glm::mat4(), transformTemp.position);
		model = glm::rotate(model, glm::radians(transformTemp.rotation.x), glm::vec3{ 1,0,0 });
		model = glm::rotate(model, glm::radians(transformTemp.rotation.y), glm::vec3{ 0,1,0 });
		model = glm::rotate(model, glm::radians(transformTemp.rotation.z), glm::vec3{ 0,0,1 });
		model = glm::scale(model, transformTemp.scale);
		glUniformMatrix4fv(program.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(gameObject->mesh.vao);
		glDrawElements(GL_TRIANGLES, gameObject->mesh.elements, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
