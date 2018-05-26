#include "ServerManager.h"

ServerManager::ServerManager() {

	// Create a socket to listen to new connections
	TcpListener listener;
	Socket::Status status = listener.listen(50000);
	if (status != Socket::Done)
	{
		cout << "Error al abrir listener\n";
		exit(0);
	}
	else
		cout << "Port ok" << endl;

	// Create a selector
	SocketSelector selector;
	// Add the listener to the selector
	selector.add(listener);
	// Endless loop that waits for new connections
	bool running = true;
	while (running)
	{
		// Make the selector wait for data on any socket
		if (selector.wait())
		{
			// Test the listener
			if (selector.isReady(listener))
			{
				// The listener is ready: there is a pending connection
				TcpSocket* client = new TcpSocket;
				if (listener.accept(*client) == Socket::Done)
				{
					// Add the new client to the clients list
					std::cout << "Llega el cliente con puerto: " << client->getRemotePort() << std::endl;

					// Creem jugador amb info basica, si la base de dades no ens l'accepta el borrarem, pero es per posarnos a escoltar que mes ens envia
					ClientProxy temp(client);
					players_in_lobby.push_back(temp);

					// Add the new client to the selector so that we will be notified when he sends something
					selector.add(*client);
				}
				else
				{
					// Error, we won't get a new connection, delete the socket
					cout << "Error al recoger conexion nueva\n";
					delete client;
				}
			}
			else
			{
				//Ningu s'ha intentat connectar, per tant fem la comunicacio amb els actuals			
				for (int i = 0; i < players_in_lobby.size(); i++)
				{
					bool shouldErase = false;
					TcpSocket& client = *(players_in_lobby[i].socket);
					if (selector.isReady(client))
					{
						// The client has sent some data, we can receive i												
						string strRec;
						Packet packet;
						players_in_lobby[i].socket->receive(packet);
						if (status == Socket::Done) //Aquesta funcio ja comprovara si estan tots
						{
							ReceiveComand(packet,i);
						}
						else if (status == Socket::Disconnected)
						{
							selector.remove(client);
							shouldErase = true;
							cout << "Elimino el socket con puerto : " << client.getRemotePort() << " que se ha desconectado" << endl;
							//Enviar a tots que s'ha desconenctat?
						}
					}
					//Si volem borrar elements de la llista hem de controlar que no ens sortim fora amb l'iterador
					if (shouldErase)
						players_in_lobby.erase(players_in_lobby.begin() + i);
				}

			}
		}

		//Cada X segons intentem fer una partida si hi ha mes de 1 jugador en cua
		Time currTime = tryMatchClock.getElapsedTime();
		if (currTime.asMilliseconds() > TRYMATCH_TIME) {
		
			if (players_in_queue.size() > 1) {
				
				TryFindMatch();
			}
			tryMatchClock.restart();
		}

		//Actualitzem partides actives
		for (int i = 0; i < matches.size(); i++) {
			matches[i].Update();
			if (matches[i].gameEnded)
				matches.erase(matches.begin()+i);
		}
		
	}

}


void ServerManager::ReceiveComand(Packet receivedPacket, int playerIndex) {
	
	int cmdIndex;
	receivedPacket >> cmdIndex;
	COMMANDS cmd = (COMMANDS)cmdIndex;

	switch (cmd)
	{
	case REGISTER:
	{
		cout << "Recibido intento de registro" << endl;
		string nick2Try;
		string p2Try;
		//////////////////////
		string e2Try;
		//////////////////////

		//receivedPacket >> nick2Try >> p2Try;
		receivedPacket >> nick2Try >> p2Try >> e2Try;

		//if (dbM.Register(nick2Try, p2Try)) {
		if (dbM.Register(nick2Try, p2Try, e2Try)) {
			cout << "New user : " << nick2Try << endl;
			SendComand(OK_REGISTER, players_in_lobby[playerIndex].socket);
		}
		else {
			cout << nick2Try << " is not new" << endl;
		}	
	}
	break;
	case LOGIN:
	{
		cout << "Recibido intento de login" << endl;
		string nick2Try;
		string p2Try;
		receivedPacket >> nick2Try >> p2Try;

		if (dbM.Login(nick2Try, p2Try, &players_in_lobby[playerIndex])) {
			cout << "Logged user: " << nick2Try << endl;
			SendComand(OK_LOGIN, players_in_lobby[playerIndex].socket);

		}
		else {
			cout << "falied to log : " << nick2Try << endl;
		}	
	}
	break;
	case STARTQUEUE:
	{
		
		//Posarlo al array de in queue
		ClientProxy temp = players_in_lobby[playerIndex];
		players_in_queue.push_back(temp);
		cout << players_in_queue.size() << endl;
		if (players_in_queue.size() == 2) //aixo pq no els hi trobi match inmediat si ja havia estat contant temps mentre nomes hi havia 1 jugador
			tryMatchClock.restart();
	}
		break;
	case ENDGAME:
		break;
	default:
		break;
	}
}

void ServerManager::SendComand(COMMANDS cmd, TcpSocket* sock) {
	Socket::Status status;
	size_t bytesSend;
	Packet packet2Send;
	packet2Send << cmd;

	switch (cmd)
	{
	case OK_REGISTER:
		break;
	case OK_LOGIN:
		break;
	default:
		break;
	}

	do
	{
		status = sock->send(packet2Send);
	} while (status == sf::Socket::Partial);
}

void ServerManager::TryFindMatch() {
	vector<ClientProxy> possible;
	bool full = false;
	//Triem un jugador aleatori per provar-li de fer match
	int rnd = rand() % players_in_queue.size();
	possible.push_back(players_in_queue[rnd]);

	for (int i = 0; i < players_in_queue.size() && !full; i++) {
		if (i != rnd && abs(players_in_queue[i].skillLevel - possible[0].skillLevel) < 1) {
			possible.push_back(players_in_queue[i]);
			if (possible.size() >= MAX_PLAYERS)
				full = true;
		}
	}

	if (possible.size() > 1) { //si hem pogut crear partida l'afegim al array de partides
		Game temp(possible); //la mateixa partida ja envia start game a tots i els nicks
		matches.push_back(temp);
	}
	else {
		cout << "No s'ha pogut trobar partida" << endl;
	}
	
}