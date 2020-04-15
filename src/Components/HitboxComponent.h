#ifndef HITBOX_COMPONENT
#define HITBOX_COMPONENT

#include "Component.h"

using namespace sf;

class HitboxComponent : public Component
{
private:
	IntRect hitbox;
	Sprite* spritePtr;
	Color color;

public:
	HitboxComponent(Sprite& sprite);
	~HitboxComponent();

	//Accessor
	inline const IntRect &getHitbox() const{return this->hitbox; }

	void draw(RenderTarget* window);
	void update(const float& dt, const float& multiplier);
};

#endif