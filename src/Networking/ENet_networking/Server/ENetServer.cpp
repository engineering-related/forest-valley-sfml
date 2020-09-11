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
void ENetServer::brodcastPacket(const size_t& channel, const sf::Packet& packet)
{
	ENetPacket* eNetPacket = enet_packet_create(packet.getData(), packet.getDataSize(), ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(host, channel, eNetPacket);
}

void ENetServer::handlePlayerState(sf::Packet& packet)
{
	std::string pENetID;
	ENetTestPlayer::State playerState;
	packet >> pENetID >> playerState;

	evaluatePlayerState(pENetID, playerState);
	updatePlayerState(pENetID, playerState);
	receivedDuringTick = true;
}

//Check if the action the player perfomed is valid (saftey & anti-cheat)
void ENetServer::evaluatePlayerState(const std::string& pENetID, ENetTestPlayer::State& pState)
{
	//If the playerstate is incorrect reset it to last state (client-side prediction)
}

void ENetServer::updatePlayerState(const std::string& pENetID, ENetTestPlayer::State& pState)
{
	game->players[pENetID]->setPlayerState(pState);
}

void ENetServer::addPlayerToServer(sf::Packet& packet, ENetPeer* peer)
{
	//Build player from data
	sf::Packet sendServerPacket;
	std::string pENetID;
	ENetTestPlayer* pPlayer = new ENetTestPlayer();

	packet >> pENetID >> pPlayer;
	sendServerPacket << (sf::Uint8)PacketType::PLAYER_CONNECTED << pENetID << pPlayer;

	//Send to all other peers that a player has been added (use channel 2)
	for(auto p: peers)
	{
		sendPacket(p.first, 2, sendServerPacket);
	}

	//Refresh the gameState
	game->refreshState();

	//Send back to the new connect client the Game-Data (use channel 2)
	sendPacket(peer, 2, game->getGameData(ENetID, PacketType::GAME_DATA));

	//Add peer to server
	peers[peer] = pENetID;

	//Add player to the game
	game->addPlayer(pENetID, pPlayer);
}

void ENetServer::removePlayerFromServer(ENetPeer* peer)
{
	sf::Packet packet;
	packet << (sf::Uint8)PLAYER_DISCONNECTED << peers[peer];

	brodcastPacket(2, packet);

	game->removePlayer(peers[peer]);

	/*Reset the peer's client information. */
	peers.erase(peer);
	peer->data = NULL;
}

/*virtual*/ void ENetServer::handleReceiveEvent(ENetEvent* event)
{
	sf::Packet packet;
    sf::Uint8 recievedPacketType;

    packet.append(event->packet->data, event->packet->dataLength);

	//Control check packet
	//evaluateReceivedPacket(packet);

	packet >> recievedPacketType;

	//Perform action based on the given packet-type
	switch (recievedPacketType)
	{
		case PacketType::PLAYER_STATE:
			handlePlayerState(packet);
			break;
		case PacketType::PLAYER_CONNECTED:
			addPlayerToServer(packet, event->peer);
			break;
		default:
			break;
	}

	//Destory packet
	enet_packet_destroy(event->packet);
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

	removePlayerFromServer(event->peer);
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
	//Send data to clients based on server-tickrate
	if(clock.getElapsedTime().asMilliseconds() >= 1000/tickRate)
	{
		clock.restart().asMilliseconds();

		std::string serverData;
		//Send data to clients if any player-state has been changed
		pthread_mutex_lock(&game->ENetMutex); //LORD HAVE MERCY ON MY DISGUSTING CODE
			if((receivedDuringTick || (game->players[ENetID]->changedState &&
				game->players[ENetID]->currentStateType != ENetTestPlayer::StateType::IDLE)) &&
				peers.size() > 0)
			{
				//Refresh the game-state for server
				game->refreshState();

				//Send packet to peers
				d();
				brodcastPacket(1, game->getChangedStateData(ENetID, PacketType::GAME_STATE));

				//Reset send checkers
				receivedDuringTick = false;
			}
		pthread_mutex_unlock(&game->ENetMutex);
	}
}

/*virtual*/ int ENetServer::disconnect()
{
	sf::Packet packet;
	packet << (sf::Uint8) PacketType::HOST_DISCONNECTED << ENetID;
	brodcastPacket(2, packet);
	puts("Disconnection succeeded.");
	return EXIT_SUCCESS;
}