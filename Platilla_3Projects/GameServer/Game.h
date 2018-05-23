#pragma once
#include "ClientProxy.h"

class Game {
public:
	Game(vector<ClientProxy>);

	vector<ClientProxy> players;

};