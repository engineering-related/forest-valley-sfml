#ifndef WORLD
#define WORLD

#include "Chunks/Chunk.h"

using namespace sf;

class World
{
private:
	void init();
	void initWorldGenerator();

	void initChunks(WorldGenerator* map);
	void updateMiniMap();
	void savePlayerChunks();
	void loadPlayerChunks();
	void updatePlayerChunks(Player* player);
	void drawTilesPlayerChunks(RenderTarget* window);

	unsigned int seed;

	Vector2i playerChunkPos;
	Vector2i oldPlayerChunkPos;

	std::vector<Object*>* entitesPtr;
	//std::vector<std::vector<DynamicTile*> dynamicTiles;
	//Dynamic tiles goes on three different textures depending on their frame
	//One of the three textures are drawn on top of the main texture
public:
	Vector2i tileAmount;
	Vector2i chunkAmount;
	Vector2i pixelSize;
	WorldGenerator* map;

	std::vector<std::vector<Chunk*>> chunks;

	World(const unsigned int seed, std::vector<Object*>* entitesPtr);
	~World();

	void draw(RenderTarget* window);
	void update(Player* player);
};

#endif
