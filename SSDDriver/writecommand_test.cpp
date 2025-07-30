#include "gmock/gmock.h"
#include "writecommand.h"
#include <iostream>

using namespace::testing;

class WriteCommandFixture : public Test {

public:
	WriteCommand write_command;

	int normalLba = 1;
	int abnormalLba = -1;

	const int MAX_VAL_SIZE = 100;
	std::string normalValue = "0xFFFFFFFF";
	std::vector<std::string> abnormalValue = { "0xFFF", "0x00000GGG", "FFFFFFFF", "" };
};

TEST_F(WriteCommandFixture, WriteNewFile) {
	for (int i = 0; i < MAX_VAL_SIZE; i++) {
		EXPECT_EQ(true, write_command.Execute(normalLba, normalValue));
	}
}


TEST_F(WriteCommandFixture, OverwirteNewLBA) {

	write_command.Execute(normalLba, normalValue);


	std::string new_value = "0xAAAABBBD";
	write_command.Execute(normalLba, new_value);

	int new_lba = 3;
	new_value = "0x1298CDEF";

	EXPECT_EQ(true, write_command.Execute(new_lba, new_value));
}