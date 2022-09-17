/** @file TestConfigName.h
    @brief
    ### Define Test configurations identifier
        - Set initial value with test::main::Init()
        - Describes command line parameters: @ref main.cpp

    Option|Init value  |Config-ID                |Description
    ------|------------|-------------------------|-----------
    -l    |"./test.log"|CONFIG_LOGFILE           |log file name
    -d    |"./plugin"  |CONFIG_DIRECTORY         |DLL search path
    -n    |1           |CONFIG_LOOPNUM           |test loop number
    -s    |""          |CONFIG_TEST_PATTERN_RUN  |Test pattern to run
    -u    |""          |CONFIG_TEST_PATTERN_UNTIL|Do test until specified pattern
    -f    |""          |CONFIG_TEST_PATTERN_FROM |Do test from specified pattern
 */
#ifndef TEST_CONFIG_NAME_H
#define TEST_CONFIG_NAME_H

#define CONFIG_DIRECTORY "Directory" /*!< DLL search path */
#define CONFIG_LOGFILE "FileName"    /*!< log file name */
#define CONFIG_LOOPNUM "LoopNumber"  /*!< test loop number */
#define CONFIG_TEST_PATTERN_RUN "TestPatternRun"     /*!< Test pattern to run */
#define CONFIG_TEST_PATTERN_UNTIL "TestPatternUntil" /*!< Do test until specified pattern */
#define CONFIG_TEST_PATTERN_FROM "TestPatternFrom"   /*!< Do test from specified pattern */

#endif
