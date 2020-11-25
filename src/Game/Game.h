#ifndef GAME
#define GAME

//Includes
#include "Camera/Camera.h"
#include "World/World.h"
#include "Platform/Platform.hpp"

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

	unsigned int seed;
	World* world;

	void init();
	void initWorld();
	void handleInput();
	void onResize();
	void pollEvents();
	void updateClock();
	void printInfoToWindow();

	void startLoop();

public:

	Game();
	~Game();
	void start();
};

#endif
