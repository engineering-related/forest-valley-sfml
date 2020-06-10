#ifndef COLISION_COMPONENT
#define COLISION_COMPONENT

#include "Component.h"
#include "Entity/Object.h"

using namespace sf;

class ColisionComponent : public Component
{
private:
	Sprite* spritePtr;
	IntRect intRect;
	RectangleShape rect;

public:
	ColisionComponent(Sprite& sprite);
	virtual ~ColisionComponent();

	void setRects(const IntRect intRect);
	inline const RectangleShape& getColisionRect() const{return this->rect; }
	inline const IntRect& getColisionIntRect() const{return this->intRect; }

	void draw(RenderTarget* window) override;
	void update(const float& dt, const float& multiplier) override;
};

#endif
