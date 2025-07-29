#include "gmock/gmock.h"
#include "testscript.h"

class SSDMock : public IProcessExecutor {
public:
	MOCK_METHOD((int), Process, (const std::string&), (override));
};

TEST(testscript, 1_FullWriteAndReadCompare) {
	SSDMock mock;
	EXPECT_CALL(mock, Process("copy")).Times(1);
}
