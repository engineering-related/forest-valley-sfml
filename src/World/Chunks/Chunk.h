#ifndef CHUNK
#define CHUNK

#include "World/WorldGenerator/WorldGenerator.h"
#include "Entity/AllEntities.hpp"

using namespace sf;

class Chunk
{
private:
	//Aliases
	using tile_data = std::pair<Tile*, Tile::Parts*>;
	using tile_neighbour_type_data = std::vector<std::vector<std::pair<Vector2i*, Vector2i>>>;
	using terrain_vec_2D = std::vector<std::vector<WorldGenerator::TerrainType>>;
	using tile_vec_2D = std::vector<std::vector<Tile*>>;
	using nature_vec_2D = std::vector<std::vector<Nature*>>;
	using bool_grid_2D = std::vector<std::vector<bool>>;
	using grid_data_1D = std::vector<std::pair<int, Vector2i>>;

	//Constants
	inline static const Vector2i drawTileExtension = Vector2i(3, 3);

	Vector2i natureSpawnMaxPos;
	Vector2i natureSpawnMinPos;

	void init();
	void deleteObjects();

public:
	Vector2i chunkGridPos;
	Vector2i gridPos;
	Vector2f drawPos;

	const Vector2i naturSpawnMinPos = drawTileExtension;

	RenderTexture* renderTexture;
	Sprite* sprite;

	terrain_vec_2D terrainVec;
	terrain_vec_2D drawVector;

	tile_vec_2D grid;
	nature_vec_2D interactableGrid;
	std::vector<Object*> dynamicEntities;

	//std::vector<std::vector<DynamicTile*> dynamicTiles;
	//Dynamic tiles goes on three different textures depending on their frame
	//One of the three textures are drawn on top of the main texture

	WorldGenerator* worldGeneratorPtr;

	Chunk(Vector2i chunkGridPos, WorldGenerator* worldGeneratorPtr);
	~Chunk();

	bool loaded;

	void load();
	void save();

	//Functions
	tile_data getTileData(const size_t& x, const size_t& y);
	tile_neighbour_type_data getNeighboursData(Ground::Parts* const parts, const size_t& x, const size_t& y);

	void updateTexture();

	void spawnNatureObj(const IntRect &type, const int& x, const int& y);
	void buildNature(unsigned int seed);

	void drawTiles(RenderTarget* window);
	void setTexture();

};



#endif