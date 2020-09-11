#include "ENetwork.h"

ENetwork::ENetwork(/* args */)
{
	initENet();
	ENetID = util::fn::random_string(keyCharacterLength);
	address.port = PORT;
	game = new ENetTestGame(this);
	game->initTestPlayer(&ENetID);
	setShouldDisconnect(true);
}

ENetwork::~ENetwork()
{
	enet_host_destroy(host);
	delete game;
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

void ENetwork::sendPacket(ENetPeer* peer, enet_uint8 channel, const sf::Packet& packet)
{
	/*WARNING: When sending packets use multiple channels. Data from players->server are sent -
	  on 0. Data from server->players are sent on 1. Other types of data are send on 2*/
	ENetPacket* eNetPacket = enet_packet_create(packet.getData(), packet.getDataSize(), ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, channel, eNetPacket);
}

void* ENetwork::traffic(void)
{
	clock.restart().asMilliseconds();
    while (getTheadLoopRunning())
	{
		//Evetents are rec
		receiveEvents();
		sendPackets();
	}
	return NULL;
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

	//Create mutex
	if (pthread_mutex_init(&game->ENetMutex, NULL) != 0) {
    	fprintf(stderr, "\n mutex init has failed\n");
    	return EXIT_FAILURE;
    }

	//Game loop started
	setThreadLoopRunning(true);
	game->loop();
	//Game loop ended

	//Check if we need to disconnect from the server
	if(shouldDissconnect)
		disconnect();

	//Exit network-thread
	setThreadLoopRunning(false);

	//Join thread to main
	pthread_join(networkThread, NULL);
	return EXIT_SUCCESS;
}