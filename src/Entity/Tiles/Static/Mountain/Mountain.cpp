#include "Mountain.h"

Mountain::Mountain(Vector2f pos, Vector2i type) :
	StaticTile(pos, util::txh::Rock->getTexture(), true, false)
{
	this->type = type;
	this->addComponent<SpriteSheetComponent>(util::txh::Rock->getNrOfSheetImages(), Vector2i(0, 0), Vector2i(this->texture->getSize().x, this->texture->getSize().y));
	this->sprite.setTextureRect(this->getComponent<SpriteSheetComponent>().getTextureRects()[type.x][type.y]);
}

Mountain::~Mountain()
{

}

Tile::Parts* Mountain::getParts(const Vector2i& gridPos)
{
	Parts* part = new Parts;
	Vector2i startPos(gridPos.x * 5, gridPos.y * 3);

	//OUTER PARTS
	part->OTL = Vector2i(startPos.x + 0, startPos.y);
	part->OTM = Vector2i(startPos.x + 1, startPos.y);
	part->OTR = Vector2i(startPos.x + 2, startPos.y);

	part->OML = Vector2i(startPos.x + 0, startPos.y + 1);
	part->OMM = Vector2i(startPos.x + 1, startPos.y + 1);
	part->OMR = Vector2i(startPos.x + 2, startPos.y + 1);

	part->OBL = Vector2i(startPos.x + 0, startPos.y + 2);
	part->OBM = Vector2i(startPos.x + 1, startPos.y + 2);
	part->OBR = Vector2i(startPos.x + 2, startPos.y + 2);

	//INNER PARTS
	part->ITL = Vector2i(startPos.x + 3, startPos.y + 0);
	part->ITR = Vector2i(startPos.x + 4, startPos.y + 0);

	part->IML = Vector2i(startPos.x + 3, startPos.y + 1);
	part->IMR = Vector2i(startPos.x + 4, startPos.y + 1);
	return part;
}

void Mountain::draw(RenderTarget* window) const
{
	StaticTile::draw(window);
}

void Mountain::update(const float& dt, const float& multiplier)
{
	StaticTile::update(dt, multiplier);
}
