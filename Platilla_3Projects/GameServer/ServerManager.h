#pragma once
#include <iostream>
#include <SFML\Network.hpp>
#include "ClientProxy.h"
#include "CommonDefinitions.h"
#include "DBManager.h"
#include "Game.h"

using namespace std;

class ServerManager {
	vector<ClientProxy> players_in_lobby;
	vector<Game> matches;
public:	
	ServerManager();
	void ReceiveComand(Packet, int);
	void SendComand(COMMANDS, TcpSocket*);
	DBManager dbM;
	int FindClient(TcpSocket*); //devuelve el indice con este user+password
};