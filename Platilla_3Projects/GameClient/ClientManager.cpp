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
		AskRegister();		
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
	packet2Send << cmd;

	switch (cmd)
	{
	case REGISTER:
		 packet2Send << me.nick << password;
		break;
	case LOGIN:
		packet2Send << me.nick << password;
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

void ClientManager::ReceiveComand(Packet receivedPacket) {

	int cmdIndex;
	receivedPacket >> cmdIndex;
	COMMANDS cmd = (COMMANDS)cmdIndex;

	switch (cmd)
	{
	case OK_REGISTER:
		AskLogin();
		break;
	case OK_LOGIN:
		//cout << "YOU ARE NOW IN LOBBY" << endl;
		break;
	default:
		break;
	}
}

void ClientManager::AskRegister() {
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

		//Enviar aquest nom pq el server el provi
		SendCmd(&mySocket, REGISTER);
	}
	else if (isNew == "N") {
		AskLogin();
	}
	else {
		cout << "Wrong answer" << endl;
		AskRegister();
	}	
}

void ClientManager::AskLogin() {
	//Enviem el nostre nom
	cout << "LOGIN" << endl;
	
	cout << "Username: ";
	cin >> me.nick;
	cout << "Password: ";
	cin >> password;

	SendCmd(&mySocket, LOGIN);	
}
