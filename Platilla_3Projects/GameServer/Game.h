#pragma once
#include "ClientProxy.h"
#include "CommonDefinitions.h"
#include "DBManager.h"

#define GAME_TIME 5000

class Game {
	DBManager dbManager;
	Clock gameClock;
	vector<ClientProxy> players;
public:
	Game(vector<ClientProxy>);
	
	bool gameEnded;
	
	void Update();

};