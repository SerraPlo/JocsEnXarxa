#include "ListOfMonsters.h"


/*
* Constructor
* @param n is the number of monsters to store
*/
ListOfMonsters::ListOfMonsters(int n) {
		//Allocate memory for the storing the N monsters
	_numMonsters = n;
	_arrayMonsters = new Sprite[_numMonsters];
	if (!_arrayMonsters)	throw std::exception("System was not able to allocate memory");

		//Initialize the information regarding to its sprite
	for (int i = 0; i < _numMonsters; i++) {
		_arrayMonsters[i].setInitialValues(0,0, SPRITE_FIRE, 0, 5);
	}
}

/*
* Destructor
*/
ListOfMonsters::~ListOfMonsters(){
	if (_arrayMonsters != 0) delete[] _arrayMonsters;
}

/*
* Store the mouse coordinates
* @param mouseCoords contains the(x, y) mouse coordinates
*/
void ListOfMonsters::setMouseCoordinates(glm::ivec2 mousePositon) {
	_mouseCoordinates = mousePositon;
}

/*
* Get the number of elements
* @return the number of stored elements
*/
int ListOfMonsters::getNumMonsters() { 
	return _numMonsters; 
}


/*
* Get a specified monster
* @param id identifies the monster
* @return a reference to the idTH monster, so, if user modifies it, the information of the data structure will be also modified
*/
Sprite& ListOfMonsters::getMonster(int id) {
	return _arrayMonsters[id]; 
};

/*
* Get a specified monster.This method is equivalent to getMonster();
* @param id identifies the monster
* @return a reference to the idTH monster, so, if user modifies it, the information of the data structure will be also modified
*/
Sprite& ListOfMonsters::operator[](int id) {
	if ((id < 0) && (id >= _numMonsters)) throw std::exception("Invalid access to the data structure, this id doesn't exist");
	return _arrayMonsters[id];
}

/*
* Initialize the position of the monsters. In this case, they are randomly initialized
* @param screenWidth is the window width
* @param screenHeight is the window height
*/
void ListOfMonsters::initMonsters(int screenWidth, int screenHeight) {
	setRandomPosition(screenWidth, screenHeight);
}

/*
* Assign random positions to each monster
* @param screenWidth is the window width
* @param screenHeight is the window height
*/
void ListOfMonsters::setRandomPosition(int screenWidth, int screenHeight) {
	for (int i = 0; i < _numMonsters; i++) {
		_arrayMonsters[i].setPositionAtWorld(rand() % (int)(screenWidth*0.9f), rand() % (int)(screenHeight*0.9f));
	}
}

/*
* Update the frame of all monsters
* @param curTicks is the current tick
*/
void ListOfMonsters::nextAnimationFrame(int curTicks) {
	for (int i = 0; i < _numMonsters; i++) {
		_arrayMonsters[i].nextFrame((int)(curTicks * SPRITE_SPEED));
	}
}

/*
* Check if user has killed a monster
*/
void ListOfMonsters::checkIfSomeMonsterDies() {
	for (int i = 0; i < _numMonsters; i++) {
		if (_mouseCoordinates.x >= _arrayMonsters[i].getXAtWorld() && _mouseCoordinates.x <= _arrayMonsters[i].getXAtWorld() + SPRITE_DEFAULT_WIDTH &&
			_mouseCoordinates.y >= _arrayMonsters[i].getYAtWorld() && _mouseCoordinates.y <= _arrayMonsters[i].getYAtWorld() + SPRITE_DEFAULT_HEIGHT)
			removeMonster(i);
	}
}

/*
* Remove the monster. This remove version doesn't alter the order of the monsters
* @param id is the monster to delete
*/
void ListOfMonsters::removeMonster(int id) {
	if ((id<_numMonsters)&&(_numMonsters > 0)) {
		Sprite * tempArrayMonsters = 0;
		tempArrayMonsters = new Sprite[_numMonsters - 1];
		if (!tempArrayMonsters) throw std::exception("System was not able to allocate memory for resizing the internal data structure");
		for (int i = 0; i < _numMonsters - 1; i++) {
			if (i < id) 
				tempArrayMonsters[i] = _arrayMonsters[i];
			else 
				tempArrayMonsters[i] = _arrayMonsters[i + 1];
		}
		delete[] _arrayMonsters;
		_arrayMonsters = tempArrayMonsters;
		_numMonsters--;
		std::cout << std::to_string(MAX_MONSTERS-_numMonsters) + " monsters killed" << std::endl;
	}else 
		throw std::exception("The specified monster cannot be deleted because it doesn't exist");
}

