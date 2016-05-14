#pragma once
#include "Transform.h"
#include <iostream>
#include <glm/glm.hpp>
#define _USE_MATH_DEFINES
#include <cmath>

#define BRAKE_FORCE 12000.0f

struct Config {
	float gravity = 9.81f;   // m/s^2
	float mass = 1200.0f; // kg
	float inertiaScale = 1.0f;    // Multiply by mass for inertia
	float halfWidth = 0.8f;    // Centre to side of chassis (metres)
	float cgToFront = 2.0f;    // Centre of gravity to front of chassis (metres)
	float cgToRear = 2.0f;    // Centre of gravity to rear of chassis
	float cgToFrontAxle = 1.25f;   // Centre gravity to front axle
	float cgToRearAxle = 1.25f;   // Centre gravity to rear axle
	float cgHeight = 0.55f;   // Centre gravity height
	float wheelRadius = 0.3f;    // Includes tire (also represents height of axle)
	float wheelWidth = 0.2f;    // Used for render only
	float tireGrip = 2.0f;    // How much grip tires have
	float lockGrip = 0.7f;    // % of grip available when wheel is locked
	float engineForce = 8000.0;
	float eBrakeForce = BRAKE_FORCE / 2.5f;
	float weightTransfer = 0.2f;    // How much weight is transferred during acceleration/braking
	float maxSteer = 0.6f;    // Maximum steering angle in radians
	float cornerStiffnessFront = 5.0f;
	float cornerStiffnessRear = 5.2f;
	float airResist = 2.5f;   // air resistance (* vel)
	float rollResist = 8.0f;   // rolling resistance force (* vel)
};

class CarPhysics
{
public:
	Transform *transform{ nullptr };
	glm::vec3 front;
	glm::vec3 velocity_c;
	glm::vec3 velocity_w;
	glm::vec3 accel;		// acceleration in world coords
	glm::vec3 accel_c;		// accleration in local car coords
	
	float absVel = 0.0;		// absolute velocity m/s
	float yawRate = 0.0;	// angular velocity in radians
	float steer = 0.0;		// amount of steering input (-1.0..1.0)
	float steerAngle = 0.0; // actual front wheel steer angle (-maxSteer..maxSteer)

	//  Use input smoothing (on by default)
	bool smoothSteer = true;
	//  Use safe steering (angle limited by speed)
	bool safeSteer = true;

	float inertia = 0.0;	// will be = mass
	float wheelBase = 0.0;  // set from axle to CG lengths
	float axleWeightRatioFront = 0.0;	// % car weight on the front axle
	float axleWeightRatioRear = 0.0;	// % car weight on the rear axle
	
	Config cfg;
	int throttle = 0;
	int brake = 0;
	int ebrake = 0;
	
	
	CarPhysics() {
		glm::vec3 front = glm::vec3(0.0f,0.0f,0.0f);
		glm::vec3 velocity_c = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 velocity_w = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 accel = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 accel_c = glm::vec3(0.0f, 0.0f, 0.0f);
		inertia = cfg.mass * cfg.inertiaScale;
		wheelBase = cfg.cgToFrontAxle + cfg.cgToRearAxle;
		axleWeightRatioFront = cfg.cgToRearAxle / wheelBase; // % car weight on the front axle
		axleWeightRatioRear = cfg.cgToFrontAxle / wheelBase;
	}
	~CarPhysics() = default;
	void AddTransform(Transform* _transform) {
		transform = _transform;
	}


	float max(float a, float b) { return(a >= b) ? a : b; }
	float min(float a, float b) { return(a <= b) ? a : b; }
	float sign(float n) { return n = (n < 0.0f) ? -1.0f : 1.0f;}
	float clamp(float n, float _min, float _max){ return min(max(n, _min), _max);}

	
	float applySmoothSteer(float steerInput, float dt){
		//float steer = 0.0f;

		if (abs(steerInput) > 0.001f) {	//  Move toward steering input
			steer = clamp(steer + steerInput * dt * 2.0f, -1.0f, 1.0f); // -inp.right, inp.left);
		}
		else {	//  No steer input - move toward centre (0)
			if (steer > 0.0f) steer = max(steer - dt * 1.0f, 0.0f);
			else if (steer < 0.0f)	steer = min(steer + dt * 1.0f, 0.0f);
		}
		return steer;
	};
	float applySafeSteer(float steerInput){
		float avel = min(absVel, 250.0f);  // m/s
		float steer = steerInput * (1.0f - (avel / 280.0f));
		return steer;
	};
	void processInput(bool arrayK[5], float dt) {
		throttle = arrayK[0]?1:0;
		brake = arrayK[1]?1:0;
		ebrake = /*arrayK[4]? 1 :*/ 0;
		float steerInput = 0.0f;
		if(arrayK[2]) steerInput += -1.0f;
		if(arrayK[3]) steerInput += 1.0f;
		//  Perform filtering on steering...
		if (smoothSteer) steer = applySmoothSteer(steerInput, dt);
		else steer = steerInput;
		if (safeSteer)	steer = applySafeSteer(steer);
		//  Now set the actual steering angle
		steerAngle = steer * cfg.maxSteer;
		std::cout << "steer: " << steer << "/ steerAngle: " << (steerAngle*180.0f) / M_PI << std::endl;
	}
	void doPhysics(float deltaTime) {
		front = glm::vec3(sin((transform->rotation.y*M_PI) / 180), 0.0f, cos((transform->rotation.y*M_PI) / 180));
		//float deltaTime = dt / 1000.0;  // delta T in seconds

		float sn = float(sin((transform->rotation.y*M_PI) / 180.0f)); float cs = float(cos((transform->rotation.y*M_PI) / 180.0f));// Pre-calc heading vector

		velocity_c =glm::vec3(cs * velocity_w.x + sn * velocity_w.z, 0.0f, cs * velocity_w.z - sn * velocity_w.x);// Get velocity in local car coordinates
		
		// Weight on axles based on centre of gravity and weight shift due to forward/reverse acceleration
		float axleWeightFront = cfg.mass * (axleWeightRatioFront * cfg.gravity - cfg.weightTransfer * accel_c.x * cfg.cgHeight / wheelBase);
		float axleWeightRear = cfg.mass * (axleWeightRatioRear * cfg.gravity + cfg.weightTransfer * accel_c.x * cfg.cgHeight / wheelBase);

		// Resulting velocity of the wheels as result of the yaw rate of the car body.
		// v = yawrate * r where r is distance from axle to CG and yawRate (angular velocity) in rad/s.
		float yawSpeedFront = cfg.cgToFrontAxle * yawRate;
		float yawSpeedRear = -cfg.cgToRearAxle * yawRate;

		// Calculate slip angles for front and rear wheels (a.k.a. alpha)
		float slipAngleFront = atan2(velocity_c.z + yawSpeedFront, abs(velocity_c.x)) - sign(velocity_c.x) * steerAngle;
		float slipAngleRear = atan2(velocity_c.z + yawSpeedRear, abs(velocity_c.x));

		float tireGripFront = cfg.tireGrip;
		float tireGripRear = cfg.tireGrip * (1.0f - ebrake * (1.0f - cfg.lockGrip)); // reduce rear grip when ebrake is on

		float frictionForceFront_cy = clamp(-cfg.cornerStiffnessFront * slipAngleFront, -tireGripFront, tireGripFront) * axleWeightFront;
		float frictionForceRear_cy = clamp(-cfg.cornerStiffnessRear * slipAngleRear, -tireGripRear, tireGripRear) * axleWeightRear;

		//  Get amount of brake/throttle from our inputs
		float _brake = min(brake * BRAKE_FORCE + ebrake * cfg.eBrakeForce, BRAKE_FORCE);
		float _throttle = throttle * cfg.engineForce;

		//  Resulting force in local car coordinates.
		//  This is implemented as a RWD car only.
		float tractionForce_cx = _throttle - _brake * sign(velocity_c.x);
		float tractionForce_cy = 0;

		float dragForce_cx = -cfg.rollResist * velocity_c.x - cfg.airResist * velocity_c.x * abs(velocity_c.x);
		float dragForce_cy = -cfg.rollResist * velocity_c.z - cfg.airResist * velocity_c.z * abs(velocity_c.z);

		// total force in car coordinates
		float totalForce_cx = dragForce_cx + tractionForce_cx;
		float totalForce_cy = dragForce_cy + tractionForce_cy + cos(steerAngle) * frictionForceFront_cy + frictionForceRear_cy;

		// acceleration along car axes
		accel_c.x = totalForce_cx / cfg.mass;  // forward/reverse accel
		accel_c.z = totalForce_cy / cfg.mass;  // sideways accel

		// acceleration in world coordinates
		accel.x = cs * accel_c.x - sn * accel_c.z;
		accel.z = sn * accel_c.x + cs * accel_c.z;

		// update velocity
		velocity_w.x += accel.x * deltaTime;
		velocity_w.z += accel.z * deltaTime;

		absVel = glm::length(velocity_w);

		// calculate rotational forces
		float angularTorque = (frictionForceFront_cy + tractionForce_cy) * cfg.cgToFrontAxle - frictionForceRear_cy * cfg.cgToRearAxle;

		//  Sim gets unstable at very slow speeds, so just stop the car
		if (abs(absVel) < 0.5 && throttle==0){
			velocity_w.x = velocity_w.z = absVel = 0.0f;
			angularTorque = yawRate = 0.0f;
		}

		float angularAccel = angularTorque / inertia;

		yawRate += angularAccel * deltaTime;

		transform->rotation.y += float(-yawRate*180.0f /M_PI) * deltaTime;

		//  finally we can update position
		transform->position.x += velocity_w.z * deltaTime;
		transform->position.y = 0.0f;
		transform->position.z += velocity_w.x * deltaTime;
	}


	void Update(bool arrayK[5], float deltaTime) {
		processInput(arrayK, deltaTime);
		doPhysics(deltaTime);
		//std::cout << "position:[" << transform->position.x << "," << transform->position.y << "," << transform->position.z << "] -> speed:"<< absVel << std::endl;
		//std::cout << "throttle: " << throttle << ", brake: " << brake << ", steerAngle: " << steerAngle << ", velocity_c.x: " << velocity_c.x << ", velocity_c.z: " << velocity_c.z << std::endl;
	}
};

