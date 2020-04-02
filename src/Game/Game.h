#ifndef GAME
#define GAME

//Includes
#include "MapGenerator/MapGenerator.h"
#include "Platform/Platform.hpp"

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
	MapGenerator* map;
	unsigned int seed;

	void pollEvents();
	void updateClock();
	void printFPS();
	void startLoop();

public:
	void init();
	Game();
	~Game();
	void start();
};

#endif
