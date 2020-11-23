#ifndef GROUND
#define GROUND

#include "Entity/Tiles/Static/StaticTile.h"

using namespace sf;

class Ground : public StaticTile
{
private:
	Vector2i type;
	float friction;

	//Get the correct type of a given ground
	static Parts* getParts(const Vector2i& gridPos);

protected:

public:

	Ground(Vector2f pos, Vector2i type);
	virtual ~Ground();

	virtual void draw(RenderTarget* window) const;
	virtual void update(const float& dt, const float& multiplier);

	//Static defintions
	inline static Parts* const GRASS_FOREST = Ground::getParts(Vector2i(0, 0));
	inline static Parts* const GRASS_FOREST_DEEP = Ground::getParts(Vector2i(0, 0));
	inline static Parts* const GRASS_FLAT = Ground::getParts(Vector2i(0, 3));
	inline static Parts* const SAND = Ground::getParts(Vector2i(1, 4));
	inline static Parts* const GRAVEL = Ground::getParts(Vector2i(2, 4));
	inline static Parts* const FIELD = Ground::getParts(Vector2i(2, 3));
	inline static Parts* const MINERAL = Ground::getParts(Vector2i(1, 3));

	//Temp
	inline static Parts* const STONE = Ground::getParts(Vector2i(0, 4));
	inline static Parts* const WATER = Ground::getParts(Vector2i(0, 2));
};


#endif