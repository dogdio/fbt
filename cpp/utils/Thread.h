#ifndef UTILS_THREAD_H
#define UTILS_THREAD_H

#include <functional>

namespace Utils {
namespace Thread {

	using FUNC_TYPE = std::function<void(void)>;

	class ThreadIF {
	public:
		ThreadIF();
		virtual ~ThreadIF();

		virtual bool Enqueue(FUNC_TYPE Func) = 0;
	};

	ThreadIF *GetInstance(const char *Name);
	ThreadIF *Create(const char *Name);
	bool Destroy(const char *Name);
	void Initialize(void);
	void Finalize(void);
	uint64_t GetID(void);

	static const char *MODEL = "Model";
	static const char *VIEW = "View";
	static const char *CONTROLLER = "Controller";

}
}

#endif
