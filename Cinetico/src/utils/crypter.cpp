
#include <string>

#include "crypter.h"

std::string Crypter::SimpleHash(const std::string &originalValue) {
	std::string ret = originalValue;

	unsigned long base = 0xf139;
	unsigned long n = base;
	for(unsigned int i = 0; i < ret.length(); ++i) {
		char c = ret[i];
		n = (n << 1) * base + c;
		c = n & 0xff;
		ret[i] = c;
	}

	return ret;

}