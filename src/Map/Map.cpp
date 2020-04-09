#include "Map.h"


Map::Map(/* args */)
{

	this->init();
}

Map::~Map()
{
	delete this->map;
	for(size_t x = 0; x < this->grid.size(); x++)
	{
		for(size_t y = 0; y < this->grid[x].size(); y++)
		{
			delete this->grid[x][y];
		}
	}
}

void Map::init()
{
	this->initMapGenerator();
	this->updateTexture();
}

void Map::initMapGenerator()
{
	this->seed = MapGenerator::generatePsuedoRandomSeed();
	this->map = new MapGenerator(this->seed, Vector2i(250, 250), 40, 5, 0.5, 2, Vector2f(0, 0), 1);
	this->map->setDisplaySize(Vector2f(WINDOW_WIDTH/4, WINDOW_WIDTH/4));
	this->map->setConstDraw(true);
	this->grid = std::vector<std::vector<Tile*>>(this->map->terrainVec2xScale.size(), std::vector<Tile*>(this->map->terrainVec2xScale[0].size(), nullptr));
}

Vector2i* Map::calcGroundType(Ground::Parts* const parts, const size_t& x, const size_t& y)
{
	Vector2i *part;
	if(x == 0 || y == 0 || x == this->map->terrainVec2xScale.size() - 1 || y == this->map->terrainVec2xScale[0].size() - 1)
	{
		part = &parts->OMM;
		return part;
	}
	MapGenerator::TerrainType *current = &this->map->terrainVec2xScale[x][y],
							  *tl = &this->map->terrainVec2xScale[x - 1][y - 1],
							  *tm = &this->map->terrainVec2xScale[x][y - 1],
							  *tr = &this->map->terrainVec2xScale[x + 1][y - 1],

							  *ml = &this->map->terrainVec2xScale[x - 1][y],
							  *mr = &this->map->terrainVec2xScale[x + 1][y],

							  *bl = &this->map->terrainVec2xScale[x - 1][y + 1],
							  *bm = &this->map->terrainVec2xScale[x][y + 1],
							  *br = &this->map->terrainVec2xScale[x + 1][y + 1];



	std::vector<int> binary = std::vector<int>(8, 0);

	//OUTER:
	if(*current != *tm){
		binary[0] = 1;
	}
	if (*current != *mr) {
		binary[1] = 2;
	}
	if (*current != *bm) {
		binary[2] = 4;
	}
	if (*current != *ml) {
		binary[3] = 8;
	}
	int sum = std::accumulate(binary.begin(), binary.end(), 0);
	if(sum == 0)
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
		if (*current != *br) {
			binary[6] = 64;
		}
		if (*current != *bl) {
			binary[7] = 128;
		}
	}

	sum = std::accumulate(binary.begin(), binary.end(), 0);

	//std::cout << sum << std::endl;
	switch(sum)
	{
		case 0:
			part = &parts->OMM;
			break;
		case 1:
			part = &parts->OTM;

			break;
		case 3:
			part = &parts->OTR;
			break;
		case 2:
			part = &parts->OMR;
			break;
		case 6:
			part = &parts->OBR;
			break;
		case 4:
			part = &parts->OBM;
			break;
		case 12:
			part = &parts->OBL;
			break;
		case 8:
			part = &parts->OML;
			break;
		case 9:
			part = &parts->OTL;
			break;
		case 16:
			part = &parts->IMR;
			break;
		case 32:
			part = &parts->IML;
			break;
		case 64:
			part = &parts->ITL;
			break;
		case 128:
			part = &parts->ITR;
			break;

		default:
			part = &parts->OMM;
			break;
	}

	return part;
}

Vector2i* Map::assignType(const size_t& x, const size_t& y)
{
	Vector2i* type;
	switch (this->map->terrainVec2xScale[x][y])
	{
		case MapGenerator::TerrainType::GRASS_LIGHT:

			type = this->calcGroundType(Ground::GRASS_FLAT, x, y);
			break;
		case MapGenerator::TerrainType::FOREST_SHALLOW:
			type = this->calcGroundType(Ground::GRASS_FOREST, x, y);
			break;
		case MapGenerator::TerrainType::FOREST_DEEP:
			type = this->calcGroundType(Ground::GRASS_FOREST, x, y);
			break;
		case MapGenerator::TerrainType::SAND:
			type = this->calcGroundType(Ground::SAND, x, y);
			break;
		case MapGenerator::TerrainType::WATER_DEEP:
			type = this->calcGroundType(Ground::WATER, x, y);
			break;
		case MapGenerator::TerrainType::WATER_SHALLOW:
			type = this->calcGroundType(Ground::WATER, x, y);
			break;
		case MapGenerator::TerrainType::WHEAT:
			type = this->calcGroundType(Ground::FIELD, x, y);
			break;
		case MapGenerator::TerrainType::MINERALS:
			type = this->calcGroundType(Ground::GRAVEL, x, y);
			break;
		default:
			type = this->calcGroundType(Ground::STONE, x, y);
			break;
	}
	return type;
}

void Map::updateTexture()
{
	//TEMP, want to devide into chunks, each having their own  and multithread
	this->renderTexture.clear();
	Vector2i textureSize(TILE_SIZE.x * this->map->terrainVec2xScale.size(), TILE_SIZE.y * this->map->terrainVec2xScale.size());
	this->renderTexture.create(textureSize.x, textureSize.y);
	for (size_t x = 0; x < this->map->terrainVec2xScale.size(); x++)
	{
		for (size_t y = 0; y < this->map->terrainVec2xScale[x].size(); y++)
		{
			Vector2f pos(x * TILE_SIZE.x, y * TILE_SIZE.y);

			Ground* currentGround = new Ground(pos, *this->assignType(x, y));
			this->grid[x][y] = currentGround;
			currentGround->draw(&renderTexture);
		}
	}
	this->renderTexture.display();
	this->sprite.setTexture(this->renderTexture.getTexture());
	this->sprite.setPosition(Vector2f(-textureSize.x/2, -textureSize.y/2));
}

void Map::draw(RenderTarget* window)
{
	window->draw(this->sprite);
	this->map->draw(window);
}

void Map::update(const float& dt, const float& multiplier)
{

}