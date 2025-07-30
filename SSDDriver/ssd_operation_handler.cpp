#include "ssd_operation_handler.h"

void SsdOperationHandler::write() {
	// 1. map or vec = fileHandler.LoadData(map or vec); // Load Data to Vector from ssd_nand.txt
	// 2. add update logic here
	// 3. fileHandler.SaveData(map or vec); // updated vector is saved in ssd_nand.txt
}

bool SsdOperationHandler::read() { return false; }