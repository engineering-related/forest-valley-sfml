#include "ENetwork.h"

ENetwork::ENetwork(/* args */)
{
	initENet();
	address.port = 24474;
	window = new sf::RenderWindow();
	initTestPlayer();
}

ENetwork::~ENetwork()
{
	enet_host_destroy(host);
	for (auto p : players)
	{
    	delete p.second;
		players.erase(p.first);
	}
	delete player;
	delete this->window;
}

int ENetwork::initENet()
{
	//Initialize Enet
    if(enet_initialize() != 0)
    {
            fprintf(stderr, "An error occurred while initializing Enet!\n");
            return EXIT_FAILURE;
    }
    //Deinitialize Enet when the program closes
    atexit(enet_deinitialize);
	return EXIT_SUCCESS;
}

void ENetwork::initTestPlayer()
{
	//Decale empty string
	std::string playerId;

	//Player enters an ID
	std::cout << "ID: ";
	std::cin >> playerId;
	std::cout << "\n";

	//Creates a player from the ID
	srand(time(NULL));
	sf::Color playerColor = sf::Color(rand() % 255, rand() % 255, rand() % 255);
	player = new ENetTestPlayer(playerId, sf::Vector2f(WINDOW_WIDTH/2, WINDOW_HEIGHT/2),
		playerColor);
}

const char* ENetwork::extractData(enet_uint8* data)
{
	return reinterpret_cast<const char*>(data);
}

void ENetwork::handleReceiveEvent(ENetEvent* event)
{
	//Extract data to c_string
	const char* receivedData = extractData(event->packet->data);

	//Split the string into a vector
	std::vector<std::string> dataVec = util::fn::stringSplitSpace(receivedData);

	//The first sequence in the data-string is always the type of the received packet
	PacketType recievedPacketType = (PacketType)std::stoi(dataVec[0]);

	//Perform action based on the given packet-type
	switch (recievedPacketType)
	{
	case PacketType::SERVER_DATA:
		printPacketData(receivedData);
		break;
	case PacketType::CLIENT_DATA:
		printPacketData(receivedData);
		break;
	case PacketType::PLAYER_CONNECTED:

		break;
	case PacketType::PLAYER_DISCONNECTED:

		break;
	case PacketType::HOST_DISCONNECTED:

		break;
	case PacketType::GAME_START:

		break;
	case PacketType::GAME_PAUSED:

		break;
	case PacketType::GAME_QUIT:

		break;
	default:
		break;
	}
	//Destory packet
	enet_packet_destroy(event->packet);
}

void ENetwork::sendPacket(ENetPeer* peer, enet_uint8 channel, const char* data)
{
	//When sending packets use multiple channels. Data from players->server are sent -
	//on 0. Data from server->players are sent on 1. Chat and messages are on 2 etc.
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, channel, packet);
}

void ENetwork::updatePlayers(const float &dt)
{
	player->update(dt);
}

void ENetwork::drawPlayers(sf::RenderTarget* target)
{
	player->draw(target);
}

void* ENetwork::traffic(void)
{
	clock.restart().asMilliseconds();
    while (getTheadsLoopRunning())
	{
		receiveEvents();
		sendPackets();
	}
	return NULL;
}

void ENetwork::printPacketData(const char* data)
{
	//Split the string into a vector
	std::vector<std::string> dataVec = util::fn::stringSplitSpace(data);
	puts("-----------------------");
	for (size_t i = 0; i < dataVec.size(); i++)
	{
		dataVec[i].insert(0, std::to_string(i)+". ");
		puts(dataVec[i].c_str());
	}
	puts("-----------------------\n");
}

int ENetwork::run()
{
	//Create window
	window->create(sf::VideoMode(800, 600, 32), "ENet Testing");
	window->setFramerateLimit(300);

	//Create network thread
	if (pthread_create(&networkThread, NULL, &trafficHelper, this) != 0)
	{
		printf("\nReceive-Thread can't be created :[%s]",
			strerror(pthread_create(&networkThread, NULL, &trafficHelper, this)));
		return EXIT_FAILURE;
	}

	//Create mutex
	if (pthread_mutex_init(&lock, NULL) != 0) {
    	fprintf(stderr, "\n mutex init has failed\n");
    	return EXIT_FAILURE;
    }

	//Start the thread-loops
	setThreadsLoopRunning(true);
	setGameLoopRunning(true);

	//Game loop
	while (getGameLoopRunning())
	{
		//SFML-events
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				setThreadsLoopRunning(false);
				window->close();
				disconnect();
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
		dt = gameClock.restart().asSeconds();
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
		//pthread_mutex_lock(&lock);
		player->handleMouse(window);
		updatePlayers(dt);
		this->drawPlayers(window);
		//pthread_mutex_unlock(&lock);
		//////////////////
		window->display();
	}
	pthread_join(networkThread, NULL);
	pthread_mutex_destroy(&lock);
	return EXIT_SUCCESS;
}