#ifndef UTILS_SCOPE_INOUT_H
#define UTILS_SCOPE_INOUT_H

namespace Utils {
namespace ScopeInOut {
	// XXX: pre include "Log.h" and define MY_LOG_TYPE

	class ScopeInOutIF final { // automatic scope in/out hooker with RAII
	public:
		ScopeInOutIF(const char *file, int line, int level, bool delta) : 
			MyFile(file), MyLine(line), LogLevel(level), DeltaTime(delta) {
			if(DeltaTime) {
				clock_gettime(CLOCK_REALTIME, &ts1);
			}
			else {
				Utils::Log::Logging(LOG_ATTR(Utils::Log::COLOR_PINK, LogLevel, MY_LOG_TYPE),
					"{SCOPE} %s:%d IN\n", MyFile, MyLine);
			}
		}
		~ScopeInOutIF() {
			if(DeltaTime) {
				clock_gettime(CLOCK_REALTIME, &ts2);
				long l1 = ts2.tv_nsec - ts1.tv_nsec;
				time_t l2 = ts2.tv_sec - ts1.tv_sec;
				double d3 = l2 + (double)l1 / 1000000000;
				Utils::Log::Logging(LOG_ATTR(Utils::Log::COLOR_PINK, LogLevel, MY_LOG_TYPE),
					"{SCOPE} %s:%d DELTA=%4.9f sec\n", MyFile, MyLine, d3);
			}
			else {
				Utils::Log::Logging(LOG_ATTR(Utils::Log::COLOR_PINK, LogLevel, MY_LOG_TYPE),
					"{SCOPE} %s:%d OUT\n", MyFile, MyLine);
			}
		}
		ScopeInOutIF (const ScopeInOutIF& rv) = delete;
		ScopeInOutIF (const ScopeInOutIF&& rv) = delete;
		ScopeInOutIF& operator=(const ScopeInOutIF& rv) = delete;
		ScopeInOutIF& operator=(const ScopeInOutIF&& rv) = delete;

	private:
		const char *MyFile;
		int MyLine;
		int LogLevel;
		bool DeltaTime;
		struct timespec ts1;
		struct timespec ts2;
	};
}
}

#ifdef ENABLE_SCOPE_INOUT
 #define SCOPE_INOUT(L)       Utils::ScopeInOut::ScopeInOutIF _sioif(__FILE__, __LINE__, L, false)
 #define SCOPE_INOUT_DELTA(L) Utils::ScopeInOut::ScopeInOutIF _sioif(__FILE__, __LINE__, L, true)
#else
 #define SCOPE_INOUT(L)
 #define SCOPE_INOUT_DELTA(L)
#endif

#endif
