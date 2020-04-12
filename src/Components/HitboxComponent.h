#ifndef HITBOX_COMPONENT
#define HITBOX_COMPONENT

using namespace sf;

class HitboxComponent
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
	void draw(RenderTarget* window) const;
	void update(const float& dt, const float& multiplier);
};

#endif