#pragma once
#include <mysql_connection.h>
#include <cppconn\driver.h>
#include <cppconn\resultset.h>
#include <cppconn\statement.h>
#include <cppconn\exception.h>

#define DB_IP "tcp://localhost:3306"
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
	bool Login(string, string);
	void AddMatch(int);
	void CloseSession(int);
};