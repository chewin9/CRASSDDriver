#include "command_runner.h"
#include "gmock/gmock.h"

#if (_DEBUG)
int main(void) {
  ::testing::InitGoogleMock();
  return RUN_ALL_TESTS();
}
#else
int main(int argc, char* argv[]) {
  CommandRunner runner;
  runner.Run(argc, argv);
  return 0;
}
#endif
