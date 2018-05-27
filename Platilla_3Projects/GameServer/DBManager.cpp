#include "DBManager.h"

DBManager::DBManager() {
	
	driver = get_driver_instance();
	con = driver->connect(DB_IP, "root", "1234"); 
	stmt = con->createStatement();
	stmt->execute("USE gamedb");
}
DBManager::~DBManager() {
	
	//delete(driver);
	//delete(con);
	//delete(stmt);
}

bool DBManager::Register(string user, string password, string email) {
	
	//Primer buscar si hi ha algun que coincideixi
	string cmd = "SELECT COUNT(*) FROM Accounts WHERE username=";
	cmd += "'" +user+ "'";
	sql::ResultSet* rS = stmt->executeQuery(cmd.c_str());
	
	rS->next();
	int count = rS->getInt(1);//agafem el primer valor
	delete(rS);
	
	//si no estava repetit l'usuari afegim, sino sortim
	if (count == 0) {
		
		cmd.clear();//no tinc clar que calgui fer aix?per?per si de cas
		cmd = "INSERT into Accounts(username, userpassword, email) VALUES(";
		
		////////////////////////////////////////////////////////////////////////
		cmd += "'" + user + "'," + "'" + password + "'," + "'" + email + "')";
		////////////////////////////////////////////////////////////////////////

		stmt->execute(cmd.c_str());		

		return true;
	}
	return false;
}
bool DBManager::Login(string user, string password, ClientProxy* player) {
	
	//Primer comprovem si tenim aquest user-psswrd a la db
	string cmd = "SELECT idAccount FROM Accounts WHERE username=";
	cmd += "\'" + user + "\' and userpassword=" + "\'" + password + "\'";
	sql::ResultSet* rS = stmt->executeQuery(cmd.c_str());

	//si hem obtingut algun valor vol dir que es valid
	if (rS->next()) { 
		//agafem id de conta per despres
		int idAcc = rS->getInt(1);
		delete(rS);

		//fer aqui un altre select per obtenir el nivell del jugador
		cmd.clear();
		cmd = "SELECT level FROM Accounts WHERE idAccount="; //NO RECORDO SI LA COLUMNA ES DEIA level EXACTAMENT
		cmd = cmd + to_string(idAcc);
		rS = stmt->executeQuery(cmd.c_str());
		
		int playerLvl = -1; //per saber si no ho hem pogut setejar
		if (rS->next()) {
			playerLvl = rS->getInt(1);
		}
		
		//Insertem a la taula de sessions una nova sessio amb el nostre id de jugador
		cmd.clear();//no tinc clar que calgui fer aix?per?per si de cas
		cmd = "INSERT INTO Sessions(idAccount) VALUES(";
		cmd = cmd + to_string(idAcc) + ")";
		stmt->execute(cmd.c_str());

		//fem un altre select per obtenir un idSession
		cmd.clear();//no tinc clar que calgui fer aix?per?per si de cas
		cmd = "SELECT MAX(idSession) FROM Sessions WHERE idAccount=";
		cmd = cmd + to_string(idAcc);
		rS = stmt->executeQuery(cmd.c_str());
		
		rS->next();
		int idSess = rS->getInt(1);//agafem el primer valor
		delete(rS);

		//fem un altre select per obtenir el nick -- POTSER AIXO FALLA NO HO HE POGUT PROVAR
		cmd.clear();//no tinc clar que calgui fer aix?per?per si de cas
		cmd = "SELECT username FROM Accounts WHERE idAccount=";
		cmd = cmd + to_string(idAcc);
		rS = stmt->executeQuery(cmd.c_str());

		rS->next();
		string nick = rS->getString("username").c_str();//agafem el primer valor
		delete(rS);

		//GUARDAR A UN PLAYER AQUESTS lvl i session || no el podem crear directament aqui pq al socket selctor ja volem tenirlo per escoltar si ens envia register o algo
		player->sessionID = idSess;
		player->skillLevel = playerLvl;
		player->username = nick;
		player->accountID = idAcc;

		

		return true;
	}
	delete(rS);
	return false;
}

void DBManager::AddMatch(int idSession) {
	
	string cmd = "UPDATE Sessions SET numGames = numGames+1 WHERE idSession =" + to_string(idSession); 
	stmt->execute(cmd.c_str());
}
void DBManager::CloseSession(ClientProxy p) {
	
	string cmd = "UPDATE Sessions SET end = current_timestamp WHERE idSession =" + to_string(p.sessionID); 
	stmt->execute(cmd.c_str());	
}

void DBManager::EndMatch(ClientProxy p) {
	string cmd = "UPDATE Accounts SET level = " + to_string(p.skillLevel) + " WHERE idAccount =" + to_string(p.accountID);
	stmt->execute(cmd.c_str());
}


