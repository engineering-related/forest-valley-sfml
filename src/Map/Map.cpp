#include "Map.h"

Map::Map(const unsigned int seed)
{
	this->seed = seed;
	this->init();
}

Map::~Map()
{
	//Delete chunks
	for (int x = 0; x < this->chunkAmount.x; ++x)
	{
		for (int y = 0; y < this->chunkAmount.y; ++y)
		{
			delete this->chunks[x][y];
		}
	}

	//Delete MapGenerator
	delete this->map;
}

void Map::init()
{
	this->initMapGenerator();
	this->initChunks(this->map);
}

void Map::initMapGenerator()
{
	this->chunkAmount = Vector2i(20, 20);
	this->tileAmount = Vector2i(CHUNK_SIZE.x * this->chunkAmount.x, CHUNK_SIZE.y * this->chunkAmount.y);
	this->pixelSize = Vector2i(this->tileAmount.x * TILE_SIZE.x, this->tileAmount.y * TILE_SIZE.y);
	this->map = new MapGenerator(this->seed, this->tileAmount, 40, 5, 0.5, 2, Vector2f(0, 0), 1);
	this->map->setDisplaySize(Vector2f(WINDOW_WIDTH/4, WINDOW_WIDTH/4));
	this->map->setConstDraw(true);
}

void Map::initChunks(MapGenerator* map)
{
	this->chunks = std::vector<std::vector<Chunk*>>(this->chunkAmount.x, std::vector<Chunk*>(this->chunkAmount.y, nullptr));
	for (int x = 0; x < this->chunkAmount.x; x++)
	{
		for (int y = 0; y < this->chunkAmount.y; y++)
		{
			this->chunks[x][y] = new Chunk(Vector2i(x, y), this->map);
		}
	}
}

void Map::draw(RenderTarget * window)
{

}

void Map::update(const float& dt, const float& multiplier)
{

}