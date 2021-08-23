#ifndef _VIEW_SERVICE_H_
#define _VIEW_SERVICE_H_

#include "TableUtil.h"
#include "DatabaseConnection.h"


class ViewUtil {

public:
	void UpdateRanking(TableUtil::ProductType type);
	void KeepUpdatingRanking(TableUtil::ProductType type, int updateIntervalInSecond);
	vector<DataObject> GetRanking();

	ViewUtil();

private:
	vector<DataObject> ranking;
	DatabaseConnection conn;
	mutex mutexLock;

};

#endif