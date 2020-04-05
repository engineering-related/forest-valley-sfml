#ifndef STATIC_TILE
#define STATIC_TILE
#include "Entity/Tiles/Tile.h"

using namespace sf;

class StaticTile: public Tile
{
private:

protected:

public:
	StaticTile(Vector2f pos, Texture* texture, bool solid, bool walkable);
	virtual ~StaticTile();

	virtual void draw(RenderTarget* window) const;
	virtual void update(const float& dt, const float& multiplier);
};

#endif