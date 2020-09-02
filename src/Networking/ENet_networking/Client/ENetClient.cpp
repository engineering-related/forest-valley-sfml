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
    std::string data;

    //Always send essential data about the player
    //Player and request data
    data += player->id + " ";
    data += std::to_string(request.id) + " ";
    data += std::to_string(request.time) + " ";
    data += std::to_string(request.playerSnapshot.type) + " ";
    //StartPosition
    data += std::to_string(request.playerSnapshot.rect.getPosition().x) + " ";
    data += std::to_string(request.playerSnapshot.rect.getPosition().y) + " ";
    //Velocity
    data += std::to_string(request.playerSnapshot.velocity.x) + " ";
    data += std::to_string(request.playerSnapshot.velocity.y) + " ";
    //Endpos
    data += std::to_string(request.playerSnapshot.endPos.x) + " ";
    data += std::to_string(request.playerSnapshot.endPos.y) + " ";

    //Evaluate the type of request and send the appropriate packet
    switch (request.playerSnapshot.type)
    {
    case RequestType::IDLE:
        //No more data needs to be added
        break;
    case RequestType::MOVE:
        //No more data needs to be added
        break;
    case RequestType::DELETE:
        //Data about what objects the player wants to delete needs to be added!
    default:
        break;
    }
    return strdup(data.c_str());
}

void ENetClient::sendRequestToServer(const Request& request)
{
    sendPacket(peer, 0, getDataFromRequest(request));
}

void ENetClient::addRequest(const Request& request)
{
    requestQueue.push_back(request);
    player->changedState = false;
}

void ENetClient::checkPlayerState()
{
    if(player->changedState)
    {
        addRequest(player->state);
    }
}

/*virtual*/ void ENetClient::receiveEvents()
{
    //Recieve data
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
                printf("Host %x:%u disconnected.\n",
                        event.peer ->address.host,
                        event.peer ->address.port);
                setThreadsLoopRunning(false);
                setGameLoopRunning(false);
                /*Reset the peer's client information. */
                event.peer -> data = NULL;
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