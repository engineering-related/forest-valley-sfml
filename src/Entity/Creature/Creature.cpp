#include "Creature.h"

Creature::Creature(Vector2f pos, util::txh::Data* textureData, Vector2i startSheetPos, Vector2i endSheetPos,
	float maxVelocity, float acceleration, float deAcceleration) :
	Object(pos, textureData->getTexture())
{
	this->createSpriteSheetComponent(textureData->getNrOfSheetImages(), startSheetPos, endSheetPos);
	this->createMovementComponent(maxVelocity, acceleration, deAcceleration);
}

Creature::~Creature()
{

}

void Creature::none()
{
	switch (this->move.direction)
	{
		case Movement::Direction::U:
			this->animationComponent->setAnimation(&this->idle.up);
			break;
		case Movement::Direction::D:
			this->animationComponent->setAnimation(&this->idle.down);
			break;
		case Movement::Direction::L:
			this->animationComponent->setAnimation(&this->idle.left);
			break;
		case Movement::Direction::R:
			this->animationComponent->setAnimation(&this->idle.right);
			break;
		default:
			break;
	}
}

void Creature::up(const float& dt, const float& multiplier)
{
	this->move.direction = Movement::Direction::U;
	this->move.NOW = true, this->move.UP = true;
	this->animationComponent->setAnimation(&this->walking.up);
	this->movementComponent->move(0.f, -1.f, dt, multiplier);
}

void Creature::left(const float& dt, const float& multiplier)
{
	this->move.direction = Movement::Direction::L;
	this->move.NOW = true, this->move.LEFT = true;
	this->animationComponent->setAnimation(&this->walking.left);
	this->movementComponent->move(-1.f, 0.f, dt, multiplier);
}

void Creature::down(const float& dt, const float& multiplier)
{
	this->move.direction = Movement::Direction::D;
	this->move.NOW = true, this->move.DOWN = true;
	this->animationComponent->setAnimation(&this->walking.down);
	this->movementComponent->move(0.f, 1.f, dt, multiplier);
}

void Creature::right(const float& dt, const float& multiplier)
{
	this->move.direction = Movement::Direction::R;
	this->move.NOW = true, this->move.RIGHT = true;
	this->animationComponent->setAnimation(&this->walking.right);
	this->movementComponent->move(1.f, 0.f, dt, multiplier);
}

void Creature::checkTextureFlip()
{
	if(this->move.direction == Movement::Direction::L || this->move.direction == Movement::Direction::R)
	{
		if (this->move.direction == Movement::Direction::L && this->move.prevDirection != this->move.direction)
		{
			this->spriteSheetComponent->flipTexture();
			this->move.prevDirection = this->move.direction;
		}
		else if (this->move.direction == Movement::Direction::R && this->move.prevDirection != this->move.direction)
		{
			this->spriteSheetComponent->flipTexture();
			this->move.prevDirection = this->move.direction;
		}
	}
}

void Creature::updateAngle()
{
	float halfQarter = PI / 4;
	//RIGHT AND UP
	if (this->move.RIGHT && this->move.UP) this->angle = halfQarter;
	else if (this->move.RIGHT && this->move.DOWN) this->angle = -halfQarter;
	else if (this->move.UP && !this->move.LEFT) this->angle = 2 * halfQarter;
	else if (this->move.direction == Movement::Direction::R && !this->move.DOWN) this->angle = 0;

	//LEFT AND DOWN
	else if (this->move.LEFT && this->move.UP) this->angle = 3 * halfQarter;
	else if (this->move.LEFT && this->move.DOWN) this->angle = 5 * halfQarter;
	else if (this->move.DOWN) this->angle = 6 * halfQarter;
	else if (this->move.direction == Movement::Direction::L) this->angle = 4 * halfQarter;
}

void Creature::draw(RenderTarget* window) const
{
	Object::draw(window);
}

void Creature::update(const float& dt, const float& multiplier)
{
	//Facing direction
	//this->checkTextureFlip();
	this->updateAngle();
	//Animation
	this->animationComponent->updateFrames(dt, multiplier);
	if (!this->move.NOW) this->none();
	Object::update(dt, multiplier);
	this->move.reset();
}

void Creature::Movement::reset()
{
	NOW = false;
	UP = false;
	LEFT = false;
	DOWN = false;
	RIGHT = false;
}