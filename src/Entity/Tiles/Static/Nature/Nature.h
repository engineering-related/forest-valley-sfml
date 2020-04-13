#ifndef NATRUE
#define NATURE

#include "Entity/Tiles/Static/StaticTile.h"

using namespace sf;

class Nature : public StaticTile
{
private:
	IntRect type;
	Vector2i tileSize;

protected:

public:
	Nature(Vector2f pos, IntRect type);
	virtual ~Nature();

	virtual void draw(RenderTarget* window) const;
	virtual void update(const float& dt, const float& multiplier);

	//Static variables
	inline static IntRect* const TREE_ROUND = new IntRect(0, 0, 2*TILE_SIZE.x, 2*TILE_SIZE.y);
	inline static IntRect* const TREE_FLUFFY = new IntRect(6 * TILE_SIZE.x+1, 0, 2*TILE_SIZE.x-2, 2*TILE_SIZE.y);
	inline static IntRect* const TREE_SLIM = new IntRect(5 * TILE_SIZE.x, 0, TILE_SIZE.x, 2*TILE_SIZE.y);

	inline static IntRect* const TREE_SEED = new IntRect(2 * TILE_SIZE.x, 0, TILE_SIZE.x, TILE_SIZE.y);
	inline static IntRect* const TREE_SAPLING = new IntRect(2 * TILE_SIZE.x, TILE_SIZE.y, TILE_SIZE.x, TILE_SIZE.y);

	inline static IntRect* const TREE_STUMP_SLIM = new IntRect(8 * TILE_SIZE.x, 0, TILE_SIZE.x, TILE_SIZE.y);
	inline static IntRect* const TREE_STUMP = new IntRect(8 * TILE_SIZE.x, TILE_SIZE.y, TILE_SIZE.x, TILE_SIZE.y);
	inline static IntRect* const TREE_LOG = new IntRect(12 * TILE_SIZE.x, TILE_SIZE.y, TILE_SIZE.x, TILE_SIZE.y);


	inline static IntRect* const STONE_ROUND = new IntRect(9 * TILE_SIZE.x, 0, TILE_SIZE.x, TILE_SIZE.y);
	inline static IntRect* const STONE_EGG = new IntRect(9 * TILE_SIZE.x, TILE_SIZE.y, TILE_SIZE.x, TILE_SIZE.y);
	inline static IntRect* const STONE_BIG = new IntRect(10 * TILE_SIZE.x+1, 0, 2*TILE_SIZE.x-2, 2*TILE_SIZE.y-2);

	inline static IntRect* const STONE_SMALL_EGG = new IntRect(2 * TILE_SIZE.x, 3*TILE_SIZE.y, TILE_SIZE.x, TILE_SIZE.y);
	inline static IntRect* const STONE_SMALL = new IntRect(3 * TILE_SIZE.x, 3*TILE_SIZE.y, TILE_SIZE.x, TILE_SIZE.y);
	inline static IntRect* const STONE_SMALL_ROUND = new IntRect(4 * TILE_SIZE.x, 3*TILE_SIZE.y, TILE_SIZE.x, TILE_SIZE.y);


	inline static IntRect* const FLOWER_ONE = new IntRect(3 * TILE_SIZE.x, 0, TILE_SIZE.x, TILE_SIZE.y);
	inline static IntRect* const FLOWER_TWO = new IntRect(3 * TILE_SIZE.x, TILE_SIZE.y, TILE_SIZE.x, TILE_SIZE.y);
	inline static IntRect* const FLOWER_THREE = new IntRect(4 * TILE_SIZE.x, TILE_SIZE.y, TILE_SIZE.x, TILE_SIZE.y);

	inline static IntRect* const FLOWER_BIG = new IntRect(4 * TILE_SIZE.x, 0, TILE_SIZE.x, TILE_SIZE.y);
	inline static IntRect* const FLOWER_WEED = new IntRect(12 * TILE_SIZE.x, 0, TILE_SIZE.x, TILE_SIZE.y);
};




#endif