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
	while (true)
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
				//Ning?s'ha intentat connectar, per tant fem la comunicacio amb els actuals			
				/*for (int i = 0; i < players.size(); i++)
				{
					bool shouldErase = false;
					TcpSocket& client = *(players[i].sock);
					if (selector.isReady(client))
					{
						// The client has sent some data, we can receive i												
						string strRec;
						Packet packet;
						players[i].sock->receive(packet);
						if (status == Socket::Done) //Aquesta funcio ja comprovara si estan tots
						{
							ComunicationManager(packet, &players[i]);

						}
						else if (status == Socket::Disconnected)
						{
							selector.remove(client);
							shouldErase = true;
							cout << "Elimino el socket con puerto : " << client.getRemotePort() << " que se ha desconectado" << endl;
							SendAllPlayers("A client disconnected", &client);
						}
						else
						{
							if (players.size() == maxPlayers) //pq sino ens imprimeix error i simplement es que encara no volem rebre missatges
								cout << "Error al recibir de " << client.getRemotePort() << endl;
						}
					}
					//Si volem borrar elements de la llista hem de controlar que no ens sortim fora amb l'iterador
					if (shouldErase)
						players.erase(players.begin() + i);
				}*/

			}
		}
	}

}

bool ServerManager::CheckIfNew(string n) {
	return true; //Fem que sempre sigui true per fer proves
}

void ServerManager::ReceiveComand(Packet receivedPacket) {
	
	int cmdIndex;
	receivedPacket >> cmdIndex;
	COMMANDS cmd = (COMMANDS)cmdIndex;

	switch (cmd)
	{
	case TRYNICK:
	{
		string nick2Try;
		receivedPacket >> nick2Try;
		if (CheckIfNew(nick2Try)) {
			//Si no estava a la base de dades creem un player, pero si que necessitem quin és l'últim id de la base de dades
			/*ClientProxy newClient(nick2Try, 0.0, 0, 0);
			players.push_back(newClient);
			cout << players.size() << endl;*/
			//I l'afegim a la base de dades

		}
		else {
			//Si ja és nou creem un nou jugador recuperant la info de la base de dades
		}

		break;
	}		
	case STARTQUEUE:
		break;
	case ENDGAME:
		break;
	default:
		break;
	}
}