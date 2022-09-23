/** @file TestMain.cpp
    @brief Test framework main code
    @see @ref page01 */
#include <stdio.h>
#include <stdint.h>
#include <cstring>

#include "TestBase.h"
using namespace test;
#include "TestBasePrivate.h"
#include "TestFactory.h"
#include "TestConfig.h"
#include "TestConfigName.h"
#include "TestMain.h"

namespace test {
/** friend of TestBase \n
    this class can access TestBase private member
    @see @ref TestBasePrivate.h
    @see TestMain.cpp */
	class TestMain {
	public:
		TestMain() {}
		~TestMain() {}

		bool DoTestEachInstance(TestBase *Base);
		bool RunTests(FILE *fp);

		bool IsTestExecutable(TEST_LIST &tl, bool &from_ok);
		bool IsTestComplete(TEST_LIST &tl, bool result);
	};
}

namespace {
	TestMain TM;
	config::ConfigIF *CIF = config::GetInstance(CONFIG_CATEGORY_GLOBAL);
}

/** verify following test complete conditions
    - @ref CONFIG_TEST_PATTERN_UNTIL
    - @ref CONFIG_FAIL_AND_EXIT

    @param[in] tl test target(test-ID and test func)
    @param[in] result test result
    @retval true test is complete
    @retval false test is not complete (goto next test) */
bool TestMain::IsTestComplete(TEST_LIST &tl, bool result)
{
	const char *Until = CIF->GetString(CONFIG_TEST_PATTERN_UNTIL);
	bool ret = false;

	if(strlen(Until) > 0) {
		if(strstr(tl.TestID,  Until) != NULL) {
			ret = true;
		}
	}
	if(CIF->GetInteger(CONFIG_FAIL_AND_EXIT)) {
		if(!result) {
			ret = true;
		}
	}
end:
	return ret;
}

/** verify following test execution conditions
    - @ref CONFIG_TEST_PATTERN_RUN
    - @ref CONFIG_TEST_PATTERN_FROM

    @param[in] tl test target(test-ID and test func)
    @param[in,out] from_ok flag that satisfies CONFIG_TEST_PATTERN_FROM
    @retval true test is executable
    @retval false test is not executable(skip this test) */
bool TestMain::IsTestExecutable(TEST_LIST &tl, bool &from_ok)
{
	const char *Pattern = CIF->GetString(CONFIG_TEST_PATTERN_RUN);
	const char *From = CIF->GetString(CONFIG_TEST_PATTERN_FROM);
	bool ret = false;

	if(strlen(Pattern) > 0) {
		if(strstr(tl.TestID,  Pattern) == NULL) {
			goto end;
		}
	}
	else if(strlen(From) > 0) {
		if(!from_ok) {
			if(strstr(tl.TestID,  From) == NULL)
				goto end;
			else
				from_ok = true; // start from here
		}
	}
	ret = true;

end:
	return ret;
}

/** Do the following for each test
    -# set CurrentTestID. (you can get this at TestBase::GetTestID())
    -# TestBase::InitializePerTest()
    -# TestBase::TEST_FUNC()
    -# TestBase::FinalizePerTest()

    Test execution depends on the any configs (Default: exec all tests once) \n
    These are setting by command line parameter: see help()
    @see @ref TestConfigName.h
    @see TestBase

    @param[in] Base test target instance
    @retval true test success
    @retval false test failed */
bool TestMain::DoTestEachInstance(TestBase *Base)
{
	bool test_ret = true;
	int loop = CIF->GetInteger(CONFIG_LOOPNUM);

	for(int i = 0; i < loop; i++) {
		bool from_ok = false;
		printf("===== %s =====\n", Base->OwnName());
		for(auto tl : Base->priv->TestList) {
			if(tl.func == NULL)
				continue;
			if(!IsTestExecutable(tl, from_ok))
				continue;

			Base->priv->CurrentTestID = tl.TestID;

			Base->InitializePerTest();
			bool ret = tl.func(Base); // exec Test
			Base->FinalizePerTest();

			printf("[%s] %s\n", Base->priv->CurrentTestID, ret == true ? "OK" : "NG");
			fprintf(Base->priv->LogFile, "[%s] %s\n\n", Base->priv->CurrentTestID, ret == true ? "OK" : "NG");
			if(!ret)
				test_ret = false;
			if(IsTestComplete(tl, ret))
				break;
		}
	}
	return test_ret;
}

/** Do the following for each instances by lambda
    -# TestBase::RegisterTests()
    -# TestBase::InitializeOnce()
    -# DoTestEachInstance()
    -# TestBase::FinalizeOnce()

    @note
      Instance queue is implement at TestBase.cpp \n
      Traverse the queue with ForeachQueue()
    @param[in] fp FILE pointer for log output (set to priv->LogFile)
    @retval true all tests success
    @retval false at least one test failed */
bool TestMain::RunTests(FILE *fp)
{
	uint32_t fail = 0;

	{
		auto TestFunc = [&](TestBase *Base) {
			Base->priv->LogFile = fp;
			fprintf(fp, "##### Load '%s' successfully #####\n", Base->OwnName());
        
			Base->priv->EnableRegister = true;
			bool ret = Base->RegisterTests();
			Base->priv->EnableRegister = false;
        
			if(ret) {
				Base->priv->CurrentTestID = "InitializeOnce";
				Base->InitializeOnce();
            
				if(!DoTestEachInstance(Base))
					fail++;
            
				Base->priv->CurrentTestID = "FinalizeOnce";
				Base->FinalizeOnce();
				fprintf(fp, "\n");
			}

			Base->priv->TestList = {};
		};
		int num = base_priv::ForeachQueue(TestFunc);
		fprintf(fp, "##### load libraries = %d #####\n", num);
	}
	base_priv::ClearQueue();

	return fail == 0 ? true: false;
}


namespace test {
namespace main {

void Init(void)
{
	CIF->Set(CONFIG_LOGFILE, "./test.log");
	CIF->Set(CONFIG_DIRECTORY, "./plugin");
	CIF->Set(CONFIG_TEST_PATTERN_RUN, "");
	CIF->Set(CONFIG_TEST_PATTERN_UNTIL, "");
	CIF->Set(CONFIG_TEST_PATTERN_FROM, "");
	CIF->Set(CONFIG_LOOPNUM, 1);
	CIF->Set(CONFIG_FAIL_AND_EXIT, (uint32_t)0);
}

void SetConfig(const char *Key, uint32_t Value)
{
	CIF->Set(Key, Value);
}

void SetConfig(const char *Key, const char *Value)
{
	CIF->Set(Key, Value);
}

bool Run(void)
{
	uint32_t fail = 0;
	FILE *fp = fopen(CIF->GetString(CONFIG_LOGFILE), "w");

	//CIF->Dump();

	if(fp == NULL) {
		printf("### open error: %s\n", CIF->GetString(CONFIG_LOGFILE));
		fail++;
		goto end;
	}
	if(!factory::LoadTarget(CIF->GetString(CONFIG_DIRECTORY))) {
		printf("### load error: %s\n", CIF->GetString(CONFIG_DIRECTORY));
		fail++;
		fclose(fp);
		goto end;
	}

	if(!TM.RunTests(fp))
		fail++;

	factory::UnLoadTarget();
	fclose(fp);
end:
	return fail == 0 ? true: false;
}

}
}
