#ifndef _DATABASE_CONNECTION_H_
#define _DATABASE_CONNECTION_H_

#include <winsock.h>
#include <mysql.h>
#include <string>
#include <mutex>

#pragma comment(lib,"wsock32.lib")
#pragma comment(lib,"libmysql.lib")

using namespace std;


class DatabaseConnection {

public:
	void InitConnection();
	void CloseConnection();
	MYSQL* GetMysql();
	void SetUsername(string un);
	void SetPassword(string pw);
	void SetHost(string h);
	void SetDatabaseName(string dn);
	void SetPort(int p);

	DatabaseConnection();

private:
	static mutex mutexLock;

	MYSQL* mysql;
	string username;
	string password;
	string host;
	string databaseName;
	int port;

};
#endif