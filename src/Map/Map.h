#ifndef MAP
#define MAP

#include "MapGenerator/MapGenerator.h"
#include "Chunks/Chunk.h"
#include "Entity/AllEntities.hpp"

using namespace sf;

class Map
{
private:
	void init();
	void initMapGenerator();
	void initChunks();
	void spawnNatureObj(const IntRect &type, const int& x, const int& y);
	unsigned int seed;

	RenderTexture renderTexture;
	Sprite sprite;

	std::vector<std::vector<Nature*>> interactableGrid;
	std::vector<std::vector<MapGenerator::TerrainType>> drawVector;

	//std::vector<std::vector<DynamicTile*> dynamicTiles;
	//Dynamic tiles goes on three different textures depending on their frame
	//One of the three textures are drawn on top of the main texture
public:
	Vector2i chunkSize;
	MapGenerator* map;
	std::vector<std::vector<Tile*>> grid;
	std::vector<Object*>* entitesPtr;
	std::vector<std::vector<Chunk*>> chunks;
	Vector2i textureSize;

	Map(std::vector<Object*>* entites);
	~Map();

	//Functions
	std::pair<Tile*, Tile::Parts*> getCellInfo(const size_t& x, const size_t& y);
	std::vector<std::vector<std::pair<Vector2i*, Vector2i>>> getNeighboursInfo(Ground::Parts* const parts, const size_t& x, const size_t& y);
	void updateTexture();

	void buildNature(unsigned int seed);
	void addNature();
	void drawNature(RenderTarget* texture);
	void draw(RenderTarget* window);
	void update(const float& dt, const float& multiplier);
};

#endif
