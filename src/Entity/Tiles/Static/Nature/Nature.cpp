#include "Nature.h"

Nature::Nature(Vector2f pos, IntRect type) :
	StaticTile(pos, util::txh::Nature->getTexture(), false, true)
{
	this->type = type;
	this->addComponent<SpriteSheetComponent>(util::txh::Nature->getNrOfSheetImages(), Vector2i(0, 0), Vector2i(this->texture->getSize().x, this->texture->getSize().y));
	this->tileSize = Vector2i((type.left + type.width/TILE_SIZE.x), (type.top + type.height/TILE_SIZE.y));
	this->sprite.setTextureRect(type);
}

Nature::~Nature()
{

}

void Nature::draw(RenderTarget* window) const
{
	StaticTile::draw(window);
}

void Nature::update(const float& dt, const float& multiplier)
{
	StaticTile::update(dt, multiplier);
}
