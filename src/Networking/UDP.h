#include <SFML/Network.hpp>
#include <iostream>

#ifndef UDP
#define UDP

void runUdpServer(unsigned short port);
void runUdpClient(unsigned short port);

#endif /* Networking/UDP.h */