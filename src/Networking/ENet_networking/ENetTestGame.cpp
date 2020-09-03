#include "ENetTestGame.h"

ENetTestGame::ENetTestGame(const ENetwork  *  const context)
{
	//Set context
	this->context = context;
	//Create window
	window = new sf::RenderWindow();
}


ENetTestGame::~ENetTestGame()
{
	pthread_mutex_lock(&ENetMutex);
	for (auto p : players)
	{
    	delete p.second;
	}
	pthread_mutex_unlock(&ENetMutex);

	pthread_mutex_destroy(&ENetMutex);
	delete window;
}


void ENetTestGame::initTestPlayer(std::string *ENetID)
{
	//Decale empty string
	std::string playerId;

	//Player enters an ID
	std::cout << "ID: ";
	std::cin >> *ENetID;
	std::cout << "\n";

	//Creates a player from the ID
	srand(time(NULL));
	sf::Color playerColor = sf::Color(rand() % 255, rand() % 255, rand() % 255);
	ENetTestPlayer* player = new ENetTestPlayer(playerId, sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2),
		playerColor);
	players[*ENetID] = player;
	this->ENetID = ENetID;
}

void ENetTestGame::updatePlayers(const float &dt)
{
	for (auto p : players)
	{
		p.second->update(dt);
	}
}

void ENetTestGame::drawPlayers(sf::RenderTarget* target)
{
	for (auto p : players)
	{
		p.second->draw(target);
	}
}

void ENetTestGame::loop()
{
	//Create the window
	window->create(sf::VideoMode(800, 600, 32), "ENet Testing");
	window->setFramerateLimit(200);
	//Start the loop
	setGameLoopRunning(true);
	while (getGameLoopRunning())
	{
		//SFML-events
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
				setGameLoopRunning(false);
				break;
			}
			if(event.type == sf::Event::Resized)
			{
				sf::Vector2f size = static_cast<sf::Vector2f>(window->getSize());
				window->setSize(static_cast<sf::Vector2u>(size));
				sf::View newView(sf::FloatRect(0.f, 0.f, size.x, size.y));
				window->setView(newView);
			}
		}

		//Game clock
		dt = clock.restart().asSeconds();
		float FPS = 1.f / dt;
		static float incer = 0;
		static float cap = 5;
		incer += dt;
		if (incer >= cap)
		{
			incer = 0;
			window->setTitle("ENet Testing | FPS: " + std::to_string(FPS));
		}
		//Rendering
		window->clear();
		/////////////////
		pthread_mutex_lock(&ENetMutex);
		ENetTestPlayer::handleMouse(players[*ENetID], window);
		updatePlayers(dt);
		drawPlayers(window);
		pthread_mutex_unlock(&ENetMutex);
		//////////////////
		window->display();
	}
}