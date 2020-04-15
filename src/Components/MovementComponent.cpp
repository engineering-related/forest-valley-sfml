#include "MovementComponent.h"

MovementComponent::MovementComponent(Sprite &sprite, float maxVelocity, float acc, float deAcc)
{
	this->spritePtr = &sprite;
	this->acc = acc;
	this->deAcc = deAcc;
	this->maxVel = maxVelocity;
	this->prevPos = this->spritePtr->getPosition();
	this->shouldUpdate = true;
}

MovementComponent::~MovementComponent()
{

}

const Vector2f & MovementComponent::getVel() const
{
	return this->vel;
}

const Vector2f & MovementComponent::getPrevPos() const
{
	if(this->spritePtr) return this->prevPos;
}

void MovementComponent::move(const float dir_x, const float dir_y, const float& dt, const float& multiplier)
{
	//Movement
	this->vel.x += this->acc * dir_x * dt * multiplier;
	this->vel.y += this->acc * dir_y * dt * multiplier;
}

void MovementComponent::updatePos(const float& dt, const float& multiplier)
{
	//Horizontal x-movement
	if (this->vel.x > 0.f)
	{
		this->vel.x -= this->deAcc*dt*multiplier;
		if (this->vel.x < 0.f)
			this->vel.x = 0.f;

		if (this->vel.x > this->maxVel)
			this->vel.x = this->maxVel;
	}
	else if (this->vel.x < 0.f)
	{
		this->vel.x += this->deAcc*dt*multiplier;
		if (this->vel.x > 0.f)
			this->vel.x = 0.f;

		if (this->vel.x < -this->maxVel)
			this->vel.x = -this->maxVel;
	}

	//Vertical y-movement
	if (this->vel.y > 0.f)
	{
		this->vel.y -= this->deAcc*dt*multiplier;
		if (this->vel.y < 0.f)
			this->vel.y = 0.f;

		if (this->vel.y > this->maxVel)
			this->vel.y = this->maxVel;
	}
	else if (this->vel.y < 0.f)
	{
		this->vel.y += this->deAcc*dt*multiplier;
		if (this->vel.y > 0.f)
			this->vel.y = 0.f;

		if (this->vel.y < -this->maxVel)
			this->vel.y = -this->maxVel;
	}
}

void MovementComponent::draw(RenderTarget* window)
{

}

void MovementComponent::update(const float& dt, const float& multiplier)
{
	if (this->shouldUpdate)
	{
		this->updatePos(dt, multiplier);
		this->prevPos = this->spritePtr->getPosition();
		this->spritePtr->move(this->getClampedMagVel(this->vel) * dt);
	}
}

Vector2f MovementComponent::getClampedMagVel(const Vector2f &vel)
{
	float rotation = atan2(vel.y, vel.x);
	Vector2f clampMagnitudeVel(abs(vel.x)*cos(rotation), abs(vel.y)*sin(rotation));
	return clampMagnitudeVel;
}
