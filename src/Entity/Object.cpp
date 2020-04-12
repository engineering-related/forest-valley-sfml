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
	delete this->hitboxComponent;
}

void Object::init()
{
	this->zIndex = 0;
	this->spriteSheetComponent = nullptr;
	this->movementComponent = nullptr;
	this->animationComponent = nullptr;
	this->hitboxComponent = nullptr;
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

void Object::createHitboxComponent()
{
	if (this->hitboxComponent != nullptr)
		delete this->hitboxComponent;
	this->hitboxComponent = new HitboxComponent(this->sprite);
}

const Vector2f Object::getCenterPosition() const
{
	if(this->hitboxComponent)
	{
		Vector2f centerPosition;
		centerPosition.x = this->sprite.getPosition().x + this->hitboxComponent->getHitbox().width / 2;
		centerPosition.y = this->sprite.getPosition().y + this->hitboxComponent->getHitbox().height / 2;
		return centerPosition;
	}
}
const IntRect& Object::getHitbox()
{
	if (!this->hitboxComponent)
		this->createHitboxComponent();
	return this->hitboxComponent->getHitbox();
}


void Object::flipTextureRect()
{
	IntRect rect = this->sprite.getTextureRect();
	rect.left += rect.width;
	rect.width = -rect.width;
	this->sprite.setTextureRect(rect);
}

void Object::setWorldGridPos()
{
	this->gridPos.topLeft.x = floor(this->sprite.getPosition().x / TILE_SIZE.x);
	this->gridPos.topLeft.y = floor(this->sprite.getPosition().y / TILE_SIZE.y);
	this->gridPos.bottomRight.x = floor((this->sprite.getPosition().x + this->hitboxComponent->getHitbox().width) / TILE_SIZE.x);
	this->gridPos.bottomRight.y = floor((this->sprite.getPosition().y + this->hitboxComponent->getHitbox().height) / TILE_SIZE.y);
	this->zIndex = this->sprite.getPosition().y + this->hitboxComponent->getHitbox().height;
}

void Object::draw(RenderTarget* window) const
{
	if(this->hitboxComponent)
	{
		//this->hitboxComponent->draw(window);
	}
	window->draw(this->sprite);
}

void Object::update(const float& dt, const float& multiplier)
{
	if(this->movementComponent) this->movementComponent->updateSprite(dt, multiplier);
	//Creates a hitboxcomponent if the sprite has a texture rect
	if (!this->hitboxComponent)
	{
			this->createHitboxComponent();
	}
	else
	{
		this->setWorldGridPos();
		this->hitboxComponent->update(dt, multiplier);
	}
}
