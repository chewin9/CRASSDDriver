#pragma once

#include "command_parser.h"
#include "file_io.h"
#include "command_buffer.h"
#include "ssd_operation_handler.h"
#include "icommand.h"

class CommandRunner {
public:
    CommandRunner(CommandParser& parser, FileIO& fileio, CommandBuffer& cmdBuffer, SsdOperationHandler& opHandler);

    void Run(int argc, char* argv[]);

private:
    CommandParser& parser;
    FileIO& fileio;
    CommandBuffer& cmdBuffer;
    SsdOperationHandler& opHandler;
};
