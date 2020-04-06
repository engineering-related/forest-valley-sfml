#ifndef ANIMATION_COMPONENT
#define ANIMATION_COMPONENT

using namespace sf;

class AnimationComponent
{
private:
	Sprite* spritePtr;
	float incer;
	unsigned int index;
	unsigned int animIndex;
	std::vector<std::vector<IntRect>> textureRects;

public:
	struct Animation
	{
		std::vector<IntRect*> rectArray;
		float cap;
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
	int findIndex(const IntRect* currentFrame);
	void updateFrames(const float& dt, const float& multiplier);
	void update(const float& dt, const float& multiplier);

	AnimationComponent(Sprite& sprite, Animation* startAnim);
	~AnimationComponent();
};


#endif