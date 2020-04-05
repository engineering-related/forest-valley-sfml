#include "StaticTile.h"

StaticTile::StaticTile(Vector2f pos, Texture* texture, bool solid, bool walkable) :
	Tile(pos, texture, solid, walkable)
{

}

StaticTile::~StaticTile()
{


}

void StaticTile::draw(RenderTarget* window) const
{
	Tile::draw(window);
}

void StaticTile::update(const float& dt, const float& multiplier)
{
	Tile::update(dt, multiplier);
}
