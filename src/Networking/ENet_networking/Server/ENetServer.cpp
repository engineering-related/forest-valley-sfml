#include "ENetServer.h"


ENetServer::ENetServer(/* args */)
{
	init();
}

ENetServer::~ENetServer()
{

}

/*Virtual*/ bool ENetServer::init()
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

void ENetServer::brodcastPacket(const char* data)
{
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(host, 1, packet);
}

/*virtual*/ void ENetServer::receiveEvents()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
   {
       int random = rand() % 100000;
       std::string str = std::to_string(random);
       brodcastPacket(str.c_str());
   }
	while(enet_host_service(host, &event, delay) > 0)
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

/*Virtual*/ bool ENetServer::disconnect()
{
	window->close();
	return EXIT_SUCCESS;
}