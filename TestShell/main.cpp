#include <gmock/gmock.h>
#include "shell.h"

int main(void) {
#if (_DEBUG)
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
#else 
	Shell shell;
	return shell.Run();
#endif 
}
