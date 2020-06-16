#include "Network.h"

const float MOVE_SPEED = 70;
const int PORT = 25565;

sf::TcpSocket socket;
sf::Mutex globalMutex;
sf::Packet serverPacket;

bool quit = false;

std::string v_command = "";

float v_offset_x = 0;
float v_offset_y = 0;


void serverListener();
void serverSend();
void clientListener();

void issueServer( std::string mode, std::string method )
{
    std::cout << "Incoming mode: " << mode << std::endl;
    std::cout << "Incoming method: " << method << std::endl;


    sf::Thread serverListenerThread(&serverListener);
    sf::Thread clientListenerThread(&clientListener);

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Playground");
    window.setFramerateLimit(60);

    sf::Clock Clock;
    sf::Clock PacketClock;
    int packet_counter = 0;

    sf::Time time;

    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(100, 50));
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(5);
    rectangle.setPosition(10, 20);

    int left_over_x = 0;
    int left_over_y = 0;
    float offset_x = 0;
    float offset_y = 0;
    std::string command = "";

    if( mode.compare("server") == 0 )
        serverListenerThread.launch();
    else
    {
        socket.connect("78.72.205.138",25565);
        clientListenerThread.launch();
    }


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                quit = true;

                if( mode.compare("server") == 0 )
                    serverListenerThread.terminate();
                else
                    clientListenerThread.terminate();

                window.close();
            }
        }
        if( mode.compare("server") == 0 )
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                rectangle.move(-1*MOVE_SPEED*time.asSeconds(),0);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                rectangle.move(0,1*MOVE_SPEED*time.asSeconds());

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                rectangle.move(1*MOVE_SPEED*time.asSeconds(),0);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                rectangle.move(0,-1*MOVE_SPEED*time.asSeconds());

        }
        else //client mode
        {
            if( v_command.compare("move") == 0 )
            {
                globalMutex.lock();
                command = v_command;

                offset_x = v_offset_x - rectangle.getPosition().x;
                offset_y = v_offset_y - rectangle.getPosition().y;

                v_command = "";
                v_offset_x = 0;
                v_offset_y = 0;
                globalMutex.unlock();

                left_over_x = offset_x;
                left_over_y = offset_y;

            } //end of if v_command is "move"

            float move_offset = MOVE_SPEED*time.asSeconds();
            //interpolation
            if( left_over_x != 0 )
            {
                if( left_over_x < 0 )
                {
                    if( (left_over_x + move_offset) < 0 )
                    {
                        rectangle.move(-1*move_offset,0);
                        left_over_x += move_offset;
                    }
                    else
                    {
                        rectangle.move(left_over_x,0);
                        left_over_x = 0;
                    }
                }
                else if( left_over_x > 0 )
                {
                    if( (left_over_x - move_offset) > 0 )
                    {
                        rectangle.move(move_offset,0);
                        left_over_x -= move_offset;
                    }
                    else
                    {
                        rectangle.move(left_over_x,0);
                        left_over_x = 0;
                    }
                }
            }
            if( left_over_y != 0 )
            {
                if( left_over_y < 0 )
                {
                    if( (left_over_y + move_offset) < 0 )
                    {
                        rectangle.move(0,-1*move_offset);
                        left_over_y += move_offset;
                    }
                    else
                    {
                        rectangle.move(0,left_over_y);
                        left_over_y = 0;
                    }
                }
                else if( left_over_y > 0 )
                {
                    if( (left_over_y - move_offset) > 0 )
                    {
                        rectangle.move(0,move_offset);
                        left_over_y -= move_offset;
                    }
                    else
                    {
                        rectangle.move(0,left_over_y);
                        left_over_y = 0;
                    }
                }
            }
            //end of interpolation

            window.draw( rectangle );
            window.display();
            window.clear();

        } //end of client mode if


        if( mode.compare("server") == 0 )
        {
            window.draw( rectangle );
            window.display();
            window.clear();

            if( PacketClock.getElapsedTime().asSeconds() >= 1 )
            {
                packet_counter = 0;
                PacketClock.restart();
            }
            else if( (PacketClock.getElapsedTime().asMilliseconds()/23) > packet_counter )
            {
                serverPacket << "move" << rectangle.getPosition().x << rectangle.getPosition().y;
                serverSend();
                packet_counter++;
            }
        }

        time = Clock.restart();
    }
}

void serverListener()
{
    std::cout << "Listening for clients." << std::endl;
    sf::TcpListener listener;
    listener.listen(PORT);
    listener.accept(socket);
    std::cout << "New client connected: " << socket.getRemoteAddress() << std::endl;
}

void clientListener()
{
    std::string command;
    float offset_x;
    float offset_y;

}
/*
sf::Mutex globalMutex;
sf::RectangleShape rect1, rect2;
sf::UdpSocket socket;
sf::IpAddress sendIp = "";
unsigned short port = 25565;
bool quit = false;

void handlePacketTraffic(void)
{
	static float incer = 0;
	static float cap = 30;
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

*/
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

