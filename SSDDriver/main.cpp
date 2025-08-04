// main.cpp (Ŭ���̾�Ʈ �ڵ�)
#include "command_runner.h"
#include "command_parser.h"
#include "file_io.h"
#include "command_buffer.h"
#include "ssd_operation_handler.h"
#include "command_creator.h"

#if (_DEBUG)
#include "gmock/gmock.h"
int main(void) {
    ::testing::InitGoogleMock();
    return RUN_ALL_TESTS();
}
#else
int main(int argc, char* argv[]) {
    CommandParser parser;
    FileIO fileio;
    CommandBuffer cmdBuffer;
    SsdOperationHandler opHandler(fileio, cmdBuffer);

    CommandRunner runner(parser, fileio, cmdBuffer, opHandler);

    runner.Run(argc, argv);

    return 0;
}
#endif
