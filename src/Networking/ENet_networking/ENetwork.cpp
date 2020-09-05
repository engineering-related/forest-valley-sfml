#include "ENetwork.h"

ENetwork::ENetwork(/* args */)
{
	initENet();
	ENetID = util::fn::random_string(keyCharacterLength);
	address.port = 24474;
	game = new ENetTestGame(this);
	game->initTestPlayer(&ENetID);
	shouldDissconnect = true;
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

ENetwork::DataVec ENetwork::extractData(enet_uint8* data)
{
	//Conver data to c-string
	DataString* convertedData = reinterpret_cast<DataString*>(data);

	//Split the string into a vector
	return util::fn::stringSplitSpace(std::string(convertedData));
}


ENetwork::DataString* ENetwork::compressData(const DataVec& dataVec)
{
	std::string data;
	for(auto s: dataVec)
	{
		data += s + " ";
	}
	return strdup(data.c_str());
}

void ENetwork::sendPacket(ENetPeer* peer, enet_uint8 channel, DataString* data)
{
	/*WARNING: When sending packets use multiple channels. Data from players->server are sent -
	  on 0. Data from server->players are sent on 1. Other types of data are send on 2*/
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, channel, packet);
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

void ENetwork::printPacketData(DataVec dataVec)
{
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

	//Create mutex
	if (pthread_mutex_init(&game->ENetMutex, NULL) != 0) {
    	fprintf(stderr, "\n mutex init has failed\n");
    	return EXIT_FAILURE;
    }

	//Game loop
	setThreadLoopRunning(true);
	game->loop();
	setThreadLoopRunning(false);

	disconnect();

	pthread_join(networkThread, NULL);
	return EXIT_SUCCESS;
}