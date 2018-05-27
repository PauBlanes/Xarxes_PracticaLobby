#pragma once
#include <iostream>
#include <SFML\Network.hpp>
#include "ClientProxy.h"
#include "CommonDefinitions.h"
#include "DBManager.h"
#include "Game.h"
#include <thread>
#include <mutex>


using namespace std;

#define MAX_PLAYERS 4 //minim es 2
#define TRYMATCH_TIME 500

class ServerManager {
	vector<ClientProxy> players_in_lobby;
	
	vector<ClientProxy> players_in_queue;
	vector<Game> matches;
	Clock tryMatchClock;
	vector<thread> some_threads;

public:	
	ServerManager();
	void ReceiveComand(Packet, int);
	void SendComand(COMMANDS, TcpSocket*);
	DBManager dbM;
	std::mutex myMutex;

	void TryFindMatch(); //Cada X temps intentem fer aquesta funcio si hi ha minim 2 jugadors a la cua

	void MyThread();

	void CreateThread();
	
};