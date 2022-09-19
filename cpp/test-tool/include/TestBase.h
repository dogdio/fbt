/** @file  TestBase.h
    @brief Test interface class
	@see   test::TestBase */
#ifndef TEST_BASE_H
#define TEST_BASE_H

namespace test {
	struct TestBasePrivate;
	class TestMain;

	/** ### Test interface class
	    @pre
	    Test target class inherit this class \n
	    Framework and client code are independent of each (Depends on the interface) \n
	    pure virtual functions are called by test framework (see following sequence) \n
	    - you must implement pure virtual functions
	    - you can't call pure virtual functions

	    @note
	    sample implementation: @emoji :bulb: TestSample.cpp \n
	    helper macro (TEST_LOG(), VERIFY(),,,) can only use in the test code context \n

		### Sequence list
        -# @ref page11
        -# @ref page12
        -# @ref page13
	 */
	class TestBase {
	public:
		TestBase();
		virtual ~TestBase();

		/** Called once before all test process \n
		    write initialize code for all test
	        @mscfile Test.msc "test sequence" */
		virtual void InitializeOnce(void) = 0;

		/** Called once after all test process \n
		    write finalize code for all test */
		virtual void FinalizeOnce(void) = 0;

		/** Called per each test when test start \n
		    write initialize code for each test
		    @see GetTestID() */
		virtual void InitializePerTest(void) = 0;

		/** Called per each test when test stop \n
		    write finalize code for each test
		    @see GetTestID() */
		virtual void FinalizePerTest(void) = 0;

		/** Called once for registration your test code \n
		    please call Register() many times \n
		    @retval true test will be processed
		    @retval false test is not processed

	        @mscfile Regist.msc "regist sequence" */
		virtual bool RegisterTests(void) = 0;

		/** Called for get your name \n
		    @return pointer to your name (you can't return NULL) */
		virtual const char *OwnName(void) = 0;

		/** Call once in your constructor() \n
		    your instance is added to internal queue \n
			@post Only instances registered by this function are tested
		    @param[in] Base your instance

	        @mscfile Init.msc "init sequence" */
		void AddBaseQueue(TestBase *Base);

		/** typedef: Function pointer to Test code (2nd arg for Register())
		    @param[in] This your instance
		    @retval true test success
		    @retval false test failed
		    @see test_1_1_1() */
		using TEST_FUNC = bool (*)(void *This);

		/** Registration your test code \n
			@post Tests registered by this function will be called by the test framework
		    @param[in] TestID Test Identifier
		    @param[in] func function pointer to Test code
		    @retval true registration success
			@retval false registration failed */
		bool Register(const char *TestID, TEST_FUNC func);

		/** Get current Test-ID \n
		    you can customize test behavior by each Test-ID(ex. init process, special error case) @emoji :bulb:
		    @return pointer of Test-ID */
		const char *GetTestID(void);

		/** output log to file follownig format \n
		    "[TestID] hoge.cpp:123 your parameter is outputed"
		    @param[in] Format logging format(ex. same as printf)
		    @param[in] va_args each parameter to output
		    @see TEST_LOG() */
		void Logging(const char *Format, ...);
	
	private:
		friend TestMain;
		struct TestBasePrivate *priv; /*!< Private data for Test interface class @see TestMain */
	};
}

/** helper macro: Logging() without *This \n
    output Test-ID, source code location with your parameter \n
	@see test::TestBase::Logging()
	@see test_1_1_1()
    @param[in] F logging format(ex. same as printf)
    @param[in] va_args each parameter to output */
#define _TEST_LOG(F, ...) do { \
	Logging("%s:%d " F "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
} while(0)

/** helper macro: Logging() with *This \n
    output Test-ID, source code location with your parameter \n
	@see test::TestBase::Logging()
    @param[in] F logging format(ex. same as printf)
    @param[in] va_args each parameter to output */
#define TEST_LOG(F, ...) do { \
	((test::TestBase *)This)->Logging("%s:%d " F "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
} while(0)

/** helper macro: veirfy and output assert log \n
    if test failed return from test code \n
    @param[in] A comparison fomula
    @retval true to the next step
	@retval false fomula failed (return from test code)  */
#define VERIFY(A) do { \
	if(!(A)) { \
		((test::TestBase *)This)->Logging("%s:%d <<<<<<<<<< Assert\n", __FILE__, __LINE__); \
		return false; \
	} \
} while(0)

/** helper macro: veirfy and output assert log \n
    if test failed return from test code \n
    @param[in] A pointer to string for compare
    @param[in] B pointer to string for compare
    @retval true to the next step
	@retval false string mismatch (return from test code)  */
#define VERIFY_STR(A, B) do { \
	if(strcmp(A, B) != 0) { \
		((test::TestBase *)This)->Logging("%s:%d <<<<<<<<<< Assert\n", __FILE__, __LINE__); \
		return false; \
	} \
} while(0)

#endif
