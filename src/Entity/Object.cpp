#include "Object.h"

Object::Object(Vector2f pos, Texture* texture)
{
	this->ID = Object::ID_counter++;
	this->sprite.setPosition(pos);
	this->texture = texture;
	this->sprite.setTexture(*this->texture);
	this->sprite.setScale(SCALE);
	this->initComponents();
}

Object::~Object()
{
	//Components are unique_ptr's, no need to delete!
}

void Object::initComponents()
{
	this->addComponent<HitboxComponent>(this->sprite);
	this->addComponent<PositionComponent>(this->sprite);

	//Update them once
	this->getComponent<HitboxComponent>().update(0.f, 0.f);
	this->getComponent<PositionComponent>().update(0.f, 0.f);
}


void Object::draw(RenderTarget* window) const
{
	window->draw(this->sprite);
	for(auto & c: this->components)
	{
		//c->draw(window);
	}
}

void Object::update(const float& dt, const float& multiplier)
{
	for(auto & c: this->components)
	{
		c->update(dt, multiplier);
	}
}
