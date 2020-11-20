#ifndef CHUNK
#define CHUNK

#include "Map/MapGenerator/MapGenerator.h"
#include "Entity/AllEntities.hpp"

using namespace sf;

class Chunk
{
private:
	void init();

public:
	Vector2i chunkGridPos;
	Vector2i gridPos;
	Vector2f drawPos;
	RenderTexture renderTexture;
	Sprite sprite;

	std::vector<std::vector<MapGenerator::TerrainType>> terrainVec;
	std::vector<std::vector<MapGenerator::TerrainType>> drawVector;

	std::vector<std::vector<Tile*>> grid;
	std::vector<std::vector<Object*>> interactableGrid;
	std::vector<Object*> dynamicEntites;

	MapGenerator* mapGeneratorPtr;

	Chunk(Vector2i chunkGridPos, MapGenerator* mapGenerator);
	~Chunk();

	void load();
	void save();

	//Functions
	std::pair<Tile*, Tile::Parts*> getCellInfo(const size_t& x, const size_t& y);
	std::vector<std::vector<std::pair<Vector2i*, Vector2i>>> getNeighboursInfo(Ground::Parts* const parts, const size_t& x, const size_t& y);

	void updateTexture();

	void spawnNatureObj(const IntRect &type, const int& x, const int& y);
	void buildNature(unsigned int seed);

	void drawTiles(RenderTarget* window);
	void setTexture();

};



#endif