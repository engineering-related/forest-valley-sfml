#include "Player.h"

Player::Player(Vector2f pos) :
	Creature(pos, util::txh::PlayerBase,
	Vector2i(0, 0), Vector2i(util::txh::PlayerBase->getTexture()->getSize().x, util::txh::PlayerBase->getTexture()->getSize().y),
	150.f*10, 30.f*10, 15.f*10) //Change movement/friction later!
{
	this->setAnimations();
	this->addComponent<AnimationComponent>(this->sprite, &this->idle.down);
	this->getComponent<AnimationComponent>().setIndex(0);
}


Player::~Player()
{
}

void Player::setAnimations()
{
	//Idle (still frame)
	float animCap = 5.f;
	//Fix as a function later
	this->idle.up = AnimationComponent::Animation(animCap);
	this->idle.down = AnimationComponent::Animation(animCap);
	this->idle.left = AnimationComponent::Animation(animCap);
	this->idle.right = AnimationComponent::Animation(animCap);
	this->idle.up.rectArray.push_back(&this->getComponent<SpriteSheetComponent>().getTextureRects()[0][3]);
	this->idle.down.rectArray.push_back(&this->getComponent<SpriteSheetComponent>().getTextureRects()[0][0]);
	this->idle.left.rectArray.push_back(&this->getComponent<SpriteSheetComponent>().getTextureRects()[0][1]);
	this->idle.right.rectArray.push_back(&this->getComponent<SpriteSheetComponent>().getTextureRects()[0][2]);

	//Walking
	this->walking.up = AnimationComponent::Animation(animCap);
	this->walking.down = AnimationComponent::Animation(animCap);
	this->walking.left = AnimationComponent::Animation(animCap);
	this->walking.right = AnimationComponent::Animation(animCap);
	for (int x = 0; x < this->getComponent<SpriteSheetComponent>().getNrOfImages().x; x++)
	{
		this->walking.up.rectArray.push_back(&this->getComponent<SpriteSheetComponent>().getTextureRects()[x][3]);
		this->walking.down.rectArray.push_back(&this->getComponent<SpriteSheetComponent>().getTextureRects()[x][0]);
		this->walking.left.rectArray.push_back(&this->getComponent<SpriteSheetComponent>().getTextureRects()[x][1]);
		this->walking.right.rectArray.push_back(&this->getComponent<SpriteSheetComponent>().getTextureRects()[x][2]);
	}
}

void Player::draw(RenderTarget* window) const
{
	this->Creature::draw(window);
}

void Player::update(const float& dt, const float& multiplier)
{
	this->Creature::update(dt, multiplier);
}