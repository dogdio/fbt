#ifndef UTILS_TLV_H
#define UTILS_TLV_H

#include <vector>
#include <unordered_map>

namespace Utils {
namespace Tlv {

	using VECTOR = std::vector<uint8_t>;
	typedef struct {
		uint32_t tag;
		VECTOR value;
	} TlvData;
	using TLV_VEC = std::vector<TlvData>;
	using TLV_MAP = std::unordered_map<uint32_t, VECTOR>;

	class TlvIF {
	public:
		TlvIF();
		virtual ~TlvIF();

		// TLV Format:
		// { tag(4) + length(4) + value(N) }, { ... },,,

		// Binary --> TLV:
		virtual bool Parse(VECTOR &buf) = 0;
		virtual const TLV_VEC &GetTlvVec(void) = 0;
		virtual const TLV_MAP &GetTlvMap(void) = 0;

		// TLV --> Binary:
		virtual void ResetBinary(void) = 0;
		virtual bool Compose(TlvData &tld) = 0;
		virtual bool Compose(uint32_t tag, VECTOR &value) = 0;
		virtual const VECTOR &GetBinary(void) = 0;

		virtual void Dump(void) = 0;
	};

	TlvIF *GetInstance(const char *Name);
	TlvIF *Create(const char *Name);
	bool Destroy(const char *Name);
}
}

#endif
