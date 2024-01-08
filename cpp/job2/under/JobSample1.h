
namespace Job {

class JobSample1 : public JobIF {
public:
	JobSample1();
	~JobSample1();
	JobIF::JOB_STATE Exec(JOBIF_ARGS &args, JOBIF_CALLBACK func) override;
};

}
