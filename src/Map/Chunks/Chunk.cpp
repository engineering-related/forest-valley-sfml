#include "Chunk.h"

Chunk::Chunk(Vector2f pos)
{
	this->pos = pos;
	this->init();
}

Chunk::~Chunk()
{

}

void Chunk::init()
{
	this->renderTexture.clear();
	this->renderTexture.create(this->size->x*TILE_SIZE.x, this->size->y*TILE_SIZE.y);
	this->sprite.setPosition(this->pos);
}

void Chunk::drawTiles(RenderTarget *window)
{
	window->draw(this->sprite);

}

void Chunk::update(const float& dt, const float& multiplier)
{
	for (Object* object : this->dynamicEntites)
	{
		object->update(dt, multiplier);
	}
}

void Chunk::setTexture()
{
	this->renderTexture.display();
	this->sprite.setTexture(this->renderTexture.getTexture());
}