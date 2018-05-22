#pragma once
#include <iostream>
#include "SFML\Network.hpp"

using namespace std;
using namespace sf;

class ClientProxy {
public:
	ClientProxy(TcpSocket* sock);
	//ClientProxy);

	//Info general
	int id;
	string nick;
	float skillLevel;
	int numConnections;
	int gamesPlayed;

	//Bases de datos
	void SetValues(string, float, int, int);

	//Info conexio
	TcpSocket* socket;
};