#ifndef MAP
#define MAP

#include "Chunks/Chunk.h"

using namespace sf;

class Map
{
private:
	void init();
	void initMapGenerator();
	void initChunks(MapGenerator* map);
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

	void draw(RenderTarget* window);
	void update(const float& dt, const float& multiplier);
};

#endif
