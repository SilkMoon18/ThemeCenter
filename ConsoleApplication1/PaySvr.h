#ifndef _PAY_SERVICE_H_
#define _PAY_SERVICE_H_

#include "TableUtil.h"


class PaySvr {

public:
	DataObject DownloadProduct(string userId, string productId);
	void SetUseCredit(bool use);
	bool PayInCredit(DataObject user, float price);
	bool PayInCash();

	PaySvr();

private:
	bool useCredit = false;
	TableUtil tableService;
};

#endif
