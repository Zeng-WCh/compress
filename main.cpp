#include "file.h"
#include <string>
#include <iostream>
#include <bitset>
#include "code.h"

using namespace std;

int main() {
	string command;
	cin >> command;
	if (command == "zip") {
		string dir, des;
		cin >> dir >> des;
		file::getalltxt(dir);
		std::vector<std::string> res;
		file::readres(res);
		std::string tmp;
		bool isrelative=!(file::switchtype(dir));
		auto local = file::getlocaldir();
		if (isrelative) {
			local += '\\';
			local += dir;
			std::cout << local << std::endl;
		}
		for (int i = 0; i < (int)res.size(); ++i) {
			file::readfromfile(res[i], tmp);
			if (isrelative) {
				int n = local.length();
				res[i] = res[i].substr(n+1);
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
		auto save = code(res);
		file::save(save, des);
	}
	else if (command == "unzip") {
		string ans;
		std::ifstream readin;
		string dir, des;
		cin >> dir >> des;
		bool isrelative = !(file::switchtype(dir));
		string local = file::getlocaldir();
		if(isrelative){
			local+="\\";
			local += dir;
			readin.open(local);
		}
		else {
			readin.open(dir);
		}
		if (readin.is_open()) {
			char tmp;
			string s;
			while (readin.get(tmp)) {
				if (tmp != ' ' && tmp != '\n') {
					s.push_back(tmp);
				}
				else if(tmp==' ') {
					unsigned long long t = stoull(s);
					s.clear();
					std::bitset<8> t1(t);
					ans += t1.to_string();
				}
				else if (tmp == '\n') {
					int lastlen = stoi(s);
					string tmp1 = ans.substr(ans.length() - 8);
					string tmp2 = tmp1.substr(0,lastlen);
					ans = ans.substr(0, ans.length() - 8);
					ans += tmp2;
				}
			}
			//ansΪbitstream
		}
	}
}