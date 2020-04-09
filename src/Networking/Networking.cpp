#include "Networking.h"


Networking::Networking(){
	this->cliPort = 2201;
	this->serPort = 2202;
	this->isServer = false; // will be assumed to be cli
}

void Networking::networkInit(){
	if (this->isServer){
		UDP::runUdpServer(Networking::getSerPort());
	}
	else {
		UDP::runUdpClient(Networking::getCliPort());

	}
}


unsigned short Networking::getCliPort(){
	return this->cliPort;
}

unsigned short Networking::getSerPort(){
	return this->serPort;
}