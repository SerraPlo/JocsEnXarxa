#pragma once
#include "GameObject.h"
#include "GLCamera.h"

namespace SerraPlo {

#define MAX_POWERUPS 1
#define LIFETIME_DELAY 4000

	class ShaderProgram;

	struct PowerUp : GameObject {
		float lifeTimeCounter = 0;
		virtual void Init(glm::vec3 *pos, glm::vec3 *front) = 0;
		virtual void Activate(void) = 0;
		virtual void Update(float dt) = 0;
		virtual void Draw(ShaderProgram & program, GLCamera & camera) = 0;
		virtual ~PowerUp() = default;
	};

#define GREEN_SHELL_STUN_DISTANCE 5.0f
#define GREEN_SHELL_STUN_DELAY 1000
#define RED_SHELL_STUN_DISTANCE 5.0f
#define RED_SHELL_STUN_DELAY 1000

	struct GreenShell : PowerUp {
		glm::vec3 *carPos;
		glm::vec3 *carFront;
		glm::vec3 front;
		float speed = 70.0f; ///TODO: change to player car trully speed
		void Init(glm::vec3 *pos, glm::vec3 *front) override;
		void Activate(void) override;
		void Update(float dt) override;
		void Draw(ShaderProgram & program, GLCamera & camera) override;
	};
	struct RedShell : PowerUp {
		glm::vec3 *carPos;
		glm::vec3 *carFront;
		glm::vec3 front;
		float speed = 70.0f; ///TODO: change to player car trully speed
		void Init(glm::vec3 *pos, glm::vec3 *front) override;
		void Activate(void) override;
		void Update(float dt) override;
		void Draw(ShaderProgram & program, GLCamera & camera) override;
	};

}
