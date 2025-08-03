// main.cpp (클라이언트 코드)
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

    CommandRunner runner(parser, fileio);

    runner.Run(argc, argv);

    return 0;
}
#endif
