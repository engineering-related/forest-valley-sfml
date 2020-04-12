#ifndef GAME_OBJECT
#define GAME_OBJECT

#include "Components/SpriteSheetComponent.h"
#include "Components/MovementComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/HitboxComponent.h"

using namespace sf;

class Object
{
private:
	void init();

protected:
	//Data
	Sprite sprite;
	Texture* texture;
	int zIndex;

	struct GridPos
	{
		Vector2i topLeft;	  //Top left pos of hitbox
		Vector2i bottomRight; //Bottom right pos of hitbox
	};
	GridPos gridPos;
	void setWorldGridPos();

	//Components
	SpriteSheetComponent* spriteSheetComponent;
	MovementComponent* movementComponent;
	AnimationComponent* animationComponent;
	HitboxComponent* hitboxComponent;

public:
	//Constructors
	Object(Vector2f pos, Texture* texture);
	virtual ~Object();

	//Components
	void createSpriteSheetComponent(const Vector2i nrOfImgs, const Vector2i startPos, const Vector2i endPos);
	void createMovementComponent(const float maxVelocity, const float acceleration, const float deAcceleration);
	void createAnimationComponent(AnimationComponent::Animation* startAnim);
	void createHitboxComponent();

	//Accesors
	inline const int&getZIndex()const{return this->zIndex; }
	//Modifiers
	void flipTextureRect();
	inline void setPosition(const Vector2f& pos){this->sprite.setPosition(pos); }

	//Functions
	virtual void draw(RenderTarget* window) const = 0;
	virtual void update(const float& dt, const float& multiplier) = 0;
};

#endif //GAME_OBJECT