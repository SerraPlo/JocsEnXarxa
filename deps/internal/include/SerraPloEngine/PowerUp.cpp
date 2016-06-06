#include "PowerUp.h"
#include "ShaderProgram.h"
#include "PathLoader.h"
#include <ctime>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

namespace SerraPlo {

	void GreenShell::Init(glm::vec3 *pos, glm::vec3 *front) { // Initialize shell with car position and car vector front
		enabled = false; //redundant
		carPos = pos;
		carFront = front;
	}
	void GreenShell::Activate(void) { // Activate item, start lifetime counter and set main attributes
		if (!enabled) {
			lifeTimeCounter = float(clock());
			transform.position = glm::vec3{ carPos->x, 2, carPos->z };
			front = *carFront;
			enabled = true;
		}
	}
	void GreenShell::Update(float dt) {
		if (enabled && clock() < lifeTimeCounter + LIFETIME_DELAY) transform.position += front*speed*dt; // Update while lifetime has not ended
		else enabled = false; // Disable powerup when lifetems ends
	}
	void GreenShell::Draw(ShaderProgram & program, GLCamera & camera) {
		if (enabled) {
			program.Bind();
			// Send camera matrix to shader (projection + view)
			glUniformMatrix4fv(program.getUniformLocation("camera"), 1, GL_FALSE, glm::value_ptr(camera.PVMatrix()));
			// Set camera position as the viewer
			glUniform3fv(program.getUniformLocation("viewerPosition"), 1, glm::value_ptr(camera.position));
			// Transform properties
			glm::mat4 model = glm::translate(glm::mat4(), transform.position);
			model = glm::rotate(model, glm::radians(transform.rotation.x), { 1,0,0 });
			model = glm::rotate(model, glm::radians(transform.rotation.y), { 0,1,0 });
			model = glm::rotate(model, glm::radians(transform.rotation.z), { 0,0,1 });
			model = glm::scale(model, transform.scale);
			glUniformMatrix4fv(program.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
			for (int i = 0; i < meshRef->numMeshes; ++i) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, materialRef->materialData[i].diffuse.id);
				glUniform3fv(program.getUniformLocation("material.specular"), 1, glm::value_ptr(materialRef->materialData[i].specular));
				glUniform3fv(program.getUniformLocation("material.emissive"), 1, glm::value_ptr(materialRef->materialData[i].emissive));
				glUniform1f(program.getUniformLocation("material.shininess"), materialRef->materialData[i].shininess);
				glBindVertexArray(meshRef->meshData[i].vao);
				glDrawElements(GL_TRIANGLES, meshRef->meshData[i].numElements, GL_UNSIGNED_INT, nullptr);
			}
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(0);
		}
	}

	void RedShell::Init(glm::vec3 *pos, glm::vec3 *front) { // Initialize shell with car position and car vector front
		enabled = false; //redundant
		carPos = pos;
		carFront = front;
	}
	void RedShell::Activate(void) { // Activate item, start lifetime counter and set main attributes
		if (!enabled) {
			lifeTimeCounter = float(clock());
			transform.position = glm::vec3{ carPos->x, 2, carPos->z };
			float minDist = 1000000.0f;
			for (int i = 0; i < pathRef->size(); i++) {
				glm::vec2 iSegment = pathRef->at(i);
				if (glm::length(glm::distance(iSegment, glm::vec2(transform.position.x, transform.position.z))) <= minDist) {
					minDist = glm::length(glm::distance(iSegment, glm::vec2(transform.position.x, transform.position.z)));
					curPathNode = i + 1;
				}
			}
			front = *carFront;
			enabled = true;
		}
	}
	void RedShell::Update(float dt) {
		if (enabled && clock() < lifeTimeCounter + LIFETIME_DELAY) {
			glm::vec2 targetSegment = pathRef->at(curPathNode); // Find Current segment to target
			float distToTarget = glm::distance(glm::vec2(transform.position.x, transform.position.z), targetSegment);
			if (distToTarget < PATH_DISTANCE_DETECTION) { // Are we near enough targetSegment
				++curPathNode; // Update targetSegment next time
				if (curPathNode >= int(pathRef->size())) curPathNode = 0;
			}
			int aim = -1;
			for (int i = 0; i < karts.size(); i++) {
				float distToCar = glm::length(transform.position - *karts[i]);
				if (distToCar >= 1.0f && distToCar < distToTarget) {
					aim = i;
					break;
				}
			}
			if (aim == -1) {front = glm::normalize(glm::vec3(targetSegment.x, 0.0f, targetSegment.y) - transform.position);}
			else {front = glm::normalize(*karts[aim] - transform.position);}
			transform.position += front*speed*dt; // Update while lifetime has not ended
		}
		else enabled = false; // Disable powerup when lifetems ends
	}
	void RedShell::Draw(ShaderProgram & program, GLCamera & camera) {
		if (enabled) {
			program.Bind();
			// Send camera matrix to shader (projection + view)
			glUniformMatrix4fv(program.getUniformLocation("camera"), 1, GL_FALSE, glm::value_ptr(camera.PVMatrix()));
			// Set camera position as the viewer
			glUniform3fv(program.getUniformLocation("viewerPosition"), 1, glm::value_ptr(camera.position));
			// Transform properties
			glm::mat4 model = glm::translate(glm::mat4(), transform.position);
			model = glm::rotate(model, glm::radians(transform.rotation.x), { 1,0,0 });
			model = glm::rotate(model, glm::radians(transform.rotation.y), { 0,1,0 });
			model = glm::rotate(model, glm::radians(transform.rotation.z), { 0,0,1 });
			model = glm::scale(model, transform.scale);
			glUniformMatrix4fv(program.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
			for (int i = 0; i < meshRef->numMeshes; ++i) {
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, materialRef->materialData[i].diffuse.id);
				glUniform3fv(program.getUniformLocation("material.specular"), 1, glm::value_ptr(materialRef->materialData[i].specular));
				glUniform3fv(program.getUniformLocation("material.emissive"), 1, glm::value_ptr(materialRef->materialData[i].emissive));
				glUniform1f(program.getUniformLocation("material.shininess"), materialRef->materialData[i].shininess);
				glBindVertexArray(meshRef->meshData[i].vao);
				glDrawElements(GL_TRIANGLES, meshRef->meshData[i].numElements, GL_UNSIGNED_INT, nullptr);
			}
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(0);
		}
	}

}