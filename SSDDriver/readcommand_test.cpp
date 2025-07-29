#include "readcommand.h"

#include <string>
#include <vector>

#include "gmock/gmock.h"

using namespace testing;

class ReadCommandFixture : public Test {
 public:
  ReadCommand rc;
  int normalLba = 1;
  int abnormalLba = -1;
  const int MAX_VAL_SIZE = 100;
  std::string normalValue = "0xFFFFFFFF";
  std::vector<std::string> abnormalValue = {"0xFFF", "0x00000GGG", "FFFFFFFF", ""};
};

TEST_F(ReadCommandFixture, NormalRead) {
  for (int i = 0; i < MAX_VAL_SIZE; i++) {
    EXPECT_TRUE(rc.Read(normalLba, normalValue));
  }
}

TEST_F(ReadCommandFixture, ReadWithAbnormalLba) {
  for (int i = 0; i < abnormalValue.size(); i++) {
    EXPECT_FALSE(rc.Read(normalLba, abnormalValue[i]));
  }
}

TEST_F(ReadCommandFixture, ReadWithAbnormalLbaAndAbnormalValue) {
  for (int i = 0; i < abnormalValue.size(); i++) {
    EXPECT_FALSE(rc.Read(abnormalLba, abnormalValue[i]));
  }
}
