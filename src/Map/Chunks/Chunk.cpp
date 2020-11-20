#include "Chunk.h"

Chunk::Chunk(Vector2i chunkGridPos, MapGenerator* mapGenerator)
{
	this->chunkGridPos = chunkGridPos;
	this->gridPos = Vector2i(this->chunkGridPos.x * CHUNK_SIZE.x, this->chunkGridPos.y * CHUNK_SIZE.y);
	this->drawPos = Vector2f(this->gridPos.x * TILE_SIZE.x, this->gridPos.y * TILE_SIZE.y);
	this->mapGeneratorPtr = mapGenerator;
}

Chunk::~Chunk()
{

}

void Chunk::init()
{
	this->renderTexture.clear();
	this->renderTexture.create(CHUNK_SIZE.x*TILE_SIZE.x, CHUNK_SIZE.y*TILE_SIZE.y);
	this->sprite.setPosition(this->drawPos);
}

void Chunk::load()
{
	this->init();
	this->terrainVec = this->mapGeneratorPtr->getMapSegment(this->gridPos - Vector2i(3, 3), CHUNK_SIZE + Vector2i(6, 6));
	this->drawVector = this->terrainVec;
	this->grid = std::vector<std::vector<Tile*>>(this->terrainVec.size(), std::vector<Tile*>(this->terrainVec[0].size(), nullptr));
	this->interactableGrid = std::vector<std::vector<Object*>>(this->terrainVec.size(), std::vector<Object*>(this->terrainVec[0].size(), nullptr));
}

std::pair<Tile*, Tile::Parts*> Chunk::getCellInfo(const size_t& x, const size_t& y)
{
	Tile* tileType;
	Tile::Parts* type;
	Vector2f pos(TILE_SIZE.x*x, TILE_SIZE.y*y);

	switch (this->terrainVec[x][y])
	{
		case MapGenerator::TerrainType::GRASS_LIGHT:
			tileType = new Ground(pos, Vector2i(0, 0));
			type = Ground::GRASS_FLAT;
			break;
		case MapGenerator::TerrainType::FOREST_SHALLOW:
			tileType = new Ground(pos, Vector2i(0, 0));
			type = Ground::GRASS_FOREST;
			break;
		case MapGenerator::TerrainType::FOREST_DEEP:
			tileType = new Ground(pos, Vector2i(0, 0));
			type = Ground::GRASS_FOREST;
			break;
		case MapGenerator::TerrainType::SAND:
			tileType = new Ground(pos, Vector2i(0, 0));
			type = Ground::SAND;
			break;
		case MapGenerator::TerrainType::WATER_DEEP:
			tileType = new Ground(pos, Vector2i(0, 0));
			type = Ground::WATER;
			break;
		case MapGenerator::TerrainType::WATER_SHALLOW:
			tileType = new Ground(pos, Vector2i(0, 0));
			type = Ground::WATER;
			break;
		case MapGenerator::TerrainType::WHEAT:
			tileType = new Ground(pos, Vector2i(0, 0));
			type = Ground::FIELD;
			break;
		case MapGenerator::TerrainType::MINERALS:
			tileType = new Ground(pos, Vector2i(0, 0));
			type = Ground::MINERAL;
			break;
		default:
			tileType = new Mountain(pos, Vector2i(0, 0));
			type = Mountain::ROCK;
			break;
	}
	std::pair<Tile*, Tile::Parts*> returnPair;
	returnPair = std::make_pair(tileType, type);
	return returnPair;
}

std::vector<std::vector<std::pair<Vector2i*, Vector2i>>> Chunk::getNeighboursInfo(Ground::Parts* const parts, const size_t& x, const size_t& y)
{
	std::vector<std::vector<std::pair<Vector2i*, Vector2i>>> returnInfo = std::vector<std::vector<std::pair<Vector2i*, Vector2i>>>(3, std::vector<std::pair<Vector2i*, Vector2i>>(3));

	returnInfo[0][0] = std::make_pair(&parts->OMM, Vector2i(x - 1, y - 1)); //tl
	returnInfo[1][0] = std::make_pair(&parts->OMM, Vector2i(x, y - 1));	   //tm
	returnInfo[2][0] = std::make_pair(&parts->OMM, Vector2i(x + 1, y - 1)); //tr

	returnInfo[0][1] = std::make_pair(&parts->OMM, Vector2i(x - 1, y)); //ml
	returnInfo[1][1] = std::make_pair(&parts->OMM, Vector2i(x, y));	   //Current
	returnInfo[2][1] = std::make_pair(&parts->OMM, Vector2i(x + 1, y)); //mr

	returnInfo[0][2] = std::make_pair(&parts->OMM, Vector2i(x - 1, y + 1)); // bl
	returnInfo[1][2] = std::make_pair(&parts->OMM, Vector2i(x, y + 1));	   //bm
	returnInfo[2][2] = std::make_pair(&parts->OMM, Vector2i(x + 1, y + 1)); //br

	for(size_t row = 0; row < returnInfo.size(); row++)
	{
		for(size_t col = 0; col < returnInfo[row].size(); col++)
		{
			Vector2i pos = std::get<1>(returnInfo[row][col]);
			MapGenerator::TerrainType *current = &this->drawVector[pos.x][pos.y],
									  *tl = &this->drawVector[pos.x - 1][pos.y - 1],
									  *tm = &this->drawVector[pos.x][pos.y - 1],
									  *tr = &this->drawVector[pos.x + 1][pos.y - 1],

									  *ml = &this->drawVector[pos.x - 1][pos.y],
									  *mr = &this->drawVector[pos.x + 1][pos.y],

									  *bl = &this->drawVector[pos.x - 1][pos.y + 1],
									  *bm = &this->drawVector[pos.x][pos.y + 1],
									  *br = &this->drawVector[pos.x + 1][pos.y + 1];

			std::vector<int> binary = std::vector<int>(8, 0);

			//OUTER:
			if (*current != *tm)
			{
				binary[0] = 1;
			}
			if (*current != *mr)
			{
				binary[1] = 2;
			}
			if (*current != *bm)
			{
				binary[2] = 4;
			}
			if (*current != *ml)
			{
				binary[3] = 8;
			}
			int sum = std::accumulate(binary.begin(), binary.end(), 0);
			if (sum == 0)
			{
				//INNER:
				if (*current != *tl)
				{
					binary[4] = 16;
				}
				if (*current != *tr)
				{
					binary[5] = 32;
				}
				if (*current != *br)
				{
					binary[6] = 64;
				}
				if (*current != *bl)
				{
					binary[7] = 128;
				}
			}

			sum = std::accumulate(binary.begin(), binary.end(), 0);

			if (sum == 0)
			{
				std::get<0>(returnInfo[row][col]) = &parts->OMM;
			}
			else
			{
				switch (sum)
				{
					case 1:
						std::get<0>(returnInfo[row][col]) = &parts->OTM;
						break;
					case 3:
						std::get<0>(returnInfo[row][col]) = &parts->OTR;
						break;
					case 2:
						std::get<0>(returnInfo[row][col]) = &parts->OMR;
						break;
					case 6:
						std::get<0>(returnInfo[row][col]) = &parts->OBR;
						break;
					case 4:
						std::get<0>(returnInfo[row][col]) = &parts->OBM;
						break;
					case 12:
						std::get<0>(returnInfo[row][col]) = &parts->OBL;
						break;
					case 8:
						std::get<0>(returnInfo[row][col]) = &parts->OML;
						break;
					case 9:
						std::get<0>(returnInfo[row][col]) = &parts->OTL;
						break;
					case 16:
						std::get<0>(returnInfo[row][col]) = &parts->IMR;
						break;
					case 32:
						std::get<0>(returnInfo[row][col]) = &parts->IML;
						break;
					case 64:
						std::get<0>(returnInfo[row][col]) = &parts->ITL;
						break;
					case 128:
						std::get<0>(returnInfo[row][col]) = &parts->ITR;
						break;
					default:
						break;
				}
			}
		}
	}
	return returnInfo;
}

void Chunk::spawnNatureObj(const IntRect& type, const int& x, const int& y)
{
	//Build object
	Vector2f pos(x * TILE_SIZE.x, y * TILE_SIZE.y);
	Nature* natureObj = new Nature(pos, type);

	bool safeSpawn = true;
	int width = 0, height = 0;

	//Check the width and height of the object
	if(natureObj->getComponent<HitboxComponent>().getHitbox().width <= TILE_SIZE.x) width = 1;
	else width = 2;
	if(natureObj->getComponent<HitboxComponent>().getHitbox().height <= TILE_SIZE.y) height = 2;
	else height = 3;

	//Dont spawn anything on rocks or in water
	for (int row = -1; row <= width; row++)
	{
		for (int col = -1; col <= height; col++)
		{
			if (this->drawVector[x + row][y + col] == MapGenerator::TerrainType::ROCK_DARK)
				safeSpawn = false;
			if (this->drawVector[x + row][y + col] == MapGenerator::TerrainType::WATER_SHALLOW ||
				this->drawVector[x + row][y + col] == MapGenerator::TerrainType::WATER_DEEP)
				safeSpawn = false;
		}
	}
	if (safeSpawn)
	{
		this->interactableGrid[x][y] = natureObj;
		this->dynamicEntites.push_back(natureObj);
	}
	else delete natureObj;
}

void Chunk::buildNature(unsigned int seed)
{
	srand(seed);
	for (int x = 4; x < CHUNK_SIZE.x - 4; x++)
	{
		for (int y = 4; y < CHUNK_SIZE.y - 4; y++)
		{
			Vector2f pos(0.f, 0.f);
			double r = ((double)rand() / (RAND_MAX));

			//Nature vectors
			std::vector<IntRect*> BIG_TREES = { Nature::TREE_ROUND, Nature::TREE_FLUFFY, Nature::TREE_SLIM };
			std::vector<IntRect*> GROWING_TREES = { Nature::TREE_SEED, Nature::TREE_SAPLING };
			std::vector<IntRect*> STUMP_TREES = { Nature::TREE_STUMP_SLIM, Nature::TREE_STUMP, Nature::TREE_LOG };
			std::vector<IntRect*> BIG_STONES = { Nature::STONE_ROUND, Nature::STONE_EGG, Nature::STONE_BIG };
			std::vector<IntRect*> SMALL_STONES = { Nature::STONE_SMALL_EGG, Nature::STONE_SMALL, Nature::STONE_SMALL_ROUND };
			std::vector<IntRect*> SMALL_FLOWERS = { Nature::FLOWER_ONE, Nature::FLOWER_TWO, Nature::FLOWER_THREE };
			std::vector<IntRect*> BIG_FLOWERS = { Nature::FLOWER_BIG, Nature::FLOWER_WEED};

			switch (this->drawVector[x][y])
			{
				case MapGenerator::TerrainType::GRASS_LIGHT:
					if (r < 0.015)
					{
						int index = rand() % SMALL_STONES.size();
						this->spawnNatureObj(*SMALL_STONES[index], x, y);
					}
					else if (r < 0.03)
					{
						std::vector<IntRect*> ALL_TREES = std::vector<IntRect*>();
						for (int i = 0; i < 20; i++)
						{
							ALL_TREES.insert(ALL_TREES.end(), GROWING_TREES.begin(), GROWING_TREES.end());
							ALL_TREES.insert(ALL_TREES.end(), STUMP_TREES.begin(), STUMP_TREES.end());
						}
						ALL_TREES.insert(ALL_TREES.end(), BIG_TREES.begin(), BIG_TREES.end());
						int index = rand() % ALL_TREES.size();
						this->spawnNatureObj(*ALL_TREES[index], x, y);
					}
					if (r < 0.02)
					{
						std::vector<IntRect*> ALL_FLOWERS = std::vector<IntRect*>();
						for (int i = 0; i < 20; i++)
						{
							ALL_FLOWERS.insert(ALL_FLOWERS.end(), SMALL_FLOWERS.begin(), SMALL_FLOWERS.end());
						}
						ALL_FLOWERS.insert(ALL_FLOWERS.end(), BIG_FLOWERS.begin(), BIG_FLOWERS.end());
						int index = rand() % ALL_FLOWERS.size();
						this->spawnNatureObj(*ALL_FLOWERS[index], x, y);
					}
					break;
				case MapGenerator::TerrainType::FOREST_SHALLOW:
					if (r < 0.2)
					{
						std::vector<IntRect*> ALL_TREES = std::vector<IntRect*>();
						for (int i = 0; i < 20; i++)
						{
							ALL_TREES.insert(ALL_TREES.end(), BIG_TREES.begin(), BIG_TREES.end());
						}
						ALL_TREES.insert(ALL_TREES.end(), GROWING_TREES.begin(), GROWING_TREES.end());
						ALL_TREES.insert(ALL_TREES.end(), STUMP_TREES.begin(), STUMP_TREES.end());

						int index = rand() % ALL_TREES.size();
						this->spawnNatureObj(*ALL_TREES[index], x, y);
					}
					break;
				case MapGenerator::TerrainType::FOREST_DEEP:
					if (r < 0.6)
					{
						int index = rand() % BIG_TREES.size();
						this->spawnNatureObj(*BIG_TREES[index], x, y);
					}
					break;
				case MapGenerator::TerrainType::SAND:

					break;
				case MapGenerator::TerrainType::WATER_DEEP:

					break;
				case MapGenerator::TerrainType::WATER_SHALLOW:

					break;
				case MapGenerator::TerrainType::WHEAT:
					if (r < 0.5)
					{
						std::vector<IntRect*> ALL_FLOWERS = std::vector<IntRect*>();
						ALL_FLOWERS.insert(ALL_FLOWERS.end(), SMALL_FLOWERS.begin(), SMALL_FLOWERS.end());
						ALL_FLOWERS.insert(ALL_FLOWERS.end(), BIG_FLOWERS.begin(), BIG_FLOWERS.end());

						int index = rand() % ALL_FLOWERS.size();
						this->spawnNatureObj(*ALL_FLOWERS[index], x, y);
					}
					break;
				case MapGenerator::TerrainType::MINERALS:
					if (r < 0.1)
					{
						std::vector<IntRect*> ALL_STONES = std::vector<IntRect*>();
						ALL_STONES.insert(ALL_STONES.end(), SMALL_STONES.begin(), SMALL_STONES.end());
						ALL_STONES.insert(ALL_STONES.end(), BIG_STONES.begin(), BIG_STONES.end());
						int index = rand() % ALL_STONES.size();
						this->spawnNatureObj(*ALL_STONES[index], x, y);
					}
					break;
				default:
					break;
			}
		}
	}
	srand(time(NULL));
}

void Chunk::updateTexture()
{
	//The container for the drawing information stored in a tuple
	std::vector<std::pair<int, Vector2i>> grid1D;

	//2D-Vector with the info if a cell has been draw
	std::vector<std::vector<bool>> gridDrawn = std::vector<std::vector<bool>>(this->terrainVec.size(), std::vector<bool>(this->terrainVec[0].size(), false));

	for (size_t x = 2; x < this->terrainVec.size() - 2; x++)
	{
		for (size_t y = 2; y < this->terrainVec[x].size() - 2; y++)
		{
			//Store building information from every type in the 2D-vector and store it in the 1D-vector
			std::pair<int, Vector2i> drawingInformation;
			int layer = static_cast<int>(this->terrainVec[x][y]);
			drawingInformation = std::make_pair(layer, Vector2i(x, y));
			grid1D.push_back(drawingInformation);
		}
	}

	//Sort the 1D-vector by the first tuple value, drawing order, lowest->higest
	std::sort(grid1D.begin(), grid1D.end(), [](auto& left, auto& right) {
		return left.first < right.first;
	});

	int currentHeight = -1;
	for(auto& tuple : grid1D)
	{
		if (std::get<0>(tuple) > currentHeight)
		{
			currentHeight = std::get<0>(tuple);
			for (auto& container : grid1D)
			{
				int loopHeight = std::get<0>(container);
				Vector2i currentGridPos = std::get<1>(container);
				if(loopHeight < currentHeight+1)
				{
					for(int row = -1; row <= 1; row++)
					{
						for(int col = -1; col <= 1; col++)
						{
							//set all terrain around the cell to it self
							this->drawVector[currentGridPos.x + row][currentGridPos.y + col] = this->terrainVec[currentGridPos.x][currentGridPos.y];
						}
					}
				}
				else break;
			}
			gridDrawn = std::vector<std::vector<bool>>(this->terrainVec.size(), std::vector<bool>(this->terrainVec[0].size(), false));

		}
		std::pair<Tile*, Tile::Parts*> cellInfo = this->getCellInfo(std::get<1>(tuple).x, std::get<1>(tuple).y);
		//Save the pointer to the Tile obj to the grid
		this->grid[std::get<1>(tuple).x][std::get<1>(tuple).y] = std::get<0>(cellInfo);

		std::vector<std::vector<std::pair<Vector2i*, Vector2i>>> neighBoursInfo = this->getNeighboursInfo(std::get<1>(cellInfo), std::get<1>(tuple).x, std::get<1>(tuple).y);

		//Loop through each neighbour and draw the Tile with the correct type at the neighbour pos
		Vector2f drawPos(0.f, 0.f);
		for(size_t x = 0; x < neighBoursInfo.size(); x++)
		{
			for (size_t y = 0; y < neighBoursInfo[x].size(); y++)
			{
				Vector2i gridPos = std::get<1>(neighBoursInfo[x][y]);

				if(!gridDrawn[gridPos.x][gridPos.y])
				{
					//Delete if an object exist in a previous layer
					if(this->grid[gridPos.x][gridPos.y] != nullptr)
					{
						delete this->grid[gridPos.x][gridPos.y];
					}
					this->grid[gridPos.x][gridPos.y] = this->getCellInfo(std::get<1>(tuple).x, std::get<1>(tuple).y).first;

					drawPos = Vector2f((gridPos.x - 3) * TILE_SIZE.x, (gridPos.y - 3)* TILE_SIZE.y);

					this->grid[gridPos.x][gridPos.y]->getComponent<PositionComponent>().setPosition(drawPos);
					this->grid[gridPos.x][gridPos.y]->changeType(*std::get<0>(neighBoursInfo[x][y]));
					this->grid[gridPos.x][gridPos.y]->getComponent<HitboxComponent>().update(0.f, 0.f); //HitboxPos needs to be updated

					//Update colision component rect
					if(this->grid[gridPos.x][gridPos.y]->hasComponent<ColisionComponent>())
					{
						this->grid[gridPos.x][gridPos.y]->getComponent<ColisionComponent>().setRects(this->grid[gridPos.x][gridPos.y]->getComponent<HitboxComponent>().getHitbox());
					}

					//Draw to the correct this texture
					this->grid[gridPos.x][gridPos.y]->draw(&this->renderTexture);

					//Reset tile object the correct world position
					this->grid[gridPos.x][gridPos.y]->getComponent<PositionComponent>().setPosition(Vector2f(gridPos.x * TILE_SIZE.x, gridPos.y  * TILE_SIZE.y));
					gridDrawn[gridPos.x][gridPos.y] = true;
				}
			}
		}
	}
	this->setTexture();
}

void Chunk::drawTiles(RenderTarget *window)
{
	window->draw(this->sprite);
}

void Chunk::setTexture()
{
	this->renderTexture.display();
	this->sprite.setTexture(this->renderTexture.getTexture());
}