#include <stdio.h>
#include <string>
#include <vector>

#include <sys/types.h>
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <string.h>

#include "TestFactory.h"

using namespace test;

namespace { // private API

std::vector<void *> VecHandles;

bool IsSharedLibrary(std::string *str)
{
	static const char *ext = ".so";
	if(str->find(ext, str->size() - strlen(ext)) == std::string::npos)
		return false;
	else
		return true;
}

bool LoadSharedLibrary(std::string *str)
{
	void *Handle = NULL;
	bool ret = false;

	Handle = dlopen(str->c_str(), RTLD_LAZY);
	if(Handle == NULL) {
		printf("dlopen error %s, ret=%s, %d\n", str->c_str(), dlerror(), errno);
		goto end;
	}

	VecHandles.push_back(Handle);
	ret = true;

end:
	return ret;
}

} // anonymous namespace


namespace test {
namespace factory {

bool LoadTarget(const char *Directory)
{
	struct dirent *Dir;
	DIR *dp;
	bool ret = false;
	uint32_t load = 0;

	if(Directory == NULL)
		goto end;
	dp = opendir(Directory);
	if(dp == NULL)
		goto end;

	while((Dir = readdir(dp))) {
		if(Dir->d_type != DT_REG) // regular file
			continue;

		std::string str(Directory);
		str += "/";
		str += Dir->d_name;

		if(IsSharedLibrary(&str)) {
			if(LoadSharedLibrary(&str)) {
				load++;
			}
		}
	}

	closedir(dp);
	if(load)
		ret = true;
end:
	return ret;
}

void UnLoadTarget(void)
{
	for(void *Handle : VecHandles) {
		if(Handle == NULL)
			continue;

		dlclose(Handle);
	}
	VecHandles = {};
}

} // factory
} // test
