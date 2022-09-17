/** @file TestMain.h
    @brief test framework main code interface */
#ifndef TEST_MAIN_H
#define TEST_MAIN_H

#include <stdint.h>

namespace test {
namespace main {
	/** Initialize test configurations */
	void Init(void);

	/** Set string configuration */
	void SetConfig(const char *Key, const char *Value);

	/** Set integer configuration */
	void SetConfig(const char *Key, uint32_t Value);

	/** Run test framework \n
        - Open log file (@ref CONFIG_LOGFILE)
        - Load shared library from @ref CONFIG_DIRECTORY
        - Do tests.

    @retval true all tests success
    @retval false at least one test failed
    @see TestMain::RunTests() */
	bool Run(void);
}
}

#endif
