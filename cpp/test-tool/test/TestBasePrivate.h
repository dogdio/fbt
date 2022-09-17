/** @file TestBasePrivate.h
    @brief
    ### Private data for Test interface class
      - test::TestBase::priv is only accessible from our friend TestMain
      - Can not accessible from client code
      - Friend class is completely hidden in TestMain.cpp

    @dotfile PrivData1.dot
    ### Data structure
	  - Base->priv is static area (common to each instances)
	    - Each Instance does not work at the same time
	  - @b TestList will be create and destroy each instances
	  - @b VecInstances have user instance which added by test::TestBase::AddBaseQueue()

    @dotfile PrivData2.dot
    @see     test::TestMain::RunTests */
#ifndef TEST_BASE_PRIVATE_H
#define TEST_BASE_PRIVATE_H

#include <functional>
#include <vector>

namespace test {
	typedef struct {
		const char *TestID;       /*!< Test identifier */
		TestBase::TEST_FUNC func; /*!< Function pointer for test code */
	} TEST_LIST;

	struct TestBasePrivate {
		FILE *LogFile;                   /*!< File pointer for log output */
		const char *CurrentTestID;       /*!< This member is available by TestBase::GetTestID() */
		std::vector<TEST_LIST> TestList; /*!< Internal test list with setting by TestBase::Register() */
		bool EnableRegister;             /*!< If this is true, TestBase::Register() is available */
	};
}

namespace test {
namespace base_priv {
	/** typedef: Function pointer for ForeachQueue
	    @param[in] Base instance of User Code */
	using DO_TEST = std::function<void(TestBase *Base)>;

	/** Clear internal queue */
	void ClearQueue(void);

	/** Call back @b func for the number of instances stored in the internal queue \n
	    This queue is setting by test::TestBase::AddBaseQueue() \n
	    @param[in] func function pointer (lambda is available)
	    @return    number of instance */
	int ForeachQueue(DO_TEST func);

}
}

#endif
