#ifndef ENET_TEST_PLAYER
#define ENET_TEST_PLAYER

#include "Entity/Object.h"

class ENetTestPlayer
{
public:
	//states
	enum StateType {IDLE, MOVE, DELETE, PICK_UP, DROP, CREATE, CRAFT, ATTACK};
	StateType currentStateType, lastStateType;

	struct State {
		int id;
		int64_t time;

		struct PlayerSnapshot{
			StateType type;
			sf::RectangleShape rect;
			sf::Vector2f endPos;
			sf::Vector2f velocity;
			PlayerSnapshot(ENetTestPlayer* context = NULL)
			{
				if(context != NULL)
				{
					this->type = context->currentStateType;
					this->rect = context->rect;
					this->endPos = context->endPos;
					this->velocity = context->velocity;
				}
			}
		}; PlayerSnapshot playerSnapshot;

		State(PlayerSnapshot playerSnapshot = PlayerSnapshot())
		{
			static int totalPerformed = 0;
			this->playerSnapshot = playerSnapshot;
			this->time = util::fn::getTimeInMsSinceEpoch().count();
			id = totalPerformed++;
		}
	}; State state;

	//Attributes
	std::string id;
	sf::RectangleShape rect;
	sf::Vector2f endPos;
	sf::Vector2f velocity;
	bool mouseClicked;
	bool changedState;
	float speedMagnitude;

	ENetTestPlayer(std::string id, sf::Vector2f spawnPos, sf::Color color)
	{
		this->id = id;
		rect.setSize(sf::Vector2f(20, 20));
		rect.setFillColor(color);
		rect.setPosition(spawnPos);
		rect.setOrigin(rect.getSize()*0.5f);
		this->speedMagnitude = 200.f;
		this->mouseClicked = false;
		this->endPos = this->rect.getPosition();
		this->velocity = sf::Vector2f(0.f, 0.f);
		this->state = State(this);
		this->changedState = false;
	}
	~ENetTestPlayer(){}

	void draw(sf::RenderTarget* target)
	{
		target->draw(rect);
	}

	void refreshState()
	{
		state = State(State::PlayerSnapshot(this));
		lastStateType = currentStateType;
		changedState = true;
	}

	void idle(const float &dt)
	{
		this->velocity = sf::Vector2f(0.f*dt, 0.f*dt);
		currentStateType = StateType::IDLE;
	}

	void update(const float &dt)
	{
		bool shouldMove = false;
		if ((this->velocity.x >= 0 && this->rect.getPosition().x < endPos.x) ||
			(this->velocity.x <= 0 && this->rect.getPosition().x > endPos.x) ||
			(this->velocity.y >= 0 && this->rect.getPosition().y < endPos.y) ||
			(this->velocity.y <= 0 && this->rect.getPosition().y > endPos.y))
				shouldMove = true;

		if(shouldMove)
			this->rect.move(this->velocity*dt);
		else
			idle(dt);

		if(lastStateType != currentStateType)
		{
			refreshState();
		}
	}

	void handleMouse(sf::RenderWindow* window)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window->getViewport(window->getView()).contains(mousePos) && !mouseClicked)
		{
			mouseClicked = true;
			endPos = (sf::Vector2f)mousePos;
			float angle = atan2f(endPos.y - this->rect.getPosition().y, endPos.x - this->rect.getPosition().x);
			velocity = sf::Vector2f(speedMagnitude*cos(angle), speedMagnitude*sin(angle));
			currentStateType = StateType::MOVE;
			refreshState();
		}
		else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			mouseClicked = false;
	}
};

#endif