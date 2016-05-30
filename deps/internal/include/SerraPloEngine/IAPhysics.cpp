#include "IAPhysics.h"
#include <SDL2/SDL_stdinc.h>
#include <iostream>

#define PATH_DISTANCE_DETECTION 10.0f
#define RAD2DEG (180.0f / M_PI)
#define DEG2RAD (M_PI / 180.0f)

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

	void AIPhysics::Update(float deltaTime) {
		glm::vec2 steeringForce{};
		glm::vec2 acceleration{};
		float angle{ .0f };
		for (auto &aiCar : aiCarArray) {
			glm::vec2 positionIA = { aiCar.transformRef->position.x, aiCar.transformRef->position.z };
			glm::vec2 targetSegment = aiPathRef->at(aiCar.curPathNode); // Find Current segment to target
			if (glm::distance(positionIA, targetSegment) < PATH_DISTANCE_DETECTION) { // Are we near enough targetSegment
				++aiCar.curPathNode; // Update targetSegment next time
				if (aiCar.curPathNode >= aiPathRef->size()) aiCar.curPathNode = 0;
			}
			// Seek target segment
			steeringForce = DoSteeringSeek(targetSegment, positionIA, aiCar.speed, aiCar.maxSpeed, aiCar.maxSteerForce);
			acceleration = steeringForce / aiCar.mass;
			aiCar.speed += acceleration * deltaTime*0.1f;
			aiCar.transformRef->position.x += aiCar.speed.x;
			aiCar.transformRef->position.z += aiCar.speed.y;
			std::cout << aiCar.speed.x << ", " << aiCar.speed.y << std::endl;
			glm::vec2 direction = glm::normalize(aiCar.speed);
			angle = float(atan2f(direction.y, -direction.x) * RAD2DEG) - 90.0f;
			if (glm::length(angle) > 1.0f) aiCar.transformRef->rotation.y = angle;
		}
	}

}
