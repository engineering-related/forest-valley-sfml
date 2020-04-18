#include "ColisionComponent.h"

ColisionComponent::ColisionComponent(Sprite& sprite)
{
	this->spritePtr = &sprite;
	this->rect.setSize(Vector2f(0.f, 0.f));
	this->rect.setPosition(this->spritePtr->getPosition());
}

ColisionComponent::~ColisionComponent()
{

}

void ColisionComponent::setRects(const IntRect intRect)
{
	this->intRect = intRect;
	this->rect.setPosition(this->spritePtr->getPosition());
	this->rect.setSize(Vector2f(this->intRect.width, this->intRect.height));
	this->rect.setOutlineThickness(1.f);
	this->rect.setOutlineColor(Color::Red);
	this->rect.setFillColor(Color::Transparent);

}

void ColisionComponent::draw(RenderTarget* window)
{
	window->draw(this->rect);
}

void ColisionComponent::update(const float& dt, const float& multiplier)
{
	(void) dt;
	(void) multiplier;
}
