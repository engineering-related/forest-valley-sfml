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
		static int totalPerformed = 0;

		for(auto p: context->players)
		{
			//Set the ENetID as key and the state of the player as value
			playerStates[p.second->playerState.playerStateID] = p.second->playerState;
		}
		timeStamp = util::fn::getTimeInMsSinceEpoch().count();
		gameStateID = totalPerformed++;
	}
}

const char* ENetTestGame::getChangedStateData(const std::string& ENetID,  const unsigned int &packetType)
{
	//Send data about the the host
	std::string gameStateData;
	gameStateData += std::to_string(packetType) + " ";
	gameStateData += ENetID + " ";
	gameStateData += std::to_string(gameState.gameStateID) + " ";
	gameStateData += std::to_string(gameState.timeStamp) + " ";

	//WARNING: Need to add information about the map etc.

	//Add data about the players if the state was changed
	unsigned int nrOfChangedPlayerStates = 0;
	for(auto p: players)
	{
		//If a player-state changed add it to the list
		if(p.second->changedState)
		{
			gameStateData += gameState.playerStates[p.second->playerState.playerStateID].getStateData(p.first, packetType);
			p.second->changedState = false;
			nrOfChangedPlayerStates++;
		}
	}
	gameStateData += std::to_string(nrOfChangedPlayerStates) + " ";
	return strdup(gameStateData.c_str());
}

void ENetTestGame::setChangedStateData(const std::vector<std::string> &gameStateDataVec)
{
	//std::string pENetID = gameStateDataVec[1];
	//Set the state data
	//gameState.gameStateID = std::stoi(gameStateDataVec[2]);
	//gameState.timeStamp = (int64_t)std::stoll(gameStateDataVec[3]);

	//WARNING: Need to add information about the map etc.

	//The length of the server data (data before playerdata)
	const size_t serverDataLength = 4;

	const size_t nrOfChangedPlayerStates = (size_t)std::stoi(gameStateDataVec[gameStateDataVec.size() - 1]);

	//The length of data for each player
	const int playerDataLength = 12;

	unsigned int dataIndex = serverDataLength;
	for (size_t i = 0; i < nrOfChangedPlayerStates; i++)
	{
		std::vector<std::string> playerStateDataVec;
		std::string pENetID = gameStateDataVec[dataIndex+1];

		if(pENetID == *ENetID) continue; //Dont set your own state

		for(size_t j = 0; j < playerDataLength; j++)
		{
			playerStateDataVec.push_back(gameStateDataVec[dataIndex++]);
		}

		players[pENetID]->setPlayerData(playerStateDataVec);
	}
	refreshState();
}

const char* ENetTestGame::getGameData(const std::string& ENetID,  const unsigned int &packetType)
{
	//Send data about the the host
	std::string gameData;
	gameData += std::to_string(packetType) + " ";
	gameData += ENetID + " ";
	gameData += std::to_string(gameState.gameStateID) + " ";
	gameData += std::to_string(gameState.timeStamp) + " ";
	gameData += std::to_string(players.size()) + " ";

	//Add data about the players
	for(auto p: players)
	{
		gameData += p.second->getPlayerData(p.first, packetType);
	}
	return strdup(gameData.c_str());
}

void ENetTestGame::setGameData(const std::vector<std::string> &gameDataVec)
{
	//std::string pENetID = playerDataVec[1];
	//Set the state data
	gameState.gameStateID = std::stoi(gameDataVec[2]);
	gameState.timeStamp = (int64_t)std::stoull(gameDataVec[3]);
	size_t playersOnServer = (size_t)std::stoi(gameDataVec[4]);

	//WARNING: Need to add information about the map etc.

	//The length of the server data (data before playerdata)
	const size_t serverDataLength = 5;

	//The length of data for each player
	const size_t playerDataLength = 14;

	unsigned int dataIndex = serverDataLength;
	for(size_t i = 0; i < playersOnServer; i++)
	{
		std::vector<std::string> playerDataVec;

		for(size_t j = 0; j < playerDataLength; j++)
		{
			playerDataVec.push_back(gameDataVec[dataIndex++]);
		}
		std::string pENetID = playerDataVec[1];
		ENetTestPlayer* player = ENetTestPlayer::buildPlayerFromData(playerDataVec);
		addPlayer(pENetID, player);
	}
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

void ENetTestGame::addPlayer(std::string ENetID, ENetTestPlayer* player)
{
	pthread_mutex_lock(&ENetMutex);
		players[ENetID] = player;
	pthread_mutex_unlock(&ENetMutex);
}

void ENetTestGame::removePlayer(const std::string& ENetID)
{
	pthread_mutex_lock(&ENetMutex);
		ENetTestPlayer* deletePlayer = players[ENetID];
		players.erase(ENetID);
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

void ENetTestGame::draw(RenderTarget* target)
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