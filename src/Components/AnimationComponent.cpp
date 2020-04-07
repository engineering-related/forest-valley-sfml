#include "AnimationComponent.h"

AnimationComponent::AnimationComponent(Sprite& sprite, Animation* startAnim)
{
	this->spritePtr = &sprite;
	this->currentAnim = startAnim;
	this->incer = rand() % int(this->currentAnim->cap) + 1;
	this->index = 0;
}

AnimationComponent::~AnimationComponent()
{
}

void AnimationComponent::setAnimation(Animation* anim)
{
	this->currentAnim = anim;
}

void AnimationComponent::setIndex(const unsigned int& index)
{
	this->index = index;
}


void AnimationComponent::updateFrames(const float& dt, const float& multiplier)
{
	this->incer += dt * multiplier;
	if (this->incer >= this->currentAnim->cap)
	{
		this->incer = 0;
		if (this->index >= this->currentAnim->rectArray.size() || this->previousAnim != currentAnim)
			this->index = 0;
		this->spritePtr->setTextureRect(*this->currentAnim->rectArray[this->index]);
		this->index++;
	}
	this->previousAnim = this->currentAnim;
}

void AnimationComponent::update(const float& dt, const float& multiplier)
{
	this->updateFrames(dt, multiplier);
}