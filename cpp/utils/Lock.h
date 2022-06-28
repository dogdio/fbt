#ifndef UTILS_LOCK_H
#define UTILS_LOCK_H

#include <mutex>

namespace Utils {
namespace Lock {

	class LockIF final { // automatic lock/unlock with RAII
	public:
		LockIF(std::mutex &Arg) : Mutex(Arg) {
			Mutex.lock();
		}
		~LockIF() {
			Mutex.unlock();
		}
		LockIF (const LockIF& rv) = delete;
		LockIF (const LockIF&& rv) = delete;
		LockIF& operator=(const LockIF& rv) = delete;
		LockIF& operator=(const LockIF&& rv) = delete;

	private:
		std::mutex &Mutex;
	};
}
}

#endif
