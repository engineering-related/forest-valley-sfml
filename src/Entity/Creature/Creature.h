#ifndef CREATURE
#define CREATURE

#include "Entity/Object.h"

using namespace sf;

class Creature: public Object
{
private:
	//All creatures must have animations!
	virtual void setAnimations() = 0;

protected:
	//Stats/desire
	struct HealthState
	{
		struct Spectrum
		{
		private:
			//i.e
			unsigned int min = 0;
			unsigned int max = 100;
			unsigned int current = max;
		public:
			inline void reduce(const unsigned int &amount){this->current -= amount;}
			inline void add(const unsigned int& amount){this->current += amount;}
			inline void set(const unsigned int& amount){this->current = amount;}
		};
		Spectrum HP;
		Spectrum HUNGER;
		Spectrum THIRST;
		Spectrum ENERGY;
		//Hunger goes up by time etc.
		void update(const float& dt, const float& multiplier);

	}; HealthState healthState;

	//Basic animations each creature must have!
	struct DirectionAnimations
	{
		AnimationComponent::Animation up;
		AnimationComponent::Animation down;
		AnimationComponent::Animation left;
		AnimationComponent::Animation right;
	};
	DirectionAnimations idle;
	DirectionAnimations walking;

	//Movement container
	struct Movement {
		bool NOW = false;
		bool UP = false;
		bool LEFT = false;
		bool DOWN = false;
		bool RIGHT = false;
		void reset();
		//Facing direction
		enum Direction
		{
			L,
			R,
			U,
			D
		};
		Direction direction;
		Direction prevDirection;
	}; Movement move;

	//Movement functions
	float angle;
	void none();
	void up(const float& dt, const float& multiplier);
	void left(const float& dt, const float& multiplier);
	void down(const float& dt, const float& multiplier);
	void right(const float& dt, const float& multiplier);
	//Check if the direction has changed when moving horizontally
	void checkTextureFlip();
	//Update the viewing angle
	void updateAngle();

public:
	//Constructors
	Creature(Vector2f pos, util::txh::Data* textureData, Vector2i startSheetPos, Vector2i endSheetPos,
		     AnimationComponent::Animation* startAnim,
			 float maxVelocity, float acceleration, float deAcceleration);
	~Creature();

	//Functions
	virtual void draw(RenderTarget* window) const;
	virtual void update(const float& dt, const float& multiplier);
};



#endif