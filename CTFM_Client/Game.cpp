#include "Game.h"
/**
* Constructor
* Tip: You can use an initialization list to set its attributes
* @param windowTitle is the window title
* @param screenWidth is the window width
* @param screenHeight is the window height
*/
Game::Game(std::string windowTitle, int screenWidth, int screenHeight) :
	_windowTitle(windowTitle),
	_screenWidth(screenWidth),
	_screenHeight(screenHeight),
	_gameState(GameState::INIT),
	_monsters(MAX_MONSTERS){
}

/**
* Destructor
*/
Game::~Game()
{
}

/*
* Game execution
*/
void Game::run() {
		//Prepare the game components
	init();
		//Start the game if everything is ready
	gameLoop();
}

/*
* Initialize all the components
*/
void Game::init() {
	srand((unsigned int)time(NULL));
		//Create a window
	_graphic.createWindow(_windowTitle, _screenWidth, _screenHeight, false);	
	_graphic.setWindowBackgroundColor(255, 255, 255, 255);
		//Load the sprites associated to the different game elements
	_graphic.loadTexture(SPRITE_FIRE, "../sharedResources/images/characters/fireSprite.png");
		//Set the font style
	_graphic.setFontStyle(TTF_STYLE_NORMAL);
		//Initialize the game elements
			//The different monsters
	_monsters.initMonsters(_screenWidth, _screenHeight);
			//The counter used to manage the score
	_lastTimeMonsterWasUpdated = clock();
}


/*
* Game execution: Gets input events, processes game logic and draws sprites on the screen
*/
void Game::gameLoop() {	
	_gameState = GameState::PLAY;

	while (_gameState != GameState::EXIT) {		
			//Detect keyboard and/or mouse events
		_graphic.detectInputEvents();
			//Execute the player commands 
		executePlayerCommands();
			//Update the game physics
		doPhysics();
			//Render game
		renderGame();			
	}
}

/**
* Executes the actions sent by the user by means of the keyboard and mouse
* Reserved keys:
- up | left | right | down moves the player object
- m opens the menu
*/
void Game::executePlayerCommands() {

	if (_graphic.isKeyPressed(SDL_BUTTON_LEFT) && _monsters.getNumMonsters() > 0){
		glm::ivec2 mouseCoords = _graphic.getMouseCoords();
		_monsters.setMouseCoordinates(mouseCoords);
	}

	if (_graphic.isKeyPressed(SDLK_ESCAPE)) {
		_gameState = GameState::EXIT;
	}
}

/*
* Execute the game physics
*/
void Game::doPhysics() {
		//Check if player has hit a/some monster/s
	_monsters.checkIfSomeMonsterDies();
	
	if (_monsters.getNumMonsters()!=0)	{
			//Update the animation of monsters
		_monsters.nextAnimationFrame(_graphic.getCurrentTicks());
			//Update the random positions based on the MONSTER_REFRESH_FREQUENCY
		if (clock() > _lastTimeMonsterWasUpdated + MONSTER_REFRESH_FREQUENCY) {
			_monsters.setRandomPosition(_screenWidth, _screenHeight);
			_lastTimeMonsterWasUpdated = clock();
		}
	}	
}

/**
* Render the game on the screen
*/
void Game::renderGame() {
		//Clear the screen
	_graphic.clearWindow();

		//Draw the screen's content based on the game state
	if (_gameState == GameState::MENU) {
		drawMenu();
	}
	else {
		drawGame();
	}
		//Refresh screen
	_graphic.refreshWindow();
}

/*
* Draw the game menu
*/
void Game::drawMenu() {

}

/*
* Draw the game, winner or loser screen
*/
void Game::drawGame()	{
		//Check if game is over
	if (_monsters.getNumMonsters() == 0) {		
			//Draw the score
		_graphic.drawText("WELL DONE!", WHITE, BLACK, _screenWidth / 2 - _screenWidth / 14, _screenHeight / 2 - _screenHeight / 6);		
		_graphic.drawText("Your score: " + std::to_string(_lastTimeMonsterWasUpdated / 1000) + " seconds", WHITE, BLACK, _screenWidth / 2 - _screenWidth / 8, _screenHeight / 2);
		_graphic.drawText("Press ESC to exit", WHITE, BLUE, _screenWidth / 2 - _screenWidth / 10, _screenHeight / 2 + _screenHeight / 4);

	}else {
			//Draw the list of monsters
		for (int i = 0; i < _monsters.getNumMonsters(); i++) {
			drawSprite(_monsters[i]);
		}
			//Draw the current time
		_graphic.drawText("Current time: " + std::to_string(clock() / 1000) + " seconds", BLACK, WHITE, 0, 0);
	}
}

/*
* Draw the sprite on the screen
* @param sprite is the sprite to be displayed
*/
void Game::drawSprite(Sprite & sprite) {
	_graphic.drawTexture(sprite.getSpriteId(), SPRITE_DEFAULT_WIDTH*sprite.getCurrentFrame(), 0, SPRITE_DEFAULT_WIDTH, SPRITE_DEFAULT_HEIGHT
		, sprite.getXAtWorld(), sprite.getYAtWorld(), SPRITE_DEFAULT_WIDTH, SPRITE_DEFAULT_HEIGHT);
}