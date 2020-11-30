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
	void initChunks();
	int startUpdateChunks();
	bool chunkIsActive(const int& x, const int& y);

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
	bool updatingChunks = false;

	//World generation
	std::shared_ptr<WorldGenerator> map;

	//ALL Entities
	std::unordered_map<sf::Uint16, std::shared_ptr<Object>> entities, entitiesUpdated;
	std::vector<std::pair<sf::Uint16, std::shared_ptr<Object>>> entitiesVec;

	bool entitiesSwap = false;

	//Chunks
	Vector2i renderDistance; // Render-radius from players current chunk (exclusive)
							// Amount of chunks rendered: (renderdistance x 2) + 1
	inline size_t chunkPosKey(int i,int j) {return (size_t) i << 32 | (unsigned int) j;}
	std::unordered_map<size_t /*chunkPosKey*/, std::shared_ptr<Chunk> /*Chunk obj.*/> chunks;

public:
	//Players
	std::unordered_map<sf::Uint16 /*ENetID*/, std::shared_ptr<Player> /*Players*/> players;

	//Accessors
	inline std::shared_ptr<Player> getPlayer(){return this->player; }
	inline std::shared_ptr<WorldGenerator> getMap(){return this->map; }

	//Modifiers

	//Functions
	World(const unsigned int seed);
	~World();

	void initTestPlayer(const sf::Uint16 *ENetID);
	void updatePlayers(const float &dt);
	void drawPlayers(sf::RenderTarget* target);
	void addPlayer(sf::Uint16 pENetID, std::shared_ptr<Player> player);
	void removePlayer(const sf::Uint16  &pENetID);
	void changePlayerID(const sf::Uint16 &old_p_ENetID, const sf::Uint16 &new_p_ENetID);

	void draw(RenderTarget* window);
	void update(const float& dt, const float& multiplier);
};

#endif
