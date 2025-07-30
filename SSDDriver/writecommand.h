#pragma once

#include "ICommand.h"

class WriteCommand : public ICommand {
public:

	bool Execute(int lba, std::string value) override;

};