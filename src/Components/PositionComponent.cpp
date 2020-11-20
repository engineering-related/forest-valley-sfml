#include "PositionComponent.h"

PositionComponent::PositionComponent(Sprite& sprite)
{
	this->spritePtr = &sprite;
	this->zIndex = 0;
	this->fixedZIndex = false;
}

PositionComponent::~PositionComponent()
{

}

void PositionComponent::setZIndexYPos()
{
	this->zIndex = this->spritePtr->getPosition().y + this->spritePtr->getTextureRect().height;
}

const Vector2f PositionComponent::getCenterPosition()
{
	Vector2f centerPosition;
	centerPosition.x = this->spritePtr->getPosition().x + this->spritePtr->getTextureRect().width / 2;
	centerPosition.y = this->spritePtr->getPosition().y + this->spritePtr->getTextureRect().height / 2;
	return centerPosition;
}

void PositionComponent::setWorldGridPos()
{
	this->gridPos.topLeft.x = floor(this->spritePtr->getPosition().x / TILE_SIZE.x);
	this->gridPos.topLeft.y = floor(this->spritePtr->getPosition().y / TILE_SIZE.y);
	this->gridPos.bottomRight.x = floor((this->spritePtr->getPosition().x + this->spritePtr->getTextureRect().width) / TILE_SIZE.x);
	this->gridPos.bottomRight.y = floor((this->spritePtr->getPosition().y + this->spritePtr->getTextureRect().height) / TILE_SIZE.y);

	this->chunkPos.x = floor(this->gridPos.bottomRight.x / CHUNK_SIZE.x);
	this->chunkPos.y = floor(this->gridPos.bottomRight.y / CHUNK_SIZE.y);
}

void PositionComponent::draw(RenderTarget* window)
{

}

void PositionComponent::update(const float& dt, const float& multiplier)
{
	this->setWorldGridPos();
	if(!this->fixedZIndex) this->setZIndexYPos();
}

