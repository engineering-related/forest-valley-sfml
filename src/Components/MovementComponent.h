#ifndef MOVEMENT_COMPONENT
#define MOVEMENT_COMPONENT

using namespace sf;

class MovementComponent
{
private:
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
	inline void setDeAcc(const float &deAcc) {this->deAcc = deAcc; }

	//Functions
	float getAngle();
	void move(const float dir_x, const float dir_y, const float& dt, const float& multiplier);
	void update(const float& dt, const float &multiplier);
	const Vector2f getClampedMagVel();
};

#endif
