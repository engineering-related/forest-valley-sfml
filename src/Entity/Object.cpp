#include "Object.h"

Object::Object(Vector2f pos, Texture* spriteSheet, Vector2i sheetSize, Vector2f scale)
{
	this->sprite.setPosition(pos);
	this->spriteSheet = spriteSheet;
	this->textureRects = Object::getTextureRects(spriteSheet, sheetSize);
	this->sprite.setTexture(*this->spriteSheet);
	this->sprite.setScale(scale);
	this->init();
}

Object::~Object()
{
}

void Object::init()
{
	this->zIndex = 0;
}

void Object::flipTexture()
{
	for (size_t x = 0; x < textureRects.size(); x++)
	{
		for (size_t y = 0; y < textureRects[x].size(); y++)
		{
			this->textureRects[x][y].left += this->textureRects[x][y].width;
			this->textureRects[x][y].width = -this->textureRects[x][y].width;
		}
	}
}

std::vector<std::vector<IntRect>> Object::getTextureRects(Texture* spriteSheet, const Vector2i& nrOfImgs)
{
	std::vector<std::vector<IntRect>> textureRects;
	Vector2f inc(spriteSheet->getSize().x / nrOfImgs.x, spriteSheet->getSize().y / nrOfImgs.y);

	for (unsigned int x = 0; x < spriteSheet->getSize().x; x += inc.x)
	{
		std::vector<IntRect> temp;
		for (unsigned int y = 0; y < spriteSheet->getSize().y; y += inc.y)
		{
			temp.push_back(IntRect(Vector2i(x, y), Vector2i(inc.x, inc.y)));
		}
		textureRects.push_back(temp);
	}
	return textureRects;
}
