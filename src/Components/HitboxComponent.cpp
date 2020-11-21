#include "HitboxComponent.h"

HitboxComponent::HitboxComponent(Sprite& sprite)
{
	this->spritePtr = &sprite;
	this->hitbox = this->spritePtr->getTextureRect();
	this->hitbox.left = round(this->spritePtr->getPosition().x);
	this->hitbox.top = round(this->spritePtr->getPosition().y);
	this->hitboxRect.setPosition(this->spritePtr->getPosition());
	this->hitboxRect.setSize(Vector2f(this->hitbox.width, this->hitbox.height));
	this->color = Color::White;
	this->update(0.f, 0.f);
}

HitboxComponent::~HitboxComponent()
{

}

void HitboxComponent::draw(RenderTarget* window)
{

	this->hitboxRect.setOutlineColor(this->color);
	this->hitboxRect.setOutlineThickness(1.f);
	this->hitboxRect.setFillColor(Color::Transparent);
	window->draw(hitboxRect);
}

void HitboxComponent::update(const float& dt, const float& multiplier)
{
	this->hitbox = this->spritePtr->getTextureRect();
	this->hitbox.left = round(this->spritePtr->getPosition().x);
	this->hitbox.top = round(this->spritePtr->getPosition().y);
	this->hitboxRect.setPosition(this->spritePtr->getPosition());
	this->hitboxRect.setSize(Vector2f(this->hitbox.width, this->hitbox.height));
}

