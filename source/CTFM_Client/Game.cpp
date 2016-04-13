#include "Game.h"
/**
* Constructor
* Tip: You can use an initialization list to set its attributes
* @param windowTitle is the window title
* @param screenWidth is the window width
* @param screenHeight is the window height
*/
Game::Game(std::string windowTitle, int screenWidth, int screenHeight, SocketAddress sA) :
	_windowTitle(windowTitle),
	_screenWidth(screenWidth),
	_screenHeight(screenHeight),
	_gameState(GameState::INIT),
	_monsters(MAX_MONSTERS),
	_server(sA){
	_id = -1;
}

Game::~Game(){
}

bool ProcessFirstMsg(const std::string &data, SocketAddress &from, UDPSocket &_sock, int &id) {
	auto pos = data.find_last_of('_');
	std::string key = data.substr(0, pos);
	std::string msg = data.substr(pos + 1, pos + 2 /*data.size() - 1*/);
	if (strcmp(key.c_str(), "WELCOME") == 0) {
		id = atoi(msg.c_str());
		return true;
	}
	return false;
}

bool ProcessMsg(const std::string &data, UDPSocket &_sock, GameState &_gameState, ListOfMonsters &_monsters, int &time, int &_nPlayers, std::string &_score) {
	auto pos = data.find_last_of('_');
	std::string key = data.substr(0, pos);
	std::string msg = data.substr(pos + 1, data.size() - 1);
	if (strcmp(key.c_str(), "TIME") == 0) {
		time = atoi(msg.c_str());
		return true;
	}
	if (strcmp(key.c_str(), "MONSTERS") == 0) {
		auto pos = msg.find_first_of('#');
		std::string nMonsters = msg.substr(0, pos);
		std::string positions = msg.substr(pos + 1, msg.size() - 1);
		std::cout << "nMons    : " << _monsters.getNumMonsters() << std::endl;
		std::cout << "new nMons: " << atoi(nMonsters.c_str()) << std::endl;
		for (int i = 0; i < _monsters.getNumMonsters()-(atoi(nMonsters.c_str())); i++) {
			std::cout << "eliminat <-----------------------------------" << std::endl;
			_monsters.removeMonster(0);
		}

		for (int i = 0; i < (atoi(nMonsters.c_str())); i++) {
			auto pos = positions.find_first_of('#');
			std::string actualPos = positions.substr(0, pos);
			std::string temp = positions;
			positions = temp.substr(pos + 1, temp.size() - 1);
			
			auto pos2 = actualPos.find_first_of(':');
			int x = atoi(actualPos.substr(0, pos2).c_str());
			int y = atoi(actualPos.substr(pos2 + 1, actualPos.size() - 1).c_str());

			std::cout << i << "->" << x << ", " << y << std::endl;

			_monsters.setPosition(i, x, y);
		}

		return true;
	}
	if (strcmp(key.c_str(), "SCORES") == 0) {
		auto pos = msg.find_first_of('#');
		int nPlayers = atoi(msg.substr(0, pos).c_str());
		msg = msg.substr(pos + 1, msg.size()-1);
		std::string temp = "";
		for (int i = 0; i < nPlayers; i++) {
			pos = msg.find_first_of('#');
			temp += msg.substr(0, pos) + '\n';
			msg = msg.substr(pos + 1, msg.size() - 1);
		}
		_nPlayers = nPlayers;
		_score = temp;
		return true;
	}
	if (strcmp(key.c_str(), "FIN") == 0) {
		_gameState = GameState::EXIT;
		return true;
	}
	return false;
}

struct threadParam{
	UDPSocket &_socket; GameState &_gameState; ListOfMonsters &_monsters; int &_time; int &_nPlayers; std::string &_score;
};

void Listen(threadParam &a) {
	SocketAddress from;
	while (true) {
		std::string data;
		a._socket.ReceiveFrom(data, from);
		std::cout << data << std::endl;
		ProcessMsg(data, a._socket, a._gameState, a._monsters, a._time, a._nPlayers, a._score);
	}
}

void Game::run() {
	init();
	while (_id < 0) {
		std::string hi = "HELLO_";
		_socket.SendTo(hi, _server);
		std::string data;
		_socket.ReceiveFrom(data, _server);
		ProcessFirstMsg(data, _server, _socket, _id);
		//std::cout << "id ->" << _id << std::endl;
	}
	std::cout << "id-> " << _id << std::endl;
	threadParam temp = { _socket, _gameState, _monsters, _time,_nPlayers,_score};
	std::thread orejaThread(Listen, temp);
	//Start the game if everything is ready
	gameLoop();
	orejaThread.join();
}

//Initialize all the components
void Game::init() {
	srand((unsigned int)time(NULL));
	_graphic.createWindow(_windowTitle, _screenWidth, _screenHeight, false);	
	_graphic.setWindowBackgroundColor(255, 255, 255, 255);
	_graphic.loadTexture(SPRITE_FIRE, _graphic.GetAssetsPath("images/characters/fireSprite.png"));
	_graphic.setFontStyle(TTF_STYLE_NORMAL);
	_monsters.initMonsters(_screenWidth, _screenHeight);
	_lastTimeMonsterWasUpdated = clock();
}


//Game execution: Gets input events, processes game logic and draws sprites on the screen
void Game::gameLoop() {	
	_gameState = GameState::PLAY;
	while (_gameState != GameState::EXIT) {		
		_graphic.detectInputEvents();
		executePlayerCommands();
		//doPhysics();
		renderGame();			
	}
}

/*
* Executes the actions sent by the user by means of the keyboard and mouse
* Reserved keys:
- up | left | right | down moves the player object
- m opens the menu
*/
void Game::executePlayerCommands() {
	if (_graphic.isKeyPressed(SDL_BUTTON_LEFT) && _monsters.getNumMonsters() > 0){
		glm::ivec2 mouseCoords = _graphic.getMouseCoords();
		_monsters.setMouseCoordinates(mouseCoords);
		_socket.SendTo("CLICK_"+std::to_string(_id)+"#"+std::to_string(mouseCoords.x) + ":" + std::to_string(mouseCoords.y), _server);
	}
	if (_graphic.isKeyPressed(SDLK_ESCAPE)) {
		_gameState = GameState::EXIT;
	}
}

//Execute the game physics
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
	_graphic.clearWindow();

	if (_gameState == GameState::MENU) {
		drawMenu();
	}
	else {
		drawGame();
	}

	_graphic.refreshWindow();
}

//Draw the game menu
void Game::drawMenu() {

}

// Draw the game, winner or loser screen
void Game::drawGame()	{
		//Check if game is over
	if (_monsters.getNumMonsters() == 0) {		
			//Draw the score
		_graphic.drawText("WELL DONE!", WHITE, BLACK, _screenWidth / 2 - _screenWidth / 14, _screenHeight / 2 - _screenHeight / 6);
		_graphic.drawText("PLAYER " + std::to_string(_id), WHITE, BLACK, _screenWidth / 2 - _screenWidth / 14, _screenHeight / 2 - _screenHeight / 5);
		std::string temp;
		std::string copy = _score;
		for (int i = 0; i < _nPlayers; i++) {
			auto pos = copy.find_first_of('\n');
			temp = copy.substr(0, pos);
			copy = copy.substr(pos+1, copy.size()-1);
			_graphic.drawText(temp, WHITE, BLACK, _screenWidth / 20, _screenHeight / 3 + 30*i);
		}
		_graphic.drawText("Your score: " + std::to_string(_time) + " seconds", WHITE, BLACK, _screenWidth / 2 - _screenWidth / 8, _screenHeight / 2);
		_graphic.drawText("Press ESC to exit", WHITE, BLUE, _screenWidth / 2 - _screenWidth / 10, _screenHeight / 2 + _screenHeight / 4);

	}else {
			//Draw the list of monsters
		for (int i = 0; i < _monsters.getNumMonsters(); i++) {
			drawSprite(_monsters[i]);
		}
			//Draw the current time
		_graphic.drawText(std::to_string(_time), BLACK, WHITE, 0, 0);
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