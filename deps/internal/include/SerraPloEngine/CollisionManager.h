#pragma once
#include <glm/glm.hpp>
#include <string>

namespace SerraPlo {

	struct Circle {
		glm::vec2 c;
		float r;
	};

	struct Box {
		glm::vec2 v[4];
		glm::vec2 axis[2];
		Circle ahorro;
	};

	class CollisionManager {
	public:
		Box* boxs;
		Circle* circles;
		int nBoxs;
		int nCircles;
		CollisionManager() = default;
		~CollisionManager() { delete[] boxs; delete[] circles;};
		void InitStructures(std::string path);	
		int CalculateCollision(glm::vec2 v[4]) const;
	};

}