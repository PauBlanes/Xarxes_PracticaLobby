#pragma once
#include <iostream>
#include <SFML\Network.hpp>
#include "Player.h"
#include "CommonDefinitions.h"

using namespace sf;
using namespace std;

class ClientManager {
public:
	
	ClientManager();
	void SendCmd(TcpSocket*, COMMANDS);
	
	string password;//nomes volem el nostre
	Player me;
};
