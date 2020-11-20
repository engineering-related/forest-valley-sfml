#include "Chunk.h"

Chunk::Chunk(Vector2i chunkGridPos, MapGenerator* mapGenerator)
{
	this->chunkGridPos = chunkGridPos;
	this->gridPos = Vector2i(this->chunkGridPos.x * CHUNK_SIZE.x, this->chunkGridPos.y * CHUNK_SIZE.y);
	this->drawPos = Vector2f(chunkGridPos.x * CHUNK_SIZE.x * TILE_SIZE.x, chunkGridPos.y * CHUNK_SIZE.y * TILE_SIZE.y);
	this->mapGeneratorPtr = mapGenerator;
}

Chunk::~Chunk()
{

}

void Chunk::load()
{
	this->init();
	this->terrainVec = this->mapGeneratorPtr->getMapSegment(this->gridPos - Vector2i(3, 3), CHUNK_SIZE + Vector2i(6, 6));
	this->drawVector = this->terrainVec;
	this->grid = std::vector<std::vector<Tile*>>(this->terrainVec.size(), std::vector<Tile*>(this->terrainVec[0].size(), nullptr));
	this->interactableGrid = std::vector<std::vector<Object*>>(this->terrainVec.size(), std::vector<Object*>(this->terrainVec[0].size(), nullptr));
}

void Chunk::init()
{
	this->renderTexture.clear();
	this->renderTexture.create(CHUNK_SIZE.x*TILE_SIZE.x, CHUNK_SIZE.y*TILE_SIZE.y);
	this->sprite.setPosition(this->drawPos);
}

void Chunk::drawTiles(RenderTarget *window)
{
	window->draw(this->sprite);
}

void Chunk::update(const float& dt, const float& multiplier)
{

}

void Chunk::setTexture()
{
	this->renderTexture.display();
	this->sprite.setTexture(this->renderTexture.getTexture());
}