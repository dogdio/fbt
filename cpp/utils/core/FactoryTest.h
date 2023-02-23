#ifndef UTILS_FACTORY_TEST_H
#define UTILS_FACTORY_TEST_H

namespace Utils {
namespace FactoryTest {

	typedef struct {
		void *Fail1;
		void *Fail2;
		void *Fail3;
		void *Create;
		void *GetInstance;
		bool Destroy;
	} Result;

	Result &CreateFail(void);
	Result &CreateSuccess(void);

}
}

#endif
