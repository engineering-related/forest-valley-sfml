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
	struct Movement
	{
		float maxVel = 200.f;

		bool NOW = false;
		bool UP = false;
		bool LEFT = false;
		bool DOWN = false;
		bool RIGHT = false;

		void reset();
	}; Movement movement;

	MovementComponent(Sprite &sprite, float maxVelocity, float acc, float deAcc);
	virtual ~MovementComponent();

	//Accessors
	const Vector2f& getVel() const;
	const Vector2f& getPrevPos() const;

	//Modifiers
	inline void setDeAcc(const float &deAcc) {this->deAcc = deAcc; }

	//Functions
	void move(const float dir_x, const float dir_y, const float& dt, const float& multiplier);
	void update(const float& dt, const float &multiplier);
	const Vector2f getClampedMagVel();
};

#endif
