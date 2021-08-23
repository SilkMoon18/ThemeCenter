#include "ViewUtil.h"

using namespace std;


ViewUtil::ViewUtil() {

}

void ViewUtil::UpdateRanking(TableUtil::ProductType type) {

	conn.InitConnection();

	string query = "select * from Product where (`product`.`Price` ";
	if (type == TableUtil::ProductType::paid)
		query += " !";
	query += "= 0) order by `product`.`Downloads` desc; ";

	if (mysql_query(conn.GetMysql(), query.c_str()))
		printf("ShowRanking failed (%s)\n", mysql_error(conn.GetMysql()));
	else {
		unique_lock<mutex> lock(mutexLock);

		ranking.clear();

		string table;
		if (type == TableUtil::ProductType::free)
			table = "TopFree";
		else if (type == TableUtil::ProductType::paid)
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

void ViewUtil::KeepUpdatingRanking(TableUtil::ProductType type, int updateIntervalInSecond) {
	while (true) {
		UpdateRanking(type);
		Sleep(updateIntervalInSecond * 1000);
	}
}

vector<DataObject> ViewUtil::GetRanking() {
	unique_lock<mutex> lock(mutexLock);
	return ranking;
}