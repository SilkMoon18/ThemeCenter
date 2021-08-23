#ifndef _TABLE_OPERATION_
#define _TABLE_OPERATION_

#include <mutex>
#include <map>
#include <windows.h>
#include <time.h>
#include <vector>

#include "DatabaseConnection.h"

using namespace std;


void ToSignIn(string userId);

struct DataObject {

	enum class DataType {
		unknown = 0,
		dataInt = 1,
		dataFloat = 2,
		dataString = 3,
	};
	struct Data {
		DataType type;
		string value;

		Data() {
			type = DataType::dataInt;
			value = "";
		}

		Data(DataType t, string v) {
			type = t;
			value = v;
		}
	};

	string tableName;
	map<string, Data> dataMap;

	DataObject() {

	}

	DataObject(string tn, map<string, Data> dm) {
		tableName = tn;
		dataMap = dm;
	}

	void SetValue(string colName, string newValue) {
		dataMap[colName].value = newValue;
	}
};


class TableUtil {

public:
	enum class ProductType {
		free = 0,
		paid = 1,
	};

	DataObject DoQuery(string targetTableName, string id);
	void DoInsert(DataObject obj);
	void DoModify(DataObject obj);
	void DoDelete(string targetTableName, string id);

	TableUtil();

private:
	mutex mutexLock;
	DatabaseConnection conn;
	bool useCredit = false;
};

#endif