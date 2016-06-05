#include "AIPhysics.h"
#include <SDL2/SDL_stdinc.h>
#include <iostream>

#define PATH_DISTANCE_DETECTION 10.0f

namespace SerraPlo {

	static inline glm::vec2 DoKinematicSeek(glm::vec2 targetPosition, glm::vec2 agentPosition, float agentMaxSpeed) {
		glm::vec2 desiredVelocity = glm::normalize(targetPosition - agentPosition); // Calculate desired velocity
		desiredVelocity *= agentMaxSpeed; // Scale by agentMaxSpeed
		return desiredVelocity;
	}

	static inline glm::vec2 DoSteeringSeek(glm::vec2 targetPosition, glm::vec2 agentPosition, glm::vec2 agentSpeed, float agentMaxSpeed, float agentMaxForce) {
		glm::vec2 desiredVelocity = DoKinematicSeek(targetPosition, agentPosition, agentMaxSpeed); // Calculate desired velocity		
		glm::vec2 steeringForce = desiredVelocity - agentSpeed; // Calculate Steering Force
		steeringForce /= agentMaxSpeed; // Divide by agentMaxSpeed to get the speed factor
		steeringForce *= agentMaxForce; // Scale this factor by agentMaxForce
		return steeringForce;
	}

	bool AIPhysics::carColCalc(glm::vec3 a, glm::vec3 b) {
		float rad = 5.0f;
		if (glm::length(a - b) <= rad) return true;
		else return false;
	}

	void AIPhysics::Update(float deltaTime){
		glm::vec2 steeringForce{};
		glm::vec2 acceleration{};
		float angle{ .0f };
		glm::vec3 posP = glm::vec3(0.0f,0.0f,0.0f);
		if (playerOn) posP = aiCarArray[0].playerRef->position;
		for (int i = 0; i < aiCarArray.size(); i++) aiCarArray[i].collisionCar = -1;
		for (int i = 0; i < aiCarArray.size(); i++) {
			glm::vec3 posI = aiCarArray[i].transformRef->position;
			if (carColCalc(posI, posP)) aiCarArray[i].collisionCar = -10;
			else if(aiCarArray[i].collisionCar == -1){
				for (int j = 0; j < aiCarArray.size()-i; j++) {
					if (i != j && carColCalc(posI, aiCarArray[j].transformRef->position)) {
						aiCarArray[i].collisionCar = j; aiCarArray[j].collisionCar = i;
						break;
					}
					else aiCarArray[i].collisionCar = -1;
				}
			}
		}
		for (auto &aiCar : aiCarArray) {
			glm::vec2 positionIA = { aiCar.transformRef->position.x, aiCar.transformRef->position.z };
			glm::vec2 newPos = { aiCar.transformRef->position.x, aiCar.transformRef->position.z };
			glm::vec2 targetSegment = aiPathRef->at(aiCar.curPathNode); // Find Current segment to target
			if (glm::distance(positionIA, targetSegment) < PATH_DISTANCE_DETECTION) { // Are we near enough targetSegment
				++aiCar.curPathNode; // Update targetSegment next time
				if (aiCar.curPathNode >= int(aiPathRef->size())) aiCar.curPathNode = 0;
			}
			// Seek target segment
			steeringForce = DoSteeringSeek(targetSegment, positionIA, aiCar.speed, aiCar.maxSpeed, aiCar.maxSteerForce);
			acceleration = (steeringForce / aiCar.mass);
			aiCar.speed += acceleration * deltaTime*0.1f;
			newPos += aiCar.speed;
			//std::cout << aiCar.speed.x << ", " << aiCar.speed.y << std::endl;
			glm::vec2 direction = glm::normalize(aiCar.speed);
			angle = float(atan2f(direction.y, -direction.x) * RAD2DEG) - 90.0f;
			if (glm::length(angle) > 1.0f) aiCar.transformRef->rotation.y = angle;
			//detect collisions and correct it
			if (aiCar.collisionForce > 0.0f) aiCar.collisionForce -= 100.0f*deltaTime;
			else aiCar.collisionForce = 0.0f;
			if (aiCar.collisionCarForce > 0.0f) aiCar.collisionCarForce -= 100.0f*deltaTime;
			else aiCar.collisionCarForce = 0.0f;
			glm::vec2 pFront = glm::vec2(-direction.y, direction.x);
			positionsCol[0] = newPos + direction*2.0f + pFront*1.25f;	positionsCol[1] = newPos + direction*2.0f - pFront*1.25f;
			positionsCol[2] = newPos + direction*2.0f + pFront*1.25f;	positionsCol[3] = newPos + direction*2.0f - pFront*1.25f;
			if (aiCar.collisionCar >= 0) {
				aiCar.speed /= 10.0f;
				aiCar.collisionCarDirection = glm::normalize(positionIA - glm::vec2(aiCarArray[aiCar.collisionCar].transformRef->position.x, aiCarArray[aiCar.collisionCar].transformRef->position.z));
				aiCar.collisionCarForce = 40.0f;
			}else if (aiCar.collisionCar == -10) {//player
				aiCar.speed /= 10.0f;
				aiCar.collisionCarDirection = glm::normalize(positionIA - glm::vec2(aiCar.playerRef->position.x, aiCar.playerRef->position.z));
				aiCar.collisionCarForce = 40.0f;
			}
			int i = collisions.CalculateCollision(positionsCol);
			if (i != -1) {
				aiCar.collisionCarForce = 0.0f;
				if (i < collisions.nBoxs) {
					aiCar.speed == glm::vec2(0.0f, 0.0f);
					aiCar.collisionDirection = glm::normalize(positionIA - newPos);
					aiCar.collisionForce = 40.0f;
				}else {
					aiCar.speed /= 2.0f;
					aiCar.collisionDirection = glm::normalize(newPos - collisions.circles[i - collisions.nBoxs].c);
					aiCar.collisionForce = 40.0f;
				}
			}
			aiCar.transformRef->position = { (newPos + (aiCar.collisionDirection*aiCar.collisionForce + aiCar.collisionCarDirection*aiCar.collisionCarForce)*deltaTime).x,
				0.0f, (newPos + (aiCar.collisionDirection*aiCar.collisionForce + aiCar.collisionCarDirection*aiCar.collisionCarForce)*deltaTime).y };
		}
	}
}
