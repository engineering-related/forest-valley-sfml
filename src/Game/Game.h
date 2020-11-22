#ifndef GAME
#define GAME

//Includes
#include "Camera/Camera.h"
#include "World/World.h"
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
	World* world;

	void init();
	void initWorld();
	void handleInput();
	void onResize();
	void pollEvents();
	void updateClock();
	void printInfoToWindow();
	/////EXTRACT TO SYSTEM CLASS////
	void sortZindex();
	void checkTileColision(Object* object, Chunk* chunk);
	/////////////////////////////////
	void startLoop();

public:

	Game();
	~Game();
	void start();
};

#endif
