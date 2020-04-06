#ifndef CREATURE
#define CREATURE

#include "Entity/Object.h"

using namespace sf;

class Creature: public Object
{
private:
	unsigned int MAX_HP;
	unsigned int HP;
	//All creatures must have animations!
	virtual void setAnimations() = 0;

protected:

public:
	Creature(Vector2f pos, Texture* spriteSheet);
	~Creature();
};



#endif