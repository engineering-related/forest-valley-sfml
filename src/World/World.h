#ifndef WORLD
#define WORLD

#include "Utility/threads/ThreadPool.h"
#include "Chunks/Chunk.h"

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
	void updatePlayerChunks();
	void drawTilesPlayerChunks(RenderTarget* window);
	void* updateMiniMap();
	static void* minimapHelper(void *context)
	{
		return ((World *)context)->updateMiniMap();
	}

	void sortZindex();
	void checkTileColision(Object* object, Chunk* chunk);

	//World data
	unsigned int seed;
	Vector2i tileAmount;
	Vector2i chunkAmount;
	Vector2i pixelSize;

	//Player
	Player* player;
	Vector2i playerChunkPos;
	Vector2i oldPlayerChunkPos;

	//Loading and saving threads
	const size_t numThreads = 9; //All surronding chunks
	ThreadPool* pool;
	pthread_t minimap_thread;
	pthread_mutex_t mutex;

	//World generation
	WorldGenerator* map;

	//ALL Entities
	std::vector<Object*> entites;

	//Chunks
	inline size_t chunkPosKey(int i,int j) {return (size_t) i << 32 | (unsigned int) j;}
	std::unordered_map<size_t /*chunkPosKey*/, Chunk* /*Chunk obj.*/> chunks;

public:
	//Accessors
	inline Player* getPlayer(){return this->player; }
	inline WorldGenerator* getMap(){return this->map; }

	//Modifiers

	//Fucntions
	World(const unsigned int seed);
	~World();

	void draw(RenderTarget* window);
	void update(const float& dt, const float& multiplier);
};

#endif
