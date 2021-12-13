#include "decode.h"
#include <fstream>

void readhaff(std::map<std::string, int>& mp,FILE *in) {
	unsigned long long total;
	char buf [201];
	fscanf(in, "%lld", &total);
	for (size_t i = 0; i < total; ++i) {
		int ch;
		fscanf(in, "%d %s", &ch, buf);
		std::string codes(buf);
		mp.insert({ codes,ch });
	}
	return;
}

void decode(const std::map<std::string, int>&mp, const std::string& in, std::string& out){
	auto it = mp.end();
	int i = 0;
	while (i != in.length()) {
		int size = 0;
		while (it == mp.end()) {
			it = mp.find(in.substr(i,size));
			++size;
		}
		out.push_back((char)it->second);
		i += size;
		i -= 1;
		it = mp.end();
	}
}
