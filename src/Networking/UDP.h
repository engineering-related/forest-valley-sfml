#include <SFML/Network.hpp>
#include <iostream>



class UDP
{
private:
	/* data */
public:
	UDP(/* args */);
	~UDP();
	void runUdpServer(unsigned short port);
	void runUdpClient(unsigned short port);
};
