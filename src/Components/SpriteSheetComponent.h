#ifndef SPRITE_SHEET_COMPONENT
#define SPRITE_SHEET_COMPONENT

using namespace sf;

class SpriteSheetComponent
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
	void addTextureRect(IntRect rect, const int&x, const int& y);
	static std::vector<std::vector<IntRect>> createTextureRects(const Vector2i& nrOfImgs, const Vector2i &startPos, const Vector2i &endPos);

};

#endif