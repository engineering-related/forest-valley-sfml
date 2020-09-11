#include "ENetClient.h"

ENetClient::ENetClient(/* args */)
{
	init();
}

ENetClient::~ENetClient()
{

}

/*virtual*/ int ENetClient::init()
{
	host = enet_host_create(NULL, 1, channels, 0, 0);

    if(host == NULL)
    {
        fprintf(stderr, "An error occurred while trying to create an Enet client host!\n");
        return EXIT_FAILURE;
    }
	return EXIT_SUCCESS;
}

sf::Packet ENetClient::getDataFromRequest(const Request &request)
{
    sf::Packet clientData;
    clientData << (sf::Uint8)PacketType::PLAYER_STATE << ENetID;

    pthread_mutex_lock(&game->ENetMutex);
        clientData << game->players[ENetID]->playerState;
    pthread_mutex_unlock(&game->ENetMutex);

    //Evaluate the type of request and send the appropriate packet
    switch (request.playerSnapshot.type)
    {
    case RequestType::IDLE:
        //No more data needs to be added
        break;
    case RequestType::MOVE:
        //No more data needs to be added
        break;
    case RequestType::DEL:
        //Data about what objects the player wants to delete needs to be added!
    default:
        break;
    }
    return clientData;
}

void ENetClient::sendRequestToServer(const Request& request)
{
    sendPacket(peer, 0, getDataFromRequest(request));
}

void ENetClient::addRequest(const Request& request)
{
    requestQueue.push_back(request);
}

void ENetClient::checkPlayerState()
{
    //Make sure game is still running
    if(game != NULL && game->getGameLoopRunning() && getTheadLoopRunning())
    {
        //If the player changed state, then send a request to the server
        if(game->players[ENetID]->changedState)
        {
            pthread_mutex_lock(&game->ENetMutex);

                game->players[ENetID]->changedState = false;
                //Dont send a request when the player is idle, endPos is already known
                if(game->players[ENetID]->currentStateType != ENetTestPlayer::StateType::IDLE)
                    addRequest(game->players[ENetID]->playerState);

            pthread_mutex_unlock(&game->ENetMutex);
        }
    }
}

void ENetClient::addPlayer(sf::Packet& packet)
{
    sf::Uint16 pENetID;
    packet >> pENetID;

    //Build player from data
	ENetTestPlayer* pPlayer = new ENetTestPlayer();
    packet >> pPlayer;

    //Print message
    std::string playerConnectedMessage = "Player " + pPlayer->playerID + " connected to the server.";
    puts(playerConnectedMessage.c_str());

    //Add player to game
	game->addPlayer(pENetID, pPlayer);
}

void ENetClient::removePlayer(sf::Packet& packet)
{
    sf::Uint16 pENetID;
    packet >> pENetID;

    //Print message
    std::string playerDisconnectedMessage = "Player " + game->players[pENetID]->playerID + " disconnected from the server.";
    puts(playerDisconnectedMessage.c_str());

    //remove player from game
    game->removePlayer(pENetID);
}

void ENetClient::hostDisconnected(sf::Packet& packet)
{
    sf::Uint16 hENetID;
    packet >> hENetID;

    //Print message 1
    std::string playerDisconnectedMessage = "Host " + game->players[ENetID]->playerID + " disconnected.";
    puts(playerDisconnectedMessage.c_str());

    //Dont need to dissconect if host quit the session
    setShouldDisconnect(false);

    //Print message 2
    playerDisconnectedMessage = "Exiting session.";
    puts(playerDisconnectedMessage.c_str());

    //Clean up resources
    event.peer->data = NULL;

    //Exit the game
    game->setGameLoopRunning(false);
}

void ENetClient::changePlayerID(sf::Packet& packet)
{
    sf::Uint16 old_p_ENetID, new_p_ENetID;
    packet >> old_p_ENetID >> new_p_ENetID;

    //Change ID of player
    game->changePlayerID(old_p_ENetID, new_p_ENetID);

    //Update your own ENetID if the target was you
    if(old_p_ENetID == ENetID)
        ENetID = new_p_ENetID;
}

/*virtual*/ void ENetClient::handleReceiveEvent(ENetEvent* event)
{
    sf::Packet packet;
    sf::Uint8 recievedPacketType;

    packet.append(event->packet->data, event->packet->dataLength);
    packet >> recievedPacketType;

	//Perform action based on the given packet-type
	switch ((PacketType)recievedPacketType)
	{
        case PacketType::GAME_STATE:
            game->setChangedStateData(packet);
            break;

        case PacketType::PLAYER_CONNECTED:
            addPlayer(packet);
            break;

        case PacketType::GAME_DATA:
            game->setGameData(packet);
            break;

        case PacketType::PLAYER_DISCONNECTED:
            removePlayer(packet);
            break;

        case PacketType::HOST_DISCONNECTED:
            hostDisconnected(packet);
            break;
        case PacketType::PLAYER_CHANGED_ID:
            changePlayerID(packet);
            break;
        //WARNING: Undefined, could be useful later
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

/*virtual*/ void ENetClient::handleDisconnectEvent(ENetEvent* event)
{
    //Print message
    printf("Lost connection to host %x:%u.\n",
            event->peer->address.host,
            event->peer->address.port);

    setThreadLoopRunning(false);
    game->setGameLoopRunning(false);

    /*Reset the peer's host information. */
    event->peer->data = NULL;
}

/*virtual*/ void ENetClient::receiveEvents()
{
    while(enet_host_service(host, &event, NONE_BLOCKING) > 0)
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_RECEIVE:
                handleReceiveEvent(&event);
                break;

            default:
                break;
        }
    }
}

/*virtual*/ void ENetClient::sendPackets()
{
    checkPlayerState();
    if(clock.getElapsedTime().asMilliseconds() >= 1000/requestTickRate)
    {
        clock.restart().asMilliseconds();
        if(requestQueue.size() > 0)
        {
             //Send the first request in queue to the server
            sendRequestToServer(requestQueue[0]);

            //Remove the request from queue once it's been sent
            requestQueue.erase(requestQueue.begin());
        }
    }
}

int ENetClient::connect()
{
	//Set the host to local IP-addres
    enet_address_set_host(&address, SERVER_IP);

    //Connect to the peer/server
    peer = enet_host_connect(host, &address, channels, 0);
    if(peer == NULL)
    {
        fprintf(stderr, "No available peers for initiating an Enet connection!\n");
        return EXIT_FAILURE;
    }
    //Check if the server has contacted us back
    std::string message = "Connection to " + (std::string)SERVER_IP + ":" + std::to_string(PORT);

    if(enet_host_service(host, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        message += " established.";
        puts(message.c_str());

        sf::Packet packet;
        packet <<
        (sf::Uint8)PacketType::PLAYER_CONNECTED << ENetID << game->players[ENetID];

        sendPacket(peer, 2, packet);
        return EXIT_SUCCESS;
    }
    else
    {
        message += " failed.";
        puts(message.c_str());
        return EXIT_FAILURE;
    }
}

/*virtual*/ int ENetClient::disconnect()
{
    setThreadLoopRunning(false);
    enet_peer_disconnect(peer, 0);

    while(enet_host_service(host, &event, 3000) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_RECEIVE:
            //Clean up the packet now that we're done using it.
            enet_packet_destroy(event.packet);
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            /*Reset the peer's client information. */
            event.peer -> data = NULL;
            puts("Disconnection succeeded.");
            return EXIT_SUCCESS;
            break;

         default:
            break;
        }
    }
    /* We've arrived here, so the disconnect attempt didn't */
    /* succeed yet. Force the connection down.             */
    enet_peer_reset (peer);
    return EXIT_FAILURE;
}