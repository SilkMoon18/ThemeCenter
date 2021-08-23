#include <iostream>
#include "DatabaseConnection.h"
#include "TableUtil.h"

#pragma comment(lib,"wsock32.lib")
#pragma comment(lib,"libmysql.lib")
using namespace std;


mutex DatabaseConnection::mutexLock;

DatabaseConnection::DatabaseConnection() {
	mysql = new MYSQL;
	host = "127.0.0.1";
	username = "root";
	password = "3321";
	databaseName = "themecenter";
	port = 3306;
}

void DatabaseConnection::InitConnection() {

	unique_lock<mutex> lock(mutexLock);

	if (mysql_init(mysql) == NULL)
		printf("Connection init failed (%s)\n", mysql_error(mysql));

	if (!mysql_real_connect(mysql, host.c_str(), username.c_str(), password.c_str(), databaseName.c_str(), port, NULL, 0)) 
		printf("Connection failed (%s)\n", mysql_error(mysql));
	else
		printf("Connection succeeded\n");
}

void DatabaseConnection::CloseConnection() {
	mysql_close(mysql);
}

MYSQL* DatabaseConnection::GetMysql() {
	return mysql;
}

void DatabaseConnection::SetUsername(string un) {
	username = un;
}
void DatabaseConnection::SetPassword(string pw) {
	password = pw;
}
void DatabaseConnection::SetHost(string h) {
	host = h;
}
void DatabaseConnection::SetDatabaseName(string dn) {
	databaseName = dn;
}
void DatabaseConnection::SetPort(int p) {
	port = p;
}
