#pragma once
#include <mysql_connection.h>
#include <cppconn\driver.h>
#include <cppconn\resultset.h>
#include <cppconn\statement.h>
#include <cppconn\exception.h>
#include "ClientProxy.h"

#define DB_IP /*"tcp://192.168.123.146:3306"*/"tcp://192.168.122.92:3306"
#define DB_USER "root"
#define DB_PSSWRD "1234"

using namespace std;

class DBManager {
	sql::Driver* driver;
	sql::Connection* con;
	sql::Statement* stmt;
public:
	 DBManager();
	~DBManager();
	bool Register(string, string);
	bool Login(string, string, ClientProxy*); //afegim un index de l'array de jugadors per poder setejar-li les dades
    void AddMatch(int);
	void CloseSession(int);
};