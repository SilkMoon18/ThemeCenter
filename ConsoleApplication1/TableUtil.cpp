#include <iostream>
#include "TableUtil.h"

using namespace std;


// TableService

TableUtil::TableUtil() {

}

DataObject TableUtil::DoQuery(string targetTableName, string id) {

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

void TableUtil::DoInsert(DataObject obj) {

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

void TableUtil::DoModify(DataObject obj) {

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

void TableUtil::DoDelete(string targetTableName, string id) {

	conn.InitConnection();

	string query = "delete from " + targetTableName + " where `" + targetTableName + " Id` = " + id;

	if (mysql_query(conn.GetMysql(), query.c_str()))
		printf("Deletion failed (%s)\n", mysql_error(conn.GetMysql()));
	else
		printf("Deletion succeeded\n");

	conn.CloseConnection();
}

void ToSignIn(string userId) {

	TableUtil tu;
	DataObject obj = tu.DoQuery("User", userId);
	float newValue = stof(obj.dataMap["Credit"].value) + 1;
	obj.dataMap["Credit"].value = to_string(newValue);
	tu.DoModify(obj);
}
