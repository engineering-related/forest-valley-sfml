#include "Player.h"

Player::Player(Vector2f pos) :
	Creature(pos, util::txh::PlayerBase,
	Vector2i(0, 0), Vector2i(util::txh::PlayerBase->getTexture()->getSize().x, util::txh::PlayerBase->getTexture()->getSize().y),
	150.f, 30.f, 15.f) //Change movement/friction later!
{
	this->setAnimations();
	this->createAnimationComponent(&this->idle.down);
	this->animationComponent->setIndex(0);
}

Player::~Player()
{
}

void Player::setAnimations()
{
	//Idle (still frame)
	float animCap = 5.f;
	this->idle.up = AnimationComponent::Animation(animCap);
	this->idle.down = AnimationComponent::Animation(animCap);
	this->idle.left = AnimationComponent::Animation(animCap);
	this->idle.right = AnimationComponent::Animation(animCap);
	this->idle.up.rectArray.push_back(&this->spriteSheetComponent->getTextureRects()[0][3]);
	this->idle.down.rectArray.push_back(&this->spriteSheetComponent->getTextureRects()[0][0]);
	this->idle.left.rectArray.push_back(&this->spriteSheetComponent->getTextureRects()[0][1]);
	this->idle.right.rectArray.push_back(&this->spriteSheetComponent->getTextureRects()[0][2]);

	//Walking
	this->walking.up = AnimationComponent::Animation(animCap);
	this->walking.down = AnimationComponent::Animation(animCap);
	this->walking.left = AnimationComponent::Animation(animCap);
	this->walking.right = AnimationComponent::Animation(animCap);
	for(int x = 0; x < this->spriteSheetComponent->getNrOfImages().x; x++)
	{
		this->walking.up.rectArray.push_back(&this->spriteSheetComponent->getTextureRects()[x][3]);
		this->walking.down.rectArray.push_back(&this->spriteSheetComponent->getTextureRects()[x][0]);
		this->walking.left.rectArray.push_back(&this->spriteSheetComponent->getTextureRects()[x][1]);
		this->walking.right.rectArray.push_back(&this->spriteSheetComponent->getTextureRects()[x][2]);
	}
}

void Player::draw(RenderTarget* window) const
{
	this->Creature::draw(window);
}

void Player::update(const float& dt, const float& multiplier)
{
	//Change input later to game file!
	//Input
	if (Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::S)) this->up(dt, multiplier);
	if (Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::W)) this->down(dt, multiplier);
	if (Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::D)) this->left(dt, multiplier);
	if (Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::A)) this->right(dt, multiplier);

	this->Creature::update(dt, multiplier);
}