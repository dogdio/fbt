#ifndef TEST_MAIN_H
#define TEST_MAIN_H

#include <stdint.h>

namespace test {
namespace main {

	void Init(void);
	void SetConfig(const char *Key, const char *Value);
	void SetConfig(const char *Key, uint32_t Value);
	bool Run(void);
}
}


#endif
