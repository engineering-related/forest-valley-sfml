#ifndef ENET_TEST_PLAYER
#define ENET_TEST_PLAYER

#include "Entity/Object.h"

class ENetTestPlayer
{
public:
	//states
	enum StateType {IDLE, MOVE, DEL, PICK_UP, DROP, CREATE, CRAFT, ATTACK};
	StateType currentStateType, lastStateType;

	struct State {
		int ID;
		int64_t time;

		struct PlayerSnapshot{
			std::string playerId;
			StateType type;
			sf::RectangleShape rect;
			sf::Vector2f endPos;
			sf::Vector2f velocity;
			PlayerSnapshot(ENetTestPlayer* context = NULL)
			{
				if(context != NULL)
				{
					this->playerId = context->ENetID;
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
			ID = totalPerformed++;
		}
	}; State state;

	//Attributes
	std::string ENetID;
	sf::RectangleShape rect;
	sf::Vector2f endPos;
	sf::Vector2f velocity;
	bool mouseClicked;
	bool changedState;
	float speedMagnitude;

	ENetTestPlayer(std::string ENetID = "",
				   sf::Vector2f spawnPos = sf::Vector2f(0.f, 0.f),
	               sf::Color color = sf::Color(255, 255, 255))
	{
		this->ENetID = ENetID;
		this->rect.setSize(sf::Vector2f(20, 20));
		this->rect.setFillColor(color);
		this->rect.setPosition(spawnPos);
		this->rect.setOrigin(rect.getSize()*0.5f);
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
		velocity = sf::Vector2f(0.f*dt, 0.f*dt);
		currentStateType = StateType::IDLE;
	}

	void update(const float &dt)
	{
		bool shouldMove = false;
		if ((velocity.x >= 0 && rect.getPosition().x < endPos.x) ||
			(velocity.x <= 0 && rect.getPosition().x > endPos.x) ||
			(velocity.y >= 0 && rect.getPosition().y < endPos.y) ||
			(velocity.y <= 0 && rect.getPosition().y > endPos.y))
				shouldMove = true;

		if(shouldMove)
			rect.move(velocity*dt);
		else
			idle(dt);

		if(lastStateType != currentStateType)
		{
			refreshState();
		}
	}

	static void handleMouse(ENetTestPlayer* player, sf::RenderWindow* window)
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window->getViewport(window->getView()).contains(mousePos) && !player->mouseClicked)
		{
			player->mouseClicked = true;
			player->endPos = (sf::Vector2f)mousePos;
			float angle = atan2f(player->endPos.y - player->rect.getPosition().y, player->endPos.x - player->rect.getPosition().x);
			player->velocity = sf::Vector2f(player->speedMagnitude*cos(angle), player->speedMagnitude*sin(angle));
			player->currentStateType = StateType::MOVE;
			player->refreshState();
		}
		else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			player->mouseClicked = false;
	}
	const char* getData()
	{
		std::string playerData;
		playerData += std::to_string(state.ID) + " ";
		playerData += std::to_string(state.time) + " ";
		playerData += std::to_string(state.playerSnapshot.type) + " ";
		//StartPosition
		playerData += std::to_string(state.playerSnapshot.rect.getPosition().x) + " ";
		playerData += std::to_string(state.playerSnapshot.rect.getPosition().y) + " ";
		//Velocity
		playerData += std::to_string(state.playerSnapshot.velocity.x) + " ";
		playerData += std::to_string(state.playerSnapshot.velocity.y) + " ";
		//Endpos
		playerData += std::to_string(state.playerSnapshot.endPos.x) + " ";
		playerData += std::to_string(state.playerSnapshot.endPos.y) + " ";
		return strdup(playerData.c_str());
	}
};

#endif //ENET_TEST_PLAYER