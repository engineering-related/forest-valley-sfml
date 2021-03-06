#ifndef TEST_PLAYER
#define TEST_PLAYER


enum class Action{DEL, PICK_UP, DROP, CREATE, CRAFT};

struct EntityState
{
	//TCP
	unsigned int action;
	sf::Vector2i chunkID;
	unsigned int chunkObjectID;
	unsigned int objectID;
	unsigned int HP;
	std::vector<unsigned int> equippedItems;
};

class TestPlayer
{
public:
	sf::RectangleShape rect;
	sf::Vector2f endPos;
	sf::Vector2f velocity;
	sf::Vector2f prevPos;
	sf::Vector2f p2Pos;
	bool mouseClicked;
	float speedMagnitude;
	EntityState state;

	void initState()
	{
		//i.e state.chunkID = Vector2i(1, 3);
	}

	TestPlayer(sf::Color color)
	{
		rect.setSize(sf::Vector2f(20, 20));
		rect.setFillColor(color);
		rect.setPosition(0.f, 0.f);
		rect.setOrigin(rect.getSize()*0.5f);
		this->speedMagnitude = 200.f;
		this->mouseClicked = false;
		this->endPos = this->rect.getPosition();
		this->velocity = sf::Vector2f(0.f, 0.f);
		this->initState();

	}
	~TestPlayer(){}

	void draw(sf::RenderTarget* target)
	{
		target->draw(rect);
	}
	void updateEntityState()
	{
		//Ofc update for everything later
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
			this->rect.move(this->velocity*dt);

		//Update entity state about the information to send
		this->updateEntityState();
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