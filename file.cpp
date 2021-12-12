#include "file.h"
#include <string>
#include <vector>
#include <bitset>
#include <fstream>


void file::save(const std::vector<std::string>& des, const std::string& filename){
	std::ofstream out;
	bool first = true;
	for (int i = 0; i < (int)des.size(); ++i) {
		unsigned long long count = 0;
		std::string s = des[i];
		unsigned long long n = s.length();
		std::vector<std::bitset<width>> ans;
		while (n - 8 > 0) {
			std::bitset<width> tmp(s);
			ans.push_back(tmp);
			s = s.substr(8);
			n -= 8;
			count++;
		}
		int more = 0;
		if (n > 0) {
			while (n - 8 < 0) {
				s.push_back('0');
				++n;
				++more;
			}
		}
		if (first) {
			first = false;
			out.open(filename);
			out << count << ' ';
		}
		else {
			out.open(filename, std::ios::app);
			out << count << ' ';
		}
		for (int j = 0; j < (int)ans.size(); ++i) {
			savebittofile(ans[j], filename);
		}
		ans.clear();
		out << more << ' ' << '\n';
		out.close();
	}
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

void file::readres(std::vector<std::string>& des){//读取文件目录
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
