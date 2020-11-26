#ifndef WORLD
#define WORLD

#include "Chunks/Chunk.h"
#include "Utility/threads/ThreadPool.h"

using namespace sf;

class World
{
private:
	void init();
	void initWorldGenerator();
	void initPlayer();
	int initThreads();

	void savePlayerChunks();
	void loadPlayerChunks();

	void* updatePlayerChunks();

	static void* updatePlayerChunksHelper(void *context)
	{
		return ((World *)context)->updatePlayerChunks();
	}
	void drawTilesPlayerChunks(RenderTarget* window);
	void updateMiniMap();

	void sortZindex();
	void checkTileColision(Object* object, Chunk* chunk);

	//World data
	unsigned int seed;
	Vector2i tileAmount;
	Vector2i chunkAmount;
	Vector2i pixelSize;

	//Player
	std::shared_ptr<Player> player;
	Vector2i playerChunkPos;
	Vector2i oldPlayerChunkPos;

	//Loading and saving threads
	pthread_t chunk_thread;
	pthread_mutex_t mutex;

	//World generation
	std::shared_ptr<WorldGenerator> map;

	//ALL Entities
	std::vector<std::shared_ptr<Object>> entites, entitiesUpdated;
	bool entitiesSwap = false;

	//Chunks & threads
	inline size_t chunkPosKey(int i,int j) {return (size_t) i << 32 | (unsigned int) j;}
	std::unordered_map<size_t /*chunkPosKey*/, std::shared_ptr<Chunk> /*Chunk obj.*/> chunks;

public:
	//Accessors
	inline std::shared_ptr<Player> getPlayer(){return this->player; }
	inline std::shared_ptr<WorldGenerator> getMap(){return this->map; }

	//Modifiers

	//Fucntions
	World(const unsigned int seed);
	~World();

	void draw(RenderTarget* window);
	void update(const float& dt, const float& multiplier);
};

#endif
