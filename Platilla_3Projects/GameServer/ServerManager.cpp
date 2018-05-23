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
		receivedPacket >> nick2Try >> p2Try;
		
		/*if (dbM.Register(nick2Try, p2Try)) {
			cout << "Registered user : " << nick2Try << endl;
			//Enviar al client OK_REGISTER
		}*/
		
		break;
	}
	case LOGIN:
	{
		cout << "Recibido intento de login" << endl;
		string nick2Try;
		string p2Try;
		receivedPacket >> nick2Try >> p2Try;

		/*if (dbM.Login(nick2Try, p2Try, &players_in_lobby[playerIndex])) {
			cout << "Logged user: " << nick2Try << endl;
			//Enviar al client OK_LOGIN
		}*/
	}
		break;
	case STARTQUEUE:
		break;
	case ENDGAME:
		break;
	default:
		break;
	}
}

int ServerManager::FindClient(TcpSocket* sock) {
	for (int i = 0; i < players_in_lobby.size(); i++) {
		if (sock == players_in_lobby[i].socket)
			return i;
	}
	return -1;//com a control derrors cutre
}