#include "ssd_operation_handler.h"

void SsdOperationHandler::write() {
	if (cmdInfo.errorFlag) {
		fileHandler.WriteValueToOutputFile("ERROR");
		return;
	}

	// 1. map or vec = fileHandler.LoadData(map or vec); // Load Data to Vector from ssd_nand.txt
	nandData = fileHandler.LoadDataFromInput();

	// 2. add update logic here
	fileHandler.UpdateData(nandData, cmdInfo);

	// 3. fileHandler.SaveData(map or vec); // updated vector is saved in ssd_nand.txt
	fileHandler.SaveData(nandData);
}

bool SsdOperationHandler::read() { return false; }