#include "AnimationComponent.h"

AnimationComponent::AnimationComponent(Sprite& sprite, Animation* startAnim)
{
	this->spritePtr = &sprite;
	this->currentAnim = startAnim;
	this->incer = rand() % int(this->currentAnim->cap) + 1;
}

AnimationComponent::~AnimationComponent()
{
}

void AnimationComponent::setAnimation(Animation* anim)
{
	this->currentAnim = anim;
}

int AnimationComponent::findIndex(const IntRect* currentFrame)
{
	//only works for sheets with one row, not very efficient fix later!!!
	for (size_t x = 0; x < this->textureRects.size(); x++)
	{
		for (size_t y = 0; y < this->textureRects[x].size(); y++)
		{
			if (currentFrame == &this->textureRects[x][y])
				return x;
		}
	}
	return -1;
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
		//this->animIndex = this->findIndex(this->currentAnim->rectArray[this->index]);
		this->index++;
	}
	this->previousAnim = this->currentAnim;
}

void AnimationComponent::update(const float& dt, const float& multiplier)
{
	this->updateFrames(dt, multiplier);
}