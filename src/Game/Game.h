#ifndef GAME
#define GAME

//Includes
#include "Camera/Camera.h"
#include "Map/Map.h"
#include "Platform/Platform.hpp"
#include "Entity/AllEntities.hpp"

using namespace sf;

class Game
{
private:
	RenderWindow* window;
	Camera* camera;
	Clock clock;
	Font* font;

	float FPS;
	float dt;
	float incer;
	float multiplier;
	float framerateLimit;
	float cap;

	Player* player;
	std::vector<Object*> entites;

	unsigned int seed;
	Map* map;

	void init();
	void initMap();
	void pollEvents();
	void updateClock();
	void printFPS();
	void sortZindex();
	void checkColision(Object* Object);
	void startLoop();

public:

	Game();
	~Game();
	void start();
};

#endif
