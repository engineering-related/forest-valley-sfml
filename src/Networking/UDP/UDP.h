#ifndef NETWORK_UDP
#define NETWORK_UDP

#include "Networking/Network.h"

class UDP: public Network
{
private:

protected:
	sf::UdpSocket socket;

public:
	struct TestPlayer
	{
		sf::RectangleShape rect;
		sf::Vector2f endPos;
		sf::Vector2f velocity;
		sf::Vector2f p2Pos;
		bool mouseClicked;
		float speedMagnitude;

		TestPlayer(sf::Color color)
		{
			rect.setSize(sf::Vector2f(20, 20));
			rect.setFillColor(color);
			rect.setPosition(0.f, 0.f);
			rect.setOrigin(rect.getSize()*0.5f);
			this->speedMagnitude = 200.f;
			this->mouseClicked = false;
			this->endPos = this->rect.getPosition();
			this->velocity = sf::Vector2f(0.f, 0.f);
		}

		void draw(sf::RenderTarget* target)
		{
			target->draw(rect);
		}
		void update(const float &dt)
		{
			bool move = false;
			if ((this->velocity.x >= 0 && this->rect.getPosition().x < endPos.x) ||
				(this->velocity.x <= 0 && this->rect.getPosition().x > endPos.x) ||
				(this->velocity.y >= 0 && this->rect.getPosition().y < endPos.y) ||
				(this->velocity.y <= 0 && this->rect.getPosition().y > endPos.y))
					move = true;
			if(move)
				this->rect.move(this->velocity*dt);
		}
		void handleMouse(sf::RenderWindow* window)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window->getViewport(window->getView()).contains(mousePos) && !mouseClicked)
			{
				mouseClicked = true;
				endPos = (sf::Vector2f)mousePos;
				float angle = atan2f(endPos.y - this->rect.getPosition().y, endPos.x - this->rect.getPosition().x);
				velocity = sf::Vector2f(speedMagnitude*cos(angle), speedMagnitude*sin(angle));
			}
			else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
				mouseClicked = false;
		}

		void updatePlayers(const float& dt, TestPlayer* p2)
		{
			sf::Vector2f d(p2Pos.x - p2->rect.getPosition().x, p2Pos.y - p2->rect.getPosition().y);
			if(abs(d.x) >= 1 && abs(d.y) >= 1)
			{
				float angle = atan2f(d.y, d.x);
				p2->velocity = sf::Vector2f(p2->speedMagnitude * cos(angle), p2->speedMagnitude * sin(angle));
				p2->rect.move(p2->velocity * dt);
			}
		}
		void drawPlayers(sf::RenderTarget* target, TestPlayer* p2)
		{
			target->draw(p2->rect);
		}
	};
	TestPlayer* p1;
	TestPlayer *p2;
	UDP();
	virtual ~UDP();
	void run();
	static void handlePacketTraffic(UDP* UDP_Network);
};

#endif