#include "Map.h"

Map::Map(std::vector<Object*>* entites)
{
	this->entitesPtr = entites;
	this->init();
}

Map::~Map()
{
	delete this->map;

	//Delete objects
	/*for(size_t x = 0; x < this->grid.size(); x++)
	{
		for(size_t y = 0; y < this->grid[x].size(); y++)
		{
			delete this->grid[x][y];
		}
	}*/

	//Delete chunks
	for (int x = 0; x < this->chunkAmount.x; ++x)
	{
		for (int y = 0; y < this->chunkAmount.y; ++y)
		{
			delete this->chunks[x][y];
		}
	}
}

void Map::init()
{
	this->initMapGenerator();
	this->initChunks(this->map);

	for (int x = 0; x < this->chunkAmount.x; ++x)
	{
		for (int y = 0; y < this->chunkAmount.y; ++y)
		{
			this->chunks[x][y]->load();
			this->chunks[x][y]->updateTexture();
			this->map->updateTexture(this->chunks[x][y]->gridPos, this->chunks[x][y]->terrainVec);
		}
	}
}

void Map::initMapGenerator()
{
	this->chunkAmount = Vector2i(4, 4);
	this->seed = MapGenerator::generatePsuedoRandomSeed();
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
			this->chunks[x][y] = new Chunk(Vector2i(x, y), map);
		}
	}
}

void Map::draw(RenderTarget * window)
{
	for (int x = 0; x < this->chunkAmount.x; ++x)
	{
		for (int y = 0; y < this->chunkAmount.y; ++y)
		{
			this->chunks[x][y]->drawTiles(window);
		}
	}
}

void Map::update(const float& dt, const float& multiplier)
{

}