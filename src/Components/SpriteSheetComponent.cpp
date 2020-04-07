#include "SpriteSheetComponent.h"

SpriteSheetComponent::SpriteSheetComponent(const Vector2i nrOfImgs, const Vector2i startPos, const Vector2i endPos)
{
	this->nrOfImgs = nrOfImgs;
	this->textureRects = SpriteSheetComponent::createTextureRects(nrOfImgs, startPos, endPos);
}

SpriteSheetComponent::~SpriteSheetComponent()
{

}

const Vector2i& SpriteSheetComponent::getNrOfImages() const
{
	return this->nrOfImgs;
}

std::vector<std::vector<IntRect>>& SpriteSheetComponent::getTextureRects()
{
	return this->textureRects;
}


void SpriteSheetComponent::flipTexture()
{
	for (size_t x = 0; x < this->textureRects.size(); x++)
	{
		for (size_t y = 0; y < this->textureRects[x].size(); y++)
		{
			this->textureRects[x][y].left += this->textureRects[x][y].width;
			this->textureRects[x][y].width = -this->textureRects[x][y].width;
		}
	}
}

//Init the component with the correct "nrOfImages" and then replace "textureRects[x][y]" with a another "IntRect"
void SpriteSheetComponent::addTextureRect(IntRect rect, const int& x, const int& y)
{
	this->textureRects[x][y] = rect;
}

std::vector<std::vector<IntRect>> SpriteSheetComponent::createTextureRects(const Vector2i& nrOfImgs, const Vector2i& startPos, const Vector2i& endPos)
{

	std::vector<std::vector<IntRect>> textureRects;
	Vector2f inc((endPos.x - startPos.x) / nrOfImgs.x, (endPos.y - startPos.y) / nrOfImgs.y);

	for (float x = startPos.x; x < endPos.x; x += inc.x)
	{
		std::vector<IntRect> temp;
		for (float y = startPos.y; y < endPos.y; y += inc.y)
		{
			temp.push_back(IntRect(Vector2i(x, y), Vector2i(inc.x, inc.y)));
		}
		textureRects.push_back(temp);
	}
	return textureRects;
}