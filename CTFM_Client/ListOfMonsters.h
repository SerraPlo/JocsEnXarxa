#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <string>
#include "Sprite.h"
#include "GameConstants.h"


class ListOfMonsters
{
	Sprite *_arrayMonsters = 0;
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
	void checkIfSomeMonsterDies();	
};

