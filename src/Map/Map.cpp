#include "Map.h"


Map::Map(/* args */)
{

	this->init();
}

Map::~Map()
{
	delete this->map;
}

void Map::init()
{
	this->initMapGenerator();
	this->updateTexture();
}

void Map::initMapGenerator()
{
	this->seed = MapGenerator::generatePsuedoRandomSeed();
	this->map = new MapGenerator(this->seed, Vector2i(500, 500), 50, 5, 0.5, 2, Vector2f(0, 0), 1);
	this->map->setDisplaySize(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
}

void Map::updateTexture()
{
	//TEMP
	this->renderTexture.clear();
	Vector2i textureSize(TILE_SIZE.x * this->map->terrainVec.size(), TILE_SIZE.y * this->map->terrainVec.size());
	this->renderTexture.create(textureSize.x, textureSize.y);
	for(size_t x = 0; x < this->map->terrainVec.size(); x++)
	{
		for (size_t y = 0; y < this->map->terrainVec.size(); y++)
		{
			Vector2f pos(x * TILE_SIZE.x, y * TILE_SIZE.y);
			Vector2i* type;
			switch (this->map->terrainVec[x][y])
			{
				case MapGenerator::TerrainType::GRASS_LIGHT:
					type = &Ground::GRASS_FLAT->OMM;
					break;
				case MapGenerator::TerrainType::FOREST_SHALLOW:
					type = &Ground::GRASS_FOREST->OMM;
					break;
				case MapGenerator::TerrainType::FOREST_DEEP:
					type = &Ground::GRASS_FOREST->OMM;
					break;
				case MapGenerator::TerrainType::SAND:
					type = &Ground::SAND->OMM;
					break;
				case MapGenerator::TerrainType::WATER_DEEP:
					type = &Ground::WATER->OMM;
					break;
				case MapGenerator::TerrainType::WATER_SHALLOW:
					type = &Ground::WATER->OMM;
					break;
				case MapGenerator::TerrainType::WHEAT:
					type = &Ground::FIELD->OMM;
					break;
				case MapGenerator::TerrainType::MINERALS:
					type = &Ground::GRAVEL->OMM;
					break;
				default:
					type = &Ground::STONE->OMM;
					break;
			}
			Ground currentGround(pos, *type);
			currentGround.draw(&renderTexture);
		}
	}
	this->renderTexture.display();
	this->sprite.setTexture(this->renderTexture.getTexture());
	this->sprite.setPosition(Vector2f(-textureSize.x/2, -textureSize.y/2));
}

void Map::draw(RenderTarget* window)
{
	window->draw(this->sprite);
}