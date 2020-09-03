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


/*virtual*/ void ENetServer::handleConnectionEvent(ENetEvent* event)
{
	printf("A new client connected from %x:%u.\n",
			event->peer->address.host,
			event->peer->address.port);
}

/*virtual*/ void ENetServer::handleDisconnectEvent(ENetEvent* event)
{
	printf("%x:%u disconnected.\n",
			event->peer->address.host,
			event->peer->address.port);

	/*Reset the peer's client information. */
	event->peer->data = NULL;
}

/*virtual*/ void ENetServer::receiveEvents()
{
	while(enet_host_service(host, &event, NONE_BLOCKING) > 0)
	{
		switch (event.type)
		{
			case ENET_EVENT_TYPE_CONNECT:
				handleConnectionEvent(&event);
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				handleReceiveEvent(&event);
				break;
			case ENET_EVENT_TYPE_DISCONNECT:
				handleDisconnectEvent(&event);
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

		std::string serverData;

		//Player and request data
		pthread_mutex_lock(&game->ENetMutex);
		serverData += std::to_string(PacketType::PLAYER_STATE) + " ";
		serverData += ID + " ";
		serverData += std::to_string(packetsSent++) + " ";
		serverData += std::to_string(util::fn::getTimeInMsSinceEpoch().count()) + " ";
		serverData += std::to_string(game->players[ID]->currentStateType) + " ";
		//StartPosition
		serverData += std::to_string(game->players[ID]->rect.getPosition().x) + " ";
		serverData += std::to_string(game->players[ID]->rect.getPosition().y) + " ";
		//Velocity
		serverData += std::to_string(game->players[ID]->velocity.x) + " ";
		serverData += std::to_string(game->players[ID]->velocity.y) + " ";
		//Endpos
		serverData += std::to_string(game->players[ID]->endPos.x) + " ";
		serverData += std::to_string(game->players[ID]->endPos.y) + " ";
		pthread_mutex_unlock(&game->ENetMutex);
		brodcastPacket(serverData.c_str());
	}
}


/*virtual*/ int ENetServer::disconnect()
{
	puts("Disconnection succeeded.");
	return EXIT_SUCCESS;
}