#ifndef PLAYER
#define PLAYER

#include "Entity/Creature/Creature.h"

class Player : public Creature
{
private:
	void setAnimations();

public:
	Player(Vector2f pos);
	~Player();

	virtual void draw(RenderTarget* window) const;
	virtual void update(const float& dt, const float& multiplier);
};

#endif