#include "ClientProxy.h"

ClientProxy::ClientProxy(TcpSocket* sock) {
	socket = sock;
}
void ClientProxy::SetValues(string n, float sLvL, int connections, int numGames) {
	
	nick = n;
	skillLevel = sLvL;
	numConnections = connections;
	gamesPlayed = numGames;
}

