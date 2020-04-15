#ifndef SPRITE_SHEET_COMPONENT
#define SPRITE_SHEET_COMPONENT


#include "Component.h"

using namespace sf;

class SpriteSheetComponent : public Component
{
private:
	Vector2i nrOfImgs;
	std::vector<std::vector<IntRect>> textureRects;


public:
	//Constructors
	SpriteSheetComponent(const Vector2i nrOfImgs, const Vector2i startPos, const Vector2i endPos);
	~SpriteSheetComponent();

	//Accessors
	const Vector2i& getNrOfImages() const;
	//Modifiable values
	std::vector<std::vector<IntRect>>& getTextureRects();

	//Functions
	void flipTexture();
	void flipTextureRect();
	void addTextureRect(IntRect rect, const int&x, const int& y);
	static std::vector<std::vector<IntRect>> createTextureRects(const Vector2i& nrOfImgs, const Vector2i &startPos, const Vector2i &endPos);
	void draw(RenderTarget* window);
	void update(const float& dt, const float& multiplier);
};

#endif