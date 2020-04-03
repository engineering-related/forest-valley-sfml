#ifndef GAME
#define GAME

//Includes
#include "MapGenerator/MapGenerator.h"
#include "Platform/Platform.hpp"
#include "Entity/AllEntities.hpp"

using namespace sf;

class Game
{
private:
	RenderWindow* window;
	Clock clock;
	Font* font;

	float FPS;
	float dt;
	float incer;
	float multiplier;
	float framerateLimit;
	float cap;

	//std::vector<Object*> entites;
	//std::vector<std::vector<Tile*>> grid
	MapGenerator* map;
	unsigned int seed;

	void init();
	void pollEvents();
	void updateClock();
	void printFPS();
	void startLoop();

public:

	Game();
	~Game();
	void start();
};

#endif
