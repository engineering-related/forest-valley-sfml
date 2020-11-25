#include "World.h"

World::World(const unsigned int seed)
{
	this->seed = seed;
	this->init();
}

World::~World()
{
	//Delete chunks
	for(auto& container: this->chunks)
	{
		this->chunks.erase(container.first);
		delete container.second;
	}
	this->chunks.clear();

	//Delete all entities
	for(Object* entity: this->entites)
	{
		delete entity;
	}
	entites.clear();

	//Delete WorldGenerator
	delete this->map;

	//Delete threadpool
	delete this->pool;
}

void World::init()
{
	this->initWorldGenerator();
	this->initPlayer();
	this->initThreads();
}

void World::initWorldGenerator()
{
	this->chunkAmount = Vector2i(50, 50); //THE MAPS SIZE ARE ONLY LIMITED BY INTEGER SIZE, "INFINITE" MAPS!
	this->tileAmount = Vector2i(CHUNK_SIZE.x * this->chunkAmount.x, CHUNK_SIZE.y * this->chunkAmount.y);
	this->pixelSize = Vector2i(this->tileAmount.x * TILE_SIZE.x, this->tileAmount.y * TILE_SIZE.y);
	this->map = new WorldGenerator(this->seed, this->tileAmount, 40, 5, 0.5, 2, Vector2f(0, 0), 1);
	//this->map->setDisplaySize(Vector2f(WINDOW_WIDTH/2, WINDOW_WIDTH/2));
	this->map->setConstDraw(true);
}

void World::initPlayer()
{
	this->player = new Player(Vector2f(this->pixelSize/2));
}

int World::initThreads()
{
	//this->pool = new ThreadPool(this->numThreads);

	//Create network thread
	if (pthread_create(&this->minimap_thread, NULL, &minimapHelper, this) != 0)
	{
		printf("\nReceive-Thread can't be created :[%s]",
			strerror(pthread_create(&this->minimap_thread, NULL, &minimapHelper, this)));
		return EXIT_FAILURE;
	}

	//Create mutex
	if (pthread_mutex_init(&this->mutex, NULL) != 0) {
    	fprintf(stderr, "\n mutex init has failed\n");
    	return EXIT_FAILURE;
    }
	return EXIT_SUCCESS;
}

void World::savePlayerChunks()
{
	//Save old chunks
	for(int x = this->oldPlayerChunkPos.x - 1; x <=  this->oldPlayerChunkPos.x + 1; x++)
	{
		for(int y = this->oldPlayerChunkPos.y - 1; y <=  this->oldPlayerChunkPos.y + 1; y++)
		{
			//Check if the chunk exists
			if(chunks.find(chunkPosKey(x, y)) != chunks.end())
			{
				if(x >= 0 && x < this->chunkAmount.x &&
					y >= 0 && y < this->chunkAmount.y)
				{
					if((x < this->playerChunkPos.x - 1 ||
						x > this->playerChunkPos.x + 1 ||
						y < this->playerChunkPos.y - 1 ||
						y > this->playerChunkPos.y + 1))
					{
						//Distrubute workload on the different threads on pool
						//this->pool->enqueue([this, x, y] {
							this->chunks[chunkPosKey(x, y)]->save();

							Chunk* deleteChunkPtr = this->chunks[chunkPosKey(x, y)];
							this->chunks.erase(chunkPosKey(x, y));
							delete deleteChunkPtr;
						//});
					}
				}
			}
		}
	}
}

void World::loadPlayerChunks()
{
	for(int x = this->playerChunkPos.x - 1; x <=  this->playerChunkPos.x + 1; x++)
	{
		for(int y = this->playerChunkPos.y - 1; y <=  this->playerChunkPos.y + 1; y++)
		{
			if(x >= 0 && x < this->chunkAmount.x &&
				y >= 0 && y < this->chunkAmount.y)
			{
				//key is not present!
				if(chunks.find(chunkPosKey(x, y)) == chunks.end())
					chunks[chunkPosKey(x, y)] = new Chunk(Vector2i(x, y), this->map);

				//Load the current chunk if it haven't been
				if(!this->chunks[chunkPosKey(x, y)]->loaded)
				{

					//Distrubute workload on the different threads on pool
					//this->pool->enqueue([this, x, y] {
						this->chunks[chunkPosKey(x, y)]->load();
					//)};
				}

				//Make sure the current chunk is loaded
				if(this->chunks[chunkPosKey(x, y)]->loaded)
				{
					this->entites.insert(this->entites.end(),
					this->chunks[chunkPosKey(x, y)]->dynamicEntities.begin(),
					this->chunks[chunkPosKey(x, y)]->dynamicEntities.end());
				}
			}
		}
	}
}

void World::updatePlayerChunks()
{
	//Testing with rendering chunks, should use a threadpool later!
	this->playerChunkPos = player->getComponent<PositionComponent>().getChunkPos();

	//Testing with rendering chunks, should use a threadpool later!
	if(this->oldPlayerChunkPos != this->playerChunkPos)
	{
		this->entites.clear();
		this->loadPlayerChunks();
		this->savePlayerChunks();
		this->entites.push_back(player);
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
				if(chunks.find(chunkPosKey(x, y)) != chunks.end() &&
					this->chunks[chunkPosKey(x, y)]->loaded)
				{
					this->chunks[chunkPosKey(x, y)]->drawTiles(window);
				}
			}
		}
	}
}

void* World::updateMiniMap()
{
	/*while(true)
	{
		int x = this->playerChunkPos.x, y = this->playerChunkPos.y;

		if(x >= 0 && x < this->chunkAmount.x &&
		   y >= 0 && y < this->chunkAmount.y &&
			chunks.find(chunkPosKey(x, y)) != chunks.end() &&
			this->chunks[chunkPosKey(x, y)]->loaded)
		{
			pthread_mutex_lock(&this->mutex);
			this->map->updateTexture(
				this->chunks[chunkPosKey(x, y)]->gridPos,
				this->chunks[chunkPosKey(x, y)]->terrainVec);
			pthread_mutex_unlock(&this->mutex);
		}
	}*/
	return NULL;
}

void World::sortZindex()
{
	std::sort(this->entites.begin(), this->entites.end(), [](Object* obj1, Object* obj2) -> bool {
		if (obj1->getComponent<PositionComponent>().getZIndex() == obj2->getComponent<PositionComponent>().getZIndex())
		{
			if (obj1->getComponent<PositionComponent>().getCenterPosition().x == obj2->getComponent<PositionComponent>().getCenterPosition().x)
			{
				return obj1->getComponent<PositionComponent>().getCenterPosition().y > obj2->getComponent<PositionComponent>().getCenterPosition().y;
			}
			else
				return obj1->getComponent<PositionComponent>().getCenterPosition().x < obj2->getComponent<PositionComponent>().getCenterPosition().x;
		}
		else
			return obj1->getComponent<PositionComponent>().getZIndex() < obj2->getComponent<PositionComponent>().getZIndex();
	});
}


void World::checkTileColision(Object* object, Chunk* chunk)
{
	if(object->hasComponent<MovementComponent>())
	{
		object->getComponent<PositionComponent>().setPosition(object->getComponent<MovementComponent>().getPrevPos());
		Vector2i topLeft = object->getComponent<PositionComponent>().getGridPos().topLeft;
		Vector2i bottomRight = object->getComponent<PositionComponent>().getGridPos().bottomRight;

		for (int x = topLeft.x - 1; x <= bottomRight.x + 1; x++)
		{
			for (int y = topLeft.y - 1; y <= bottomRight.y + 1; y++)
			{
				//Ensure bounds
				if(x < 0 || x > (int)chunk->grid.size() - 1 ||
		  		   y < 0 || y > (int)chunk->grid[0].size() - 1) return;
				//Check if tile is colidable
				else if (chunk->grid[x][y]->hasComponent<ColisionComponent>())
				{
					//chunk->grid[x][y]->getComponent<ColisionComponent>().draw(window);
					IntRect r1 = object->getComponent<HitboxComponent>().getHitbox();
					IntRect r2 = chunk->grid[x][y]->getComponent<ColisionComponent>().getColisionIntRect();
					IntRect nextFrame = r1;
					Vector2f vel = MovementComponent::getClampedMagVel(object->getComponent<MovementComponent>().getVel());//*dt;

					//Nextframe vel needs to be a rounded to the closest int in the direction of the wall
					if(vel.x < 0.f) vel.x = floor(vel.x);
					if(vel.x > 0.f) vel.x = ceil(vel.x);
					if(vel.y < 0.f) vel.y = floor(vel.y);
					if(vel.y > 0.f) vel.y = ceil(vel.y);

					//Calc the position of the next frame
					nextFrame.left += vel.x;
					nextFrame.top += vel.y;

					if(r2.intersects(nextFrame))
					{
						//Bottom colision
						if(r1.top < r2.top &&
						   r1.top + r1.height < r2.top + r2.height &&
						   r1.left < r2.left + r2.width &&
						   r1.left + r1.width > r2.left)
						{
							object->getComponent<MovementComponent>().setVel(Vector2f(object->getComponent<MovementComponent>().getVel().x, 0.f));
							object->getComponent<PositionComponent>().setPosition(Vector2f(r1.left, r2.top - r1.height));
						}
						//Top colision
						else if(r1.top > r2.top &&
						   r1.top + r1.height > r2.top + r2.height &&
						   r1.left < r2.left + r2.width &&
						   r1.left + r1.width > r2.left)
						{
							object->getComponent<MovementComponent>().setVel(Vector2f(object->getComponent<MovementComponent>().getVel().x, 0.f));
							object->getComponent<PositionComponent>().setPosition(Vector2f(r1.left, r2.top + r2.height));
						}

						//Right colision
						if(r1.left < r2.left &&
						   r1.left + r1.width < r2.left + r2.width &&
						   r1.top < r2.top + r2.height &&
						   r1.top + r1.height > r2.top)
						{
							object->getComponent<MovementComponent>().setVel(Vector2f(0.f, object->getComponent<MovementComponent>().getVel().y));
							object->getComponent<PositionComponent>().setPosition(Vector2f(r2.left - r1.width, r1.top));
						}
						//Left colision
						else if(r1.left > r2.left &&
						   r1.left + r1.width > r2.left + r2.width &&
						   r1.top < r2.top + r2.height &&
						   r1.top + r1.height > r2.top)
						{
							object->getComponent<MovementComponent>().setVel(Vector2f(0.f, object->getComponent<MovementComponent>().getVel().y));
							object->getComponent<PositionComponent>().setPosition(Vector2f(r2.left + r2.width, r1.top));
						}
					}
				}
				else continue;
			}
		}
		//object->getComponent<MovementComponent>().moveSprite(object->getComponent<MovementComponent>().getVel(), dt);
	}
}


void World::draw(RenderTarget * window)
{
	this->drawTilesPlayerChunks(window);

	//Draw Objects
	for(Object* object: this->entites)
	{
		object->draw(window);
	}
}

void World::update(const float& dt, const float& multiplier)
{
	this->updatePlayerChunks();
	//Sort the Objects based on zIndex

	this->sortZindex();

	//Update objects
	for(Object* object: this->entites)
	{
		object->update(dt, multiplier);
		//this->checkTileColision(object, this->map->chunks[playerChunkPos.x][playerChunkPos.y]);

		//object->getComponent<HitboxComponent>().draw(this->window);
	}
}