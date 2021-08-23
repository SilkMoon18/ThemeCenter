#include <iostream>
#include "TableUtil.h"
#include "ViewUtil.h"
#include "DatabaseConnection.h"
#include "PaySvr.h"

using namespace std;


int main() {
	PaySvr ps;
	ps.SetUseCredit(true);
	DataObject obj = ps.DownloadProduct("1", "6");



	return 0;
}

