#include <string>
#include "String.h"

namespace Utils {
namespace String {

bool MatchAll(std::string &str, std::smatch &sm, const char *reg)
{
	if(reg == NULL)
		return false;
	return std::regex_match(str, sm, std::regex(reg));
}

void Replace(std::string &str, const char *before, const char *after)
{
	if(before == NULL)
		return;
	if(after == NULL)
		return;
	str = std::regex_replace(str, std::regex(before), after);
}

}
}

