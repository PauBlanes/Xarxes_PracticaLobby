#include "Game.h"

Game::Game(vector<ClientProxy>p) {
	players = p;

	Socket::Status status;
	size_t bytesSend;
	Packet packet2Send;
	packet2Send << COMMANDS::STARTGAME;
	packet2Send << players.size();
	//Afegim tots els noms i actualitzem base de dades
	for (int i = 0; i < players.size(); i++) {
		packet2Send << players[i].username;
		dbManager.AddMatch(players[i].sessionID);
	}
	//Enviem a tots
	for (int i = 0; i < players.size(); i++) {
		do
		{
			status = players[i].socket->send(packet2Send);
		} while (status == sf::Socket::Partial);
	}
}

void Game::Update() {

	//Actualitzar temportizador
	Time currTime = gameClock.getElapsedTime();
	if (currTime.asMilliseconds() > GAME_TIME ) {
		//Quan s'ha acabat enviem comando fin i actualitzem base de dades
		cout << "END OF GAME" << endl;
		gameEnded = true;//pq el serverManager el pugui treure del array
		Socket::Status status;
		size_t bytesSend;
		Packet packet2Send;
		packet2Send << COMMANDS::ENDGAME;
		for (int i = 0; i < players.size(); i++) {
		players[i].skillLevel += 3; //com que no hi ha guanyar/perdre de fet aixo seria com experiencia
		dbManager.CloseSession(players[i]);
		do
		{
			status = players[i].socket->send(packet2Send);
		} while (status == sf::Socket::Partial);
		}	
		gameClock.restart();
	}

	
}