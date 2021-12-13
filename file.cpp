#include "file.h"
#include <string>
#include <vector>
#include <bitset>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <iostream>


void file::save(const std::vector<std::string>& des, const std::string& filename) {
	FILE* p=NULL;
	p = fopen(filename.c_str(), "wb");
	auto ans = file::cat(des);
	std::vector<std::bitset<width>> sfl;
	int add = width - (ans.length() % width);
	for (int i = 0; i < add; ++i) {
		ans.push_back('0');
	}
	while (ans.length() != 0) {
		std::bitset<width> tmp(ans);
		sfl.push_back(tmp);
		ans = ans.substr(width);
	}
	unsigned long long _s = sfl.size();
	unsigned long long tmp[2] = { _s,add };
	fwrite(tmp, sizeof(unsigned long long), 2, p);
	auto buf = new unsigned long long [_s];
	memset(buf, 0, sizeof(unsigned long long) * _s);
	size_t i = 0;
	for (const auto& x : sfl) {
		buf[i] = x.to_ullong();
		++i;
	}
	if (p != NULL) {
		fwrite(buf, sizeof(unsigned long long), _s, p);
	}
	fprintf(p, "\n");
	fclose(p);
	delete[] buf;
}

std::string file::getlocaldir() {//获取程序当前运行路径
	system("powershell dir >> d.tmp");
	std::ifstream in;
	in.open("d.tmp");
	std::string s;
	in >> s;
	in >> s;
	in.close();
	remove("d.tmp");
	return s;
}

void file::readres(std::vector<std::string>& des) {//读取文件目录
	std::ifstream in;
	in.open(dirname);
	if (in.is_open()) {
		std::string ins;
		while (in >> ins) {
			des.push_back(ins);
		}
		in.close();
	}
	remove(dirname);
	return;
}
