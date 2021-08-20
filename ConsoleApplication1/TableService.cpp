#include <iostream>
#include "TableService.h"

using namespace std;


// TableService

TableService::TableService() {

}

DataObject TableService::DoQuery(string targetTableName, string id) {

	conn.InitConnection();

	DataObject obj;

	string query = "select * from " + targetTableName + " where `" + targetTableName + " id` = " + id + ";";
	if (mysql_query(conn.GetMysql(), query.c_str())) {
		printf("Query failed (%s)\n", mysql_error(conn.GetMysql()));
		return obj;
	}
	printf("Query succeeded\n");

	MYSQL_RES* mysqlRes;
	mysqlRes = mysql_store_result(conn.GetMysql());
	if (mysqlRes) {
		MYSQL_ROW sqlRow;
		sqlRow = mysql_fetch_row(mysqlRes);

		MYSQL_FIELD* mysqlField;
		map<string, DataObject::Data> dm;
		for (int i = 0; mysqlField = mysql_fetch_field(mysqlRes); i++) {
			DataObject::DataType type;
			switch (mysqlField->type) {
			case MYSQL_TYPE_LONG:
				type = DataObject::DataType::dataInt;
				break;

			case MYSQL_TYPE_NEWDECIMAL:
				type = DataObject::DataType::dataFloat;
				break;

			case MYSQL_TYPE_VAR_STRING:
				type = DataObject::DataType::dataString;
				break;
			defualt:
				type = DataObject::DataType::unknown;
				break;
			}

			dm[mysqlField->name] = DataObject::Data(type, sqlRow[i]);
		}
		obj = DataObject(targetTableName, dm);
	}
	else {
		printf("No result found\n");
		return obj;
	}
	if (mysqlRes != NULL) mysql_free_result(mysqlRes);

	conn.CloseConnection();

	return obj;
}

void TableService::DoInsert(DataObject obj) {

	conn.InitConnection();

	string cols;
	string vals;
	string query = "insert into " + obj.tableName + " ";
	for (auto item = obj.dataMap.begin(); item != obj.dataMap.end(); item++) {
		if (item == obj.dataMap.begin()) {
			cols += "(`" + item->first + "`";
			vals += "('" + item->second.value + "'";
		}
		else {
			cols += ", `" + item->first + "`";
			vals += ", '" + item->second.value + "'";
		}
	}
	cols += ")";
	vals += ")";
	query += cols + " values " + vals + ";";

	if (mysql_query(conn.GetMysql(), query.c_str()))
		printf("Insertion failed (%s)\n", mysql_error(conn.GetMysql()));
	else
		printf("Insertion succeeded\n");

	conn.CloseConnection();
}

void TableService::DoModify(DataObject obj) {

	conn.InitConnection();

	string query = "update " + obj.tableName + " set ";
	for (auto item = obj.dataMap.begin(); item != obj.dataMap.end(); item++) {
		if (item == obj.dataMap.begin()) {
			query += "`" + item->first + "` = '" + item->second.value + "'";
		}
		else {
			query += ", `" + item->first + "` = '" + item->second.value + "'";
		}
	}
	query += " where `" + obj.tableName + " Id` = " + obj.dataMap[obj.tableName + " Id"].value + ";";

	if (mysql_query(conn.GetMysql(), query.c_str()))
		printf("Modification failed (%s)\n", mysql_error(conn.GetMysql()));
	else
		printf("Modification succeeded\n");

	conn.CloseConnection();
}

void TableService::DoDelete(string targetTableName, string id) {

	conn.InitConnection();

	string query = "delete from " + targetTableName + " where `" + targetTableName + " Id` = " + id;

	if (mysql_query(conn.GetMysql(), query.c_str()))
		printf("Deletion failed (%s)\n", mysql_error(conn.GetMysql()));
	else
		printf("Deletion succeeded\n");

	conn.CloseConnection();
}



// ViewService

ViewService::ViewService() {

}

void ViewService::UpdateRanking(TableService::ProductType type) {

	conn.InitConnection();

	string query = "select * from Product where (`product`.`Price` ";
	if (type == TableService::ProductType::paid)
		query += " !";
	query += "= 0) order by `product`.`Downloads` desc; ";

	if (mysql_query(conn.GetMysql(), query.c_str()))
		printf("ShowRanking failed (%s)\n", mysql_error(conn.GetMysql()));
	else {
		unique_lock<mutex> lock(mutexLock);

		ranking.clear();

		string table;
		if (type == TableService::ProductType::free)
			table = "TopFree";
		else if (type == TableService::ProductType::paid)
			table = "TopPaid";

		MYSQL_RES* mysqlRes;
		mysqlRes = mysql_store_result(conn.GetMysql());

		MYSQL_FIELD* mysqlField;
		map<string, DataObject::Data> dm;
		vector<string> cols;
		while (mysqlField = mysql_fetch_field(mysqlRes)) {
			DataObject::DataType type;
			switch (mysqlField->type) {
			case MYSQL_TYPE_LONG:
				type = DataObject::DataType::dataInt;
				break;

			case MYSQL_TYPE_NEWDECIMAL:
				type = DataObject::DataType::dataFloat;
				break;

			case MYSQL_TYPE_VAR_STRING:
				type = DataObject::DataType::dataString;
				break;
			defualt:
				type = DataObject::DataType::unknown;
				break;
			}
			cols.push_back(mysqlField->name);
			dm[mysqlField->name] = DataObject::Data(type, "");
		}

		for (int i = 0; i < mysqlRes->row_count; i++) {
			MYSQL_ROW sqlRow;
			sqlRow = mysql_fetch_row(mysqlRes);

			MYSQL_FIELD* mysqlField;
			map<string, DataObject::Data> dm;
			for (int j = 0; j < cols.size(); j++) {

				dm[cols.at(j)].value = sqlRow[j];
			}

			DataObject obj = DataObject(table, dm);
			ranking.push_back(obj);
		}
		conn.CloseConnection();

	}

}

void ViewService::KeepUpdatingRanking(TableService::ProductType type, int updateIntervalInSecond) {
	while (true) {
		UpdateRanking(type);
		Sleep(updateIntervalInSecond * 1000);
	}
}

vector<DataObject> ViewService::GetRanking() {
	unique_lock<mutex> lock(mutexLock);
	return ranking;
}