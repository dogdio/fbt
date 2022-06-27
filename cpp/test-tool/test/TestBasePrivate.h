#ifndef TEST_BASE_PRIVATE_H
#define TEST_BASE_PRIVATE_H

#include <functional>
#include <vector>

typedef struct {
	const char *TestID;
	TestBase::TEST_FUNC func;
} TEST_LIST;

struct TestBasePrivate {
	FILE *LogFile;
	const char *CurrentTestID;
	std::vector<TEST_LIST> TestList;
	bool EnableRegister;
};

namespace test {
namespace base_priv {
	using DO_TEST = std::function<void(TestBase *Base)>;
	void ClearQueue(void);
	int ForeachQueue(DO_TEST func);
}
}

#endif
