#ifndef _VIEW_SERVICE_H_
#define _VIEW_SERVICE_H_

#include "TableService.h"
#include "DatabaseConnection.h"


class ViewService {

public:
	void UpdateRanking(TableService::ProductType type);
	void KeepUpdatingRanking(TableService::ProductType type, int updateIntervalInSecond);
	vector<DataObject> GetRanking();

	ViewService();

private:
	vector<DataObject> ranking;
	DatabaseConnection conn;
	mutex mutexLock;

};

#endif