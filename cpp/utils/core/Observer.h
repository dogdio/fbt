#ifndef UTILS_OBSERVER_H
#define UTILS_OBSERVER_H

#include <functional>
#include <string>

namespace Utils {
namespace Observer {

	typedef struct {
		std::string Str;
	} Msg_t;
	using FUNC_TYPE = std::function<void(Msg_t &)>;

	class ObserverIF {

	public:
		ObserverIF();
		virtual ~ObserverIF();

		// for receiver
		virtual bool Subscribe(const char *Reader, FUNC_TYPE Func) = 0;
		virtual bool UnSubscribe(const char *Reader) = 0;

		// for sender
		virtual bool Notify(Msg_t &Msg) = 0;
	};

	ObserverIF *GetInstance(const char *Name);
	ObserverIF *Create(const char *Name);
	bool Destroy(const char *Name);
}
}

#endif
