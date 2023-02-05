#ifndef JOB_H
#define JOB_H

#include <functional>
#include <vector>

namespace Job {

	typedef enum {
		JOB_RET_NOT_FOUND,      // cmd is not found
		JOB_RET_NOT_SUPPORTED,  // cmd is not supported
		JOB_RET_BUSY,           // after previous job is finished, it is avaiable
		JOB_RET_UNAVAILABLE,    // unavailable on currnet state/configuration
		JOB_RET_INVALID_PARAM,  // parameter is invalid syntax

		// if job is executed, following code is returned
		JOB_RET_COMPLETE,       // job is completely finished
		JOB_RET_PROCESSING,     // job is now processing
		JOB_RET_ABORT,          // job is aborted
	} JOB_RET;

	using EXEC_ARGS = std::vector<const char *>;

	typedef struct {
		EXEC_ARGS args;
		JOB_RET ret;
	} JOB_CB_ARG;

	// When ret=JOB_RET_COMPLETE/JOB_RET_ABORT, async job is finish
	using CALLBACK = std::function<void(JOB_CB_ARG &arg)>;

	class PublicIF {
	public:
		PublicIF();
		virtual ~PublicIF();

		virtual JOB_RET Exec(EXEC_ARGS &args, CALLBACK func=NULL) = 0;
		virtual const char *RetToStr(JOB_RET ret) = 0;
	};

	PublicIF *GetInstance(const char *name);
	PublicIF *Create(const char *name);
	bool Destroy(const char *name);
}

#endif
