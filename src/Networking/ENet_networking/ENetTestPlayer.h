#ifndef ENET_TEST_PLAYER
#define ENET_TEST_PLAYER


#define MAP_WIDTH WINDOW_WIDTH*2
#define MAP_HEIGHT WINDOW_HEIGHT*2

#include "Entity/Creature/Humans/Player.h"

class ENetTestPlayer
{
private:
	sf::Text text;
	const unsigned int textSize = 20;
	Player* player;

public:
	//Player States
	///////////////////////////////////////////////////////////////////////////////////
	enum StateType {IDLE, MOVE, DEL, PICK_UP, DROP, CREATE, CRAFT, ATTACK};
	StateType currentStateType, lastStateType;

	struct State {
		inline static sf::Uint32 totalStates;
		sf::Uint32 ID;
		int64_t timeStamp;

		struct PlayerSnapshot{
			sf::Uint8 type;
			sf::Vector2f startPos;
			sf::Vector2f endPos;
			sf::Vector2f velocity;
			PlayerSnapshot(ENetTestPlayer* context = NULL)
			{
				if(context != NULL)
				{
					this->type = (sf::Uint8)context->currentStateType;
					this->startPos = context->rect.getPosition();
					this->endPos = context->endPos;
					this->velocity = context->velocity;
				}
			}
		}; PlayerSnapshot playerSnapshot;

		State(PlayerSnapshot playerSnapshot = PlayerSnapshot())
		{
			this->playerSnapshot = playerSnapshot;
			this->timeStamp = util::fn::getTimeInMsSinceEpoch().count();
			this->ID = State::totalStates++;
		}
	}; State playerState;
	////////////////////////////////////////////////////////////////////////////////////////////////

	//Attributes
	std::string playerID;
	sf::RectangleShape rect;
	sf::Vector2f endPos;
	sf::Vector2f velocity;
	bool mouseClicked;
	bool changedState;
	float speedMagnitude;

	std::vector<State> instructions;

	//Constructor
	ENetTestPlayer(std::string playerID = "",
				   sf::Vector2f spawnPos = sf::Vector2f(0.f, 0.f),
				   sf::Color color = sf::Color(255, 255, 255),
				   StateType stateType = StateType::IDLE,
				   sf::Vector2f velocity = sf::Vector2f(0.f, 0.f),
				   sf::Vector2f endPos = sf::Vector2f(WINDOW_WIDTH/2, WINDOW_WIDTH/2))

	{
		this->player = new Player(sf::Vector2f(spawnPos));
		this->playerID = playerID;
		this->rect.setSize(sf::Vector2f(30, 30));
		this->rect.setFillColor(color);
		this->rect.setPosition(spawnPos);
		this->rect.setOrigin(rect.getSize()*0.5f);
		this->speedMagnitude = 200.f;
		this->mouseClicked = false;
		this->endPos = endPos;
		this->currentStateType = stateType;
		this->velocity = velocity;
		this->playerState = State(this);
		this->changedState = false;
		this->initText();

	}
	//Destructor
	~ENetTestPlayer()
	{
		delete player;
	}

	void setPlayerState(State& state)
	{
		currentStateType = (StateType)state.playerSnapshot.type;
		/*-------------------WARNING-----------------------*/
		//rect.setPosition(state.playerSnapshot.startPos);
		/*-------------------------------------------------*/
		endPos = state.playerSnapshot.endPos;
		velocity = getVelocityVector(rect.getPosition(), endPos);
		refreshState();
	}

	//Functions

	void initText()
	{
		text.setOrigin(text.getGlobalBounds().width/2, text.getGlobalBounds().height/2);
		text.setString(playerID);
		text.setFont(*util::fnt::PixelFont->font);
		text.setCharacterSize(textSize);
		text.setFillColor(rect.getFillColor());
		text.setOutlineColor(rect.getFillColor());
	}

	void renderText(RenderTarget* target)
	{
		util::fn::mapTextToWindowCoords(target, &text, rect.getPosition(), sf::Vector2f(1.f, 1.f));

		text.setString(playerID + " " + std::to_string((int)round(rect.getPosition().x)) +
			" " + std::to_string((int)round(rect.getPosition().y)));
		static float time;
		//time += 5*dt;
		text.setPosition(rect.getPosition().x - text.getGlobalBounds().width/2,
			rect.getPosition().y - rect.getGlobalBounds().height -
			1.2*text.getGlobalBounds().height + sin(time) * text.getGlobalBounds().height/20);

		target->draw(text);
	}

	void draw(sf::RenderTarget* target)
	{
		//target->draw(rect);
		renderText(target);
		player->draw(target);
	}

	void refreshState()
	{
		playerState = State(State::PlayerSnapshot(this));
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

		//WARNING: Just testing, no acc added
		if(currentStateType == StateType::MOVE)
		{
			sf::Vector2f pPos = player->getComponent<PositionComponent>().getCenterPosition();
			float a = -(atan2(pPos.y-endPos.y, pPos.x-endPos.x) * 180 / PI -180 + 45);

			if(a < 0)
				player->right(dt, MULTIPLIER);
			else if(a < 90)
				player->up(dt, MULTIPLIER);
			else if(a < 180)
				player->left(dt, MULTIPLIER);
			else if(a < 270)
				player->down(dt, MULTIPLIER);
			else
				player->right(dt, MULTIPLIER);
		}
		else
		{
			player->none();
		}
		player->update(dt, MULTIPLIER);
		player->getComponent<PositionComponent>().setPosition(
			sf::Vector2f(rect.getPosition().x - TILE_SIZE.x, rect.getPosition().y - TILE_SIZE.y));

		//Ensure bounds
		if(this->rect.getPosition().x < 0.f)
			this->rect.setPosition(0.f, this->rect.getPosition().y);
		if(this->rect.getPosition().y < 0.f)
			this->rect.setPosition(this->rect.getPosition().x, 0.f);

		if(this->rect.getPosition().x > MAP_WIDTH)
			this->rect.setPosition(MAP_WIDTH, this->rect.getPosition().y);
		if(this->rect.getPosition().y > MAP_HEIGHT)
			this->rect.setPosition(this->rect.getPosition().x, MAP_HEIGHT);
	}

	sf::Vector2f getVelocityVector(const sf::Vector2f& p1, const sf::Vector2f& p2)
	{
		float angle = atan2f(p2.y - p1.y, p2.x - p1.x);
		return sf::Vector2f(speedMagnitude*cos(angle), speedMagnitude*sin(angle));
	}

	//Static functions
	static void handleMouse(ENetTestPlayer* player, sf::RenderWindow* window)
	{

		Vector2f mousePosRelative = window->mapPixelToCoords(Mouse::getPosition(*window), window->getView());
		sf::Vector2i mousePosAbsolute = sf::Mouse::getPosition(*window);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window->getViewport(window->getView()).contains(mousePosAbsolute) &&
		 !player->mouseClicked)
		{
			player->mouseClicked = true;
			player->endPos = mousePosRelative;
			player->velocity = player->getVelocityVector(player->rect.getPosition(), player->endPos);
			player->currentStateType = StateType::MOVE;
			player->refreshState();
		}
		else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			player->mouseClicked = false;
	}

};

//Overloading

//Player state
sf::Packet& operator <<(sf::Packet& packet, const ENetTestPlayer::State& state);
sf::Packet& operator >>(sf::Packet& packet, ENetTestPlayer::State &state);

//Player
sf::Packet& operator <<(sf::Packet& packet, const ENetTestPlayer* player);
sf::Packet& operator >>(sf::Packet& packet, ENetTestPlayer* player);


#endif //ENET_TEST_PLAYER