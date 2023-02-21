#ifndef UTILS_QUEUE_H
#define UTILS_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

namespace Utils {
namespace Queue {

	template <typename Msg_t>
	class QueueIF final {

	public:
		QueueIF() { }
		~QueueIF() { }
		QueueIF (const QueueIF& rv) = delete;
		QueueIF (const QueueIF&& rv) = delete;
		QueueIF& operator=(const QueueIF& rv) = delete;
		QueueIF& operator=(const QueueIF&& rv) = delete;

		Msg_t &WaitMsg(void)
		{
			{
				std::unique_lock<std::mutex> lock(CondMutex);

				Cond.wait(lock, [&] { return !JobQ.empty(); });
				Buf = JobQ.front();
				JobQ.pop();
			}
			return Buf;
		}

		void WakeMsg(Msg_t &Msg)
		{
			{
				std::lock_guard<std::mutex> lock(CondMutex);

				JobQ.push(Msg);
				Cond.notify_one();
			}
		}

	private:
		std::mutex CondMutex;
		std::condition_variable Cond;
		std::queue<Msg_t> JobQ;
		Msg_t Buf;
	};
}
}

#endif
