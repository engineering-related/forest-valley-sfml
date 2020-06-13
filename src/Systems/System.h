#ifndef SYSTEM
#define SYSTEM

class System
{
public:
	System(/* args */);
	virtual ~System();

	virtual void update(const float& dt, const float& multiplier) = 0;
	virtual void draw(sf::RenderTarget* window) = 0;
};


#endif