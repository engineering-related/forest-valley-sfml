#include "ENetTestGame.h"

ENetTestGame::ENetTestGame(const ENetwork  *  const context)
{
	//Set context
	this->context = context;
	//Create window
	window = new sf::RenderWindow();
	this->gameState = State(this);
}

ENetTestGame::~ENetTestGame()
{
	//Delete players
	pthread_mutex_lock(&ENetMutex);
	for (auto p : players)
	{
    	delete p.second;
	}
	pthread_mutex_unlock(&ENetMutex);

	pthread_mutex_destroy(&ENetMutex);
	delete window;
}


ENetTestGame::State::State(ENetTestGame* context)
{
	if(context != NULL)
	{
		for(auto p: context->players)
		{
			//Set the ENetID as key and the state of the player as value
			playerStates[p.first] = p.second->playerState;
		}
		timeStamp = util::fn::getTimeInMsSinceEpoch().count();
		this->ID = State::totalStates++;
	}
}

sf::Packet ENetTestGame::getChangedStateData(const std::string& ENetID,  const unsigned int &packetType)
{
	sf::Packet gameStateData;
	gameStateData << (sf::Uint8)packetType;
	//Add data about the players if the state was changed

	for(auto p: players)
	{
		//If a player-state changed add it to the list
		if(p.second->changedState)
		{
			gameStateData << p.first << gameState.playerStates[p.first];
			p.second->changedState = false;
		}
	}
	return gameStateData;
}

void ENetTestGame::setChangedStateData(sf::Packet& packet)
{
	while(!packet.endOfPacket())
	{
		std::string pENetID;
		ENetTestPlayer::State playerState;

		packet >> pENetID;
		packet >> playerState;

		if(pENetID == *ENetID) continue;
		else players[pENetID]->setPlayerState(playerState);
	}
	refreshState();
}

sf::Packet ENetTestGame::getGameData(const std::string& ENetID,  const unsigned int &packetType)
{
	//Send data about the the host
	sf::Packet gameData;
	gameData << (sf::Uint8)packetType << ENetID << (sf::Uint8)players.size();

	//Add data about the players
	for(auto p: players)
	{
		gameData << p.first << p.second;
	}
	return gameData;

	//Need info about the map later
}

void ENetTestGame::setGameData(sf::Packet& packet)
{
	std::string hENetID;
	sf::Uint8 playersOnServer;
	packet >> hENetID >> playersOnServer;

	for(size_t i = 0; i < (size_t)playersOnServer; i++)
	{
		std::string pENetID;
		ENetTestPlayer* pPLayer = new ENetTestPlayer();
		packet >> pENetID >> pPLayer;
		addPlayer(pENetID, pPLayer);
	}
	//Need info about the map later
}

void ENetTestGame::refreshState()
{
	lastGameState = gameState;
	gameState = State(this);
}

void ENetTestGame::initTestPlayer(const std::string *ENetID)
{
	std::string playerID;

	//Player enters an ID
	std::cout << "ID: ";
	std::cin >> playerID;
	std::cout << "\n";

	//Creates a player from the ID
	srand(time(NULL));
	sf::Color playerColor = sf::Color(rand() % 255, rand() % 255, rand() % 255);
	ENetTestPlayer* player = new ENetTestPlayer(playerID, sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2),
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

void ENetTestGame::addPlayer(std::string pENetID, ENetTestPlayer* player)
{
	pthread_mutex_lock(&ENetMutex);
		players[pENetID] = player;
	pthread_mutex_unlock(&ENetMutex);
}

void ENetTestGame::removePlayer(const std::string& pENetID)
{
	pthread_mutex_lock(&ENetMutex);
		ENetTestPlayer* deletePlayer = players[pENetID];
		players.erase(pENetID);
		delete deletePlayer;
	pthread_mutex_unlock(&ENetMutex);
}

void ENetTestGame::update(const float &dt)
{
	//Update game
	pthread_mutex_lock(&ENetMutex);
		ENetTestPlayer::handleMouse(players[*ENetID], window);
		updatePlayers(dt);
	pthread_mutex_unlock(&ENetMutex);
}

void ENetTestGame::draw(sf::RenderTarget* target)
{
	pthread_mutex_lock(&ENetMutex);
		drawPlayers(target);
	pthread_mutex_unlock(&ENetMutex);
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

		update(dt);
		draw(window);

		window->display();
	}
}