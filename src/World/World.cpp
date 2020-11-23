#include "World.h"

World::World(const unsigned int seed, std::vector<Object*>* entitesPtr)
{
	this->seed = seed;
	this->entitesPtr = entitesPtr;
	this->init();
}

World::~World()
{
	//Delete chunks
	for (int x = 0; x < this->chunkAmount.x; ++x)
	{
		for (int y = 0; y < this->chunkAmount.y; ++y)
		{
			delete this->chunks[x][y];
		}
	}

	//Delete WorldGenerator
	delete this->map;
}

void World::init()
{
	this->initWorldGenerator();
	this->initChunks(this->map);
}

void World::initWorldGenerator()
{
	this->chunkAmount = Vector2i(1000, 1000); //WARNING: CAN NOT BE TO BIG, MAP IS NOT MADE TO BE INFINITE, MAX SHOULD BE AROUND 1000, 1000 AND THAT IS VERY BIG!
	this->tileAmount = Vector2i(CHUNK_SIZE.x * this->chunkAmount.x, CHUNK_SIZE.y * this->chunkAmount.y);
	this->pixelSize = Vector2i(this->tileAmount.x * TILE_SIZE.x, this->tileAmount.y * TILE_SIZE.y);
	this->map = new WorldGenerator(this->seed, this->tileAmount, 40, 5, 0.5, 2, Vector2f(0, 0), 1);
	this->map->setDisplaySize(Vector2f(WINDOW_WIDTH/4, WINDOW_WIDTH/4));
	this->map->setConstDraw(true);
}

void World::initChunks(WorldGenerator* map)
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

void World::updatePlayerChunks(Player* player)
{
	//Testing with rendering chunks, should use a threadpool later!
	this->playerChunkPos = player->getComponent<PositionComponent>().getChunkPos();

	//Testing with rendering chunks, should use a threadpool later!
	if(this->oldPlayerChunkPos != this->playerChunkPos)
	{
		this->entitesPtr->clear();

		//Save old chunks
		for(int x = this->oldPlayerChunkPos.x - 1; x <=  this->oldPlayerChunkPos.x + 1; x++)
		{
			for(int y = this->oldPlayerChunkPos.y - 1; y <=  this->oldPlayerChunkPos.y + 1; y++)
			{
				if(x >= 0 && x < this->chunkAmount.x &&
				   y >= 0 && y < this->chunkAmount.y &&
				   chunks[x][y]->loaded)
				{
					if(x < this->playerChunkPos.x - 1 ||
					   x > this->playerChunkPos.x + 1 ||
					   y < this->playerChunkPos.y - 1 ||
					   y > this->playerChunkPos.y + 1)
					{
						this->chunks[x][y]->save();
					}
				}
			}
		}

		for(int x = this->playerChunkPos.x - 1; x <=  this->playerChunkPos.x + 1; x++)
		{
			for(int y = this->playerChunkPos.y - 1; y <=  this->playerChunkPos.y + 1; y++)
			{
				if(x >= 0 && x < this->chunkAmount.x &&
					y >= 0 && y < this->chunkAmount.y)
				{

					//Load the current chunk if it haven't been
					if(!this->chunks[x][y]->loaded)
					{
						this->chunks[x][y]->load();
						this->map->updateTexture(this->chunks[x][y]->gridPos, this->chunks[x][y]->terrainVec);
					}

					//Make sure the current chunk is loaded
					if(this->chunks[x][y]->loaded)
					{
						this->entitesPtr->insert(this->entitesPtr->end(),
						this->chunks[x][y]->dynamicEntities.begin(),
						this->chunks[x][y]->dynamicEntities.end());
					}
				}
			}
		}
		this->entitesPtr->push_back(player);
	}
	this->oldPlayerChunkPos = this->playerChunkPos;
}

void World::drawTilesPlayerChunks(RenderTarget* window)
{
	for(int x = this->playerChunkPos.x - 1; x <= this->playerChunkPos.x + 1; x++)
	{
		for(int y = this->playerChunkPos.y - 1; y <= this->playerChunkPos.y + 1; y++)
		{
			if(x >= 0 && x < this->chunkAmount.x &&
				y >= 0 && y < this->chunkAmount.y)
			{
				if(this->chunks[x][y]->loaded)
					this->chunks[x][y]->drawTiles(window);
			}
		}
	}
}

void World::draw(RenderTarget * window)
{
	this->drawTilesPlayerChunks(window);
}

void World::update(Player* player)
{
	this->updatePlayerChunks(player);
}