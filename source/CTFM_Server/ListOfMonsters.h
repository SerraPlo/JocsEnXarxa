#pragma once
#include <iostream>
#include <glm.hpp>
#include <string>
#include <GameConstants.h>
#include <vector>

class ListOfMonsters
{
	std::vector<std::pair<int, int>> monsters;
	int _numMonsters;
	glm::ivec2 _mouseCoordinates;
	void removeMonster(int id);
public:
	ListOfMonsters(int n);
	~ListOfMonsters();
		//Getters and Setters
	void setMouseCoordinates(glm::ivec2 mousePositon);
	int getNumMonsters();
	Sprite& getMonster(int id);
	Sprite& operator[](int p_index);
		//Functions
	void initMonsters(int screenWidth, int screenHeight);
	void setRandomPosition(int screenWidth, int screenHeight);
	void nextAnimationFrame(int curTicks);
	bool checkIfSomeMonsterDies(int x, int y);	
};

