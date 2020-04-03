#ifndef GAME_OBJECT
#define GAME_OBJECT

using namespace sf;

class Object
{
private:
	virtual void init();

protected:
	//Data
	Sprite sprite;
	Texture* spriteSheet;
	std::vector<std::vector<IntRect>> textureRects;
	int zIndex;

	struct GridPos
	{
		Vector2i topLeft; //Top left pos of hitbox
		Vector2i bottomRight; //Bottom right pos of hitbox
	}; GridPos gridPos;

	//Components

public:
	//Constructors
	Object(Vector2f pos, Texture* spriteSheet, Vector2i sheetSize, Vector2f scale);
	virtual ~Object();

	//Accesors

	//Modifiers

	//Functions
	virtual void draw(RenderWindow* window) const = 0;
	virtual void update(const float& dt, const float& multiplier) = 0;
	void flipTexture();
	static std::vector<std::vector<IntRect>> getTextureRects(Texture* spriteSheet, const Vector2i& nrOfImgs);
	//static std::vector<std::vector<IntRect>> getTextureRect(Texture* spriteSheet, const Vector2i& nrOfImgs);
};

#endif //GAME_OBJECT