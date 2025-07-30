#include "writecommand.h"
#include "file_io.h"

#include <iostream>
#include <vector>

bool WriteCommand::Execute() {
	
	file_io.WriteOutput(cmdInfo);

	return true;
};