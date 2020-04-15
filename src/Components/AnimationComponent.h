#ifndef ANIMATION_COMPONENT
#define ANIMATION_COMPONENT

#include "Component.h"

using namespace sf;

class AnimationComponent : public Component
{
private:
	Sprite* spritePtr;
	float incer;
	unsigned int index;

public:
	struct Animation
	{
		std::vector<IntRect*> rectArray;
		float cap;
		Animation(){}
		Animation(float cap)
		{
			this->cap = cap;
		}
	};

private:
	Animation *currentAnim;
	Animation *previousAnim;

public:
	void setAnimation(Animation* anim);
	void setIndex(const unsigned int &index);
	int findIndex(const IntRect* currentFrame);
	void updateFrames(const float& dt, const float& multiplier);

	AnimationComponent(Sprite& sprite, Animation* startAnim);
	~AnimationComponent();

	void draw(RenderTarget* window);
	void update(const float& dt, const float& multiplier);
};


#endif