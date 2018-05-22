#pragma once
#include <iostream>
#include <SFML\Network.hpp>
#include "ClientProxy.h"
#include "CommonDefinitions.h"

using namespace std;

class ServerManager {
	vector<ClientProxy> players;
	int idCounter; //Com a cosa temporal mentre no podem agafar lultim id des de la base de dades
public:	
	ServerManager();
	bool CheckIfNew(string);
	void ReceiveComand(Packet);
};