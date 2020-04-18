#include "Object.h"

Object::Object(Vector2f pos, Texture* texture)
{
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
	this->addComponent<PositionComponent>(this->sprite);
	this->addComponent<HitboxComponent>(this->sprite);
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
