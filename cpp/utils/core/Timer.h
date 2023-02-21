#ifndef UTILS_TIMER_H
#define UTILS_TIMER_H

#include <functional>

namespace Utils {
namespace Timer {

	using FUNC_TYPE = std::function<void(void)>;

	class TimerIF {
	public:
		TimerIF();
		virtual ~TimerIF();

		virtual bool SetTick(uint32_t Tick) = 0; // Tick=usec

		// Repeat == 0 endless
		// Repeat != 0 only specified number of times
		// Timeout=msec
		virtual bool Add(const char *Name, FUNC_TYPE Func, uint32_t Timeout, uint32_t Repeat=0) = 0;
		virtual bool Remove(const char *Name) = 0;

		virtual bool Start(void) = 0;
		virtual bool Stop(void) = 0;
	};

	TimerIF *GetInstance(const char *Name);
	TimerIF *Create(const char *Name);
	bool Destroy(const char *Name);
}
}

#endif
