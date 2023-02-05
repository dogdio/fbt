#ifndef JOB_IF_H
#define JOB_IF_H

#include <functional>
#include <vector>

namespace Job {

	class JobIF {
	public:
		typedef enum {
			JOB_STATE_NOT_FOUND,
			JOB_STATE_NOT_SUPPORTED,
			JOB_STATE_BUSY,
			JOB_STATE_UNAVAILABLE,
			JOB_STATE_INVALID_PARAM,

			JOB_STATE_COMPLETE,
			JOB_STATE_PROCESSING,
			JOB_STATE_ABORT,
		} JOB_STATE;

		using JOBIF_CALLBACK = std::function<void(JOB_STATE)>;
		using QUEUE_FUNC = std::function<void(void)>;
		using JOBIF_ARGS = std::vector<const char *>;

		JobIF(const char *job_id, const char *cmd_id);
		virtual ~JobIF();

		virtual const char *GetCmdID(void) { return CmdID; }
		virtual const char *GetJobID(void) { return JobID; }

		// call before Exec
		virtual bool IsSupported(void) { return true; }
		virtual bool IsAvailable(void) { return true; }
		virtual bool IsBusy(void) { return false; }
		virtual bool IsValidParam(JOBIF_ARGS &args) { return true; }

		// XXX: implement your job
		virtual JOB_STATE Exec(JOBIF_ARGS &args, JOBIF_CALLBACK func) = 0;

	protected:
		// enqueue any action to job context
		bool Enqueue(QUEUE_FUNC func);
		// exec another command associated 'JobID'
		JOB_STATE ExecMyself(JOBIF_ARGS &args, JOBIF_CALLBACK func);

		// Set Constructor
		const char *JobID;
		const char *CmdID;
	};
}

#endif
