#pragma once
#include <iostream>
#include "SFML\Network.hpp"

using namespace std;
using namespace sf;

class ClientProxy {
public:
	ClientProxy(TcpSocket*);
	//ClientProxy();

	//Info de la conta
	int sessionID;
	string username;
	int skillLevel;
	int accountID;

	//Info conexio
	TcpSocket* socket;
};