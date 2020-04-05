#ifndef TILE
#define TILE

#include "Entity/Object.h"

using namespace sf;

class Tile: public Object
{
private:
	bool solid;
	bool walkable;

protected:

public:
	Tile(Vector2f pos, Texture* texture, bool solid, bool walkable);
	virtual ~Tile();

	virtual void draw(RenderTarget* window) const;
	virtual void update(const float& dt, const float& multiplier);
};


#endif