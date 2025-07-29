#include "gmock/gmock.h"
#include <iostream>
#include "ssd.h"

using namespace::testing;

TEST(SSD, Wirte3) {
	FileIO file;

	int LBA = 3;
	std::string value = "0x1298CDEF";

	EXPECT_EQ(true, file.Write(LBA, value));


}
//ssd W 2 0xAAAABBBB
TEST(SSD, Write2) {

}

int main(void) {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}