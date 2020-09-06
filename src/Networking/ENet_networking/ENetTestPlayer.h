#ifndef ENET_TEST_PLAYER
#define ENET_TEST_PLAYER

#include "Entity/Creature/Humans/Player.h";

class ENetTestPlayer
{
private:
	sf::Text text;
	const unsigned int textSize = 16;
	Player* player;

public:
	//Player States
	///////////////////////////////////////////////////////////////////////////////////
	enum StateType {IDLE, MOVE, DEL, PICK_UP, DROP, CREATE, CRAFT, ATTACK};
	StateType currentStateType, lastStateType;

	struct State {
		int playerStateID;
		int64_t timeStamp;

		struct PlayerSnapshot{
			std::string playerID;
			StateType type;
			sf::RectangleShape rect;
			sf::Vector2f endPos;
			sf::Vector2f velocity;
			PlayerSnapshot(ENetTestPlayer* context = NULL)
			{
				if(context != NULL)
				{
					this->playerID = context->playerID;
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
			this->timeStamp = util::fn::getTimeInMsSinceEpoch().count();
			playerStateID = totalPerformed++;
		}

		const char* getStateData(const std::string& ENetID,  const unsigned int &packetType)
		{
			std::string playerStateData;
			playerStateData += std::to_string(packetType) + " ";
			playerStateData += ENetID + " ";
			playerStateData += playerSnapshot.playerID + " ";
			playerStateData += std::to_string(playerStateID) + " ";
			playerStateData += std::to_string(timeStamp) + " ";
			playerStateData += std::to_string(playerSnapshot.type) + " ";
			//StartPosition
			playerStateData += std::to_string(playerSnapshot.rect.getPosition().x) + " ";
			playerStateData += std::to_string(playerSnapshot.rect.getPosition().y) + " ";
			//Velocity
			playerStateData += std::to_string(playerSnapshot.velocity.x) + " ";
			playerStateData += std::to_string(playerSnapshot.velocity.y) + " ";
			//Endpos
			playerStateData += std::to_string(playerSnapshot.endPos.x) + " ";
			playerStateData += std::to_string(playerSnapshot.endPos.y) + " ";
			return strdup(playerStateData.c_str());
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

	//Functions
	void initText()
	{
		text.setOrigin(text.getGlobalBounds().width/2, text.getGlobalBounds().height/2);
		text.setString(playerID);
		text.setFont(*util::fnt::PixelFont->font);
		text.setCharacterSize(textSize);
		text.setFillColor(rect.getFillColor());
		text.setOutlineColor(rect.getFillColor());
		updateTextPos(0.f);
	}

	void updateTextPos(const float & dt)
	{
		static float time;
		time += dt;
		text.setPosition(rect.getPosition().x - text.getGlobalBounds().width/2,
			rect.getPosition().y - rect.getGlobalBounds().height -
			text.getGlobalBounds().height + sin(time) * text.getGlobalBounds().height/8);
	}

	void drawText(RenderTarget* target)
	{
		target->draw(text);
	}

	void draw(sf::RenderTarget* target)
	{
		//target->draw(rect);
		drawText(target);
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

			if(a < 90)
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

		updateTextPos(dt);
	}

	const char* getPlayerData(const std::string &ENetID, const unsigned int &packetType)
	{
		std::string playerData;
		playerData += std::to_string(packetType) + " ";
		playerData += ENetID + " ";
		playerData += playerID + " ";
		playerData += std::to_string(util::fn::getTimeInMsSinceEpoch().count()) + " ";
		playerData += std::to_string(currentStateType) + " ";
		//Color
		playerData += std::to_string(rect.getFillColor().r) + " ";
		playerData += std::to_string(rect.getFillColor().g) + " ";
		playerData += std::to_string(rect.getFillColor().b) + " ";
		//StartPosition
		playerData += std::to_string(rect.getPosition().x) + " ";
		playerData += std::to_string(rect.getPosition().y) + " ";
		//Velocity
		playerData += std::to_string(velocity.x) + " ";
		playerData += std::to_string(velocity.y) + " ";
		//Endpos
		playerData += std::to_string(endPos.x) + " ";
		playerData += std::to_string(endPos.y) + " ";
		return strdup(playerData.c_str());
	}

	void setPlayerData(const std::vector<std::string> &playerDataVec)
	{
		//std::string pENetID = playerDataVec[1];
		playerID = playerDataVec[2];
		playerState.playerSnapshot.playerID = playerDataVec[3];
		playerState.timeStamp = (int64_t)std::stoll(playerDataVec[4]);
		currentStateType = (StateType)std::stoi(playerDataVec[5]);
		rect.setPosition(sf::Vector2f(std::stof(playerDataVec[6]), std::stof(playerDataVec[7])));
		velocity = sf::Vector2f(std::stof(playerDataVec[8]), std::stof(playerDataVec[9]));
		endPos = sf::Vector2f(std::stof(playerDataVec[10]), std::stof(playerDataVec[11]));
		refreshState();
	}

	//Static functions
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

	static ENetTestPlayer* buildPlayerFromData(const std::vector<std::string> &playerDataVec)
	{
		//std::string pENetID = playerDataVec[1];
		std::string pPlayerID = playerDataVec[2];
		//int64_t pTimeStamp = (int64_t)std::stoll(playerDataVec[3]);
		ENetTestPlayer::StateType statetype = (ENetTestPlayer::StateType)std::stoi(playerDataVec[4]);
		sf::Color pfillcolor = sf::Color(
				(Uint8)std::stoi(playerDataVec[5]),
				(Uint8)std::stoi(playerDataVec[6]),
				(Uint8)std::stoi(playerDataVec[7]));
		sf::Vector2f pPos = sf::Vector2f((float)std::stof(playerDataVec[8]), (float)std::stof(playerDataVec[9]));
		sf::Vector2f pVel = sf::Vector2f((float)std::stof(playerDataVec[10]), (float)std::stof(playerDataVec[11]));
		sf::Vector2f pEndPos = sf::Vector2f((float)std::stof(playerDataVec[12]), (float)std::stof(playerDataVec[13]));

		return new ENetTestPlayer(pPlayerID, pPos, pfillcolor, statetype, pVel, pEndPos);
	}
};

#endif //ENET_TEST_PLAYER