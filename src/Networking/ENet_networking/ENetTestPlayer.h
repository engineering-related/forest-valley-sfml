#ifndef ENET_TEST_PLAYER
#define ENET_TEST_PLAYER


class ENetTestPlayer
{
public:
	enum Action {NONE, MOVE, DEL, PICK_UP, DROP, CREATE, CRAFT};
	//Attributes
	Action action;
	std::string id;
	sf::RectangleShape rect;
	sf::Vector2f endPos;
	sf::Vector2f velocity;
	sf::Vector2f prevPos;
	sf::Vector2f p2Pos;
	bool mouseClicked;
	float speedMagnitude;
	struct State
	{
		const ENetTestPlayer* cpPtr;

		State(const ENetTestPlayer* const p = NULL){ this->cpPtr = p; }

		const char * getData() const
		{

			std::string dataString = " " + cpPtr->id + " " +
							   std::to_string((int)cpPtr->action) + " " +
							   std::to_string(cpPtr->rect.getPosition().x) + " " +
							   std::to_string(cpPtr->rect.getPosition().y) + " ";

			return strdup(dataString.c_str());
		}
		const Action& getAction() const
		{
			return cpPtr->action;
		}

		bool operator != (const State &s2)
		{
			if(s2.cpPtr == NULL || this->cpPtr == NULL)
				return false;
			else
				return !(s2.cpPtr->action == this->getAction() &&
						 s2.cpPtr->rect.getPosition().x == this->cpPtr->rect.getPosition().x &&
						 s2.cpPtr->rect.getPosition().y == this->cpPtr->rect.getPosition().y);
		}
	}; State state;

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
	}
	~ENetTestPlayer(){}

	void draw(sf::RenderTarget* target)
	{
		target->draw(rect);
	}

	void update(const float &dt)
	{
		bool move = false;
		if ((this->velocity.x >= 0 && this->rect.getPosition().x < endPos.x) ||
			(this->velocity.x <= 0 && this->rect.getPosition().x > endPos.x) ||
			(this->velocity.y >= 0 && this->rect.getPosition().y < endPos.y) ||
			(this->velocity.y <= 0 && this->rect.getPosition().y > endPos.y))
				move = true;
		if(move)
		{
			this->action = Action::MOVE;
			this->rect.move(this->velocity*dt);
		}
		else
			this->action = Action::NONE;
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
		}
		else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
			mouseClicked = false;
	}
};

#endif