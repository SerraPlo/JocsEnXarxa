#pragma once

//Third-party libraries
#define GLM_FORCE_RADIANS
#include <glm.hpp>			//OpenGL Mathematics 
#include <iostream>
#include <time.h>
#include "GameConstants.h"
#include "SDLInterface.h"
#include "InputManager.h"
#include "Sprite.h"
#include "ListOfMonsters.h"


/*
* The Game class manages the game execution
*/
class Game {
	public:						
		Game(std::string windowTitle, int screenWidth, int screenHeight);	//Constructor
		~Game();															//Destructor
		void run();															//Game execution	

	private:
			//Attributes	
		std::string _windowTitle;		//SDLInterface Title
		int _screenWidth;				//Screen width in pixels				
		int _screenHeight;				//Screen height in pixels				
		GameState _gameState;			//It describes the game state				
		SDLInterface _graphic;			//Manage the SDL graphic library		
		ListOfMonsters _monsters;		//It manages the data regarding to the monsters
		int _lastTimeMonsterWasUpdated;				//Counter to compute the time
		
			//Internal methods for the game execution
		void init();
		void gameLoop();		
		void executePlayerCommands();
		void doPhysics();
		void renderGame();
		void drawMenu();
		void drawGame();
		void drawSprite(Sprite & e);		
};

