#include <iostream>
#include "TableService.h"
#include "ViewService.h"
#include "DatabaseConnection.h"
#

using namespace std;

void Modify(DataObject obj) {
	//
	//cout << this_thread::get_id() << " is " << obj.dataMap["Password"].value << " " << endl;
	////
	TableService ts;
	ts.DoModify(obj);
}

void test(int s) {
	DatabaseConnection c;
	c.SetPort(s);
	c.InitConnection();
}


int main() {

	//thread t1(test, 3306);
	//t1.detach();
	//thread t2(test, 3306);
	//t2.detach(); 
	
	
	map<string, DataObject::Data> dataMap;
	dataMap["User Id"] = DataObject::Data(DataObject::DataType::unknown, "999");
	dataMap["Password"] = DataObject::Data(DataObject::DataType::unknown, "t1");

	DataObject obj("User", dataMap);

	//
	thread t1(Modify, obj);
	t1.detach();

	obj.SetValue("Password", "t2");
	thread t2(Modify, obj);
	t2.detach();
	//
	

	//
	//TableService* ts = TableService::GetInstance();
	//
	//thread t(&TableService::ShowRanking, ts, TableService::ProductType::paid, 3000);
	//t.detach();


	Sleep(20000000);


	return 0;
}

