#ifndef GROUND
#define GROUND

#include "Entity/Tiles/Static/StaticTile.h"

using namespace sf;

class Ground : public StaticTile
{
private:
	Vector2i type;
	float friction;

protected:

public:

	Ground(Vector2f pos, Vector2i type);
	virtual ~Ground();

	virtual void draw(RenderTarget* window) const;
	virtual void update(const float& dt, const float& multiplier);

	//Static defintions
	//Get the correct type of a given ground
	static Parts* getParts(const Vector2i& gridPos);

	inline static Parts* const GRASS_FOREST = getParts(Vector2i(0, 0));
	inline static Parts* const GRASS_FLAT = getParts(Vector2i(0, 3));
	inline static Parts* const SAND = getParts(Vector2i(1, 4));
	inline static Parts* const GRAVEL = getParts(Vector2i(2, 4));
	inline static Parts* const FIELD = getParts(Vector2i(2, 3));

	//Temp
	inline static Parts* const STONE = getParts(Vector2i(0, 4));
	inline static Parts* const WATER = getParts(Vector2i(0, 1));
};


#endif