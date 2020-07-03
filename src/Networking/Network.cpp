#include "Network.h"

Network::Network()
{
	this->port = 24474;
	this->quit = false;
	this->UDP_send_packet = true;
	this->TCP_send_packet = true;
	this->delay = 20;

	//Player creation
	srand(time(NULL));
	sf::Color playerColor = sf::Color(rand() % 255, rand() % 255, rand() % 255);
	this->player = new TestPlayer(playerColor);
}

Network::~Network()
{
	for (auto p : players)
		delete p.second;
	delete player;
}

void Network::updatePlayers(const float &dt)
{
	for (auto p : players)
	{
		sf::Vector2f d(p.second->player->p2Pos.x - p.second->player->rect.getPosition().x,
					   p.second->player->p2Pos.y - p.second->player->rect.getPosition().y);
		if(abs(d.x) >= 1 && abs(d.y) >= 1)
		{
			float angle = atan2f(d.y, d.x);
			p.second->player->velocity = sf::Vector2f(p.second->player->speedMagnitude * cos(angle),
													  p.second->player->speedMagnitude * sin(angle));
			p.second->player->rect.move(p.second->player->velocity * dt);
		}
	}
	this->player->update(dt);
}

void Network::drawPlayers(sf::RenderTarget* target)
{
	for (auto p : players)
	{
		p.second->player->draw(target);
	}
	this->player->draw(target);
}


void Network::start()
{
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Packets");
	window.setFramerateLimit(144);
	this->thread->launch();
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				quit = true;
				window.close();
			}
			if(event.type == sf::Event::Resized)
			{
				sf::Vector2f size = static_cast<sf::Vector2f>(window.getSize());
				window.setSize(static_cast<sf::Vector2u>(size));
				sf::View newView(sf::FloatRect(0.f, 0.f, size.x, size.y));
				window.setView(newView);
			}
		}
		if(quit)
			window.close();

		dt = gameClock.restart().asSeconds();
		float FPS = 1.f / dt;
		static float incer = 0;
		static float cap = 5;
		incer += dt;
		if (incer >= cap)
		{
			incer = 0;
			window.setTitle("Forest Valley | FPS: " + std::to_string(FPS));
		}

		globalMutex.lock();
		this->player->handleMouse(&window);
		this->updatePlayers(dt);
		this->drawPlayers(&window);
		globalMutex.unlock();
		window.display();
		window.clear();
	}
	if (thread)
	{
		thread->wait();
		delete thread;
	}
}
