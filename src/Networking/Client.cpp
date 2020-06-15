#include "Client.h"


/*
int notMain(){
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;

	sf::SocketSelector selector;

	char connectionType;
	char mode = 'r';
	std::vector<sf::TcpSocket*> clients;

	short int port = 25565;
	char buffer[2000];
	std::size_t received;

	std::string text = "Conected to: ";

	std::cout << "s)erver or c)lient" << std::endl;
	std::cin >> connectionType;

	if(connectionType == 's'){
		sf::TcpListener listener;
		listener.listen(port);
		listener.accept(socket);
		text += "server ";
		mode = 's';
	}
	else if(connectionType == 'c'){
		socket.connect(ip, port);
		text += " client";
		mode = 'r';
	}

	socket.send(text.c_str(), text.length() +1);

	socket.receive(buffer, sizeof(buffer), received);
	std::cout << buffer << std::endl;

	bool done = false;
	socket.setBlocking(false);

	while (!done){
		if(mode == 's'){
			std::getline(std::cin, text);
			socket.send(text.c_str(), text.length() +1);
			mode = 'r';
		}
		else if (mode == 'r')
		{
			socket.receive(buffer, sizeof(buffer), received);
			if(received > 0){
				std::cout << buffer << std::endl;
				mode = 's';
			}

		}

	}


	return 0;
}


int runClient(){

	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	short int port = 25565;

	bool done = false;
	std::string id;
	std::string text = "";

	std::cout << "Enter online id: ";
	std::cin >> id;

	socket.connect(ip, port);

	std::vector<sf::Text> chat;
	sf::Packet packet;
	packet << id;
	socket.send(packet);
	socket.setBlocking(false);

	while (!done)
	{

	}


	return 0;
}*/