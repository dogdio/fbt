#include <stdio.h>
#include <stdint.h>

#include "Lock.h"
#include "TLV.h"
#define MY_LOG_TYPE Log::TYPE_UTILS
#include "Log.h"
#include "Factory.h"

using namespace Utils;

namespace {
class TlvPriv : public Tlv::TlvIF {
public:
	~TlvPriv();
	TlvPriv(const char *Name);

	bool Parse(Tlv::VECTOR &buf) override;
	const Tlv::TLV_VEC &GetTlvVec(void) override;
	const Tlv::TLV_MAP &GetTlvMap(void) override;
	void ResetBinary(void) override;
	bool Compose(Tlv::TlvData &tld) override;
	bool Compose(uint32_t tag, Tlv::VECTOR &value) override;
	const Tlv::VECTOR &GetBinary(void) override;
	void Dump(void) override;

	void ToBinData(uint32_t num);

	Tlv::TLV_VEC TlvVec;
	Tlv::TLV_MAP TlvMap;
	Tlv::VECTOR BinData;
	const char *MyName;
};
Factory::FactoryIF<TlvPriv, Tlv::TlvIF> Inst("Utils.Tlv");
const static int MIN_SIZE = sizeof(uint32_t) * 2; // tag + length

}
#define EXTRACT_4BYTE(V, A) do { \
	V = (A[0] << 24 | A[1] << 16 | A[2] << 8 | A[3]); \
	A += 4; \
} while(0)

TlvPriv::TlvPriv(const char *Name)
{
	if(Name == NULL)
		throw std::invalid_argument("Name is null");

	MyName = Name;
}

TlvPriv::~TlvPriv()
{

}

bool TlvPriv::Parse(Tlv::VECTOR &buf)
{
	size_t remain = buf.size();
	uint8_t *p = buf.data();
	TlvVec = {};
	TlvMap = {};

	LOG_DBG("[%s] Parse: rem=%d, %d", MyName, remain, MIN_SIZE);
	if(remain < MIN_SIZE)
		return false;

	while(remain >= MIN_SIZE) {
		Tlv::TlvData tld = {};
		uint32_t length;

		EXTRACT_4BYTE(tld.tag, p);
		EXTRACT_4BYTE(length, p);

		remain -= MIN_SIZE;
		if(length != 0) {
			if(length > remain)
				return false;
            
			LOG_DBG("[%s] Parse: tag=%08x, len=%d", MyName, tld.tag, length);
			tld.value.resize(length);
			for(uint32_t i = 0; i < length; i++)
				tld.value[i] = p[i];
            
			remain -= length;
			p += length;
		}
		TlvVec.push_back(tld);
		TlvMap[tld.tag] = tld.value;
	}
	return true;
}

const Tlv::TLV_MAP &TlvPriv::GetTlvMap(void)
{
	return TlvMap;
}

const Tlv::TLV_VEC &TlvPriv::GetTlvVec(void)
{
	return TlvVec;
}

void TlvPriv::ResetBinary(void)
{
	BinData = {};
	LOG_DBG("[%s] Reset: Bin", MyName);
}

void TlvPriv::ToBinData(uint32_t num)
{
	for(int i = 0; i < 4; i++) {
		uint8_t tmp;
		int bit = 24 - (i * 8);
		tmp = ((num >> bit) & 0xff);
		BinData.push_back(tmp);
	}
}

bool TlvPriv::Compose(uint32_t tag, Tlv::VECTOR &value)
{
	ToBinData(tag);
	ToBinData(value.size());

	size_t tail = BinData.size();
	BinData.resize(tail + value.size());

	for(uint32_t i = 0; i < value.size(); i++)
		BinData[tail+i] = value[i];

	LOG_DBG("[%s] Compose: tag=%08x, len=%d", MyName, tag, BinData.size());
	return true;
}

bool TlvPriv::Compose(Tlv::TlvData &tld)
{
	return Compose(tld.tag, tld.value);
}

const Tlv::VECTOR &TlvPriv::GetBinary(void)
{
	return BinData;
}

void TlvPriv::Dump(void)
{
#if WITH_GCOV == 0
	for(auto it: TlvVec) {
		printf("V: tag=[%8x]: len=%ld, ", it.tag, it.value.size());
		for(auto v: it.value) {
			printf("%02x ", v);
		}
		printf("\n");
	}

	for(auto it: TlvMap) {
		printf("M: tag=[%8x]: len=%ld, ", it.first, it.second.size());
		for(auto v: it.second) {
			printf("%02x ", v);
		}
		printf("\n");
	}

	printf("Bin: ");
	for(auto b: BinData) {
		printf("%02x ", b);
	}
	printf("\n");
#endif
}


namespace Utils {
namespace Tlv {

class TlvNull : public Tlv::TlvIF {
public:
	~TlvNull() { }
	TlvNull() { }

	bool Parse(Tlv::VECTOR &buf) { return true; }
	const Tlv::TLV_VEC &GetTlvVec(void) { return TlvVec; }
	const Tlv::TLV_MAP &GetTlvMap(void) { return TLVm; }
	void ResetBinary(void) {} 
	bool Compose(Tlv::TlvData &tld) { return true; }
	bool Compose(uint32_t tag, Tlv::VECTOR &value) { return true; }
	const Tlv::VECTOR &GetBinary(void) { return BinData; }
	void Dump(void) {}

	Tlv::TLV_VEC TlvVec;
	Tlv::TLV_MAP TLVm;
	Tlv::VECTOR BinData;
};

TlvNull TlvNullInst;

TlvIF::TlvIF() {}
TlvIF::~TlvIF() {}

TlvIF *GetInstance(const char *Name)
{
	TlvIF *tif = Inst.GetInstance(Name);
	if(tif == NULL)
		return &TlvNullInst;
	else
		return tif;
}

TlvIF *Create(const char *Name)
{
	TlvIF *tif = Inst.Create(Name);
	if(tif == NULL)
		return &TlvNullInst;
	else
		return tif;
}

bool Destroy(const char *Name)
{
	return Inst.Destroy(Name);
}

} // Tlv
} // Utils

