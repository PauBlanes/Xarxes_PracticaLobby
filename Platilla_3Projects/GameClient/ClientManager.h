#pragma once
#include <iostream>
#include <SFML\Network.hpp>
#include "Player.h"
#include "CommonDefinitions.h"

using namespace sf;
using namespace std;

#define GAME_DURATION 5000;

enum states {ASKREGISTER, ASKLOGIN, ASKFINDMATCH};
class ClientManager {
public:
	
	ClientManager();
	void SendCmd(TcpSocket*, COMMANDS);
	void ReceiveComand(Packet);
	void Asker(states);
	//void AskLogin();

	TcpSocket mySocket;	
	string password;//nomes volem el nostre
	//////////////////////////////
	string email;
	//////////////////////////////

	Player me;
	vector<string> others; //nomes volem els nicks de fet
	bool ingame;
	Clock ingameClock;
};
