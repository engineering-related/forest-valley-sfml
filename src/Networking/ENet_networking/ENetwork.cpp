#include "ENetwork.h"

ENetwork::ENetwork(/* args */)
{
	initENet();
	address.port = 24474;
	delay = 0;
	this->window = new sf::RenderWindow();
}

ENetwork::~ENetwork()
{
	enet_host_destroy(host);
	delete this->window;
}

bool ENetwork::initENet()
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

void ENetwork::sendPacket(ENetPeer* peer, enet_uint8 channel, const char* data)
{
	//When sending a packets use 2 channels. Data from players->server are sent -
	//on first. Data from server->players are sent on second.
	ENetPacket* packet = enet_packet_create(data, strlen(data) + 1, ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, channel, packet);
}

void ENetwork::updatePlayers(const float &dt)
{

}

void ENetwork::drawPlayers(sf::RenderTarget* target)
{

}

void ENetwork::run()
{
	//Create window
	window->create(sf::VideoMode(800, 600, 32), "ENet Testing");
	window->setFramerateLimit(300);
	//this->thread->launch();
	while (window->isOpen())
	{
		//Networking, Overriden function (should be put in a separte thread later)
		receiveEvents();

		//SFML-events
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
				disconnect();
			}
			if(event.type == sf::Event::Resized)
			{
				sf::Vector2f size = static_cast<sf::Vector2f>(window->getSize());
				window->setSize(static_cast<sf::Vector2u>(size));
				sf::View newView(sf::FloatRect(0.f, 0.f, size.x, size.y));
				window->setView(newView);
			}
		}

		//Game clock
		dt = gameClock.restart().asSeconds();
		float FPS = 1.f / dt;
		static float incer = 0;
		static float cap = 5;
		incer += dt;
		if (incer >= cap)
		{
			incer = 0;
			window->setTitle("ENet Testing | FPS: " + std::to_string(FPS));
		}

		//Rendering

		/*window.clear();
		globalMutex.lock();
		this->player->handleMouse(&window);
		this->updatePlayers(dt);
		this->drawPlayers(&window);
		globalMutex.unlock();
		window.display();*/

		window->clear();
		window->display();
	}
	/*if (thread)
	{
		thread->wait();
		delete thread;
	}*/
}