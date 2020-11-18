#ifndef CHUNK
#define CHUNK

#include "Entity/Object.h"
#include "Entity/Tiles/Tile.h"
//#include "Entity/Tiles/Static/Nature/Nature.h"
#include "Map/MapGenerator/MapGenerator.h"

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

	void drawTiles(RenderTarget* window);
	void update(const float &dt, const float &multiplier);
	void setTexture();

};



#endif