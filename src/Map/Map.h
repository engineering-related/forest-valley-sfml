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
	void initChunks(MapGenerator* map);
	void spawnNatureObj(const IntRect &type, const int& x, const int& y, Chunk* chunk);
	unsigned int seed;

	//std::vector<std::vector<DynamicTile*> dynamicTiles;
	//Dynamic tiles goes on three different textures depending on their frame
	//One of the three textures are drawn on top of the main texture
public:
	Vector2i tileAmount;
	Vector2i chunkAmount;
	Vector2i pixelSize;
	MapGenerator* map;

	std::vector<Object*>* entitesPtr;
	std::vector<std::vector<Chunk*>> chunks;

	Map(std::vector<Object*>* entites);
	~Map();

	//Functions
	static std::pair<Tile*, Tile::Parts*> getCellInfo(const size_t& x, const size_t& y, Chunk* chunk);
	static std::vector<std::vector<std::pair<Vector2i*, Vector2i>>> getNeighboursInfo(Ground::Parts* const parts, const size_t& x, const size_t& y, Chunk* chunk);

	void updateTexture(Chunk* chunk);

	void buildNature(unsigned int seed, Chunk* chunk);
	void draw(RenderTarget* window);
	void update(const float& dt, const float& multiplier);
};

#endif
