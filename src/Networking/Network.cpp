#include "Network.h"



int main(){
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	char connectionType;
	char mode = 'r';

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