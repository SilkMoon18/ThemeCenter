#ifndef _PAY_SERVICE_H_
#define _PAY_SERVICE_H_

#include "TableService.h"


class PayService {

public:
	DataObject DownloadProduct(string userId, string productId);
	void SetUseCredit(bool use);
	bool PayInCredit(DataObject user, float price);
	bool PayInCash();

	PayService();

private:
	bool useCredit = false;
	TableService tableService;
};

#endif
