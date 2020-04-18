#ifndef HITBOX_COMPONENT
#define HITBOX_COMPONENT

#include "Component.h"

using namespace sf;

class HitboxComponent : public Component
{
private:
	IntRect hitbox;
	RectangleShape hitboxRect;
	Sprite* spritePtr;
	Color color;

public:
	HitboxComponent(Sprite& sprite);
	~HitboxComponent();

	//Accessor
	inline const IntRect &getHitbox() const{return this->hitbox; }
	inline const RectangleShape& getHitboxRect() const{return this->hitboxRect; }

	//Modifier
	inline void setColor(const Color color){this->color = color; }
	inline void setHitboxRect(const RectangleShape hitboxRect){this->hitboxRect = hitboxRect; }

	//Fucntions
	void draw(RenderTarget* window);
	void update(const float& dt, const float& multiplier);
};

#endif