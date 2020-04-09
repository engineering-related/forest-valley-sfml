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

	std::vector<std::vector<Tile*>> grid;
	//std::vector<std::vector<DynamicTile*> dynamicTiles;
	//Dynamic tiles goes on three different textures depending on their frame
	//One of the three textures are drawn on top of the main texture

public:
	Map();
	~Map();

	//Functions
	//TODO: CHANGE Ground::Parts TO Tile::Parts so it can be used for others
	Vector2i* calcGroundType(Ground::Parts* const parts, const size_t& x, const size_t& y);
	Vector2i* assignType(const size_t& x, const size_t& y);
	void updateTexture();
	void draw(RenderTarget* window);
	void update(const float& dt, const float& multiplier);
};

#endif
