#pragma once
#include <glm/glm.hpp>
#include "Transform.h"
#include "CollisionManager.h"
#include "PathLoader.h"
#include <vector>

namespace SerraPlo {

#define RAD2DEG (180.0f / M_PI)
#define DEG2RAD (M_PI / 180.0f)

	class AIPhysics {
		struct AICarPhysics {
			Transform *playerRef;
			Transform *transformRef;
			glm::vec2 speed;
			float maxSpeed; // = 1.5f;
			float maxSteerForce; // = 6.0f * 60.0f; // Normal
			float mass;
			int curPathNode;
			float collisionForce;
			glm::vec2 collisionDirection;
			int collisionCar;
			float collisionCarForce;
			glm::vec2 collisionCarDirection;
		};
	public:

		CollisionManager collisions;
		glm::vec2 positionsCol[4];
		std::vector<AICarPhysics> aiCarArray;
		std::vector<glm::vec2> *aiPathRef;
		bool playerOn = false;
		AIPhysics(){collisions.InitStructures(GetPathToAsset("models/circuit_col/colisions.txt"));}
		~AIPhysics() = default;
		void AddPath(std::vector<glm::vec2> *pathR) { aiPathRef = pathR; }
		void AddAICar(Transform *playerRef, Transform *transformRef, float maxSpeed, float maxSteerForce, float mass = 800.0f) {
			AICarPhysics aiCar = { playerRef, transformRef, {}, maxSpeed, maxSteerForce, mass, 0 };
			aiCarArray.push_back(aiCar);
		}
		bool carColCalc(glm::vec3 a, glm::vec3 b);
		void Update(float deltaTime);
		void Reset() { for (auto &aiCar : aiCarArray) { aiCar.speed = {}; aiCar.curPathNode = 0; } }
	};

}
