#include "ENetwork.h"

ENetwork::ENetwork(/* args */)
{
	initENet();
	address.port = 24474;
	game = new ENetTestGame(this);
	game->initTestPlayer(&ID);
}

ENetwork::~ENetwork()
{
	enet_host_destroy(host);
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
	case PacketType::GAME_STATE:
		printPacketData(receivedData);
		break;
	case PacketType::PLAYER_STATE:
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
	/*WARNING: When sending packets use multiple channels. Data from players->server are sent -
	  on 0. Data from server->players are sent on 1. Chat and messages are on 2*/
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, channel, packet);
}

void* ENetwork::traffic(void)
{
	clock.restart().asMilliseconds();
    while (getTheadLoopRunning())
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
	//Create network thread
	if (pthread_create(&networkThread, NULL, &trafficHelper, this) != 0)
	{
		printf("\nReceive-Thread can't be created :[%s]",
			strerror(pthread_create(&networkThread, NULL, &trafficHelper, this)));
		return EXIT_FAILURE;
	}

	/*//Create mutex
	if (pthread_mutex_init(&game->ENetMutex, NULL) != 0) {
    	fprintf(stderr, "\n mutex init has failed\n");
    	return EXIT_FAILURE;
    }*/

	//Game loop
	setThreadLoopRunning(true);
	game->loop();
	setThreadLoopRunning(false);

	disconnect();
	pthread_join(networkThread, NULL);
	//pthread_mutex_destroy(&game->ENetMutex);
	delete game;
	return EXIT_SUCCESS;
}