#include "Tile.h"

Tile::Tile(Vector2f pos, Texture* texture, bool solid, bool walkable) :
	Object(pos, texture)
{
	this->solid = solid;
	this->walkable = walkable;
}

Tile::~Tile()
{

}

void Tile::changeType(const Vector2i& type)
{
	if(this->hasComponent<SpriteSheetComponent>())
		this->sprite.setTextureRect(this->getComponent<SpriteSheetComponent>().getTextureRects()[type.x][type.y]);
}

void Tile::draw(RenderTarget * window) const
{
	Object::draw(window);
}

void Tile::update(const float& dt, const float& multiplier)
{
	Object::update(dt, multiplier);
}
