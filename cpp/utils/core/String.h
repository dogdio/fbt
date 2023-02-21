#ifndef UTILS_STRING_H
#define UTILS_STRING_H

#include <regex>

namespace Utils {
namespace String {

	bool MatchAll(std::string &str, std::smatch &sm, const char *reg);
	void Replace(std::string &str, const char *before, const char *after);

}
}

#endif
