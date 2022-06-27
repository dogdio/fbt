#ifndef TEST_BASE_H
#define TEST_BASE_H

class TestMain;
struct TestBasePrivate;

namespace test {

	class TestBase {
	public:
		TestBase();
		virtual ~TestBase();

		// implement by Test target
		virtual void InitializeOnce(void) = 0;
		virtual void FinalizeOnce(void) = 0;
	
		virtual void InitializePerTest(void) = 0;
		virtual void FinalizePerTest(void) = 0;
	
		virtual bool RegisterTests(void) = 0;
		virtual const char *OwnName(void) = 0;
	
		// call once in constructor()
		void AddBaseQueue(TestBase *Base);

		// call many time in RegisterTests()
		using TEST_FUNC = bool (*)(void *This);
		bool Register(const char *TestID, TEST_FUNC func);

		// helper function
		const char *GetTestID(void);
		void Logging(const char *Format, ...);
	
	private:
		friend TestMain;
		struct TestBasePrivate *priv;
	};
}

// helper macro: without *This
#define _TEST_LOG(F, ...) Logging("%s:%d " F "\n", __FILE__, __LINE__, ##__VA_ARGS__)

// helper macro: with *This
#define TEST_LOG(F, ...) ((test::TestBase *)This)->Logging("%s:%d " F "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define VERIFY(A) do { \
	if(!(A)) { \
		((test::TestBase *)This)->Logging("%s:%d <<<<<<<<<< Assert\n", __FILE__, __LINE__); \
		return false; \
	} \
} while(0)

#define VERIFY_STR(A, B) do { \
	if(strcmp(A, B) != 0) { \
		((test::TestBase *)This)->Logging("%s:%d <<<<<<<<<< Assert\n", __FILE__, __LINE__); \
		return false; \
	} \
} while(0)

#endif
