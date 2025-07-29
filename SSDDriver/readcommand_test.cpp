#include "readcommand.h"
#include "gmock/gmock.h"
#include <string>

TEST(ReadCommand, NormalRead) {
  ReadCommand rc;
  int normalLba = 10;
  std::string normalValue = "0xFFFFFFFF";
  EXPECT_TRUE(rc.Read(normalLba, normalValue));
}

TEST(ReadCommand, ReadWithAbnormalLba) { 
	ReadCommand rc;
  int abnormalLba = -1;
   std::string normalValue = "0xFFFFFFFF";
  EXPECT_FALSE(abnormalLba, normalValue);
}

TEST(ReadCommand, ReadWithAbnormalValue_NoFullWords) {
  ReadCommand rc;
  int abnormalLba = 1;
  std::string normalValue = "0xFFF";
  EXPECT_FALSE(abnormalLba, normalValue);
}

TEST(ReadCommand, ReadWithAbnormalValue_NoHexadecimalVal) {
  ReadCommand rc;
  int abnormalLba = 1;
  std::string normalValue = "0x00000GGG";
  EXPECT_FALSE(abnormalLba, normalValue);
}