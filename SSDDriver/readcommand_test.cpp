#include "readcommand.h"

#include <string>
#include <vector>

#include "gmock/gmock.h"

using namespace testing;

class ReadCommandFixture : public Test {
 public:

  const int MAX_LBA = 100;
  const std::string NO_NEED_VALUE = "";
  const bool ENABLE_ERROR_FLAG = true;
  std::string normalValue = "0xFFFFFFFF";
};

TEST_F(ReadCommandFixture, NormalRead) {
  for (int normalLba = 0; normalLba < MAX_LBA; normalLba++) {
    ParsedCommand cmdInfo = {"R", normalLba};
    ReadCommand rc(cmdInfo);
    EXPECT_TRUE(rc.Execute());
  }
}

TEST_F(ReadCommandFixture, AbNormalRead) {
  for (int normalLba = 0; normalLba < MAX_LBA; normalLba++) {
    ParsedCommand cmdInfo = {"R", normalLba, NO_NEED_VALUE, ENABLE_ERROR_FLAG};
    ReadCommand rc(cmdInfo);
    EXPECT_FALSE(rc.Execute());
  }
}
