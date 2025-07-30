#include "gmock/gmock.h"
#include "file_io.h"
#include <iostream>

using namespace::testing;


TEST(FileIoTest, WriteErrorOutput) {
	FileIO file_io;
	EXPECT_EQ(true, file_io.WriteErrorOutput());
}
