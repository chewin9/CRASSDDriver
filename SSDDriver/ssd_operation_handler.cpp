#include "ssd_operation_handler.h"

void SsdOperationHandler::write()
{
	if (cmdInfo.errorFlag) {
		fileHandler.WriteValueToOutputFile("ERROR");
		return;
	}

	nandData = fileHandler.LoadDataFromInput();
	UpdateData();
	fileHandler.SaveData(nandData);
}

bool SsdOperationHandler::read() { return false; }

bool SsdOperationHandler::UpdateData()
{
	auto it = nandData.find(cmdInfo.lba);
	if (it != nandData.end()) {
		it->second = cmdInfo.value;
		return true;
	}

	nandData.emplace(cmdInfo.lba, cmdInfo.value);
	return false;
}