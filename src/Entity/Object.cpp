#include "Object.h"

Object::Object(Vector2f pos, Texture* texture)
{
	this->sprite.setPosition(pos);
	this->texture = texture;
	this->sprite.setTexture(*this->texture);
	this->sprite.setScale(SCALE);
	this->init();
}

Object::~Object()
{
	delete this->spriteSheetComponent;
	delete this->movementComponent;
	delete this->animationComponent;
}

void Object::init()
{
	this->zIndex = 0;
	this->spriteSheetComponent = nullptr;
	this->movementComponent = nullptr;
	this->animationComponent = nullptr;
}

void Object::createSpriteSheetComponent(const Vector2i nrOfImgs, const Vector2i startPos, const Vector2i endPos)
{
	if (this->spriteSheetComponent != nullptr)
		delete this->spriteSheetComponent;
	this->spriteSheetComponent = new SpriteSheetComponent(nrOfImgs, startPos, endPos);
}

void Object::createMovementComponent(const float maxVelocity, const float acceleration, const float deAcceleration)
{
	if (this->movementComponent != nullptr)
		delete this->movementComponent;
	this->movementComponent = new MovementComponent(this->sprite, maxVelocity, acceleration, deAcceleration);
}

void Object::createAnimationComponent(AnimationComponent::Animation* startAnim)
{
	if (this->animationComponent != nullptr)
		delete this->animationComponent;
	this->animationComponent = new AnimationComponent(this->sprite, startAnim);
}

void Object::flipTextureRect()
{
	IntRect rect = this->sprite.getTextureRect();
	rect.left += rect.width;
	rect.width = -rect.width;
	this->sprite.setTextureRect(rect);
}


void Object::draw(RenderTarget* window) const
{
	window->draw(this->sprite);
}

void Object::update(const float& dt, const float& multiplier)
{
	if(this->movementComponent) this->movementComponent->update(dt, multiplier);
}
