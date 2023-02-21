#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <mutex>
#include <map>
#include "Log.h"
#include "Lock.h"

using namespace Utils;

namespace {

std::map<uint16_t, uint8_t> LevelMap = {
	{ Log::TYPE_MAIN,   Log::LEVEL_NONE},
	{ Log::TYPE_CONFIG, Log::LEVEL_NONE},
	{ Log::TYPE_UTILS,  Log::LEVEL_NONE},
};
std::map<uint16_t, const char *> StrMap = {
	{ Log::TYPE_MAIN,   "MAIN" },
	{ Log::TYPE_CONFIG, "CONF" },
	{ Log::TYPE_UTILS,  "UTIL" },
};

bool IsWriteStdOut = true;
bool IsWriteFile = true;
std::mutex Mutex;
const char *FileName = "sample.log";
FILE *Fp = NULL;
bool KeepOpen = true;
Log::TIME_FUNC TimeFunc = NULL;

class TestSeam : public Log::TestIF {
public:
	void InsertLevelMap(uint16_t type, Log::LEVEL level) {
		LevelMap[type] = level;
	}
	void InsertStrMap(uint16_t type, const char *str) {
		StrMap[type] = str;
	}
};
TestSeam TS_Inst;
Log::TestIF *TIF = &TS_Inst;

#define GET_COLOR(A) (Log::COLOR)((A & 0xff000000) >> 24)
#define GET_LEVEL(A) (Log::LEVEL)((A & 0x00ff0000) >> 16)
#define GET_TYPE(A)  (Log::TYPE)(A & 0x0000ffff)

void ColorEnd(void)
{
	unsigned char s2[] = {0x1b, 0x5b, 0x30, 0x6d};
	printf("%c%c%c%c", 
				s2[0], s2[1], s2[2], s2[3]);
}

void ColorStart(int fg, int bg)
{
	// 0x30 black
	// 0x31 red
	// 0x32 green
	// 0x33 yellow
	// 0x34 blue
	// 0x35 pink
	// 0x36 cyan
	// 0x37 white 
	// 0x38 bg keep
	unsigned char s1[] = {0x1b, 0x5b, 0x34, 0x30, 0x3b, 0x33, 0x30, 0x6d};
	s1[6] = fg;
	s1[3] = bg;
	printf("%c%c%c%c%c%c%c%c", 
				s1[0], s1[1], s1[2], s1[3], s1[4], s1[5], s1[6], s1[7]);
}

uint8_t GetLogLevel(uint16_t type)
{
	try {
		return LevelMap.at(type);
	}
	catch (const std::exception & e) {
		return Log::LEVEL_NONE;
	}
}

const char *GetLogStr(uint16_t type)
{
	try {
		return StrMap.at(type);
	}
	catch (const std::exception & e) {
		return "";
	}
}

bool WritePrefix(uint32_t attr, FILE *fp, bool color)
{
	bool color_end = false;
	Log::LEVEL level = GET_LEVEL(attr);
	Log::TYPE type = GET_TYPE(attr);
	Log::COLOR custom_color = GET_COLOR(attr);
	char buf[64];

	if(color) {
		color_end = true;
		switch(custom_color) {
			case Log::COLOR_RED:    ColorStart(0x31, 0x30); break;
			case Log::COLOR_YELLOW: ColorStart(0x33, 0x30); break;
			case Log::COLOR_GREEN:  ColorStart(0x32, 0x30); break;
			case Log::COLOR_CYAN:   ColorStart(0x36, 0x30); break;
			case Log::COLOR_PINK:   ColorStart(0x35, 0x30); break;
			default: color_end = false; break;
		}
	}

	if(TimeFunc != NULL) {
		TimeFunc(buf, sizeof(buf));
		fprintf(fp, "%s ", buf);
	}

	switch(level) {
		case Log::LEVEL_ERR:  fprintf(fp, "[%s] ERR:", GetLogStr(type)); break;
		case Log::LEVEL_WARN: fprintf(fp, "[%s] WARN:", GetLogStr(type)); break;
		case Log::LEVEL_INFO: fprintf(fp, "[%s] INFO:", GetLogStr(type)); break;
		case Log::LEVEL_DBG:  fprintf(fp, "[%s] DBG:", GetLogStr(type)); break;
		default: break;
	}

	return color_end;
}

void WriteStdOut(uint32_t attr, const char *format, va_list args)
{
	bool color = true;

	color = WritePrefix(attr, stdout, color);
	vprintf(format, args);

	if(color)
		ColorEnd();
}

void WriteFile(uint32_t attr, const char *format, va_list args)
{
	if(Fp == NULL)
		Fp = fopen(FileName, "a+");

	if(Fp != NULL) {
		WritePrefix(attr, Fp, false);
		vfprintf(Fp, format, args);

		if(!KeepOpen) {
			fclose(Fp);
			Fp = NULL;
		}
	}
}

} // anonymous namespace

namespace Utils {
namespace Log {

void Logging(uint32_t attr, const char *format, ...)
{
	Lock::LockIF lock(Mutex);
	va_list args;

	if(GetLogLevel(GET_TYPE(attr)) < GET_LEVEL(attr))
		return;

	if(IsWriteStdOut) {
		va_start(args, format);
		WriteStdOut(attr, format, args);
		va_end(args);
	}
	if(IsWriteFile) {
		va_start(args, format);
		WriteFile(attr, format, args);
		va_end(args);
	}
}

void SetKeepOpen(bool on_off)
{
	Lock::LockIF lock(Mutex);
	KeepOpen = on_off;
}

void SetFileName(const char *Name)
{
	Lock::LockIF lock(Mutex);
	FileName = Name;
}

bool SetLevel(uint16_t type, LEVEL level)
{
	Lock::LockIF lock(Mutex);

	if(level < Log::LEVEL_MAX) {
		try {
			TIF->InsertLevelMap(type, level);
			return true;
		}
		catch (const std::exception & e) {

		}
	}
	return false;
}

bool SetString(uint16_t type, const char *str)
{
	Lock::LockIF lock(Mutex);

	if(str == NULL)
		return false;

	try {
		TIF->InsertStrMap(type, str);
		return true;
	}
	catch (const std::exception & e) {
		return false;
	}
}

bool UnSet(uint16_t type)
{
	Lock::LockIF lock(Mutex);
	bool ret = false;

	if(StrMap.erase(type) == 1)
		ret = true;

	if(LevelMap.erase(type) == 1)
		ret = true;

	return ret;
}

bool GetToStdOut(void)
{
	Lock::LockIF lock(Mutex);
	return IsWriteStdOut;
}

void SetToStdOut(bool on_off)
{
	Lock::LockIF lock(Mutex);
	IsWriteStdOut = on_off;
}

bool GetToFile(void)
{
	Lock::LockIF lock(Mutex);
	return IsWriteFile;
}

void SetToFile(bool on_off)
{
	Lock::LockIF lock(Mutex);
	IsWriteFile = on_off;
}

bool OpenFile(const char *name, bool append)
{
	Lock::LockIF lock(Mutex);
	bool ret = false;

	if(Fp == NULL) {
		FileName = name;
		if(append)
			Fp = fopen(FileName, "a");
		else
			Fp = fopen(FileName, "w");

		if(Fp != NULL)
			ret = true;
	}
	return ret;
}

bool CloseFile(void)
{
	Lock::LockIF lock(Mutex);
	bool ret = false;

	if(Fp != NULL) {
		if(fclose(Fp) == 0) {
			Fp = NULL;
			ret = true;
		}
	}
	return ret;
}

void SetTimeFunc(Log::TIME_FUNC func)
{
	Lock::LockIF lock(Mutex);
	TimeFunc = func;
}

void SetTestIF(TestIF *obj)
{
	Lock::LockIF lock(Mutex);
	if(obj == NULL)
		TIF = &TS_Inst;
	else
		TIF = obj;
}

} // Log
} // Utils
