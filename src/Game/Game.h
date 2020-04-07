#ifndef GAME
#define GAME

//Includes
#include "Map/Map.h"
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

	std::vector<Object*> entites;

	unsigned int seed;
	Map* map;

	void init();
	void initMap();
	static void zoomViewAt(const sf::Vector2i& pixel, sf::RenderWindow& window, const float& zoom);
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
