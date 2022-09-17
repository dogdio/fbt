/** @file TestFactory.h
    @brief Load/UnLoad shared library interface */
#ifndef TEST_FACTORY_H
#define TEST_FACTORY_H

namespace test {
/** @attention
    - APIs that don't need to be instantiated don't need classes
    - Enclose in namespace and avoid global scope
    - no need for singletons */
namespace factory {

	/** Load shared library from @b Directory
        @param[in] Directory the directory you wanto to load
	    @retval true load success
	    @retval false load failed */
	bool LoadTarget(const char *Directory);

	/** UnLoad shared library */
	void UnLoadTarget(void);
}
}

#endif
