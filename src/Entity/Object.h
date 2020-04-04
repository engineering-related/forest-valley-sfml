#ifndef GAME_OBJECT
#define GAME_OBJECT

#include "Components/SpriteSheetComponent.h"
#include "Components/MovementComponent.h"

using namespace sf;

class Object
{
private:
	virtual void init();

protected:
	//Data
	Sprite sprite;
	Texture* texture;
	IntRect* textureRect;
	int zIndex;

	struct GridPos
	{
		Vector2i topLeft;	  //Top left pos of hitbox
		Vector2i bottomRight; //Bottom right pos of hitbox
	};
	GridPos gridPos;

	//Components
	SpriteSheetComponent* spriteSheetComponent;
	MovementComponent* movementComponent;

public:
	//Constructors
	Object(Vector2f pos, Texture* texture, Vector2f scale);
	virtual ~Object();

	//Components
	void createSpriteSheetComponent(const Vector2i nrOfImgs, const Vector2i startPos, const Vector2i endPos);
	void createMovementComponent(const float maxVelocity, const float acceleration, const float deAcceleration);

	//Accesors

	//Modifiers

	//Functions
	virtual void draw(RenderTarget* window) const = 0;
	virtual void update(const float& dt, const float& multiplier) = 0;
	static void flipTexture(IntRect& rect);
	static void setTextureRect(Sprite &sprite, const IntRect& rect);
};

#endif //GAME_OBJECT