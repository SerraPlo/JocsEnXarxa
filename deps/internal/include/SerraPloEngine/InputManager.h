#pragma once
#include <glm/glm.hpp>
#include <unordered_map>

namespace SerraPlo {

class InputManager
{
	glm::vec2 m_mouseCoords;
	std::unordered_map<unsigned int, bool> m_keyMap;
	std::unordered_map<unsigned int, bool> m_prevKeyMap;
public:
	explicit InputManager();
	~InputManager() = default;
	void update();

	void pressKey(unsigned keyID);
	void releaseKey(unsigned keyID);
	bool isKeyDown(unsigned keyID);
	static bool isKeyDown(const std::unordered_map<unsigned int, bool> &keyMap, unsigned keyID);
	bool isKeyPressed(unsigned keyID);

	void setMouseCoords(float x, float y);

	glm::vec2 getMouseCoords() const { return m_mouseCoords; };
};

}


