/** @file TestConfig.h
    @brief Test configuration interface */
#ifndef TEST_CONFIG_H
#define TEST_CONFIG_H

#include <stdint.h>

namespace test {
namespace config {

	/** provide abstract class instead of regular class @emoji :bulb: \n
	  @snippet TestConfig.h Snippet1
		- You don't have to write private members in header files
		  - Private members are fully encapsulated in the implementing side (TestConfig.cpp)
		  - No PIMPL idiom required
		  - No forward references required
		- Client code cannot be instantiated
		  - no more trouble copying/moving instances
		  - You can adopt the null object pattern if necessary
		- Say goodbye to the simple and innocent way of defining class hoge in hoge.h and implementing it in hoge.cpp

	    @dotfile Config.dot

        - @ref TestMain.cpp (Caller)
          @dontinclude TestMain.cpp
            @skip CONFIG_CATEGORY_GLOBAL
            @until ;
          @dontinclude TestMain.cpp
            @skip void Init
            @until signal
	 */
//[Snippet1]
	class ConfigIF {
	public:
		ConfigIF() {}
		virtual ~ConfigIF() {}
	
		virtual void Set(const char *Name, const char *Value) = 0; /*!< Set string value associated with @b Name */
		virtual void Set(const char *Name, uint32_t Value) = 0;    /*!< Set integer value associated with @b Name */
		virtual const char *GetString(const char *Name) = 0; /*!< Get string value associated with @b Name */
		virtual uint32_t GetInteger(const char *Name) = 0;   /*!< Get integer value associated with @b Name */
		virtual void Dump(void) = 0; /*!< show all values */
	};

	/** Get instance of test::config::ConfigIF associated with @b Name \n
	    you can call Set/Get API via config instance (get from this API) \n
	    @param[in] Name Instance Name(CONFIG_CATEGORY_XXX)
	    @retval !NULL valid ConfigIF pointer
	    @retval NULL no Instance specified by @b Name */
	ConfigIF *GetInstance(const char *Name);
//[Snippet1]
}
}

#define CONFIG_CATEGORY_GLOBAL "Global"    /*!< parameter for @ref GetInstance() 1st argument */
#define CONFIG_CATEGORY_FUNC1  "Function1"
#define CONFIG_CATEGORY_FUNC2  "Function2"

#endif
