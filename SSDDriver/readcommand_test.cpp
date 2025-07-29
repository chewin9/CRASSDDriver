#include "readcommand.h"

#include <string>

#include "gmock/gmock.h"

TEST(ReadCommand, NormalRead) {
  ReadCommand rc;
  int normalLba = 10;
  std::string normalValue = "0xFFFFFFFF";
  for (int i = 0; i < 100; i++) {
    EXPECT_TRUE(rc.Read(normalLba, normalValue));
  }
}

TEST(ReadCommand, ReadWithAbnormalLba) {
  ReadCommand rc;
  int abnormalLba = -1;
  std::string normalValue = "0xFFFFFFFF";
  EXPECT_FALSE(rc.Read(abnormalLba, normalValue));
}

TEST(ReadCommand, ReadWithAbnormalValue_NoFullWords) {
  ReadCommand rc;
  int normalLba = 1;
  std::string abnormalValue = "0xFFF";
  EXPECT_FALSE(rc.Read(normalLba, abnormalValue));
}

TEST(ReadCommand, ReadWithAbnormalValue_NoHexadecimalVal) {
  ReadCommand rc;
  int normalLba = 1;
  std::string abnormalValue = "0x00000GGG";
  EXPECT_FALSE(rc.Read(normalLba, abnormalValue));
}

TEST(ReadCommand, ReadWithAbnormalValue_NoStartWith0x) {
  ReadCommand rc;
  int normalLba = 1;
  std::string abnormalValue = "FFFFFFFF";
  EXPECT_FALSE(rc.Read(normalLba, abnormalValue));
}