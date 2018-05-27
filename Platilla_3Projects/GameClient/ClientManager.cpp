#include "ClientManager.h"

ClientManager::ClientManager() {
	//Primer conectem amb el server
	IpAddress ip = sf::IpAddress::getLocalAddress();	

	Socket::Status status = mySocket.connect(ip, 50000);
	mySocket.setBlocking(false);

	//Conect to server and send the name
	if (status != Socket::Done) {
		cout << "Error de connexion al servidor\n";
		//exit(0);
	}
	else if (status == Socket::Done) {
		Asker(ASKREGISTER);		
	}

	while (true) {
		Packet packet;
		Socket::Status s = mySocket.receive(packet);
		if (s == Socket::Done)
		{
			ReceiveComand(packet);
		}
		
	}

}

void ClientManager::SendCmd(TcpSocket* sock, COMMANDS cmd) {
	
	Socket::Status status;
	size_t bytesSend;
	Packet packet2Send;

	switch (cmd)
	{
	case REGISTER:
		 //packet2Send << cmd << me.nick << password;
		packet2Send << cmd << me.nick << password << email;
		break;
	case LOGIN:
		packet2Send << cmd << me.nick << password;
		break;
	case STARTQUEUE:
		packet2Send << cmd;
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

void ClientManager::ReceiveComand(Packet receivedPacket) {

	int cmdIndex;
	receivedPacket >> cmdIndex;
	COMMANDS cmd = (COMMANDS)cmdIndex;

	switch (cmd)
	{
	case OK_REGISTER:
		Asker(ASKLOGIN);
		break;
	case OK_LOGIN:
		Asker(ASKFINDMATCH);
		break;
	case STARTGAME:
		cout << "MATCH FOUND!" << endl;
		//agafar nick dels altres per poder-lo imprimir per pantalla
		int numOthers;
		receivedPacket >> numOthers;
		cout << numOthers << endl;
		cout << "You are in a game with: " << endl;
		for (int i = 0; i < numOthers; i++) {
			string newNick;
			receivedPacket >> newNick;
			if (newNick != me.nick) {
				others.push_back(newNick);
				cout << newNick << endl;
			}
		}
		
		break;
	case ENDGAME:
	{
		others.clear();
		cout << "GAME ENDED" << endl;

		/////////////UPDATE LEVEL////////////////
		
		/////////////////////////////


		Asker(ASKFINDMATCH);
	}
	break;
	default:
		break;
	}
}

void ClientManager::Asker(states state) {
	
	switch (state)
	{
	case ASKREGISTER:
	{
		//Enviem el nostre nom
		cout << "Welcome, Are you a new player [Y/N]" << endl;
		string isNew;
		cin >> isNew;
		if (isNew == "Y") {
			cout << "REGISTER" << endl;
			cout << "Username: ";
			cin >> me.nick;
			cout << "Password: ";
			cin >> password;
			
			//////////////////////
			cout << "Email: ";
			cin >> email;
			//////////////////////

			//Enviar aquest nom pq el server el provi
			SendCmd(&mySocket, REGISTER);
		}
		else if (isNew == "N") {
			Asker(ASKLOGIN);
		}
		else {
			cout << "Wrong answer" << endl;
			Asker(ASKREGISTER);
		}
	}
		break;
	case ASKLOGIN:
	{
		//Enviem el nostre nom
		cout << "LOGIN" << endl;

		cout << "Username: ";
		cin >> me.nick;
		cout << "Password: ";
		cin >> password;

		SendCmd(&mySocket, LOGIN);
	}
		break;
	case ASKFINDMATCH:
	{
		cout << "WELCOME TO THE LOBBY" << endl;
		cout << "FIND MATCH? [Y]" << endl;
		string answer;
		cin >> answer;
		if (answer == "Y"){
			//Enviar al server QUEUE
			SendCmd(&mySocket, STARTQUEUE);
		}
		else {
			cout << "Wrong answer" << endl;
			Asker(ASKFINDMATCH);
		}
	}
		break;
	default:
		break;
	}

	
}


