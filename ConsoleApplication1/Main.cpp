#include <iostream>
#include "TableService.h"
#include "ViewService.h"
#include "DatabaseConnection.h"
#include "PayService.h"

using namespace std;


int main() {
	PayService ps;
	ps.SetUseCredit(true);
	DataObject obj = ps.DownloadProduct("1", "6");



	return 0;
}

