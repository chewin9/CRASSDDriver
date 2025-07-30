#include "gmock/gmock.h"
#include "file_io.h"
#include <iostream>

using namespace::testing;


TEST(FileIoTest, WriteErrorOutput) {
	FileIO file_io;
	file_io.WriteErrorOutput();

	EXPECT_EQ(true, file_io.OpenOutput(file_io.SSD_OUTPUT_FILE));
}
