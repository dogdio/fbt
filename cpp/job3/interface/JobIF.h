#ifndef JOB_IF_H
#define JOB_IF_H

#include <functional>
#include <vector>
#include <cstdint>

namespace Job {

	class JobIF {
	public:
		typedef enum {
			JOB_STATE_COMPLETE,
			JOB_STATE_PROCESSING,
			JOB_STATE_ABORT,
		} JOB_STATE;

		using JOBIF_CALLBACK = std::function<void(JOB_STATE)>;
		using JOBIF_ARGS = std::vector<std::string>;

		JobIF() {}
		virtual ~JobIF() {}

		// call before Exec
		virtual bool IsSupported(void) { return true; }
		virtual bool IsAvailable(void) { return true; }
		virtual bool IsBusy(void) { return false; }
		virtual bool IsValidParam(JOBIF_ARGS &args) { return true; }

		// XXX: implement your job
		virtual JOB_STATE Exec(JOBIF_ARGS &args, JOBIF_CALLBACK func=nullptr) = 0;
	};
	const static uint16_t LOG_TYPE = 0x1000;
}

#endif
