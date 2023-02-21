#ifndef UTILS_LOG_H
#define UTILS_LOG_H

#include <functional>

namespace Utils {
namespace Log {
	
	enum COLOR {
		COLOR_NONE,
		COLOR_RED,
		COLOR_YELLOW,
		COLOR_GREEN,
		COLOR_CYAN,
		COLOR_PINK,
		COLOR_MAX,
	};
	enum LEVEL {
		LEVEL_NONE,
		LEVEL_ERR,
		LEVEL_WARN,
		LEVEL_INFO,
		LEVEL_DBG,
		LEVEL_MAX,
	};
	enum TYPE { // #define MY_LOG_TYPE TYPE_XXX: you must define before include
		TYPE_MAIN,
		TYPE_CONFIG,
		TYPE_UTILS,
		// XXX: subsequent values can be added

		TYPE_MAX = 0xFFFF,
	};

	// attr 4byte: XXYYZZZZ (XX=COLOR, YY=LEVEL, ZZZZ=TYPE)
	void Logging(uint32_t attr, const char *format, ...);
	bool SetLevel(uint16_t type, LEVEL level); // default: all NONE
	bool SetString(uint16_t type, const char *str);
	bool UnSet(uint16_t type);

	void SetKeepOpen(bool on_off); // default: on
	void SetFileName(const char *Name); // default: sample.log

	bool OpenFile(const char *name, bool append = false);
	bool CloseFile(void);

	bool GetToStdOut(void);
	void SetToStdOut(bool on_off); // default: true
	bool GetToFile(void);
	void SetToFile(bool on_off);   // default: true

	using TIME_FUNC = std::function<void(char *buf, int size)>;
	void SetTimeFunc(TIME_FUNC func); // default: NULL

	// for test
	class TestIF {
	public:
		TestIF() {}
		virtual ~TestIF() {}

		virtual void InsertLevelMap(uint16_t type, LEVEL level) = 0;
		virtual void InsertStrMap(uint16_t type, const char *str) = 0;
	};
	void SetTestIF(TestIF *obj);
}
}

#define LOG_ATTR(C, L, T) ((C<<24)|(L<<16)|T)
#define LOG_ERR(A, ...)  Utils::Log::Logging(LOG_ATTR(Utils::Log::COLOR_RED,    Utils::Log::LEVEL_ERR,  MY_LOG_TYPE), "%s:%d " A "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_WARN(A, ...) Utils::Log::Logging(LOG_ATTR(Utils::Log::COLOR_YELLOW, Utils::Log::LEVEL_WARN, MY_LOG_TYPE), "%s:%d " A "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(A, ...) Utils::Log::Logging(LOG_ATTR(Utils::Log::COLOR_NONE,   Utils::Log::LEVEL_INFO, MY_LOG_TYPE), "%s:%d " A "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_DBG(A, ...)  Utils::Log::Logging(LOG_ATTR(Utils::Log::COLOR_NONE,   Utils::Log::LEVEL_DBG,  MY_LOG_TYPE), "%s:%d " A "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define LOG_INFO_GREEN(A, ...) Utils::Log::Logging(LOG_ATTR(Utils::Log::COLOR_GREEN, Utils::Log::LEVEL_INFO, MY_LOG_TYPE), "%s:%d " A "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO_CYAN(A, ...)  Utils::Log::Logging(LOG_ATTR(Utils::Log::COLOR_CYAN,  Utils::Log::LEVEL_INFO, MY_LOG_TYPE), "%s:%d " A "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO_PINK(A, ...)  Utils::Log::Logging(LOG_ATTR(Utils::Log::COLOR_PINK,  Utils::Log::LEVEL_INFO, MY_LOG_TYPE), "%s:%d " A "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define LOG_DBG_GREEN(A, ...) Utils::Log::Logging(LOG_ATTR(Utils::Log::COLOR_GREEN, Utils::Log::LEVEL_DBG, MY_LOG_TYPE), "%s:%d " A "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_DBG_CYAN(A, ...)  Utils::Log::Logging(LOG_ATTR(Utils::Log::COLOR_CYAN,  Utils::Log::LEVEL_DBG, MY_LOG_TYPE), "%s:%d " A "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_DBG_PINK(A, ...)  Utils::Log::Logging(LOG_ATTR(Utils::Log::COLOR_PINK,  Utils::Log::LEVEL_DBG, MY_LOG_TYPE), "%s:%d " A "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#endif
