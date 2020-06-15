#include "Network.h"



int main(){
	sf::IpAddress ip = sf::IpAddress::getLocalAddress(); //Use IP as client "78.72.205.138"
	sf::TcpSocket socket;
	char connectionType;

	std::cout << "s)erver or c)lient" << std::endl;
	std::cin >> connectionType;

	short int port = 25565;

	if(connectionType == 's'){
		sf::TcpListener listener;
		listener.listen(port);
		listener.accept(socket);
	}
	else
		socket.connect(ip, port);

	sf::RectangleShape rect1, rect2;
	rect1.setSize(sf::Vector2f(20, 20));
	rect2.setSize(sf::Vector2f(20, 20));

	rect1.setFillColor(sf::Color::Red);
	rect2.setFillColor(sf::Color::Blue);

	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Packets");
	sf::Vector2f prevPosition, p2Position;

	socket.setBlocking(false);

	while(window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape)
				window.close();
		}

		prevPosition = rect1.getPosition();


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			rect1.move(0.5f, 0.0f);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			rect1.move(-0.5f, 0.0f);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			rect1.move(0.0f, -0.5f);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			rect1.move(0.0f, 0.5f);

		sf::Packet packet;
		if (prevPosition != rect1.getPosition()){
			packet << rect1.getPosition().x << rect1.getPosition().y;
			socket.send(packet);
		}

		socket.receive(packet);
		 if(packet >> p2Position.x >> p2Position.y)
			 rect2.setPosition(p2Position);


		window.draw(rect1);
		window.draw(rect2);

		window.display();
		window.clear();
	}

	return 0;
}
