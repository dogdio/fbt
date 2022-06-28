#ifndef UTILS_EVENT_H
#define UTILS_EVENT_H

#include <mutex>
#include <condition_variable>

namespace Utils {
namespace Event {

	template <typename Msg_t>
	class EventIF final {

	public:
		EventIF() { }
		~EventIF() { }
		EventIF (const EventIF& rv) = delete;
		EventIF (const EventIF&& rv) = delete;
		EventIF& operator=(const EventIF& rv) = delete;
		EventIF& operator=(const EventIF&& rv) = delete;

		Msg_t &WaitMsg(void)
		{
			{
				std::unique_lock<std::mutex> lock(CondMutex);
				Cond.wait(lock, [&] { return wakeup; });
				wakeup = false;
			}
			return Buf;
		}

		void WakeMsg(Msg_t &Msg)
		{
			{
				std::lock_guard<std::mutex> lock(CondMutex);
				wakeup = true;
				Buf = Msg;
				Cond.notify_one();
			}
		}

	private:
		std::mutex CondMutex;
		std::condition_variable Cond;
		bool wakeup = false;
		Msg_t Buf;

	};
}
}

#endif
