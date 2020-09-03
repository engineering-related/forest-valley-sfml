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

const char* ENetClient::getDataFromRequest(const Request &request)
{
    std::string clientData;
    //Always send essential data about the player
    //Player and request data
    clientData += std::to_string(PacketType::PLAYER_STATE) + " ";
    clientData += ID + " ";

    pthread_mutex_lock(&game->ENetMutex);
    clientData += game->players[ID]->state.getData();
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
    return strdup(clientData.c_str());
}

void ENetClient::sendRequestToServer(const Request& request)
{
    sendPacket(peer, 0, getDataFromRequest(request));
}

void ENetClient::addRequest(const Request& request)
{
    requestQueue.push_back(request);

    pthread_mutex_lock(&game->ENetMutex);
    game->players[ID]->changedState = false;
    pthread_mutex_unlock(&game->ENetMutex);
}

void ENetClient::checkPlayerState()
{
    //Make sure game is still running
    if(game != NULL && game->getGameLoopRunning() && getTheadLoopRunning())
    {
        //If the player changed state, then send a request to the server
        if(game->players[ID]->changedState)
            addRequest(game->players[ID]->state);
    }
}

/*virtual*/ void ENetClient::handleDisconnectEvent(ENetEvent* event)
{
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
    //Recieve data
    while(enet_host_service(host, &event, NONE_BLOCKING) > 0)
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_RECEIVE:
                ENetwork::handleReceiveEvent(&event);
                break;

            case ENET_EVENT_TYPE_DISCONNECT:
                this->handleDisconnectEvent(&event);
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
        //Send the first request int he queue to the server
        if(requestQueue.size() > 0)
        {
            sendRequestToServer(requestQueue[0]);
            //Remove the request from queue once it's been sent
            requestQueue.erase(requestQueue.begin());
        }
    }
}

int ENetClient::connect()
{
	//Set the host to local IP-addres
    enet_address_set_host(&address, "192.168.1.104");

    //Connect to the peer/server
    peer = enet_host_connect(host, &address, channels, 0);
    if(peer == NULL)
    {
        fprintf(stderr, "No available peers for initiating an Enet connection!\n");
        return EXIT_FAILURE;
    }

    //Check if the server has contacted us back
    if(enet_host_service(host, &event, 5000) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        puts("Connection to 192.168.1.104:24474 established.");
        return EXIT_SUCCESS;
    }
    else
    {
        enet_peer_reset(peer);
        puts("Connection to 192.168.1.104:24474 failed.");
        return EXIT_FAILURE;
    }
}

/*virtual*/ int ENetClient::disconnect()
{
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