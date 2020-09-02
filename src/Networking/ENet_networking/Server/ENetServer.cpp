#include "ENetServer.h"


ENetServer::ENetServer(/* args */)
{
	init();
}

ENetServer::~ENetServer()
{

}

/*virtual*/ int ENetServer::init()
{
	//Initialize Server
	address.host = ENET_HOST_ANY;
    host = enet_host_create(&address, MaxNumberOfPlayers, channels, 0, 0);

    if(host == NULL)
    {
        fprintf(stderr, "An error occurred while trying to create an Enet server host!\n");
        return EXIT_FAILURE;
    }
	return EXIT_SUCCESS;
}

//Send a packet to all peers
void ENetServer::brodcastPacket(const char* data)
{
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(host, 1, packet);
}

/*virtual*/ void ENetServer::receiveEvents()
{
	while(enet_host_service(host, &event, NONE_BLOCKING) > 0)
	{
		switch (event.type)
		{
			case ENET_EVENT_TYPE_CONNECT:
				printf("A new client connected from %x:%u.\n",
						event.peer -> address.host,
						event.peer -> address.port);
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				printf("A packet of length %lu containing %s was received from %x:%u on channel %u.\n",
					event.packet->dataLength,
					event.packet->data,
					event.peer->address.host,
					event.peer->address.port,
					event.channelID);
					/* Clean up the packet now that we're done using it. */
					enet_packet_destroy(event.packet);
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				printf("%x:%u disconnected.\n",
						event.peer ->address.host,
						event.peer ->address.port);
				/*Reset the peer's client information. */
				event.peer -> data = NULL;
				break;

			default:
				break;
		}
	}
}

/*virtual*/ void ENetServer::sendPackets()
{
	static int packetsSent = 0;
	if(clock.getElapsedTime().asMilliseconds() >= 1000/tickRate)
	{
		clock.restart().asMilliseconds();

		std::string serverData = std::to_string(++packetsSent);
		brodcastPacket(serverData.c_str());
	}
}


/*virtual*/ int ENetServer::disconnect()
{
	puts("Disconnection succeeded.");
	return EXIT_SUCCESS;
}