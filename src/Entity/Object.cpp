#include "Object.h"

Object::Object(Vector2f pos, Texture* texture, Vector2f scale)
{
	this->sprite.setPosition(pos);
	this->texture = texture;
	this->sprite.setTexture(*this->texture);
	this->sprite.setScale(scale);
	this->init();
}

Object::~Object()
{
	delete this->spriteSheetComponent;
	delete this->movementComponent;
	delete this->textureRect;
}

void Object::init()
{
	this->zIndex = 0;
	this->spriteSheetComponent = NULL;
}

void Object::createSpriteSheetComponent(const Vector2i nrOfImgs, const Vector2i startPos, const Vector2i endPos)
{
	if (this->spriteSheetComponent != NULL)
		delete this->spriteSheetComponent;
	this->spriteSheetComponent = new SpriteSheetComponent(nrOfImgs, startPos, endPos);
}

void Object::createMovementComponent(const float maxVelocity, const float acceleration, const float deAcceleration)
{
	if (this->movementComponent != NULL)
		delete this->movementComponent;
	this->movementComponent = new MovementComponent(this->sprite, maxVelocity, acceleration, deAcceleration);
}

void Object::flipTexture(IntRect& rect)
{
	rect.left += rect.width;
	rect.width = -rect.width;
}

void Object::setTextureRect(Sprite &sprite, const IntRect& rect)
{
	sprite.setTextureRect(rect);
}

