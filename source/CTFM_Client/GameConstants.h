#pragma once

//Game general information
#define GAME_SPEED 0.1f
#define GAME_TEXTURES 2
#define MAX_MONSTERS 10
#define MONSTER_REFRESH_FREQUENCY 500

//Sprite information
#define SPRITE_SPEED 0.01f
#define SPRITE_DEFAULT_WIDTH 64
#define SPRITE_DEFAULT_HEIGHT 64
#define SPRITE_HERO 0
#define SPRITE_FIRE 1

//Color information
#define GAME_BASIC_COLORS 5
#define RED 0
#define GREEN 1
#define BLUE 2
#define BLACK 3
#define WHITE 4


//Game has four possible states: INIT (Preparing environment), PLAY (Playing), EXIT (Exit from the game) or MENU (Game menu)
enum class GameState{ INIT, PLAY, EXIT, MENU };

