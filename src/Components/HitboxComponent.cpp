#include "HitboxComponent.h"

HitboxComponent::HitboxComponent(Sprite& sprite)
{
	this->spritePtr = &sprite;
	this->hitbox = this->spritePtr->getTextureRect();
}

HitboxComponent::~HitboxComponent()
{
}

void HitboxComponent::draw(RenderTarget* window) const
{
	RectangleShape hitboxRect;
	hitboxRect.setPosition(this->spritePtr->getPosition());
	hitboxRect.setSize(Vector2f(this->hitbox.width, this->hitbox.height));
	window->draw(hitboxRect);
}