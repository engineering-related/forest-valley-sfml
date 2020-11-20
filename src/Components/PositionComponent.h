#ifndef POSITION_COMPONENT
#define POSITION_COMPONENT

#include "Component.h"

using namespace sf;

class PositionComponent : public Component
{
private:
	Sprite* spritePtr;
	int zIndex;
	bool fixedZIndex;

	struct GridPos
	{
		Vector2i topLeft;	  //Top left pos of hitbox
		Vector2i bottomRight; //Bottom right pos of hitbox
	};
	GridPos gridPos;
	Vector2i chunkPos;
	void setWorldGridPos();

public:
	PositionComponent(Sprite& sprite);
	~PositionComponent();

	//Accesors
	inline const int&getZIndex()const{return this->zIndex; }
	const Vector2f getCenterPosition();
	inline const Vector2f &getPosition()const{return this->spritePtr->getPosition(); }
	inline const bool& getFixedZIndex() const{return this->fixedZIndex; }
	inline const GridPos& getGridPos() const{return this->gridPos; }
	inline const Vector2i& getChunkPos() const {return this->chunkPos; }

	//Modifiers
	inline void setPosition(const Vector2f& pos){this->spritePtr->setPosition(pos); }
	inline void setZIndex(const int &index){this->zIndex = index; }
	void setZIndexYPos();
	inline void setFixedZIndex(const bool& state){this->fixedZIndex = state;}

	//Functions
	void draw(RenderTarget* window);
	void update(const float& dt, const float& multiplier);
};

#endif