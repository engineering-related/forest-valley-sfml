#include "Network.h"

sf::Mutex globalMutex;
sf::RectangleShape rect1, rect2;
sf::UdpSocket socket;
sf::IpAddress sendIp = "";
unsigned short port = 25565;
bool quit = false;

void handlePacketTraffic(void)
{
	static sf::Vector2f prevPosition, p2Position;
	while(!quit)
	{
		sf::Packet packet;

		//Send packet
		globalMutex.lock();
		if (prevPosition != rect1.getPosition())
			packet << rect1.getPosition().x << rect1.getPosition().y;
		globalMutex.unlock();

		socket.send(packet, sendIp, port);

		//Receive packet
		socket.receive(packet, sendIp, port);
		if(packet >> p2Position.x >> p2Position.y)
		{
			globalMutex.lock();
			rect2.setPosition(p2Position);
			prevPosition = rect1.getPosition();
			globalMutex.unlock();
		}
	}
}
/*
// ----- The client -----
// Create a socket and bind it to the port 55001
sf::UdpSocket socket;
socket.bind(55001);
// Send a message to 192.168.1.50 on port 55002
std::string message = "Hi, I am " + sf::IpAddress::getLocalAddress().toString();
socket.send(message.c_str(), message.size() + 1, "192.168.1.50", 55002);

// Receive an answer (most likely from 192.168.1.50, but could be anyone else)
char buffer[1024];
std::size_t received = 0;
sf::IpAddress sender;
unsigned short port;

socket.receive(buffer, sizeof(buffer), received, sender, port);
std::cout << sender.ToString() << " said: " << buffer << std::endl;
// ----- The server -----
// ----- The server -----
// Create a socket and bind it to the port 55002
sf::UdpSocket socket;
socket.bind(55002);
// Receive a message from anyone
char buffer[1024];

std::size_t received = 0;
sf::IpAddress sender;
unsigned short port;

socket.receive(buffer, sizeof(buffer), received, sender, port);
std::cout << sender.ToString() << " said: " << buffer << std::endl;
// Send an answer
std::string message = "Welcome " + sender.toString();
socket.send(message.c_str(), message.size() + 1, sender, port);
*/

int main(){
	sf::Thread* thread = 0;

	char connectionType;

	std::cout << "s)erver or c)lient" << std::endl;
	std::cin >> connectionType;

	//std::map<unsigned short, sf:IpAddress> computerID;

	socket.bind(port);
	//socket.setBlocking(false);

	if(connectionType == 's'){
		bool receviedIp = false;
		while(!receviedIp)
		{
			// Receive a message from anyone
			char buffer[1024];
			std::size_t received = 0;
			sf::IpAddress sender;
			unsigned short port;
			socket.receive(buffer, sizeof(buffer), received, sender, port);
			if(received > 0){
				std::cout << received << std::endl;
				std::cout << sender.toString() << " said: " << buffer << std::endl;
				receviedIp = true;
				sendIp = sender;
			}
		}
	}
	else
	{
		sendIp = "78.72.205.138";
		std::string message = "Hi, I am " + sf::IpAddress::getLocalAddress().toString();
		socket.send(message.c_str(), message.size() + 1, sendIp, port);
	}

	rect1.setSize(sf::Vector2f(20, 20));
	rect2.setSize(sf::Vector2f(20, 20));

	rect1.setFillColor(sf::Color::Red);
	rect2.setFillColor(sf::Color::Blue);

	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Packets");

	socket.setBlocking(false);

	thread = new sf::Thread(&handlePacketTraffic);
	thread->launch();

	while(window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape){
				quit = true;
				window.close();
			}
		}
		globalMutex.lock();
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			rect1.move(0.1f, 0.0f);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			rect1.move(-0.1f, 0.0f);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			rect1.move(0.0f, -0.1f);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			rect1.move(0.0f, 0.1f);

		window.draw(rect1);
		window.draw(rect2);
		globalMutex.unlock();

		window.display();
		window.clear();
	}
	if(thread)
	{
		thread->wait();
		delete thread;
	}
	return 0;
}


/*
#include <SFML/Network.hpp>

const unsigned short PORT = 5000;
const std::string IPADDRESS("192.168.0.100");//change to suit your needs

std::string msgSend;

sf::TcpSocket socket;
sf::Mutex globalMutex;
bool quit = false;

void DoStuff(void)
{
	static std::string oldMsg;
	while(!quit)
	{
		sf::Packet packetSend;
		globalMutex.lock();
		packetSend << msgSend;
		globalMutex.unlock();

		socket.send(packetSend);

		std::string msg;
		sf::Packet packetReceive;

		socket.receive(packetReceive);
		if ((packetReceive >> msg) && oldMsg != msg && !msg.empty())
		{
			std::cout << msg << std::endl;
			oldMsg = msg;
		}
	}
}

void Server(void)
{
	sf::TcpListener listener;
	listener.listen(PORT);
	listener.accept(socket);
	std::cout << "New client connected: " << socket.getRemoteAddress() << std::endl;
}

bool Client(void)
{
	if(socket.connect(IPADDRESS, PORT) == sf::Socket::Done)
	{
		std::cout << "Connected\n";
		return true;
	}
	return false;
}

void GetInput(void)
{
	std::string s;
	std::cout << "\nEnter \"exit\" to quit or message to send: ";
	getline(std::cin,s);
	if(s == "exit")
		quit = true;
	globalMutex.lock();
	msgSend = s;
	globalMutex.unlock();
}


int main(int argc, char* argv[])
{
	sf::Thread* thread = 0;

	char who;
	std::cout << "Do you want to be a server (s) or a client (c) ? ";
	std::cin  >> who;

	if (who == 's')
		Server();
	else
		Client();

	thread = new sf::Thread(&DoStuff);
	thread->launch();

	while(!quit)
	{
		GetInput();
	}

	if(thread)
	{
		thread->wait();
		delete thread;
	}
	return 0;
}*/

