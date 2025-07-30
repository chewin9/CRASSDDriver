#include <gmock/gmock.h>
#include "shell.h"

#define UNIT_TEST (0)

#if (UNIT_TEST)
int main(void) {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}
#else 
int main(void) {
	Shell shell;
	return shell.Run();
}
#endif