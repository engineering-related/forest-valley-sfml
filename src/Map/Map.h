#ifndef MAP
#define MAP

#include "MapGenerator/MapGenerator.h"
#include "Entity/AllEntities.hpp"

using namespace sf;

class Map
{
private:
	void init();
	void initMapGenerator();
	unsigned int seed;
	MapGenerator* map;

	RenderTexture renderTexture;
	Sprite sprite;

public:
	Map();
	~Map();

	//Functions
	void updateTexture();
	void draw(RenderTarget* window);
	void update(const float& dt, const float& multiplier);
};

#endif
