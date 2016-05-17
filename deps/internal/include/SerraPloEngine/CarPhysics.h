#pragma once
#include "Transform.h"
#include <iostream>
#include <glm/glm.hpp>
#define _USE_MATH_DEFINES
#include <cmath>
#include "ColManager.h"

#define BRAKE_FORCE 100000.0f

struct Config {
	float gravity = 9.81f;   // m/s^2
	float mass = 1200.0f; // kg
	float engineForce = 25000.0;
	float eBrakeForce = BRAKE_FORCE / 2.5f;
	float maxSteer = 1.0f;    // Maximum steering angle in radians
	float airResist = 5.5f;   // air resistance (* vel)
	float rollResist = 8.0f;   // rolling resistance force (* vel)
};

class CarPhysics
{
public:
	ColManager collisions;
	Transform *transform{ nullptr };
	glm::vec3 front;
	float accel = 0.0f;
	float velocity = 0.0f;
	bool smoothSteer = true;
	bool safeSteer = true;
	Config cfg;
	int throttle = 0;
	int brake = 0;
	int ebrake = 0;
	float steer = 0.0;
	float steerAngle = 0.0;

	CarPhysics() { glm::vec3 front = glm::vec3(0.0f, 0.0f, 0.0f); collisions.InitStructures("C:/Users/Pol/Documents/GitHub/JocsEnXarxa/assets/models/circuit_col/colisions.txt"); }

	~CarPhysics() = default;

	void AddTransform(Transform* _transform) { transform = _transform;}

	float max(float a, float b) { return(a >= b) ? a : b; }

	float min(float a, float b) { return(a <= b) ? a : b; }

	float sign(float n) { return n = (n < 0.0f) ? -1.0f : 1.0f;}

	float clamp(float n, float _min, float _max){ return min(max(n, _min), _max);}
	
	float applySmoothSteer(float steerInput, float dt){
		if (abs(steerInput) > 0.001f) {	//  Move toward steering input
			steer = clamp(steer + steerInput * dt * 3.0f, -1.0f, 1.0f); // -inp.right, inp.left);
		}else {//return to 0
			if (steer > 0.0f) steer = max(steer - dt * 3.0f, 0.0f);
			else if (steer < 0.0f)	steer = min(steer + dt * 3.0f, 0.0f);
		}return steer;
	};

	float applySafeSteer(float steerInput){
		float avel = min(velocity, 60.0f);  // m/s
		float steer = steerInput * (1.0f - (avel / 1260.0f));
		return steer;
	};

	void processInput(bool arrayK[5], float dt) {
		throttle = arrayK[0]?1:0;
		brake = arrayK[1]?1:0;
		ebrake = arrayK[4]?1:0;
		float steerInput = 0.0f;
		if(arrayK[2]) steerInput += -1.0f;
		if(arrayK[3]) steerInput += 1.0f;
		//  Perform filtering on steering...
		if (smoothSteer) steer = applySmoothSteer(steerInput, dt);
		else steer = steerInput;
		if (safeSteer)	steer = applySafeSteer(steer);
		//  Now set the actual steering angle
		steerAngle = steer * cfg.maxSteer;
		//std::cout << "steer: " << steer << "/ steerAngle: " << (steerAngle*180.0f) / M_PI << std::endl;
	}

	void doPhysics(float deltaTime) {
		front = glm::vec3(sin((transform->rotation.y*M_PI) / 180), 0.0f, cos((transform->rotation.y*M_PI) / 180));
		float sn = float(sin((transform->rotation.y*M_PI) / 180.0f)); float cs = float(cos((transform->rotation.y*M_PI) / 180.0f));

		float _brake = min(brake * BRAKE_FORCE + ebrake * cfg.eBrakeForce, BRAKE_FORCE);
		float _throttle = throttle * cfg.engineForce;
			
		float tractionForce = _throttle - _brake * sign(velocity);
		float dragForce = -cfg.rollResist * velocity -cfg.airResist * velocity * abs(velocity);

		float totalForce = dragForce + tractionForce;
		accel = totalForce / cfg.mass;  // forward/reverse accel

		velocity += accel * deltaTime;
			
		if (velocity < 0.5 && throttle == 0) velocity = 0.0f;

		if (velocity > 0.0f) transform->rotation.y += -((steerAngle*180.0f) / M_PI)*1.5f *deltaTime;
		transform->position += front* velocity *deltaTime;
		
		glm::vec2 front2 = glm::normalize(glm::vec2(front.x, front.z));
		glm::vec2 pFront2 = glm::vec2(-front2.y, front2.x);
		glm::vec2 positionsCol[4];
		positionsCol[0] = glm::vec2(transform->position.x, transform->position.z) + front2*2.0f + pFront2*1.25f;
		positionsCol[1] = glm::vec2(transform->position.x, transform->position.z) + front2*2.0f - pFront2*1.25f;
		positionsCol[2] = glm::vec2(transform->position.x, transform->position.z) - front2*2.0f + pFront2*1.25f;
		positionsCol[3] = glm::vec2(transform->position.x, transform->position.z) - front2*2.0f - pFront2*1.25f;
		collisions.CalculateCollision(positionsCol[0], positionsCol[1], positionsCol[2], positionsCol[3]);
		//std::cout << "velocity: " << velocity*3.6f/5 << "km/h" << std::endl;//escala mapa a tenir en compte (5 = creible)
	}

	void Update(bool arrayK[5], float deltaTime) {
		processInput(arrayK, deltaTime);
		doPhysics(deltaTime);
		//std::cout << "position:[" << transform->position.x << "," << transform->position.y << "," << transform->position.z << "] -> speed:"<< absVel << std::endl;
		//std::cout << "throttle: " << throttle << ", brake: " << brake << ", steerAngle: " << steerAngle << ", velocity_c.x: " << velocity_c.x << ", velocity_c.z: " << velocity_c.z << std::endl;
	}
};

