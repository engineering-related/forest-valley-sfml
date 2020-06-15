
using namespace sf;

/*
// Funktionsdeklarationer
void doClientTCP(unsigned short Port);
void doClientUDP(unsigned short Port);
void doServerTCP(unsigned short Port);
void doServerUDP(unsigned short Port);

int main(int argc, char** argv)
{ // Välj valfritt nummer över 1024 för port, de < 1024 är reserverade
	const unsigned short port = 2435;

	// TCP eller UDP ?
	char protocol;
	std::cout << "Vill du använda TCP ('t') eller UDP ('u') ? ";
	std::cin >> protocol;

	// Klient eller server ?
	char who;
	std::cout << "Vill du vara server ('s') eller en klient ('k') ? ";
	std::cin >> who;

	if (who == 's')
	{ // Kör programmet som server
		if (protocol == 't')
			doServerTCP(port);
		else
			doServerUDP(port);
	}
	else
	{ // Kör programmet som klient
		if (protocol == 't')
			doClientTCP(port);
		else
			doClientUDP(port);
	}

	// Vänta tills användaren trycker ner 'enter' tangenten
	std::cout << "Tryck ner ENTER för att avsluta..." << std::endl;
	std::cin.ignore(10000, '\n');
	std::cin.ignore(10000, '\n');

	return 0;
}*/

//FUNKTIONERNA

// ****************************************************** //
// Skapa en TCP klient och anslut till en startad server
// ****************************************************** //
void doClientTCP(unsigned short Port)
{ // Fråga efter serverns adress
	sf::IpAddress serverAddress;

	std::cout << "Skriv adresss eller namn på servern du vill ansluta till : ";
	std::cin >> serverAddress;

	// Skapa en TCP socket för att kommunicera med servern
	sf::TcpSocket client;

	// Anslut till servern
	if (client.connect(serverAddress, Port) != sf::Socket::Done)
		return;
	std::cout << "Ansluten till server " << serverAddress << std::endl;

	// Få meddelande från en klient
	char message[128];
	std::size_t received;
	if (client.receive(message, sizeof(message), received) != sf::Socket::Done)
		return;
	// Visa upp meddelandet

	std::cout << "Meddelande mottaget från server : \"" << message << "\"" << std::endl;
	// Definiera ett meddelande att skicka tillbaka till servern
	char toSend[] = "Hej, klient pratar !";

	// Skicka meddelandet
	if (client.send(toSend, sizeof(toSend)) != sf::Socket::Done)
		return;

	std::cout << "Meddelande skickat till server : \"" << toSend << "\"" << std::endl;
	// Stäng socketen när vi är klara
	client.disconnect();
}

// ****************************************************** //
// Skapa en TCP server och vänta på inkommande meddelanden
// ****************************************************** //
void doServerTCP(unsigned short Port)
{ // Skapa en TCP socket för att kommunicera med klienter
	sf::TcpListener server;
	// Lyssna på en port för inkommande meddelanden
	if (!server.listen(Port))
		return;

	std::cout << "Servern lyssnar på port " << Port << ", väntar på anslutning... " << std::endl;

	// Vänta på kontakt
	sf::IpAddress clientAddress;
	sf::TcpSocket client;
	server.accept(client);
	std::cout << "Klient ansluten : " << clientAddress << std::endl;

	// Skicka ett meddelande till klienten
	char toSend[] = "Hej, server meddelar";
	if (client.send(toSend, sizeof(toSend)) != sf::Socket::Done)
		return;

	std::cout << "Meddelande skickat till klient : \"" << toSend << "\"" << std::endl;
	// Motta meddelande från klienten
	char message[128];
	std::size_t received;

	if (client.receive(message, sizeof(message), received) != sf::Socket::Done)
		return;

	// Visa meddelandet
	std::cout << "Meddelande mottaget från klient : \"" << message << "\"" << std::endl;

	// Stäng sockets när vi är klara
	client.disconnect();
	server.close();
}

// ************************************************************** //
// Skapa en UDP klient och skicka meddelande med UDP till en server
// ************************************************************** //

void doClientUDP(unsigned short Port)
{ // Fråga efter serverns IP-adress
	sf::IpAddress serverAddress;

	std::cout << "Skriv in IP-adressen eller namnet på servern du vill skicka ett meddelande till : ";
	std::cin >> serverAddress;

	// Skapa en UDP socket för att kunna kommunicera med servern
	sf::UdpSocket client;
	// Skicka ett meddelande till servern
	char message[] = "Hej, skickat av klient!";
	if (client.send(message, sizeof(message), serverAddress, Port) != sf::Socket::Done)
		return;

	std::cout << "Meddelande skickat till server : \"" << message << "\"" << std::endl;

	// Stäng ner socketen när det är klart
	client.unbind();
}

// ****************************************************** //
// Starta en UDP server och vänta på inkommande meddelanden
// ****************************************************** //

void doServerUDP(unsigned short Port)
{ // Skapa en UDP socket för att kommunicera med klienten
	sf::UdpSocket server;

	// Koppla den till en specifik port
	if (!server.bind(Port))
		return;

	// Ta emot ett meddelande från vem som helst
	sf::IpAddress clientAddress;
	unsigned short clientPort;
	char message[128];
	std::size_t received;

	if (server.receive(message, sizeof(message), received, clientAddress, clientPort) != sf::Socket::Done)
		return;
	// Visa upp det
	std::cout << "Meddelande mottaget från " << clientAddress << " från port " << clientPort << " : \"" << message << "\"" << std::endl;

	// Stäng socketen när vi är klara
	server.unbind();
}


/*
class spelare
{
public:
	sf::Uint8 ID;		  //0=pojke, 1=ko
	sf::Int32 hastighetX; //Hastighet i X-led, kan vara negativ
	sf::Int32 hastighetY; //Hastighet i Y-led, kan vara negativ
	sf::Sprite sprite;	  //Bilden som representerar pojken eller kon
};

// Server = styr pojken
// Client = styr ko

//--------------------------------------------------------------
//Skapa paketen som skall skickas mellan datorerna
//--------------------------------------------------------------

sf::Packet& operator<<(sf::Packet& Packet, const spelare& C)
{
	return Packet << C.ID << C.hastighetX << C.hastighetY;
}

sf::Packet& operator>>(sf::Packet& Packet, spelare& C)
{
	return Packet >> C.ID >> C.hastighetX >> C.hastighetY;
}

//Funktioner för att köra programmet som en server eller som en klient
void runClient(unsigned short Port);
void runServer(unsigned short Port);

void playerSend(class spelare& s);
void playerRecieve(class spelare& s);
//---------------------------------------------------------
// Programstart
//---------------------------------------------------------
int main(int argc, char** argv)
{							  //Början av programkörningen
	char who = 'Z';			  //Avgör om man skall vara klient eller server
	float elapsedTime = 0.0f; //Skapar en konstant för att hålla hastigheten likvärdig på olika datorer

	//--------------------------------------------------------
	// Visa upp lokal adress, troligen 192.168.x.x eller 10.100.x.x eller 169.254.x.x
	//-------------------------------------------
	sf::IpAddress address1 = sf::IpAddress::getPublicAddress();
	std::string IP1 = address1.toString();
	std::cout << "Din egen offentliga IP-adress = " << IP1 << std::endl
			  << std::endl;

	//Skapa paket
	sf::Packet toSend;	 //Paketet skapas för att skicka uppgifter
	sf::Packet received; //Paket att ta emot uppgifter skapas

	//Skapa en port som vi skall använda till vår socket
	//(portarna < 1024 är reserverade)
	const unsigned short Port = 2435;

	// Client eller server ?------------------------------------------------
	std::cout << "Vill du vara server ('s') eller en klient ('c') ? ";
	std::cin >> who;

	if (who == 's')
		runServer(Port); //Vänta
	else
		runClient(Port); //Anslut till server

	//Skapa inte spelfönstret innan anslutning skett
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Test - nätverksspel");

	//Skapa en pojke
	spelare pojke;		  //pojke blir en kopia av klassen spelare
	pojke.hastighetX = 0; //grundhastighet, skall multipliceras med ”ElapsedTime ”.
	pojke.hastighetY = 0; //grundhastighet, skall multipliceras med ”ElapsedTime ”.
	pojke.ID = 0;

	//Skapa en ko
	spelare ko;		   //ko blir en kopia av klassen spelare
	ko.hastighetX = 0; //grundhastighet, skall multipliceras med ”ElapsedTime ”.
	ko.hastighetY = 0; //grundhastighet, skall multipliceras med ”ElapsedTime ”.
	ko.ID = 1;

	//Skapa bildhållarna
	sf::Texture pojkbild;
	sf::Texture kobild;

	//Ladda in bildfilerna
	pojkbild.loadFromFile("content/characters/PlayerBase.png");
	kobild.loadFromFile("content/characters/PlayerPoke.png");

	//Dela ut bild till ko
	ko.sprite.setTexture(kobild);
	ko.sprite.setPosition(500, 500);

	//Dela ut bild till pojke
	pojke.sprite.setTexture(pojkbild);
	pojke.sprite.setPosition(100, 100);

	Clock clock;

	while (window.isOpen())
	{
		elapsedTime = clock.restart().asSeconds(); //Skapar en konstant för att hålla hastigheten likvärdig på olika datorer
		Event event;

		while (window.pollEvent(event))			 // Ta hand om händelser
		{										 //while 2
			if (event.type == sf::Event::Closed) //kryssat på [x] symbolen? stäng programmet
				window.close();

			if (event.type == sf::Event::KeyPressed)	// En tangent har tryckts ner
			{											//if 1
				if (event.key.code == Keyboard::Escape) // ESC tangenten = stäng programmet
					window.close();

			} //slut if 1

		} //slut, while 2

		if (who == 's')
		{ //Du är en server
			//X-led värden
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				pojke.hastighetX = -100;
				playerSend(pojke); //Skicka ut var pojken är
			}

			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				pojke.hastighetX = 100;
				playerSend(pojke); //Skicka ut var pojken är
			}
			//Spring ingenstans om inte knapparna är nedtryckta
			if (!Keyboard::isKeyPressed(Keyboard::Left) && !Keyboard::isKeyPressed(Keyboard::Right))
			{
				pojke.hastighetX = 0;
				playerSend(pojke); //Skicka ut var pojken är
			}

			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				pojke.hastighetY = -100;
				playerSend(pojke); //Skicka ut var pojken är
			}

			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				pojke.hastighetY = 100;
				playerSend(pojke); //Skicka ut var pojken är
			}

			//Spring ingenstans om inte knapparna är nedtryckta
			if (!Keyboard::isKeyPressed(Keyboard::Up) && !Keyboard::isKeyPressed(Keyboard::Down))
			{
				pojke.hastighetY = 0;
				playerSend(pojke); //Skicka ut var pojken är
			}

			playerRecieve(ko);

		} //Du är en server

		{ //Du är en server
			//X-led värden
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				ko.hastighetX = -100;
				playerSend(ko); //Skicka ut var pojken är
			}

			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				ko.hastighetX = 100;
				playerSend(ko); //Skicka ut var pojken är
			}
			//Spring ingenstans om inte knapparna är nedtryckta
			if (!Keyboard::isKeyPressed(Keyboard::Left) && !Keyboard::isKeyPressed(Keyboard::Right))
			{
				ko.hastighetX = 0;
				playerSend(ko); //Skicka ut var pojken är
			}

			if (Keyboard::isKeyPressed(Keyboard::Up))
			{
				ko.hastighetY = -100;
				playerSend(ko); //Skicka ut var pojken är
			}

			if (Keyboard::isKeyPressed(Keyboard::Down))
			{
				ko.hastighetY = 100;
				playerSend(ko); //Skicka ut var pojken är
			}

			//Spring ingenstans om inte knapparna är nedtryckta
			if (!Keyboard::isKeyPressed(Keyboard::Up) && !Keyboard::isKeyPressed(Keyboard::Down))
			{
				ko.hastighetY = 0;
				playerSend(ko); //Skicka ut var pojken är
			}

			playerRecieve(pojke);

		} //Du är en client

		//Flytta spelpjäserna
		pojke.sprite.move(pojke.hastighetX * elapsedTime, pojke.hastighetY * elapsedTime);
		ko.sprite.move(ko.hastighetX * elapsedTime, ko.hastighetY * elapsedTime);

		//Rita ut ändringarna på skärmen
		window.clear();
		window.draw(pojke.sprite);
		window.draw(ko.sprite);

		window.display();
	} //Gameloop
	return 0;

} //slut på programkörningen

void runClient(unsigned short Port)
{
	// Fråga efter serveradressen
	sf::IpAddress serverAddress;

	std::cout << "Skriv in IP adress eller namn att ansluta till : ";
	std::cin >> serverAddress;

	// Skapa en TCP socket för att samarbeta med servern
	sf::TcpSocket client;

	// Anslut till servern
	if (client.connect(serverAddress, Port) != sf::Socket::Done)
		return;
	std::cout << "Ansluten till server " << serverAddress << std::endl;
}
//Serverfunktionerna

void runServer(unsigned short Port)
{
	// Skapa en TCP socket för att kommunicera med klienterna
	sf::TcpListener server;

	// Lyssna på en port för inkommande anrop
	if (!server.listen(Port))
		return;
	std::cout << "Servern lyssnar på port " << Port << ", väntar på anslutning... " << std::endl;

	// Väntar på kontakt
	sf::IpAddress clientAddress;
	sf::TcpSocket client;
	server.accept(client);
	std::cout << "Klient ansluten : " << clientAddress << std::endl;
}

//Skicka iväg informationen om spelaren till mottagaren
void playerSend(class spelare& s)
{
	spelare C = { s.ID, s.hastighetX, s.hastighetY };
	sf::Packet regularPacket; //Paket skapas med tre variabler
	sf::TcpSocket client;	  //Socket skapas
	regularPacket << C;		  //Paketet tilldelas de tre variablerna

	if (C.ID == 0)
	{
		std::cout << "Pojke skickad till klienten: " << std::endl;
	}
	else
	{
		std::cout << "Ko skickad till klienten: " << std::endl;
	}

	if (client.send(regularPacket) != sf::Socket::Done) //Paketet skickas
		return;
}

void playerRecieve(class spelare& s)

{
	sf::Packet regularPacket; //Paket skapas med tre variabler
	sf::TcpSocket client;
	//Client.SetBlocking(false); //Tar emot utan avbrott
	spelare C;

	if (client.receive(regularPacket) != sf::Socket::Done)
		return;
	std::cout << "Tar emot..." << std::endl; //test
	if (regularPacket >> C)
	{
		if (C.ID == 0)
		{
			std::cout << "Pojke mottagen från servern: " << std::endl;
		}
		else
		{
			std::cout << "Ko mottagen från servern: " << std::endl;
		}
	}
	else
	{
		std::cout << "Ingenting mottaget från servern: " << std::endl;
	}
	//std::cout << s.ID << " ID, " << s.hastighetX << " hastighet x, " << s.hastighetY << " hastighet y" << std::endl;
}
*/