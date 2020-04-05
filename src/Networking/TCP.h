#include <SFML/Network.hpp>
#include <iostream>


class TCP
{
private:
	/* data */
public:
	TCP(/* args */);
	~TCP();
	void runTcpServer(unsigned short port);
	void runTcpClient(unsigned short port);
};

