#include <stdio.h>
#include <stdint.h>
#include <vector>
std::vector<uint8_t> Vec = {1, 2, 3, 4, 5};

// 戻り値が参照の場合:
// 呼び出し側では左辺の記述(&の有無)によって、値渡し/参照渡し が変わる
std::vector<uint8_t> &GetVec(void)
{
	return Vec;
}

const std::vector<uint8_t> &GetVec2(void)
{
	return Vec;
}

std::vector<uint8_t> *GetVec3(void) // pointer返し
{
	return &Vec;
}

int main(int argc, char **argv)
{
	std::vector<uint8_t> &v1 = GetVec(); // 参照渡し
	printf("v1: %02x %02x %02x %02x %02x\n", v1[0],v1[1],v1[2],v1[3],v1[4]);

	v1[2] = 0xff; // Vec[2]が更新される(v1は参照)

	std::vector<uint8_t> v2 = GetVec(); // 値渡し
	printf("v2: %02x %02x %02x %02x %02x\n", v2[0],v2[1],v2[2],v2[3],v2[4]);

	v2[2] = 0x00; // Vec[2]は変化しない(v2はコピー)

	std::vector<uint8_t> v3 = GetVec(); // 値渡し
	printf("v3: %02x %02x %02x %02x %02x\n", v3[0],v3[1],v3[2],v3[3],v3[4]);

	//std::vector<uint8_t> &v4 = GetVec2();  // build error
	std::vector<uint8_t>    v5 = GetVec2();  // 値渡しなのでconstなしでもOK

	v5[0] = 0xff;   // writable(not const)だが、Vec[0]は変化しない
	//v6[0] = 0xff; // build error(const)
	//v7[0] = 0xff; // build error(const)
	printf("v5: %02x %02x %02x %02x %02x\n", v5[0],v5[1],v5[2],v5[3],v5[4]);

	const std::vector<uint8_t> &v6 = GetVec2(); // const ref
	const std::vector<uint8_t>  v7 = GetVec2(); // const copy
	printf("v6: %02x %02x %02x %02x %02x\n", v6[0],v6[1],v6[2],v6[3],v6[4]);

	//std::vector<uint8_t> &v8 = v7; // build error
	std::vector<uint8_t> &v9  = const_cast<std::vector<uint8_t> &>(v7); // const外し
	std::vector<uint8_t> &v10 = (std::vector<uint8_t> &)(v7); // const外し
	v9[1] = 0xff;   // writable(not const)
	v10[1] = 0xff;   // writable(not const)

	// pointerの場合は左辺、右辺の両方を変える
	std::vector<uint8_t> *v11 = GetVec3();   // 参照渡し
	//std::vector<uint8_t> v12  = GetVec3(); // build error
	std::vector<uint8_t> v13  = *GetVec3();  // 値渡し

	return 0;
}
