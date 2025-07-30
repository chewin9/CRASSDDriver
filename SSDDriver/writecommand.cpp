#include "writecommand.h"

bool WriteCommand::Execute() {
	file_io.WriteOutput(cmdInfo);
	return true;
};