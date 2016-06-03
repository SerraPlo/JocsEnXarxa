#pragma once
#include <glm/glm.hpp>
#include "Transform.h"
#include <vector>

namespace SerraPlo {

#define RAD2DEG (180.0f / M_PI)
#define DEG2RAD (M_PI / 180.0f)

	class AIPhysics {
		struct AICarPhysics {
			Transform *transformRef;
			glm::vec2 speed;
			float maxSpeed; // = 1.5f;
			float maxSteerForce; // = 6.0f * 60.0f; // Normal
			float mass;
			int curPathNode;
		};
	public:
		std::vector<AICarPhysics> aiCarArray;
		std::vector<glm::vec2> *aiPathRef;
		AIPhysics() = default;
		~AIPhysics() = default;
		void AddPath(std::vector<glm::vec2> *pathR) { aiPathRef = pathR; }
		void AddAICar(Transform *transformRef, float maxSpeed, float maxSteerForce, float mass = 800.0f) {
			AICarPhysics aiCar = { transformRef, {}, maxSpeed, maxSteerForce, mass, 0 };
			aiCarArray.push_back(aiCar);
		}
		void Update(float deltaTime);
		void Reset() { for (auto &aiCar : aiCarArray) { aiCar.speed = {}; aiCar.curPathNode = 0; } }
	};

}
