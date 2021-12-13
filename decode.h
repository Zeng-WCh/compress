#ifndef DECODE_H
#define DECODE_H

#include <map>
#include <vector>
#include <string>

inline int lenofint(size_t a) {
	int s = 0;
	while (a) {
		a /= 10;
		s++;
	}
	return s;
}

void readhaff(std::map<std::string, int>&, FILE* in);

void decode(const std::map<std::string, int>&, const std::string &in, std::string& out);

#endif