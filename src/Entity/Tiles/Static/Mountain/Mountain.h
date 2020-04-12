#ifndef MOUNTAIN
#define MOUNTAIN

#include "Entity/Tiles/Static/StaticTile.h"

using namespace sf;

class Mountain : public StaticTile
{
private:
	Vector2i type;

protected:

public:
	Mountain(Vector2f pos, Vector2i type);
	virtual ~Mountain();

	virtual void draw(RenderTarget* window) const;
	virtual void update(const float& dt, const float& multiplier);

	//Static defintions
	//Get the correct type of a given mountain
	static Parts* getParts(const Vector2i& gridPos);

	inline static Parts* const ROCK = Mountain::getParts(Vector2i(0, 0));
};




#endif