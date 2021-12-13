#include <string>
#include <iostream>
#include <bitset>
#include <cstring>
#include <map>
#include "file.h"
#include "htree.h"
#include "decode.h"

using namespace std;

void readcommand(string& s) {
	char* buf = new char[1024];
	memset(buf, 0, sizeof(char) * 1024);
	size_t count = 0;
	int tmp = getchar();
	while (tmp != ' ' && tmp != '\n') {
		buf[count] = tmp;
		count++;
		tmp = getchar();
	}
	buf[count] = '\0';
	if (!s.empty()) {
		s.clear();
	}
	s.resize(count);
	for (int i = 0; i <= count; ++i) {
		s[i] = buf[i];
	}
	return;
}

inline void printcli() {
	cout << ">>> ";
}

int main() {
	string command;
	printcli();
	readcommand(command);
	while (command != "exit") {
		if (command == "zip") {
			string dir, des;
			cin >> dir >> des;
			getchar();
			file::getalltxt(dir);
			std::vector<std::string> res;
			file::readres(res);
			std::string tmp;
			bool isrelative = !(file::switchtype(dir));
			auto local = file::getlocaldir();
			if (isrelative) {
				local += '\\';
				local += dir;
				//std::cout << local << std::endl;
			}
			for (int i = 0; i < (int)res.size(); ++i) {
				file::readfromfile(res[i], tmp);
				if (isrelative) {
					int n = local.length();
					res[i] = res[i].substr(n + 1);
				}
				else {
					int n = dir.length();
					res[i] = res[i].substr(n + 1);
				}
				res[i] += " ";
				res[i] += tmp;
				tmp.clear();
			}
			for (const auto& x : res) {
				std::cout << x << std::endl;
			}
			HTree htree;
			htree.code(res);
			std::vector<std::string> ans;
			transtohaff(res, ans, htree);
			file::save(ans, des);
			htree.save(des);
		}
		else if (command == "unzip") {
			FILE* p = NULL;
			string ans;
			string dir, des;
			cin >> dir >> des;
			getchar();
			bool isrelative = !(file::switchtype(dir));//是否是相对路径
			string local = file::getlocaldir();
			if (isrelative) {
				local += "\\";
				local += dir;
				p = fopen(local.c_str(), "rb");
			}
			else {
				p = fopen(dir.c_str(), "rb");
			}
			if (p!=NULL) {
				unsigned long long val[2]; 
				fread(val, sizeof(unsigned long long), 2, p);
				auto s = val[0];
				auto add = val[1];
				auto tmpbul = new unsigned long long[s];
				fread(tmpbul, sizeof(unsigned long long), s, p);
				fclose(p);
				p = NULL;
				if (isrelative) {
					p = fopen(local.c_str(), "r");
				}
				else {
					p = fopen(dir.c_str(), "r");
				}
				fseek(p, sizeof(unsigned long long) * (s + 2), SEEK_CUR);
				std::map<std::string,int>mp;
				readhaff(mp, p);
				std::string bitstr;
				for (size_t i = 0; i < s; ++i) {
					bitset<file::width> tmp(tmpbul[i]);
					bitstr += (tmp.to_string());
				}
				bitstr = bitstr.substr(0, bitstr.length() - add);
				std::string out;
				decode(mp, bitstr, out);
				std::cout << out << std::endl;
				fclose(p);
			}
		}
		command.clear();
		printcli();
		readcommand(command);
	}
}