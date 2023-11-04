#include "TEA.h"

#define BITLEN32 4
#define BITLEN8 1
#define KEYLEN 4

void string2uint32(const std::string& in, uint32_t** out, size_t& retLen, bool isPad = false) {

	size_t strLen, mallocLen, padBitLen;

	std::string padString = in;
	strLen = padString.length();
	mallocLen = strLen / BITLEN32;
	if (isPad) {
		mallocLen += BITLEN8;
		padBitLen = BITLEN32 - (strLen % BITLEN32);
		if (mallocLen % 2 == 1)
		{
			mallocLen += BITLEN8;  padBitLen += BITLEN32;
		}
		padString.append(padBitLen - 1, '0');
		padString.append(std::to_string(padBitLen));
	}

	retLen = mallocLen;
	uint32_t* ptr = (uint32_t*)calloc(BITLEN32, mallocLen);
	memcpy((void*)ptr, (void*)padString.c_str(), mallocLen * BITLEN32);
	*out = ptr;
}

std::string uint322string(const uint32_t* in, const size_t& inLen) {
	char* ptr = (char*)calloc(BITLEN8, inLen * BITLEN32 + 1);
	ptr[inLen * BITLEN32] = '\0';
	memcpy((void*)ptr, (void*)in, inLen * BITLEN32);
	std::string ret(ptr);
	free(ptr);
	return ret;
}


void key2uint(const std::string& in, uint32_t** out) {
	uint32_t* ptr = (uint32_t*)calloc(BITLEN32, KEYLEN);
	memcpy((void*)ptr, (void*)in.c_str(), KEYLEN * BITLEN32);
	*out = ptr;
}

void uintencrypt(uint32_t* v, uint32_t* k) {
	uint32_t v0 = v[0], v1 = v[1], sum = 0, i;
	uint32_t delta = 0x9e3779b9;
	uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];
	for (i = 0; i < 32; i++) {
		sum += delta;
		v0 += ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
		v1 += ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
	}
	v[0] = v0; v[1] = v1;
}

void uintdecrypt(uint32_t* v, uint32_t* k) {
	uint32_t v0 = v[0], v1 = v[1], sum = 0xC6EF3720, i;
	uint32_t delta = 0x9e3779b9;
	uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];
	for (i = 0; i < 32; i++) {
		v1 -= ((v0 << 4) + k2) ^ (v0 + sum) ^ ((v0 >> 5) + k3);
		v0 -= ((v1 << 4) + k0) ^ (v1 + sum) ^ ((v1 >> 5) + k1);
		sum -= delta;
	}
	v[0] = v0; v[1] = v1;
}

std::string TEA::encrypt(const std::string& in, const std::string& key)
{
	uint32_t* dataPtr, * keyPtr;
	size_t ptrLen;
	string2uint32(in, &dataPtr, ptrLen, true);
	key2uint(key, &keyPtr);
	for (int i = 0; i < ptrLen; i += 2) {
		uintencrypt(dataPtr + i, keyPtr);
	}
	std::string encryptStr = uint322string(dataPtr, ptrLen);
	free(dataPtr);
	free(keyPtr);
	return encryptStr;
}

std::string TEA::decrypt(const std::string& in, const std::string& key)
{
	uint32_t* dataPtr, * keyPtr;
	size_t ptrLen;
	string2uint32(in, &dataPtr, ptrLen, false);
	key2uint(key, &keyPtr);
	for (int i = 0; i < ptrLen; i += 2) {
		uintdecrypt(dataPtr + i, keyPtr);
	}
	std::string decryptStr = uint322string(dataPtr, ptrLen);
	size_t delInt = atoi(&decryptStr[decryptStr.length() - 1]);
	decryptStr = decryptStr.erase(decryptStr.length() - delInt, delInt);
	free(dataPtr);
	free(keyPtr);
	return decryptStr;
}

