
namespace Job {

class JobSample2 : public JobIF {
public:
	JobSample2();
	~JobSample2();
	JobIF::JOB_STATE Exec(JOBIF_ARGS &args, JOBIF_CALLBACK func) override;
	bool IsBusy(void) override;

private:
	bool busy;
};

}
