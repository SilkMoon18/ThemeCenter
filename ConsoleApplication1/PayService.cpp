#include <iostream>
#include "PayService.h"

using namespace std;


PayService::PayService() {

}

DataObject PayService::DownloadProduct(string userId, string productId) {

	DataObject product = tableService.DoQuery("Product", productId);
	DataObject user = tableService.DoQuery("User", userId);

	float price = stof(product.dataMap["Price"].value);
	bool paid = false;
	if (price > 0) {
		if (useCredit) {
			paid = PayInCredit(user, price);
		}
		else {
			paid = PayInCash();
		}

		if (!paid) {
			return DataObject();
		}
	}

	return product;
}

void PayService::SetUseCredit(bool use) {
	useCredit = use;
}

bool PayService::PayInCredit(DataObject user, float price) {

	float credit = stof(user.dataMap["Credit"].value);
	float cost = price * 10;

	if (credit >= cost) {
		user.dataMap["Credit"].value = to_string(credit - cost);
		tableService.DoModify(user);
		cout << "Paid in Credit" << endl;
		return true;
	}
	else {
		cout << "Failed to Pay in Credit" << endl;
		return false;
	}
}

bool PayService::PayInCash() {
	cout << "Paid in Cash" << endl;
	return true;
}