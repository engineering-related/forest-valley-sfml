#ifndef MOVEMENT_COMPONENT
#define MOVEMENT_COMPONENT

#include "Component.h"

using namespace sf;

class MovementComponent : public Component
{
private:
	bool shouldUpdate;
	Sprite* spritePtr;
	float maxVel;
	Vector2f vel;
	Vector2f prevPos;
	float acc;
	float deAcc;

public:
	MovementComponent(Sprite &sprite, float maxVelocity, float acc, float deAcc);
	virtual ~MovementComponent();

	//Accessors
	const Vector2f& getVel() const;
	const Vector2f& getPrevPos() const;

	//Modifiers
	//set "deAcc" based on the friction of tile under moving object
	inline void setAcc(const float &acc) {this->acc = acc; }
	inline void setDeAcc(const float &deAcc) {this->deAcc = deAcc; }
	inline void setMaxVel(const float &maxVel) {this->maxVel = maxVel;}
	inline void setUpdate(const bool& state) {this->shouldUpdate = state; }

	//Functions
	float getAngle();
	void move(const float dir_x, const float dir_y, const float& dt, const float& multiplier);
	void updatePos(const float & dt, const float & multiplier);
	void draw(RenderTarget* window);
	void update(const float& dt, const float& multiplier);
	static Vector2f getClampedMagVel(const Vector2f &vel);
};

#endif
