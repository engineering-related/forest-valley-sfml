#ifndef MAP
#define MAP

#include "MapGenerator/MapGenerator.h"
#include "Entity/AllEntities.hpp"

using namespace sf;

class Map
{
private:
	void init();
	void initNature(unsigned int seed);
	void initMapGenerator();
	unsigned int seed;
	MapGenerator* map;

	RenderTexture renderTexture;
	Sprite sprite;

	std::vector<std::vector<Tile*>> grid;
	std::vector<std::vector<Nature*>> interactableGrid;
	std::vector<std::vector<MapGenerator::TerrainType>> drawVector;
	std::vector<std::vector<float>> natureMap;
	//std::vector<std::vector<DynamicTile*> dynamicTiles;
	//Dynamic tiles goes on three different textures depending on their frame
	//One of the three textures are drawn on top of the main texture

public:
	Map();
	~Map();

	//Functions
	//TODO: CHANGE Ground::Parts TO Tile::Parts so it can be used for others
	std::pair<Tile*, Tile::Parts*> getCellInfo(const size_t& x, const size_t& y);
	std::vector<std::vector<std::pair<Vector2i*, Vector2i>>> getNeighboursInfo(Ground::Parts* const parts, const size_t& x, const size_t& y);
	void updateTexture();

	void buildNature();
	void draw(RenderTarget* window);
	void update(const float& dt, const float& multiplier);
};

#endif
