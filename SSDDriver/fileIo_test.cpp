#include "gmock/gmock.h"
#include <iostream>
#include "file_io.h"

using namespace::testing;

TEST(SSD, WriteNewFile) {
	FileIO file;

	int LBA = 2;
	std::string value = "0xAAAABBBB";

	EXPECT_EQ(true, file.Write(LBA, value));
}

TEST(SSD, OverwirteNewLBA) {
	FileIO file;

	
	int LBA = 2;
	std::string value = "0xAAAABBBB";
	file.Write(LBA, value);

	LBA = 3;
	value = "0x1298CDEF";

	EXPECT_EQ(true, file.Write(LBA, value));
}