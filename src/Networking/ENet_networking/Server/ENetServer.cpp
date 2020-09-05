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
void ENetServer::brodcastPacket(DataString* data, const size_t& channel)
{
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_host_broadcast(host, channel, packet);
}

void ENetServer::handlePlayerState(const DataVec& playerDataVec)
{
	evaluatePlayerState(playerDataVec);
	updatePlayerState(playerDataVec);
	receivedDuringTick = true;
}

//Check if the action the player perfomed is valid (saftey & anti-cheat)
void ENetServer::evaluatePlayerState(const DataVec& playerDataVec)
{
	//If the playerstate is incorrect reset it to last state (client-side prediction)
}

void ENetServer::updatePlayerState(const DataVec& playerDataVec)
{
	std::string pENetID = playerDataVec[1];
	game->players[pENetID]->setPlayerData(playerDataVec);
}

void ENetServer::addPlayerToServer(const DataVec& playerDataVec, ENetPeer* peer)
{
	//Build player from data
	ENetTestPlayer* pPlayer = ENetTestPlayer::buildPlayerFromData(playerDataVec);
	std::string pENetID = playerDataVec[1];

	//Send to all other peers that a player has been added (use channel 2)
	brodcastPacket(pPlayer->getPlayerData(pENetID, PLAYER_CONNECTED), 2);


	//Send back to the new connect client the Game-Data (use channel 2)
	sendPacket(peer, 2, game->getGameData(ENetID, PacketType::GAME_DATA));

	//Add peer to server
	peers[peer] = pENetID;

	//Add player to game
	game->addPlayer(pENetID, pPlayer);
}

void ENetServer::removePlayerFromServer(ENetPeer* peer)
{
	DataVec diconnectedDataVec;
	diconnectedDataVec.push_back(std::to_string(PacketType::PLAYER_DISCONNECTED));
	diconnectedDataVec.push_back(ENetID);
	diconnectedDataVec.push_back(peers[peer]);

	brodcastPacket(compressData(diconnectedDataVec), 2);
	/*Reset the peer's client information. */
	peer->data = NULL;
	game->removePlayer(peers[peer]);
	peers.erase(peer);
}

/*virtual*/ void ENetServer::handleReceiveEvent(ENetEvent* event)
{
	//Control packet data (security and data control)
	//evaluateReceivedPacket(event->packet);

	//Extract data to a vector of strings
	DataVec receivedDataVec = extractData(event->packet->data);

	//The first sequence in the data-string is always the type of the received packet
	PacketType recievedPacketType = (PacketType)std::stoi(receivedDataVec[0]);

	//Perform action based on the given packet-type
	switch (recievedPacketType)
	{
		case PacketType::PLAYER_STATE:
			handlePlayerState(receivedDataVec);
			break;
		case PacketType::PLAYER_CONNECTED:
			addPlayerToServer(receivedDataVec, event->peer);
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

	removePlayerFromServer(event->peer); //CHECK
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
		pthread_mutex_lock(&game->ENetMutex);
			if((receivedDuringTick || game->players[ENetID]->changedState) && peers.size() > 0)
			{
				//Refresh the game-state for server
				game->refreshState();

				//Send packet to peers
				brodcastPacket(game->getChangedStateData(ENetID, PacketType::GAME_STATE), 1);

				//Reset send checkers
				receivedDuringTick = false;
				game->players[ENetID]->changedState = false;
			}
		pthread_mutex_unlock(&game->ENetMutex);
	}
}


/*virtual*/ int ENetServer::disconnect()
{
	DataVec diconnectedDataVec;
	diconnectedDataVec.push_back(std::to_string(PacketType::HOST_DISCONNECTED));
	diconnectedDataVec.push_back(ENetID);
	diconnectedDataVec.push_back(game->players[ENetID]->playerID);

	brodcastPacket(compressData(diconnectedDataVec), 2);
	puts("Disconnection succeeded.");
	return EXIT_SUCCESS;
}