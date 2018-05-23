#include "ClientManager.h"

ClientManager::ClientManager() {
	//Primer conectem amb el server
	IpAddress ip = sf::IpAddress::getLocalAddress();
	TcpSocket socket;

	Socket::Status status = socket.connect(ip, 50000);
	socket.setBlocking(false);

	//Conect to server and send the name
	if (status != Socket::Done) {
		cout << "Error de connexion al servidor\n";
		//exit(0);
	}
	else if (status == Socket::Done) {

		//Enviem el nostre nom
		cout << "Conected, Are you a new player [Y/N]" << endl;
		string isNew;
		cin >> isNew;
		if (isNew == "Y") {
			cout << "REGISTER" << endl;
			cout << "Username: ";
			cin >> me.nick;
			cout << "Password: ";
			cin >> password;
		}

		//Enviar aquest nom pq el server el provi
		SendCmd(&socket, REGISTER);
	}	

}

void ClientManager::SendCmd(TcpSocket* sock, COMMANDS cmd) {
	
	Socket::Status status;
	size_t bytesSend;
	Packet packet2Send;

	switch (cmd)
	{
	case REGISTER:
		packet2Send << cmd << me.nick << password;
		break;
	case LOGIN:
		break;
	case STARTQUEUE:
		break;
	case STARTGAME:
		break;
	case ENDGAME:
		break;
	default:
		break;
	}

	do
	{
		status = sock->send(packet2Send);
	} while (status == sf::Socket::Partial);
	
}
