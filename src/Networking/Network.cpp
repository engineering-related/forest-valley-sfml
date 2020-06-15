#include "Network.h"



int main(){
	sf::IpAddress ip = sf::IpAddress::getLocalAddress();
	sf::TcpSocket socket;
	char connectionType, mode;

	short int port = 3203;
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
	}
	else if(connectionType == 'c'){
		socket.connect(ip, port);
		text += " client";
	}

	socket.send(text.c_str(), text.length() +1);

	socket.receive(buffer, sizeof(buffer), received);

	std::cout << buffer << std::endl;

	system("pause");

	return 0;
}